//
// Created by cefour on 30.05.22.
//

#ifndef DATACONTAINER_TIME_TREE_H
#define DATACONTAINER_TIME_TREE_H


#include "../api/macro_helper.h"

#include <chrono>
#include <cmath>
#include <tuple>
#include <utility>
#include <vector>

namespace ptl {
    using namespace std::chrono_literals;

    using INDEX_T = long long int;

    struct tree_index_point {
        INDEX_T tree_index;
        INDEX_T node_index;
    };

    template<class VALUE>
    struct time_tree_item {
        std::chrono::milliseconds timestamp;
        VALUE value;

        bool operator<(const time_tree_item &rhs) const {
            return timestamp < rhs.timestamp;
        }

        bool operator<=(const time_tree_item &rhs) const {
            return timestamp <= rhs.timestamp;
        }

        bool operator==(const time_tree_item &rhs) const {
            return timestamp <= rhs.timestamp;
        }
    };

    template<class VALUE>
    struct time_tree_node {
        std::chrono::milliseconds start_point;
        std::chrono::milliseconds end_point;
        std::vector<time_tree_item<VALUE>> items;
    };

    using VECSIZE_T = std::vector<long>::size_type;

    template<class VALUE>
    class time_tree {
        struct iterator {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = VALUE;
            using pointer = VALUE *;  // or also value_type*
            using reference = VALUE &;  // or also value_type&


            iterator(std::vector<time_tree_node<VALUE>> *tree, std::size_t parent_index, std::size_t child_index)
                    : inner_tree(tree) {
                current_index_parent = parent_index;
                current_index_node = child_index;
                _ptr = &tree->at(parent_index).items[child_index].value;

            }

            iterator(std::vector<time_tree_node<VALUE>> *tree) : inner_tree(tree) {
                current_index_parent = 0;
                current_index_node = 0;
                if (!tree->empty() && !tree->at(0).items.empty()) {
                    _ptr = &tree->at(0).items[0].value;
                } else {
                    _ptr = nullptr;
                }
            }

            iterator(std::vector<time_tree_node<VALUE>> *tree, bool end) : inner_tree(tree) {
                UNUSED(end);
                if (!tree->empty() && !tree->at(0).items.empty()) {
                    current_index_parent = inner_tree->size();
                    current_index_node = inner_tree->at(current_index_parent - 1).items.size();
                }
                _ptr = nullptr;
            }

            reference operator*() const { return *_ptr; }

            pointer operator->() { return _ptr; }

            // Prefix increment
            iterator &operator++() {
                if (_ptr == nullptr) {
                    return *this;
                }
                if (current_index_node < (inner_tree->at(current_index_parent).items.size() - 1)) {
                    current_index_node++;
                    _ptr = &inner_tree->at(current_index_parent).items[current_index_node].value;
                } else {
                    if (current_index_parent < inner_tree->size() - 1) {
                        current_index_parent++;
                        current_index_node = 0;
                        if (current_index_node < inner_tree->at(current_index_parent).items.size()) {
                            _ptr = &inner_tree->at(current_index_parent).items[current_index_node].value;
                        } else {
                            _ptr = nullptr;
                        }
                    } else {
                        _ptr = nullptr;
                    }
                }
                return *this;
            }

            // Postfix increment
            iterator operator++(int) {
                iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            friend bool operator==(const iterator &a, const iterator &b) { return a._ptr == b._ptr; };

            friend bool operator!=(const iterator &a, const iterator &b) { return a._ptr != b._ptr; };


        private:
            std::size_t current_index_parent;
            std::size_t current_index_node;
            std::vector<time_tree_node<VALUE>> *inner_tree;
            pointer _ptr;
        };

    public:
        iterator begin() { return iterator(&tree); }

        iterator end() { return iterator(&tree, true); }

        iterator lower_bound(const std::chrono::milliseconds &point) {
            auto p = get_left_point(point);
            return iterator(&tree, p.tree_index, p.node_index);
        }

        iterator upper_bound(const std::chrono::milliseconds &point) {
            auto p = get_right_point(point);
            return iterator(&tree, p.tree_index, p.node_index);
        }

    public:
        time_tree() {
            interval_width = 3600s;
        }

        void add(const time_tree_item<VALUE> &item) {
            if (tree.empty()) {
                time_tree_node<VALUE> node;
                node.start_point = std::chrono::milliseconds(item.timestamp);
                node.end_point =
                        node.start_point + std::chrono::duration_cast<std::chrono::milliseconds>(interval_width);
                node.items.push_back(item);
                tree.push_back(node);
            } else {
                VECSIZE_T start_index = find_start_index(item);
                if (start_index >= tree.size()) {
                    tree.resize(start_index * 2);
                }
                if (tree[start_index].items.empty()) {
                    std::chrono::milliseconds startPointMS = tree[0].start_point +
                                                             (std::chrono::duration_cast<std::chrono::milliseconds>(
                                                                     interval_width) * start_index);
                    tree[start_index].start_point = startPointMS;
                    tree[start_index].end_point =
                            startPointMS + std::chrono::duration_cast<std::chrono::milliseconds>(interval_width);
                    tree[start_index].items.push_back(item);

                } else {
                    tree[start_index].items.push_back(item);
                }

            }
        }

