//
// Created by cefour on 19.05.22.
//

#ifndef DATACONTAINER_MULTIINDEXTABLE_H
#define DATACONTAINER_MULTIINDEXTABLE_H

#include <map>
#include <memory>
#include <string>
#include <tuple>


#include "Index.h"
#include "Table.h"
#include "../storage/DataTypes.h"




struct TableCol{
    std::string columnName;
    DataType value;
};
using HeaderRow = std::pair<std::string,TYPE_KIND>;
using TableItem = std::vector<TableCol>;


class MultiIndexTable{
public:
    MultiIndexTable();
    ~MultiIndexTable();
public:
    void createHeader(const HeaderRow & row,...);
    void createIndex(const std::string colName,const std::shared_ptr<Index> & index );
public:
    void insertRow();
public:
    std::vector<TableItem> find(const std::string & colName, const DataType & find  );

private:
    std::map<std::string,TYPE_KIND> header;
};

/*
 *
 * int foo(){
 * //CREATE DATAS [ID,TEMPERATURE,MESSAGE,TIME]
 *  Table<DataType,std::vector<DataType>> datas;
 *
 *  MultiIndexTable mTable;
 *  mTable.createHeader({"ID",TYPE_KIND::INT},{"TEMPERATURE",TYPE_KIND::STRING},{"MESSAGE",TYPE_KIND::STRING},{"TIME",TYPE_KIND::string})
 *  mTable.createIndex("ID",UNIQUE_INDEX);
 *  mTable.createIndex("TIME",DATETIME_INDEX);
 *  mTable.createIndex("TEMPERATURE",INTERVALL_INDEX);
 *
 *   auto value = mTable.find("ID",12);
 *   auto times = mTable.find("Time","12-05-12 13:00:44")
 *
 *
 * }
 *
 *
 */

#endif //DATACONTAINER_MULTIINDEXTABLE_H
