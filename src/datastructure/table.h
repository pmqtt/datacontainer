#pragma once
#ifndef TABLE_H
#define TABLE_H
#include <boost/thread.hpp>
#include <iostream>
#include <utility>
#include <list>
#include <functional>
#include <boost/pool/singleton_pool.hpp>
#include "../api/hash_stream.h"
#include "../api/memory.h"
#include "../storage/data_types.h"

template<class KEY,class VALUE>
struct bucket {
    KEY key;
    VALUE value;
    bool has_item;
    virtual ~bucket()=default;
    bucket() {
        key = KEY{};
        value = VALUE{};
        has_item = false;
    }

    bucket(const bucket & rhs) {
        key = rhs.key;
        value = rhs.value;
        has_item = rhs.has_item;
    }

    bucket(bucket && rhs) noexcept {
        key = std::move(rhs.key);
        value = std::move(rhs.value);
        has_item = std::move(rhs.has_item);
    }


    bucket& operator=(bucket && rhs) noexcept {
        if (this != &rhs) {
            key = std::move(rhs.key);
            value = std::move(rhs.value);
            has_item = std::move(rhs.has_item);
        }
        return *this;
    }

    bucket & operator=(const bucket & rhs) {
        if (this != &rhs) {
            key = rhs.key;
            value = rhs.value;
            has_item = rhs.has_item;
        }
        return *this;
    }
    void append(const KEY & k, const VALUE & v) {
        this->key = k;
        this->value = v;
        this->has_item = true;
    }
    void Append(const bucket & bck) {
        this->key = bck.key;
        this->value = bck.value;
        this->has_item = bck.has_item;
    }
    static bucket<KEY, VALUE> EMPTY;
};
template<class KEY,class VALUE> bucket<KEY, VALUE> bucket<KEY,VALUE>::EMPTY;

#if defined(__GNUC__) || defined(__GNUG__)
template<class KEY, class VALUE, int N = 1009, int M = 1013, class STREAM = hash_stream >
#else
template<class KEY,class VALUE, int N=hash_stream::PRIME_ARRAY[0], int M = hash_stream::PRIME_ARRAY[1], class STREAM = hash_stream >
#endif
class table {
public:
    table(){
        first_container = nullptr;
        second_container = nullptr;
        allocate();
        init_container(N, M);
    }
    virtual ~table() {
        deallocate();
        current_size_first_container = -1;
        current_size_second_container = -1;
    }

    table(const table & rhs) {
        first_container = nullptr;
        second_container = nullptr;
        allocate(rhs.current_size_first_container, rhs.current_size_second_container);
        init_container(rhs.current_size_first_container, rhs.current_size_second_container);

        hash_values = rhs.hash_values;
        for (int i = 0; i < current_size_first_container; ++i) {
            first_container[i] = rhs.first_container[i];
        }
        for (int i = 0; i < current_size_second_container; ++i) {
            second_container[i] = rhs.second_container[i];
        }

    }

    table(table && rhs) noexcept{
        first_container = nullptr;
        second_container = nullptr;
        current_size_first_container = rhs.current_size_first_container;
        current_size_second_container = rhs.current_size_second_container;
        first_container = rhs.first_container;
        second_container = rhs.second_container;
        hash_values = rhs.hash_values;
        rhs.first_container = nullptr;
        rhs.second_container = nullptr;
        rhs.current_size_first_container = -1;
        rhs.current_size_second_container = -1;

    }

    table & operator=(const table & rhs) {
        if (this != &rhs) {
            deallocate();
            first_container = nullptr;
            second_container = nullptr;
            allocate(rhs.current_size_first_container, rhs.current_size_second_container);
            current_size_first_container = rhs.current_size_first_container;
            current_size_second_container = rhs.current_size_second_container;
            hash_values = rhs.hash_values;
            for (int i = 0; i < current_size_first_container; ++i) {
                first_container[i] = rhs.first_container[i];
            }
            for (int i = 0; i < current_size_second_container; ++i) {
                second_container[i] = rhs.second_container[i];
            }
        }
        return *this;
    }
    table & operator=(table && rhs)  noexcept {
        if (this != &rhs) {
            deallocate();
            current_size_first_container = rhs.current_size_first_container;
            current_size_second_container = rhs.current_size_second_container;
            first_container = rhs.first_container;
            second_container = rhs.second_container;
            hash_values = rhs.hash_values;
            rhs.first_container = nullptr;
            rhs.second_container = nullptr;
            rhs.current_size_first_container = -1;
            rhs.current_size_second_container = -1;
        }
        return *this;
    }


