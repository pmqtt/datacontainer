#include "storage.h"
#include "../exceptions/storage_node_insertion_exception.h"

void storage_node::add_field(const std::string & name, TYPE_KIND kind, const std::shared_ptr<field_property> & prop){
    storage_field field;
    field.name = name;
    field.kind = kind;
    field.property = prop;
    fields[name] = field;
}

void storage_node::insert_row(const std::vector<std::pair<std::string,data_type>> & row){
    if(row.size() != fields.size()){
        throw storage_node_insertion_exception("Size of inserted row with " + std::to_string(row.size()) + " differ from expected value " + std::to_string(fields.size()));
    }
    std::pair<std::string,data_type> p;
    std::vector<data_type> rest;
    for(auto & iter : row){
        if(fields.count(iter.first) == 0){
        }else {
            storage_field field = fields[iter.first];
            if (field.property->is("key")) {
                p = iter;
            } else {
                rest.push_back(iter.second);
                fields[iter.first].position_in_data = rest.size() - 1;
            }
        }
    }
    datas[p.second] = rest;


}


chakra::base_catalog_item chakra::make_catalog_item(chakra::CATALOG_ITEM_TYPE item){
    switch(item){
        case CATALOG_ITEM_TYPE::CATALOG_LIST: {
            base_catalog_item i = chakra::list_container{};
            return i;
        }
        case CATALOG_ITEM_TYPE::CATALOG_KEY_VALUE: {
            base_catalog_item i = chakra::key_value_container{};
            return i;
        }
        case CATALOG_ITEM_TYPE::CATALOG_TIME_SERIE: {
            base_catalog_item i = chakra::time_series_container{};
            return i;
        }
        case CATALOG_ITEM_TYPE::CATALOG_TABLE: {
            base_catalog_item i = chakra::table_container{};
            return i;
        }
        default:
            break;
    }
    base_catalog_item i = chakra::key_value_container{};
    return i;
}


chakra::header_desc chakra::make_header(const std::vector<std::pair<std::string,base_storage_object>> & header){
    chakra::header_desc description;
    for(std::size_t i =0; i < header.size(); ++i){
        description[header[i].first] = std::pair<std::size_t,base_storage_object>(i,header[i].second);
    }
    return description;
}


#if 0

void createD01Table(){
    container container;
    std::shared_ptr<storage_node> table = container.addTable("D01");
    table->addField("UID",TYPE_KIND::STRING,std::make_shared<key_field>());
    table->addField("DESTINATIONS",TYPE_KIND::STRING,std::make_shared<list_field>(";"));
    table->addField("LOOP",TYPE_KIND::INT,std::make_shared<simple_field>());
    table->addField("ALGO",TYPE_KIND::STRING,std::make_shared<enum_field>(std::vector<std::string> { "PRIO","ANY" }));

    table->insertRow({
                        {"UID",data_type(new string_type("012345678"),TYPE_KIND::STRING)},
                        {"DESTINATIONS",data_type(new string_type("11511;11211"),TYPE_KIND::STRING)},
                        {"LOOP",data_type(new int_type(1),TYPE_KIND::INT)},
                        {"ALGO",data_type(new string_type("ANY"),TYPE_KIND::STRING)},
                    });

    auto values = table->getRow(data_type(new string_type("012345678"),TYPE_KIND::STRING));


    for(const auto & iter : values){
        iter.print();
        std::cout<<std::endl;
    }


}
#endif