        time_tree_item<VALUE> find_exact(std::chrono::milliseconds timepoint) {
            VECSIZE_T startIndex = find_start_index(timepoint);

            if (startIndex >= tree.size()) {
                throw std::invalid_argument("StartIndex > tree.size()");
            }
            auto &items = tree[startIndex].items;
            INDEX_T index = binsearch(items, 0, items.size() - 1, timepoint);
            if (index == -1) {
                throw std::invalid_argument("KEY doesn't exist: " + std::to_string(timepoint.count()));
            }
            return items[index];
        }

        std::vector<time_tree_item<VALUE>>
        find_in_range(std::chrono::milliseconds left, std::chrono::milliseconds right) {
            auto pos = get_interval_range(left, right);
            std::vector<time_tree_item<VALUE>> buffer;
            if (pos.first.tree_index == pos.second.tree_index) {
                for (int j = pos.first.node_index; j <= pos.second.node_index; j++) {
                    buffer.push_back(tree[pos.first.tree_index].items[j]);
                }
                return buffer;
            }
            bool first_iteration = true;
            for (INDEX_T i = pos.first.tree_index; i <= pos.second.tree_index; i++) {
                if (tree[i].end_point <= right) {
                    INDEX_T start_position = 0;
                    if (first_iteration) {
                        start_position = pos.first.node_index;
                    }
                    for (INDEX_T j = start_position; std::cmp_less(j, tree[i].items.size()); j++) {
                        buffer.push_back(tree[i].items[j]);
                    }
                } else {
                    for (INDEX_T j = 0; j <= pos.second.node_index; j++) {
                        buffer.push_back(tree[i].items[j]);
                    }
                }

            }
            return buffer;

        }


        std::pair<tree_index_point, tree_index_point>
        get_interval_range(std::chrono::milliseconds left, std::chrono::milliseconds reight) {
            return {
                    get_left_point(left),
                    get_right_point(reight)
            };
        }


    private:
        INDEX_T find_start_index(std::chrono::milliseconds timestamp) {
            INDEX_T x = std::floor((double) (timestamp.count() - tree[0].start_point.count()) / (3600.0 * 1000.0));
            return x;
        }

        int find_start_index(const time_tree_item<VALUE> &item) {
            return find_start_index(item.timestamp);
        }

    private:
        //TODO: REMOVE RECURSION AND GENERALIZ BINSEARCH
        INDEX_T binsearch(const std::vector<time_tree_item<VALUE>> &items, INDEX_T start, INDEX_T end,
                          std::chrono::milliseconds timepoint) {
            if (start > end) return -1;
            INDEX_T mid = (start + end) / 2;
            if (items[mid].timestamp == timepoint) {
                return mid;
            }
            if (items[mid].timestamp < timepoint) {
                return binsearch(items, mid + 1, end, timepoint);
            } else {
                return binsearch(items, start, mid - 1, timepoint);
            }
        }

        tree_index_point get_left_point(std::chrono::milliseconds timepoint) {
            INDEX_T index = find_start_index(timepoint);
            if (index < 0) {
                return {0, 0};
            }
            if (tree[index].items.size() == 1) {
                return {index, 0};
            }
            if (tree[index].start_point >= timepoint) {
                return {index, 0};
            }
            auto &items = tree[index].items;

            INDEX_T start = 0;
            INDEX_T end = items.size() - 1;
            INDEX_T mid = (end + start) / 2;
            while (mid != end) {
                if (items[mid].timestamp == timepoint) {
                    return {index, mid};
                }

                if (items[mid].timestamp < timepoint) {
                    start = ((end + start) / 2) + 1;
                } else {
                    end = ((end + start) / 2) - 1;
                }
                mid = (end + start) / 2;
            }
            return {index, start};
        }

        tree_index_point get_right_point(std::chrono::milliseconds timepoint) {
            INDEX_T index = find_start_index(timepoint);
            if (index < 0) {
                return {0, 0};
            }
            if (tree[index].items.size() == 1) {
                return {index, 0};
            }
            if (tree[index].start_point >= timepoint) {
                return {index, 0};
            }
            auto &items = tree[index].items;

            INDEX_T start = 0;
            INDEX_T end = items.size() - 1;
            INDEX_T mid = (end + start) / 2;
            while (mid != end) {
                if (items[mid].timestamp == timepoint) {
                    return {index, mid};
                }

                if (items[mid].timestamp < timepoint) {
                    start = ((end + start) / 2) + 1;
                } else {
                    end = ((end + start) / 2) - 1;
                }
                mid = (end + start) / 2;
            }
            return {index, start};
        }

    private:
        std::chrono::seconds interval_width;
        std::vector<time_tree_node<VALUE>> tree;
    };
}

#endif //DATACONTAINER_TIME_TREE_H
