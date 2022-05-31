//
// Created by cefour on 30.05.22.
//

#ifndef DATACONTAINER_TIMETREE_H
#define DATACONTAINER_TIMETREE_H


#include "../api/MacroHelper.h"

#include <chrono>
#include <cmath>
#include <tuple>
#include <utility>
#include <vector>

using namespace std::chrono_literals;

using INDEX_T = long long int;

struct TreeIndexPoint{
    INDEX_T treeIndex;
    INDEX_T nodeIndex;
};
template<class VALUE>
struct TimeTreeItem{
    std::chrono::milliseconds timestampMS;
    VALUE value;
    bool operator <(const TimeTreeItem & rhs)const{
        return timestampMS < rhs.timestampMS;
    }
    bool operator <=(const TimeTreeItem & rhs)const{
        return timestampMS <= rhs.timestampMS;
    }
    bool operator ==(const TimeTreeItem & rhs)const{
        return timestampMS <= rhs.timestampMS;
    }
};
template<class VALUE>
struct TimeTreeNode{
    std::chrono::milliseconds startPointMS;
    std::chrono::milliseconds endPointMS;
    std::vector<TimeTreeItem<VALUE>> items;
};

using VECSIZE_T = std::vector<long>::size_type;
template<class VALUE>
class TimeTree{
    struct Iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = VALUE;
        using pointer           = VALUE*;  // or also value_type*
        using reference         = VALUE&;  // or also value_type&


        Iterator(std::vector<TimeTreeNode<VALUE>> * tree,std::size_t parentIndex,std::size_t childIndex ) : innerTree(tree){
            currentIndexParent = parentIndex;
            currentIndexNode = childIndex;
            _ptr = &tree->at(parentIndex).items[childIndex].value;

        }

        Iterator(std::vector<TimeTreeNode<VALUE>> * tree) : innerTree(tree){
            currentIndexParent = 0;
            currentIndexNode = 0;
            if(!tree->empty() && !tree->at(0).items.empty()) {
                _ptr = &tree->at(0).items[0].value;
            }else{
                _ptr = nullptr;
            }
        }
        Iterator(std::vector<TimeTreeNode<VALUE>> * tree,bool end) : innerTree(tree) {
            UNUSED(end);
            if(!tree->empty() && !tree->at(0).items.empty()) {
                currentIndexParent = innerTree->size();
                currentIndexNode = innerTree->at(currentIndexParent - 1).items.size();
            }
            _ptr = nullptr;
        }

        reference operator*() const { return *_ptr; }
        pointer operator->() { return _ptr; }

        // Prefix increment
        Iterator& operator++() {
            if(_ptr == nullptr){
                return *this;
            }
            if(currentIndexNode < (innerTree->at(currentIndexParent).items.size()-1)){
                currentIndexNode++;
                _ptr = &innerTree->at(currentIndexParent).items[currentIndexNode].value;
            }else{
                if(currentIndexParent < innerTree->size()-1){
                    currentIndexParent++;
                    currentIndexNode = 0;
                    if(currentIndexNode < innerTree->at(currentIndexParent).items.size()){

                        _ptr = &innerTree->at(currentIndexParent).items[currentIndexNode].value;
                    }else{
                        _ptr = nullptr;
                    }
                }else{
                    _ptr = nullptr;
                }
            }
            return *this;
        }

        // Postfix increment
        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator== (const Iterator& a, const Iterator& b) { return a._ptr == b._ptr; };
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a._ptr != b._ptr; };


    private:
        std::size_t currentIndexParent;
        std::size_t currentIndexNode;
        std::vector<TimeTreeNode<VALUE>> * innerTree;
        pointer _ptr;
        value_type lastItem;
    };
public:
    Iterator begin() { return Iterator(&tree); }
    Iterator end()   { return Iterator(&tree,true); }

    Iterator lower_bound(const std::chrono::milliseconds & point){
        auto p = getLeftPoint(point);
        return Iterator(&tree,p.treeIndex,p.nodeIndex);
    }

    Iterator upper_bound(const std::chrono::milliseconds & point){
        auto p = getRightPoint(point);
        return Iterator(&tree,p.treeIndex,p.nodeIndex);
    }

