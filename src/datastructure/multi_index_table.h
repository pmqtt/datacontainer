//
// Created by cefour on 19.05.22.
//

#ifndef DATACONTAINER_MULTI_INDEX_TABLE_H
#define DATACONTAINER_MULTI_INDEX_TABLE_H

#include <map>
#include <memory>
#include <string>
#include <tuple>


#include "index.h"
#include "hash_map.h"
#include "../storage/data_types.h"




struct table_col{
    std::string column_name;
    data_type value;
};
using header_row = std::pair<std::string,TYPE_KIND>;
using table_item = std::vector<table_col>;


class multi_index_table{
public:
    multi_index_table();
    ~multi_index_table();
public:
    void create_header(const header_row & row, ...);
    void create_index(const std::string col_name, const std::shared_ptr<index> & idx );
public:
    void insert_row();
public:
    std::vector<table_item> find(const std::string & col_name, const data_type & find  );

private:
    std::map<std::string,TYPE_KIND> header;
};

/*
 *
 * int foo(){
 * //CREATE DATAS [ID,TEMPERATURE,MESSAGE,TIME]
 *  hash_map<data_type,std::vector<data_type>> datas;
 *
 *  multi_index_table mTable;
 *  mTable.insert_header_description({"ID",TYPE_KIND::INT},{"TEMPERATURE",TYPE_KIND::STRING},{"MESSAGE",TYPE_KIND::STRING},{"TIME",TYPE_KIND::string})
 *  mTable.createIndex("ID",UNIQUE_INDEX);
 *  mTable.create_index("TIME",DATETIME_INDEX);
 *  mTable.create_index("TEMPERATURE",INTERVALL_INDEX);
 *
 *   auto value = mTable.find("ID",12);
 *   auto times = mTable.find("Time","12-05-12 13:00:44")
 *
 *
 * }
 *
 *
 */

#endif //DATACONTAINER_MULTI_INDEX_TABLE_H
