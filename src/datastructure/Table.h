#pragma once
#ifndef TABLE_H
#define TABLE_H
#include <boost/thread.hpp>
#include <iostream>
#include <utility>
#include <list>
#include <functional>
#include <boost/pool/singleton_pool.hpp>
#include "../api/HashStream.h"
#include "../api/Memory.h"
#include "../storage/DataTypes.h"

template<class KEY,class VALUE>
struct Bucket {
    KEY key;
    VALUE value;
    bool hasItem;
    virtual ~Bucket()=default;
    Bucket() {
        key = KEY{};
        value = VALUE{};
        hasItem = false;
    }

    Bucket(const Bucket & rhs) {
        key = rhs.key;
        value = rhs.value;
        hasItem = rhs.hasItem;
    }

    Bucket(Bucket && rhs) noexcept {
        key = std::move(rhs.key);
        value = std::move(rhs.value);
        hasItem = std::move(rhs.hasItem);
    }


    Bucket& operator=(Bucket && rhs) noexcept {
        if (this != &rhs) {
            key = std::move(rhs.key);
            value = std::move(rhs.value);
            hasItem = std::move(rhs.hasItem);
        }
        return *this;
    }

    Bucket & operator=(const Bucket & rhs) {
        if (this != &rhs) {
            key = rhs.key;
            value = rhs.value;
            hasItem = rhs.hasItem;
        }
        return *this;
    }
    void Append(const KEY & k, const VALUE & v) {
        this->key = k;
        this->value = v;
        this->hasItem = true;
    }
    void Append(const Bucket & bucket) {
        this->key = bucket.key;
        this->value = bucket.value;
        this->hasItem = bucket.hasItem;
    }
    static Bucket<KEY, VALUE> EMPTY;
};
template<class KEY,class VALUE> Bucket<KEY, VALUE> Bucket<KEY,VALUE>::EMPTY;

#if defined(__GNUC__) || defined(__GNUG__)
template<class KEY, class VALUE, int N = 1009, int M = 1013, class STREAM = HashStream >
#else
template<class KEY,class VALUE, int N=HashStream::PRIME_ARRAY[0], int M = HashStream::PRIME_ARRAY[1], class STREAM = HashStream >
#endif
class Table {
public:
    Table(){
        firstContainer = nullptr;
        secondContainer = nullptr;
        allocate();
        initContainer(N, M);
    }
    virtual ~Table() {
        deallocate();
        currentSizeFirstContainer = -1;
        currentSizeSecondContainer = -1;
    }

    Table(const Table & rhs) {
        firstContainer = nullptr;
        secondContainer = nullptr;
        allocate(rhs.currentSizeFirstContainer, rhs.currentSizeSecondContainer);
        initContainer(rhs.currentSizeFirstContainer, rhs.currentSizeSecondContainer);

        hashStream = rhs.hashStream;
        for (int i = 0; i < currentSizeFirstContainer; ++i) {
            firstContainer[i] = rhs.firstContainer[i];
        }
        for (int i = 0; i < currentSizeSecondContainer; ++i) {
            secondContainer[i] = rhs.secondContainer[i];
        }

    }

    Table(Table && rhs) noexcept{
        firstContainer = nullptr;
        secondContainer = nullptr;
        currentSizeFirstContainer = rhs.currentSizeFirstContainer;
        currentSizeSecondContainer = rhs.currentSizeSecondContainer;
        firstContainer = rhs.firstContainer;
        secondContainer = rhs.secondContainer;
        hashStream = rhs.hashStream;
        rhs.firstContainer = nullptr;
        rhs.secondContainer = nullptr;
        rhs.currentSizeFirstContainer = -1;
        rhs.currentSizeSecondContainer = -1;

    }

    Table & operator=(const Table & rhs) {
        if (this != &rhs) {
            deallocate();
            firstContainer = nullptr;
            secondContainer = nullptr;
            allocate(rhs.currentSizeFirstContainer, rhs.currentSizeSecondContainer);
            currentSizeFirstContainer = rhs.currentSizeFirstContainer;
            currentSizeSecondContainer = rhs.currentSizeSecondContainer;
            hashStream = rhs.hashStream;
            for (int i = 0; i < currentSizeFirstContainer; ++i) {
                firstContainer[i] = rhs.firstContainer[i];
            }
            for (int i = 0; i < currentSizeSecondContainer; ++i) {
                secondContainer[i] = rhs.secondContainer[i];
            }
        }
        return *this;
    }
    Table & operator=(Table && rhs)  noexcept {
        if (this != &rhs) {
            deallocate();
            currentSizeFirstContainer = rhs.currentSizeFirstContainer;
            currentSizeSecondContainer = rhs.currentSizeSecondContainer;
            firstContainer = rhs.firstContainer;
            secondContainer = rhs.secondContainer;
            hashStream = rhs.hashStream;
            rhs.firstContainer = nullptr;
            rhs.secondContainer = nullptr;
            rhs.currentSizeFirstContainer = -1;
            rhs.currentSizeSecondContainer = -1;
        }
        return *this;
    }