public:
    TimeTree(){
        intervalInSeconds = 3600s;
    }
    void add(const TimeTreeItem<VALUE> & item){
        if(tree.empty()){
            TimeTreeNode<VALUE> node;
            node.startPointMS = std::chrono::milliseconds(item.timestampMS);
            node.endPointMS = node.startPointMS + std::chrono::duration_cast<std::chrono::milliseconds>(intervalInSeconds);
            node.items.push_back(item);
            tree.push_back(node);
        }else{
            VECSIZE_T startIndex = findStartIndex(item);
            if(startIndex >= tree.size()){
                tree.resize(startIndex *2);
            }
            if ( tree[startIndex].items.empty() ){
                std::chrono::milliseconds startPointMS = tree[0].startPointMS + (std::chrono::duration_cast<std::chrono::milliseconds>(intervalInSeconds)* startIndex);
                tree[startIndex].startPointMS = startPointMS;
                tree[startIndex].endPointMS = startPointMS + std::chrono::duration_cast<std::chrono::milliseconds>(intervalInSeconds);
                tree[startIndex].items.push_back(item);

            }else {
                tree[startIndex].items.push_back(item);
            }

        }
    }
    TimeTreeItem<VALUE> findExact(std::chrono::milliseconds timepointMS){
        VECSIZE_T startIndex = findStartIndex(timepointMS);

        if(startIndex >= tree.size() ){
            throw std::invalid_argument("StartIndex > tree.size()");
        }
        auto & items = tree[startIndex].items;
        INDEX_T index = binsearch(items,0,items.size()-1,timepointMS);
        if(index == -1){
            throw std::invalid_argument("KEY doesn't exist: " + std::to_string(timepointMS.count()));
        }
        return items[index];
    }
    std::vector<TimeTreeItem<VALUE>> findInRange(std::chrono::milliseconds left,std::chrono::milliseconds right){
        auto pos = getIntervalRange(left,right);
        std::vector<TimeTreeItem<VALUE>> buffer;
        if(pos.first.treeIndex == pos.second.treeIndex){
            for(int j = pos.first.nodeIndex; j <= pos.second.nodeIndex; j++){
                buffer.push_back(tree[pos.first.treeIndex].items[j]);
            }
            return buffer;
        }
        bool firstIteration = true;
        for(INDEX_T i = pos.first.treeIndex; i <= pos.second.treeIndex; i++){
            if(tree[i].endPointMS <= right){
                INDEX_T startPosition = 0;
                if(firstIteration){
                    startPosition = pos.first.nodeIndex;
                }
                for(INDEX_T j = startPosition; std::cmp_less(j, tree[i].items.size()); j++){
                    buffer.push_back(tree[i].items[j]);
                }
            }else{
                for(INDEX_T j = 0; j <= pos.second.nodeIndex; j++){
                    buffer.push_back(tree[i].items[j]);
                }
            }

        }
        return buffer;

    }


    std::pair<TreeIndexPoint,TreeIndexPoint> getIntervalRange(std::chrono::milliseconds leftTimePointInMs, std::chrono::milliseconds rightTimePointInMs){
        return {
                getLeftPoint(leftTimePointInMs),
                getRightPoint(rightTimePointInMs)
        };
    }


private:
    INDEX_T findStartIndex(std::chrono::milliseconds timestampMS){
        INDEX_T x = std::floor( (double) (timestampMS.count() - tree[0].startPointMS.count())/(3600.0*1000.0) );
        return x;
    }
    int findStartIndex(const TimeTreeItem<VALUE> &item){
        return findStartIndex(item.timestampMS);
    }
private:
    //TODO: REMOVE RECURSION AND GENERALIZ BINSEARCH
    INDEX_T binsearch(const std::vector<TimeTreeItem<VALUE>> & items,INDEX_T start,INDEX_T end, std::chrono::milliseconds timePointMS){
        if(start > end) return -1;
        INDEX_T mid = (start+end)/2;
        if(items[mid].timestampMS == timePointMS){
            return mid;
        }
        if(items[mid].timestampMS < timePointMS){
            return binsearch(items,mid+1,end,timePointMS);
        }else{
            return binsearch(items,start,mid-1,timePointMS);
        }
    }
    TreeIndexPoint getLeftPoint(std::chrono::milliseconds timePointInMs){
        INDEX_T index = findStartIndex(timePointInMs);
        if(index < 0){
            return {0,0};
        }
        if(tree[index].items.size() == 1){
            return {index,0};
        }
        if(tree[index].startPointMS >= timePointInMs){
            return {index,0};
        }
        auto & items = tree[index].items;

        INDEX_T start = 0;
        INDEX_T end = items.size()-1;
        INDEX_T mid = (end+start)/2;
        while(mid != end){
            if(items[mid].timestampMS == timePointInMs ){
                return {index,mid};
            }

            if(items[mid].timestampMS < timePointInMs){
                start = ((end+start)/2)+1;
            }else{
                end = ((end+start)/2)-1;
            }
            mid = (end+start)/2;
        }
        return {index,start};
    }
    TreeIndexPoint getRightPoint(std::chrono::milliseconds timePointInMs){
        INDEX_T index = findStartIndex(timePointInMs);
        if(index < 0){
            return {0,0};
        }
        if(tree[index].items.size() == 1){
            return {index,0};
        }
        if(tree[index].startPointMS >= timePointInMs){
            return {index,0};
        }
        auto & items = tree[index].items;

        INDEX_T start = 0;
        INDEX_T end = items.size()-1;
        INDEX_T mid = (end+start)/2;
        while(mid != end){
            if(items[mid].timestampMS == timePointInMs ){
                return {index,mid};
            }

            if(items[mid].timestampMS < timePointInMs){
                start = ((end+start)/2)+1;
            }else{
                end = ((end+start)/2)-1;
            }
            mid = (end+start)/2;
        }
        return {index,start};
    }

private:
    std::chrono::seconds intervalInSeconds;
    std::vector<TimeTreeNode<VALUE>> tree;
};


#endif //DATACONTAINER_TIMETREE_H
