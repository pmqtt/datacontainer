#include "Storage.h"

void StorageNode::addField(const std::string & name,TYPE_KIND kind,const std::shared_ptr<FieldProperty> & prop){
    StorageField field;
    field.name = name;
    field.kind = kind;
    field.property = prop;
    fields[name] = field;
}


void StorageNode::insertRow(const std::vector<std::pair<std::string,DataType>> & row){
    if(row.size() != fields.size()){
        std::cout<<"-4:"<<row.size() << " | " <<fields.size()<<std::endl;
        exit(-4);
    }
    std::pair<std::string,DataType> p;
    std::vector<DataType> rest;
    for(auto & iter : row){
        StorageField field = fields[iter.first];
        if(field.property->is("key")){         
                p = iter;
        }else{
            rest.push_back(iter.second);
        }
    }
    datas[p.second] = rest;

}



void createD01Table(){
    Storage storage;
    StorageNode table = storage.addTable("D01");
    table.addField("UID",TYPE_KIND::STRING,std::make_shared<KeyField>());
    table.addField("DESTINATIONS",TYPE_KIND::STRING,std::make_shared<ListField>(';'));
    table.addField("LOOP",TYPE_KIND::INT,std::make_shared<SimpleField>());
    table.addField("ALGO",TYPE_KIND::STRING,std::make_shared<EnumField>(std::vector<std::string> { "PRIO","ANY" }));

    table.insertRow({
                        {"UID",DataType(new StringType("012345678"),TYPE_KIND::STRING)},
                        {"DESTINATIONS",DataType(new StringType("11511;11211"),TYPE_KIND::STRING)},
                        {"LOOP",DataType(new IntType(1),TYPE_KIND::INT)},
                        {"ALGO",DataType(new StringType("ANY"),TYPE_KIND::STRING)},
                    });

    auto values = table.getRow(DataType(new StringType("012345678"),TYPE_KIND::STRING));


    for(const auto & iter : values){
        iter.print();
        std::cout<<std::endl;
    }


}