    VALUE & operator[](const KEY & key) {
        std::size_t pos1 = hash_for_first_container(key);
        if (first_container[pos1].has_item && first_container[pos1].key == key) {
            return first_container[pos1].value;
        }
        pos1 = hash_for_second_container(key);
        if (second_container[pos1].has_item && second_container[pos1].key == key) {
            return second_container[pos1].value;
        }
        add_item(key, VALUE{});
        return operator[](key);
    }

    VALUE & operator[](KEY && key) {
        std::size_t pos1 = hash_for_first_container(key);
        if (first_container[pos1].has_item && first_container[pos1].key == key) {
            return first_container[pos1].value;
        }
        pos1 = hash_for_second_container(key);
        if (second_container[pos1].has_item && second_container[pos1].key == key) {
            return second_container[pos1].value;
        }
        add_item(key, VALUE{});
        return operator[](key);
    }
    //[[deprecated( "use operator [ ] ")]]
    void insert(const KEY &key, VALUE && value){
        std::size_t pos1 = hash_for_first_container(key);
        if (first_container[pos1].has_item && first_container[pos1].key == key) {
            VALUE tmp = std::move(first_container[pos1].value);
            first_container[pos1].value = std::move(value);
            add_item(key, tmp);
            return;
        }
        pos1 = hash_for_second_container(key);
        if (second_container[pos1].has_item && second_container[pos1].key == key) {
            VALUE tmp = std::move(second_container[pos1].value);
            second_container[pos1].value = std::move(value);
            add_item(key, tmp);
            return;
        }
        add_item(key, value);

    }

    void remove(const KEY & key) {
        std::size_t pos1 = hash_for_first_container(key);
        if (first_container[pos1].key == key) {
            first_container[pos1].key = KEY{};
            first_container[pos1].value = VALUE{};
            first_container[pos1].has_item = false;
            return;
        }
        pos1 = hash_for_second_container(key);
        if (second_container[pos1].key == key) {
            second_container[pos1].key = KEY{};
            second_container[pos1].value = VALUE{};
            second_container[pos1].has_item = false;
        }
    }

    void remove_all(){
        for(int i =0 ; i < current_size_first_container; ++i){
            first_container[i].has_item = false;
        }
        for(int i =0 ; i < current_size_second_container; ++i){
            second_container[i].has_item = false;
        }

        //allocate(this->current_size_first_container,this->current_size_second_container);
    }


    void add_item(const KEY & key, const VALUE & item) {
        if (lookup(key)) {
            return;
        }
        bucket < KEY, VALUE> bck = store(key, item);
        if (bck.has_item) {
            rehash(bck);
        }

    }

    constexpr bool lookup(const KEY & key)  {

        std::size_t pos = hash_for_first_container(key);

       if(std::cmp_less(pos, get_size_of_first_container()) && first_container[pos].has_item && first_container[pos].key == key) {
            return true;
        }
        pos = hash_for_second_container(key);

        if(std::cmp_less(pos, get_size_of_second_container()) && second_container[pos].has_item && second_container[pos].key == key) {
            return true;
        }

        return false;
    }


    [[nodiscard]] constexpr int get_size_of_first_container()const {
        return current_size_first_container;
    }

    [[nodiscard]] constexpr int get_size_of_second_container()const {
        return current_size_second_container;
    }


    std::list<KEY> filter(std::function<bool (const VALUE&)> func ){
        std::list<KEY> result_list;

        for(int i = 0 ; i < current_size_first_container; ++i){
            if(first_container[i].has_item){
                if(func(first_container[i].value)){
                    result_list.push_back(first_container[i].key);
                }
            }

        }

        for(int i = 0; i < current_size_second_container; ++i){
            if(second_container[i].has_item){
                if(func(second_container[i].value)){
                    result_list.push_back(second_container[i].key);
                }
            }
        }
        return result_list;
    }

    std::list<VALUE> convert_to_list(){
        std::list<VALUE> result_list;
        for(int i = 0 ; i < current_size_first_container; ++i){
            if(first_container[i].has_item){
               result_list.push_back(first_container[i].value);
            }

        }

        for(int i = 0; i < current_size_second_container; ++i){
            if(second_container[i].has_item){
                result_list.push_back(second_container[i].value);
            }
        }
        return result_list;
    }

