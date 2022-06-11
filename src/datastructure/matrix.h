//
// Created by cefour on 10.06.22.
//

#ifndef DATACONTAINER_MATRIX_H
#define DATACONTAINER_MATRIX_H

#include <vector>
namespace ptl {
    template<class T>
    using matrix = std::vector<std::vector<T>>;

    template<class T>
    std::vector<T> get_column(const matrix<T> & data,std::size_t col){
        std::vector<T> column;
        for(const auto & iter: data){
            column.push_back(iter[col]);
        }
        return column;
    }
}

#endif //DATACONTAINER_MATRIX_H
