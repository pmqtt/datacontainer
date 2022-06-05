#define BOOST_TEST_DYN_LINK 1
#define BOOST_TEST_MODULE SegmentTreeTest

#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <string>
#include "../src/datastructure/table.h"
#include "../src/api/prime_stream.h"
#include "../src/api/time_measure.h"
#include "../src/storage/data_types.h"
#include "../src/datastructure/time_tree.h"
#include <algorithm>
#include <set>
#include <list>
#include <stdexcept>
#include <unordered_set>
#include <chrono>
#include <utility>

using namespace std;
using namespace ptl;

BOOST_AUTO_TEST_SUITE(TreeTimeSuite)

    BOOST_AUTO_TEST_CASE(ExactMatchTest) {
        time_tree<std::string> tree;
        time_tree_item<std::string> item;
        item.timestamp = 400ms;
        item.value = "HALLO";
        tree.add(item);
        time_tree_item i = tree.find_exact(400ms);
        BOOST_CHECK(i.value == item.value);
        item.timestamp = 401ms + std::chrono::milliseconds(3600 * 1000);
        item.value = "WELT";
        tree.add(item);
        i = tree.find_exact(401ms + std::chrono::milliseconds(3600 * 1000));
        BOOST_CHECK(i.value == item.value);
    }

    BOOST_AUTO_TEST_CASE(CHECK_MANY_KEYS_IN_ONE_NODE_TEST) {
        time_tree<std::string> tree;
        for (int i = 0; i < 200; i++) {
            time_tree_item<std::string> item;
            item.timestamp = 400ms + std::chrono::milliseconds(i);
            item.value = "HALLO";
            tree.add(item);
        }

        for (int i = 0; i < 200; i++) {
            time_tree_item<std::string> item;
            item.timestamp = 610ms + std::chrono::milliseconds(i);
            item.value = "HALLO";
            tree.add(item);
        }
        for (int i = 0; i < 200; i++) {
            time_tree_item<std::string> item = tree.find_exact(400ms + std::chrono::milliseconds(i));
            BOOST_CHECK(item.timestamp == 400ms + std::chrono::milliseconds(i));
        }

    }

    BOOST_AUTO_TEST_CASE(GetCorrectLeftIntervalTest) {
        time_tree<std::string> tree;
        for (int i = 0; i < 200; i++) {
            time_tree_item<std::string> item;
            item.timestamp = 400ms + std::chrono::milliseconds(i);
            item.value = "HALLO";
            tree.add(item);
        }

        for (int i = 0; i < 200; i++) {
            time_tree_item<std::string> item;
            item.timestamp = 610ms + std::chrono::milliseconds(i);
            item.value = "HALLO";
            tree.add(item);
        }
        auto x = tree.get_interval_range(500ms, 611ms);

        BOOST_CHECK(x.first.tree_index == 0);
        BOOST_CHECK_MESSAGE(x.first.node_index == 100, " Should x.first.node_index == 100 and not x.first.node_index == " +
                                                       std::to_string(x.first.node_index));
        x = tree.get_interval_range(100ms, 611ms);
        BOOST_CHECK_MESSAGE(x.first.node_index == 0, " Should x.first.node_index == 0 and not x.first.node_index == " +
                                                     std::to_string(x.first.node_index));
        x = tree.get_interval_range(500ms, 611ms);
        BOOST_CHECK_MESSAGE(x.second.node_index == 201,
                            " Should x.second.node_index == 201 and not x.first.node_index == " +
                            std::to_string(x.first.node_index));

        auto buffer = tree.find_in_range(500ms, 611ms);
        for (int i = 0; i < 100; i++) {

            BOOST_CHECK_MESSAGE(buffer[i].timestamp == 500ms + std::chrono::milliseconds(i),
                                "BUFFER[i]:= " + std::to_string(buffer[i].timestamp.count()));
        }
        BOOST_CHECK(buffer[100].timestamp == 610ms);
        BOOST_CHECK(buffer[101].timestamp == 611ms);

    }

#if 1

    BOOST_AUTO_TEST_CASE(MASSIVE_ADD_AND_SEARCH_TEST) {
        measure<100, std::chrono::milliseconds> m;

        m.method_to_compare("time_tree", []() {
            time_tree<std::string> tree;
            for (int i = 0; i < 10000; i++) {
                time_tree_item<std::string> item;
                item.timestamp = 400ms + std::chrono::milliseconds(i);
                item.value = "HALLO";
                tree.add(item);
            }
            for (int i = 360500; i < 1000000; i++) {
                time_tree_item<std::string> item;
                item.timestamp = std::chrono::milliseconds(i);
                item.value = "Welt";
                tree.add(item);
            }
            time_tree_item i = tree.find_exact(360500ms);
            BOOST_CHECK(i.timestamp == 360500ms);

            auto buffer = tree.find_in_range(9000ms, 500000ms);
            BOOST_CHECK(buffer.size() == 140901);
        });
        m.method_to_compare("std::set", []() {
            std::set<time_tree_item<std::string>> tree;
            for (int i = 0; i < 10000; i++) {
                time_tree_item<std::string> item;
                item.timestamp = 400ms + std::chrono::milliseconds(i);
                item.value = "HALLO";
                tree.insert(item);
            }
            for (int i = 360500; i < 1000000; i++) {
                time_tree_item<std::string> item;
                item.timestamp = std::chrono::milliseconds(i);
                item.value = "Welt";
                tree.insert(item);
            }
            time_tree_item<std::string> f;
            f.timestamp = 360500ms;

            auto i = tree.find(f);
            BOOST_CHECK(i->timestamp == 360500ms);

            time_tree_item<std::string> lb;
            time_tree_item<std::string> ub;
            lb.timestamp = 9000ms;
            ub.timestamp = 500000ms;

            auto start = tree.lower_bound(lb);
            auto end = tree.upper_bound(ub);
            std::vector<time_tree_item<std::string>> buffer;
            std::copy(start, end, std::back_inserter(buffer));
            BOOST_CHECK(buffer.size() == 140901);
        });

        std::cout << m << "\n";


    }

#endif

    BOOST_AUTO_TEST_CASE(FOR_EACH_EMPTY_TREE_TEST) {
        time_tree<std::string> tree;

        int k = 0;
        for (auto &x: tree) {
            UNUSED(x);
            k++;
        }
        BOOST_CHECK(k == 0);

    }

    BOOST_AUTO_TEST_CASE(FOR_EACH_ONE_NODE_TEST) {
        time_tree<std::string> tree;
        for (int i = 0; i < 200; i++) {
            time_tree_item<std::string> item;
            item.timestamp = 400ms + std::chrono::milliseconds(i);
            item.value = "HALLO";
            tree.add(item);
        }

        for (int i = 0; i < 200; i++) {
            time_tree_item<std::string> item;
            item.timestamp = 610ms + std::chrono::milliseconds(i);
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
        time_tree<std::string> tree;
        int k = 0;
        for (int i = 0; i < 10000; i++) {
            time_tree_item<std::string> item;
            item.timestamp = 400ms + std::chrono::milliseconds(i);
            item.value = std::to_string(item.timestamp.count()) + "ms";
            tree.add(item);
            k++;
        }
        for (int i = 360500; i < 1000000; i++) {
            time_tree_item<std::string> item;
            item.timestamp = std::chrono::milliseconds(i);
            item.value = std::to_string(item.timestamp.count()) + "ms";
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
        BOOST_CHECK_MESSAGE(buffer2.size() == 140900,"SHOULD 140900 is " + std::to_string(buffer2.size()));

    }

BOOST_AUTO_TEST_SUITE_END()