    void update_all_items(std::function<void (const KEY &key, VALUE&)> func){
        for(int i = 0 ; i < current_size_first_container; ++i){
            if(first_container[i].has_item){
                func(first_container[i].key, first_container[i].value);
            }

        }

        for(int i = 0; i < current_size_second_container; ++i){
            if(second_container[i].has_item){
                func(second_container[i].key, second_container[i].value);
            }
        }
    }



    void update_all_items(std::function<void (VALUE&)> func){
        for(int i = 0 ; i < current_size_first_container; ++i){
            if(first_container[i].has_item){
                func(first_container[i].value);
            }

        }

        for(int i = 0; i < current_size_second_container; ++i){
            if(second_container[i].has_item){
                func(second_container[i].value);
            }
        }
    }

private:

    std::size_t hash(const KEY & key, int size) {
        return std::hash<KEY>{}(key) % size;
    }
    std::size_t hash_for_first_container(const KEY & key) {
        return hash(key, current_size_first_container);
    }
    std::size_t hash_for_second_container(const KEY & key) {
        return hash(key, current_size_second_container);
    }

    bucket<KEY, VALUE> store(const KEY & key, const VALUE & item) {
        bucket<KEY, VALUE> bck;
        bck.append(key, item);

        for (int i = 0; i < current_size_second_container + current_size_first_container; ++i) {
            std::size_t pos = hash_for_first_container(bck.key);

            if (!first_container[pos].has_item) {
                first_container[pos] = std::move(bck);
                return bucket<KEY, VALUE>::EMPTY;
            }

            bucket<KEY, VALUE> tmp = std::move(first_container[pos]);
            first_container[pos] = std::move(bck);
            bck = std::move(tmp);

            pos = hash_for_second_container(bck.key);
            if (!second_container[pos].has_item) {
                second_container[pos] = std::move(bck);
                return bucket<KEY,VALUE>::EMPTY;
            }
            tmp = std::move(second_container[pos]);
            second_container[pos] = std::move(bck);
            bck = std::move(tmp);
        }
        return bck;
    }

private:
    void init_container(int firstContainerSize, int secondContainerSize) {
        current_size_first_container = firstContainerSize;
        current_size_second_container = secondContainerSize;
    }
    auto copy_buckets_from_container_to_list(const bucket<KEY, VALUE> * container, std::size_t size) {
        std::list < bucket<KEY, VALUE>> tmp_bucket_list;
        for (std::size_t i = 0; i < size; ++i) {
            if (container[i].has_item) {
                tmp_bucket_list.push_back(container[i]);
            }
        }
        return tmp_bucket_list;
    }
private:
    void rebuild(const std::list<bucket<KEY, VALUE>> & tmp_container_1, const std::list<bucket<KEY, VALUE>> & tmp_container_2){

        int new_size_for_first_container = hash_values.next();
        int new_size_for_second_container = hash_values.next();
        allocate(new_size_for_first_container, new_size_for_second_container);
        init_container(new_size_for_first_container, new_size_for_second_container);

        for (auto item : tmp_container_1) {
            if (store(item.key, item.value).has_item) {
                rebuild(tmp_container_1, tmp_container_2);
                return;
            }
        }
        for (auto item : tmp_container_2) {
            if (store(item.key, item.value).has_item) {
                rebuild(tmp_container_1, tmp_container_2);
                return;
            }
        }



    }
    void rehash(const bucket<KEY,VALUE> & bck) {
        auto tmp_container_1 = copy_buckets_from_container_to_list(first_container, current_size_first_container);
        auto tmp_container_2 = copy_buckets_from_container_to_list(second_container, current_size_second_container);

        rebuild(tmp_container_1, tmp_container_2);
        bucket < KEY, VALUE > b = store(bck.key, bck.value);
        if ( b.has_item ) {
            rehash(b);
        }

    }
private:

    void allocate(int n = N,int m = M) {
        deallocate();
        //first_container = static_cast<bucket<KEY,VALUE>*>(memoryPool::ordered_malloc(n));
        //second_container = static_cast<bucket<KEY,VALUE>*>(memoryPool::ordered_malloc(m));
        first_container = new bucket<KEY, VALUE>[n]();
        second_container = new bucket<KEY, VALUE>[m]();
    }


    void deallocate() {
        deallocate_memory_array(first_container);
        deallocate_memory_array(second_container);
    }



private:
    bucket<KEY,VALUE> * first_container;
    bucket<KEY,VALUE> * second_container;
    int current_size_first_container;
    int current_size_second_container;
    STREAM hash_values;


};

#endif
