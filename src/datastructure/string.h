//
// Created by cefour on 03.06.22.
//

#ifndef DATACONTAINER_STRING_H
#define DATACONTAINER_STRING_H
#include <cstdlib>
namespace ptl{
    class string{
    public:
        struct iterator{
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = char;
            using pointer = char *;  // or also value_type*
            using reference = char &;  // or also value_type&

            iterator(char * data) : _ptr(data) {
                if(data != nullptr){
                    current_index = 0;
                }else{
                    current_index = -1;
                }
            }
            reference operator*() const { return (_ptr == nullptr) ?  *_ptr : _ptr[current_index]; }

            pointer operator->() { return (_ptr == nullptr) ?  _ptr : &_ptr[current_index];; }

            // Prefix increment
            iterator &operator++() {
                if(_ptr == nullptr) return *this;
                current_index++;
                if(_ptr[current_index] != '\0'){
                    return *this;
                }
                current_index = -1;
                _ptr = nullptr;
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
            pointer _ptr;
            std::size_t current_index;
        };
    public:
        iterator begin(){
            return iterator(data);
        }
        iterator end(){
            return iterator(nullptr);
        }
    public:
        string() : data(nullptr){}
        string(const char * null_terminated_string){
            auto size = ::strlen(null_terminated_string);
            data = new char[size];
        }
        ~string(){
            if(data != nullptr) {
                delete[] data;
                data = nullptr;
            }
        }


    private:
        char *data;
    };

}

#endif //DATACONTAINER_STRING_H
