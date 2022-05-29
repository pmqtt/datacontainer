#define BOOST_TEST_DYN_LINK 1
#define BOOST_TEST_MODULE SegmentTreeTest
#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <string>
#include "../src/datastructure/Table.h"
#include "../src/api/PrimeStream.h"
#include "../src/api/TimeMeasure.h"
#include "../src/storage/DataTypes.h"
#include <algorithm>
#include <set>
#include <list>
#include <stdexcept>
#include <unordered_set>
#include <chrono>


#include <bits/stdc++.h>
using namespace std;

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
                TimeTreeNode<VALUE> node;
                tree[startIndex].startPointMS = startPointMS;
                tree[startIndex].endPointMS = node.startPointMS + std::chrono::duration_cast<std::chrono::milliseconds>(intervalInSeconds);
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
                int startPosition = 0;
                if(firstIteration){
                    startPosition = pos.first.nodeIndex;
                }
                for(INDEX_T j = startPosition; j < tree[i].items.size(); j++){
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

        bool found = false;
        while(!found){
            int mid = (end+start)/2;
            if(mid == end){
                return {index,start};
            }
            if(items[mid].timestampMS == timePointInMs ){
                return {index,mid};
            }

            if(items[mid].timestampMS < timePointInMs){
                start = ((end+start)/2)+1;
            }else{
                end = ((end+start)/2)-1;
            }

        }
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

        bool found = false;
        while(!found){
            INDEX_T mid = (end+start)/2;
            if(mid == end){
                return {index,start};
            }
            if(items[mid].timestampMS == timePointInMs ){
                return {index,mid};
            }

            if(items[mid].timestampMS < timePointInMs){
                start = ((end+start)/2)+1;
            }else{
                end = ((end+start)/2)-1;
            }

        }
    }

private:
    std::chrono::seconds intervalInSeconds;
    std::vector<TimeTreeNode<VALUE>> tree;
};



BOOST_AUTO_TEST_SUITE(TreeTimeSuite)

    BOOST_AUTO_TEST_CASE(ExactMatchTest){
        TimeTree<std::string> tree;
        TimeTreeItem<std::string> item;
        item.timestampMS = 400ms;
        item.value = "HALLO";
        tree.add(item);
        TimeTreeItem i = tree.findExact(400ms);
        BOOST_CHECK(i.value == item.value);
        item.timestampMS = 401ms + std::chrono::milliseconds(3600*1000);
        item.value = "WELT";
        tree.add(item);
        i = tree.findExact(401ms + std::chrono::milliseconds(3600*1000));
        BOOST_CHECK(i.value == item.value);
    }

    BOOST_AUTO_TEST_CASE(CHECK_MANY_KEYS_IN_ONE_NODE_TEST){
        TimeTree<std::string> tree;
        for(int i =0; i < 200;i++) {
            TimeTreeItem<std::string> item;
            item.timestampMS = 400ms+ std::chrono::milliseconds(i);
            item.value = "HALLO";
            tree.add(item);
        }

        for(int i =0; i < 200;i++) {
            TimeTreeItem<std::string> item;
            item.timestampMS = 610ms+std::chrono::milliseconds(i);
            item.value = "HALLO";
            tree.add(item);
        }
        for(int i = 0; i < 200; i++) {
            TimeTreeItem<std::string> item = tree.findExact(400ms+std::chrono::milliseconds(i));
            BOOST_CHECK(item.timestampMS == 400ms+std::chrono::milliseconds(i));
        }

    }

    BOOST_AUTO_TEST_CASE(GetCorrectLeftIntervalTest){
        TimeTree<std::string> tree;
        for(int i =0; i < 200;i++) {
            TimeTreeItem<std::string> item;
            item.timestampMS = 400ms+std::chrono::milliseconds(i);
            item.value = "HALLO";
            tree.add(item);
        }

        for(int i =0; i < 200;i++) {
            TimeTreeItem<std::string> item;
            item.timestampMS = 610ms+std::chrono::milliseconds(i);
            item.value = "HALLO";
            tree.add(item);
        }
        auto x = tree.getIntervalRange(500ms,611ms);

        BOOST_CHECK(x.first.treeIndex == 0);
        BOOST_CHECK_MESSAGE(x.first.nodeIndex == 100, " Should x.first.nodeIndex == 100 and not x.first.nodeIndex == " + std::to_string(x.first.nodeIndex));
        x = tree.getIntervalRange(100ms,611ms);
        BOOST_CHECK_MESSAGE(x.first.nodeIndex == 0, " Should x.first.nodeIndex == 0 and not x.first.nodeIndex == " + std::to_string(x.first.nodeIndex));
        x = tree.getIntervalRange(500ms,611ms);
        BOOST_CHECK_MESSAGE(x.second.nodeIndex == 201, " Should x.second.nodeIndex == 201 and not x.first.nodeIndex == " + std::to_string(x.first.nodeIndex));

        auto buffer = tree.findInRange(500ms,611ms);
        for(int i = 0; i < 100; i++){

            BOOST_CHECK_MESSAGE(buffer[i].timestampMS == 500ms+std::chrono::milliseconds(i),"BUFFER[i]:= "+ std::to_string(buffer[i].timestampMS.count()));
        }
        BOOST_CHECK(buffer[100].timestampMS == 610ms);
        BOOST_CHECK(buffer[101].timestampMS == 611ms);

    }

    BOOST_AUTO_TEST_CASE(MASSIVE_ADD_AND_SEARCH_TEST) {
        measure<100,std::chrono::milliseconds> m;

        m.method_to_compare("TimeTree",[](){
            TimeTree<std::string> tree;
            for (int i = 0; i < 10000; i++) {
                TimeTreeItem<std::string> item;
                item.timestampMS = 400ms + std::chrono::milliseconds(i);
                item.value = "HALLO";
                tree.add(item);
            }
            for(int i = 360500; i < 1000000; i++ ){
                TimeTreeItem<std::string> item;
                item.timestampMS = std::chrono::milliseconds(i);
                item.value = "Welt";
                tree.add(item);
            }
            TimeTreeItem i = tree.findExact(360500ms);
            BOOST_CHECK(i.timestampMS == 360500ms);

            auto buffer = tree.findInRange(9000ms,500000ms);
            BOOST_CHECK(buffer.size()==140901) ;
        });
        m.method_to_compare("std::set",[](){
            std::set<TimeTreeItem<std::string>> tree;
            for (int i = 0; i < 10000; i++) {
                TimeTreeItem<std::string> item;
                item.timestampMS = 400ms + std::chrono::milliseconds(i);
                item.value = "HALLO";
                tree.insert(item);
            }
            for(int i = 360500; i < 1000000; i++ ){
                TimeTreeItem<std::string> item;
                item.timestampMS = std::chrono::milliseconds(i);
                item.value = "Welt";
                tree.insert(item);
            }
            TimeTreeItem<std::string> f;
            f.timestampMS =360500ms;

            auto i = tree.find(f);
            BOOST_CHECK(i->timestampMS == 360500ms);

            TimeTreeItem<std::string> lb;
            TimeTreeItem<std::string> ub;
            lb.timestampMS =9000ms;
            ub.timestampMS =500000ms;

            auto start = tree.lower_bound(lb);
            auto end = tree.upper_bound(ub);
            std::vector<TimeTreeItem<std::string>> buffer;
            std::copy(start,end,std::back_inserter(buffer));
            BOOST_CHECK(buffer.size()==140901) ;
        });

        std::cout<<m<<"\n";



    }

BOOST_AUTO_TEST_SUITE_END()