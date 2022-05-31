#define BOOST_TEST_DYN_LINK 1
#define BOOST_TEST_MODULE SegmentTreeTest

#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <string>
#include "../src/datastructure/Table.h"
#include "../src/api/PrimeStream.h"
#include "../src/api/TimeMeasure.h"
#include "../src/storage/DataTypes.h"
#include "../src/datastructure/TimeTree.h"
#include <algorithm>
#include <set>
#include <list>
#include <stdexcept>
#include <unordered_set>
#include <chrono>
#include <utility>

using namespace std;


BOOST_AUTO_TEST_SUITE(TreeTimeSuite)

    BOOST_AUTO_TEST_CASE(ExactMatchTest) {
        TimeTree<std::string> tree;
        TimeTreeItem<std::string> item;
        item.timestampMS = 400ms;
        item.value = "HALLO";
        tree.add(item);
        TimeTreeItem i = tree.findExact(400ms);
        BOOST_CHECK(i.value == item.value);
        item.timestampMS = 401ms + std::chrono::milliseconds(3600 * 1000);
        item.value = "WELT";
        tree.add(item);
        i = tree.findExact(401ms + std::chrono::milliseconds(3600 * 1000));
        BOOST_CHECK(i.value == item.value);
    }

    BOOST_AUTO_TEST_CASE(CHECK_MANY_KEYS_IN_ONE_NODE_TEST) {
        TimeTree<std::string> tree;
        for (int i = 0; i < 200; i++) {
            TimeTreeItem<std::string> item;
            item.timestampMS = 400ms + std::chrono::milliseconds(i);
            item.value = "HALLO";
            tree.add(item);
        }

        for (int i = 0; i < 200; i++) {
            TimeTreeItem<std::string> item;
            item.timestampMS = 610ms + std::chrono::milliseconds(i);
            item.value = "HALLO";
            tree.add(item);
        }
        for (int i = 0; i < 200; i++) {
            TimeTreeItem<std::string> item = tree.findExact(400ms + std::chrono::milliseconds(i));
            BOOST_CHECK(item.timestampMS == 400ms + std::chrono::milliseconds(i));
        }

    }

    BOOST_AUTO_TEST_CASE(GetCorrectLeftIntervalTest) {
        TimeTree<std::string> tree;
        for (int i = 0; i < 200; i++) {
            TimeTreeItem<std::string> item;
            item.timestampMS = 400ms + std::chrono::milliseconds(i);
            item.value = "HALLO";
            tree.add(item);
        }

        for (int i = 0; i < 200; i++) {
            TimeTreeItem<std::string> item;
            item.timestampMS = 610ms + std::chrono::milliseconds(i);
            item.value = "HALLO";
            tree.add(item);
        }
        auto x = tree.getIntervalRange(500ms, 611ms);

        BOOST_CHECK(x.first.treeIndex == 0);
        BOOST_CHECK_MESSAGE(x.first.nodeIndex == 100, " Should x.first.nodeIndex == 100 and not x.first.nodeIndex == " +
                                                      std::to_string(x.first.nodeIndex));
        x = tree.getIntervalRange(100ms, 611ms);
        BOOST_CHECK_MESSAGE(x.first.nodeIndex == 0, " Should x.first.nodeIndex == 0 and not x.first.nodeIndex == " +
                                                    std::to_string(x.first.nodeIndex));
        x = tree.getIntervalRange(500ms, 611ms);
        BOOST_CHECK_MESSAGE(x.second.nodeIndex == 201,
                            " Should x.second.nodeIndex == 201 and not x.first.nodeIndex == " +
                            std::to_string(x.first.nodeIndex));

        auto buffer = tree.findInRange(500ms, 611ms);
        for (int i = 0; i < 100; i++) {

            BOOST_CHECK_MESSAGE(buffer[i].timestampMS == 500ms + std::chrono::milliseconds(i),
                                "BUFFER[i]:= " + std::to_string(buffer[i].timestampMS.count()));
        }
        BOOST_CHECK(buffer[100].timestampMS == 610ms);
        BOOST_CHECK(buffer[101].timestampMS == 611ms);

    }

