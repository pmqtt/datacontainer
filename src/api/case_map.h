//
// Created by cefour on 13.06.22.
//

#ifndef DATACONTAINER_CASE_MAP_H
#define DATACONTAINER_CASE_MAP_H
#include <map>
namespace ptl{

    template<class KEY,class VALUE>
    class case_map{
    private:
        std::map<KEY,VALUE> switched_map;
        VALUE default_item;
    public:
        case_map() = default;
        ~case_map() = default;
        case_map(const std::initializer_list<std::pair<KEY,VALUE>> & list, const VALUE & default_item ){
            for(const auto & x : list){
                switched_map[x.first] = x.second;
            }

            this->default_item = default_item;
        }
        case_map & operator()(const KEY & key, const VALUE & value){
            switched_map[key] = value;
            return *this;
        }
        VALUE & operator[](const KEY & key){
            if(switched_map.count(key)> 0){
                return switched_map[key];
            }
            return default_item;
        }

    };

}

#endif //DATACONTAINER_CASE_MAP_H
