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


#include <bits/stdc++.h>
using namespace std;

// limit for array size
const int N = 100000;

int n; // array size


struct SegmentNode{
    int min;
    int max;
    int value;
    bool isLeaf;
};

// Max size of tree
SegmentNode tree[2 * N];

// function to build the tree
void build( int arr[])
{
    // insert leaf nodes in tree
    for (int i=0; i<n; i++) {
        tree[n + i].value = arr[i];
        tree[n +i ].isLeaf = true;;
    }

    // build the tree by calculating parents
    for (int i = n - 1; i > 0; --i) {
        tree[i].min = tree[2*i].min;
        tree[i].max = tree[2*i+1].max;
        tree[i].isLeaf = false;
    }
}

// function to update a tree node
/*void updateTreeNode(int p, int value)
{
    // set value at position p
    tree[p+n] = value;
    p = p+n;

    // move upward and update parents
    for (int i=p; i > 1; i >>= 1)
        tree[i>>1] = tree[i] + tree[i^1];
}*/

// function to get sum on interval [l, r)
std::vector<int> query(int l, int r)
{
    std::vector<int> res;

    // loop to find the sum in the range
    for (l += n, r += n; l < r; l >>= 1, r >>= 1)
    {
        if (l&1) {
            if(tree[l].isLeaf)
                res.push_back(tree[l++].value);
        }

        //std::cout<<"================================================="<<std::endl;
        if (r&1) {
            if(tree[r-1].isLeaf)
                res.push_back(tree[--r].value);
        }
    }

    return res;
}

BOOST_AUTO_TEST_SUITE(SegmentTreeTestSuite)

    BOOST_AUTO_TEST_CASE(SegmentTree){
        std::set<int> s={1,2,3,4,5,6,7,8,9,10,11,12};

        auto start = s.lower_bound(4);
        auto end = s.upper_bound(11);

        for(auto & x = start; x != end; x++){
            std::cout<<*x<<std::endl;
        }


    }

BOOST_AUTO_TEST_SUITE_END()