    VALUE & operator[](const KEY & key) {
        std::size_t pos1 = hashForFirstContainer(key);
        if (firstContainer[pos1].hasItem && firstContainer[pos1].key == key) {
            return firstContainer[pos1].value;
        }
        pos1 = hashForSecondContainer(key);
        if (secondContainer[pos1].hasItem && secondContainer[pos1].key == key) {
            return secondContainer[pos1].value;
        }
        addItem(key, VALUE{});
        return operator[](key);
    }

    VALUE & operator[](KEY && key) {
        std::size_t pos1 = hashForFirstContainer(key);
        if (firstContainer[pos1].hasItem && firstContainer[pos1].key == key) {
            return firstContainer[pos1].value;
        }
        pos1 = hashForSecondContainer(key);
        if (secondContainer[pos1].hasItem && secondContainer[pos1].key == key) {
            return secondContainer[pos1].value;
        }
        addItem(key, VALUE{});
        return operator[](key);
    }
    //[[deprecated( "use operator [ ] ")]]
    void insert(const KEY &key, VALUE && value){
        std::size_t pos1 = hashForFirstContainer(key);
        if (firstContainer[pos1].hasItem && firstContainer[pos1].key == key) {
            VALUE tmp = std::move(firstContainer[pos1].value);
            firstContainer[pos1].value = std::move(value);
            addItem(key, tmp);
            return;
        }
        pos1 = hashForSecondContainer(key);
        if (secondContainer[pos1].hasItem && secondContainer[pos1].key == key) {
            VALUE tmp = std::move(secondContainer[pos1].value);
            secondContainer[pos1].value = std::move(value);
            addItem(key, tmp);
            return;
        }
        addItem(key,value);

    }

    void remove(const KEY & key) {
        std::size_t pos1 = hashForFirstContainer(key);
        if (firstContainer[pos1].key == key) {
            firstContainer[pos1].key = KEY{};
            firstContainer[pos1].value = VALUE{};
            firstContainer[pos1].hasItem = false;
            return;
        }
        pos1 = hashForSecondContainer(key);
        if (secondContainer[pos1].key == key) {
            secondContainer[pos1].key = KEY{};
            secondContainer[pos1].value = VALUE{};
            secondContainer[pos1].hasItem = false;
        }
    }

    void removeAll(){
        for(int i =0 ; i < currentSizeFirstContainer; ++i){
            firstContainer[i].hasItem = false;
        }
        for(int i =0 ; i < currentSizeSecondContainer; ++i){
            secondContainer[i].hasItem = false;
        }

        //allocate(this->currentSizeFirstContainer,this->currentSizeSecondContainer);
    }


    void addItem(const KEY & key, const VALUE & item) {
        if (lookup(key)) {
            return;
        }
        Bucket < KEY, VALUE> bucket = store(key, item);
        if (bucket.hasItem) {
            rehash(bucket);
        }

    }

    constexpr bool lookup(const KEY & key)  {

        std::size_t pos = hashForFirstContainer(key);

       if(std::cmp_less(pos, getSizeOfFirstContainer()) && firstContainer[pos].hasItem && firstContainer[pos].key == key) {
            return true;
        }
        pos = hashForSecondContainer(key);

        if(std::cmp_less(pos, getSizeOfSecondContainer()) && secondContainer[pos].hasItem && secondContainer[pos].key == key) {
            return true;
        }

        return false;
    }


    [[nodiscard]] constexpr int getSizeOfFirstContainer()const {
        return currentSizeFirstContainer;
    }

    [[nodiscard]] constexpr int getSizeOfSecondContainer()const {
        return currentSizeSecondContainer;
    }


    std::list<KEY> filter(std::function<bool (const VALUE&)> func ){
        std::list<KEY> resultList;

        for(int i = 0 ; i < currentSizeFirstContainer; ++i){
            if(firstContainer[i].hasItem){
                if(func(firstContainer[i].value)){
                    resultList.push_back(firstContainer[i].key);
                }
            }

        }

        for(int i = 0; i < currentSizeSecondContainer; ++i){
            if(secondContainer[i].hasItem){
                if(func(secondContainer[i].value)){
                    resultList.push_back(secondContainer[i].key);
                }
            }
        }
        return resultList;
    }

    std::list<VALUE> convertToList(){
        std::list<VALUE> resultList;
        for(int i = 0 ; i < currentSizeFirstContainer; ++i){
            if(firstContainer[i].hasItem){
               resultList.push_back(firstContainer[i].value);
            }

        }

        for(int i = 0; i < currentSizeSecondContainer; ++i){
            if(secondContainer[i].hasItem){
                resultList.push_back(secondContainer[i].value);
            }
        }
        return resultList;
    }