#if 1

    BOOST_AUTO_TEST_CASE(MASSIVE_ADD_AND_SEARCH_TEST) {
        measure<100, std::chrono::milliseconds> m;

        m.method_to_compare("TimeTree", []() {
            TimeTree<std::string> tree;
            for (int i = 0; i < 10000; i++) {
                TimeTreeItem<std::string> item;
                item.timestampMS = 400ms + std::chrono::milliseconds(i);
                item.value = "HALLO";
                tree.add(item);
            }
            for (int i = 360500; i < 1000000; i++) {
                TimeTreeItem<std::string> item;
                item.timestampMS = std::chrono::milliseconds(i);
                item.value = "Welt";
                tree.add(item);
            }
            TimeTreeItem i = tree.findExact(360500ms);
            BOOST_CHECK(i.timestampMS == 360500ms);

            auto buffer = tree.findInRange(9000ms, 500000ms);
            BOOST_CHECK(buffer.size() == 140901);
        });
        m.method_to_compare("std::set", []() {
            std::set<TimeTreeItem<std::string>> tree;
            for (int i = 0; i < 10000; i++) {
                TimeTreeItem<std::string> item;
                item.timestampMS = 400ms + std::chrono::milliseconds(i);
                item.value = "HALLO";
                tree.insert(item);
            }
            for (int i = 360500; i < 1000000; i++) {
                TimeTreeItem<std::string> item;
                item.timestampMS = std::chrono::milliseconds(i);
                item.value = "Welt";
                tree.insert(item);
            }
            TimeTreeItem<std::string> f;
            f.timestampMS = 360500ms;

            auto i = tree.find(f);
            BOOST_CHECK(i->timestampMS == 360500ms);

            TimeTreeItem<std::string> lb;
            TimeTreeItem<std::string> ub;
            lb.timestampMS = 9000ms;
            ub.timestampMS = 500000ms;

            auto start = tree.lower_bound(lb);
            auto end = tree.upper_bound(ub);
            std::vector<TimeTreeItem<std::string>> buffer;
            std::copy(start, end, std::back_inserter(buffer));
            BOOST_CHECK(buffer.size() == 140901);
        });

        std::cout << m << "\n";


    }

#endif

    BOOST_AUTO_TEST_CASE(FOR_EACH_EMPTY_TREE_TEST) {
        TimeTree<std::string> tree;

        int k = 0;
        for (auto &x: tree) {
            UNUSED(x);
            k++;
        }
        BOOST_CHECK(k == 0);

    }

    BOOST_AUTO_TEST_CASE(FOR_EACH_ONE_NODE_TEST) {
        TimeTree<std::string> tree;
        for (int i = 0; i < 200; i++) {
            TimeTreeItem<std::string> item;
            item.timestampMS = 400ms + std::chrono::milliseconds(i);
            item.value = "HALLO";
            tree.add(item);
        }

        for (int i = 0; i < 200; i++) {
            TimeTreeItem<std::string> item;
            item.timestampMS = 610ms + std::chrono::milliseconds(i);
            item.value = "HALLO";
            tree.add(item);
        }
        int k = 0;
        for (auto &x: tree) {
            UNUSED(x);
            k++;
        }
        BOOST_CHECK_MESSAGE(k == 400, "SHOULD 400 is " + std::to_string(k));

    }

    BOOST_AUTO_TEST_CASE(FOR_EACH_MORE_NODE_TEST) {
        TimeTree<std::string> tree;
        int k = 0;
        for (int i = 0; i < 10000; i++) {
            TimeTreeItem<std::string> item;
            item.timestampMS = 400ms + std::chrono::milliseconds(i);
            item.value = std::to_string(item.timestampMS.count()) + "ms";
            tree.add(item);
            k++;
        }
        for (int i = 360500; i < 1000000; i++) {
            TimeTreeItem<std::string> item;
            item.timestampMS = std::chrono::milliseconds(i);
            item.value = std::to_string(item.timestampMS.count()) + "ms";
            tree.add(item);
            k++;
        }
        std::vector<std::string> buffer;
        std::copy(tree.begin(), tree.end(), std::back_inserter(buffer));
        BOOST_CHECK_MESSAGE(std::cmp_equal(buffer.size(), k),
                            "k:" + std::to_string(k) + " size():= " + std::to_string(buffer.size()));



        auto start = tree.lower_bound(9000ms);
        auto end = tree.upper_bound(500000ms);

        std::vector<std::string> buffer2;
        std::copy(start, end, std::back_inserter(buffer2));
        BOOST_CHECK(buffer2.size() == 140901);

    }

BOOST_AUTO_TEST_SUITE_END()