    void updateAllItems(std::function<void (const KEY &key,VALUE&)> func){
        for(int i = 0 ; i < currentSizeFirstContainer; ++i){
            if(firstContainer[i].hasItem){
                func(firstContainer[i].key,firstContainer[i].value);
            }

        }

        for(int i = 0; i < currentSizeSecondContainer; ++i){
            if(secondContainer[i].hasItem){
                func(secondContainer[i].key,secondContainer[i].value);
            }
        }
    }



    void updateAllItems(std::function<void (VALUE&)> func){
        for(int i = 0 ; i < currentSizeFirstContainer; ++i){
            if(firstContainer[i].hasItem){
                func(firstContainer[i].value);
            }

        }

        for(int i = 0; i < currentSizeSecondContainer; ++i){
            if(secondContainer[i].hasItem){
                func(secondContainer[i].value);
            }
        }
    }

private:

    std::size_t hash(const KEY & key, int size) {
        return std::hash<KEY>{}(key) % size;
    }
    std::size_t hashForFirstContainer(const KEY & key) {
        return hash(key,currentSizeFirstContainer);
    }
    std::size_t hashForSecondContainer(const KEY & key) {
        return hash(key, currentSizeSecondContainer);
    }

    Bucket<KEY, VALUE> store(const KEY & key, const VALUE & item) {
        Bucket<KEY, VALUE> bucket;
        bucket.Append(key, item);

        for (int i = 0; i < currentSizeSecondContainer + currentSizeFirstContainer; ++i) {
            std::size_t pos = hashForFirstContainer(bucket.key);

            if (!firstContainer[pos].hasItem) {
                firstContainer[pos] = std::move(bucket);
                return Bucket<KEY, VALUE>::EMPTY;
            }

            Bucket<KEY, VALUE> tmp = std::move(firstContainer[pos]);
            firstContainer[pos] = std::move(bucket);
            bucket = std::move(tmp);

            pos = hashForSecondContainer(bucket.key);
            if (!secondContainer[pos].hasItem) {
                secondContainer[pos] = std::move(bucket);
                return Bucket<KEY,VALUE>::EMPTY;
            }
            tmp = std::move(secondContainer[pos]);
            secondContainer[pos] = std::move(bucket);
            bucket = std::move(tmp);
        }
        return bucket;
    }

private:
    void initContainer(int firstContainerSize, int secondContainerSize) {
        currentSizeFirstContainer = firstContainerSize;
        currentSizeSecondContainer = secondContainerSize;
    }
    auto copyBucketsFromContainerToList(const Bucket<KEY, VALUE> * container, std::size_t size) {
        std::list < Bucket<KEY, VALUE>> tmpBucketList;
        for (std::size_t i = 0; i < size; ++i) {
            if (container[i].hasItem) {
                tmpBucketList.push_back(container[i]);
            }
        }
        return tmpBucketList;
    }
private:
    void rebuild(const std::list<Bucket<KEY, VALUE>> & tmpContainer1, const std::list<Bucket<KEY, VALUE>> & tmpContainer2){

        int newSizeForFirstContainer = hashStream.next();
        int newSizeForSecondContainer = hashStream.next();
        allocate(newSizeForFirstContainer, newSizeForSecondContainer);
        initContainer(newSizeForFirstContainer, newSizeForSecondContainer);

        for (auto item : tmpContainer1) {
            if (store(item.key, item.value).hasItem) {
                rebuild(tmpContainer1, tmpContainer2);
                return;
            }
        }
        for (auto item : tmpContainer2) {
            if (store(item.key, item.value).hasItem) {
                rebuild(tmpContainer1, tmpContainer2);
                return;
            }
        }



    }
    void rehash(const Bucket<KEY,VALUE> & bucket) {
        auto tmpContainer1 = copyBucketsFromContainerToList(firstContainer, currentSizeFirstContainer);
        auto tmpContainer2 = copyBucketsFromContainerToList(secondContainer, currentSizeSecondContainer);

        rebuild(tmpContainer1,tmpContainer2);
        Bucket<KEY, VALUE> b = store(bucket.key, bucket.value);
        if ( b.hasItem ) {
            rehash(b);
        }

    }
private:

    void allocate(int n = N,int m = M) {
        deallocate();
        //firstContainer = static_cast<Bucket<KEY,VALUE>*>(memoryPool::ordered_malloc(n));
        //secondContainer = static_cast<Bucket<KEY,VALUE>*>(memoryPool::ordered_malloc(m));
        firstContainer = new Bucket<KEY, VALUE>[n]();
        secondContainer = new Bucket<KEY, VALUE>[m]();
    }


    void deallocate() {
        DeallocateMemoryArray(firstContainer);
        DeallocateMemoryArray(secondContainer);
    }



private:
    Bucket<KEY,VALUE> * firstContainer;
    Bucket<KEY,VALUE> * secondContainer;
    int currentSizeFirstContainer;
    int currentSizeSecondContainer;
    STREAM hashStream;


};

#endif
