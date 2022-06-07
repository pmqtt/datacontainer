//
// Created by cefour on 02.06.22.
//

#define BOOST_TEST_DYN_LINK 1
#define BOOST_TEST_MODULE STORAGE_TEST_MODULE
#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <string>
#include "../src/datastructure/hash_map.h"
#include "../src/api/prime_stream.h"
#include "../src/api/time_measure.h"
#include "../src/storage/data_types.h"
#include "../src/storage/storage.h"
#include <algorithm>

BOOST_AUTO_TEST_SUITE(StorageTestSuite)

BOOST_AUTO_TEST_CASE(CONSTRUCTOR_TEST){
      chakra::storage_manager db;
      db.add_catalog_entry("test1",chakra::CATALOG_ITEM_TYPE::CATALOG_KEY_VALUE);
      chakra::storage_table st_table = db.get_table("test1");
      auto item = st_table.get_inner_table();

      std::visit([&](auto & entry){
          using T = std::decay_t<decltype(entry)>;
          if constexpr (std::is_same<T,chakra::key_value_container>::value){
              BOOST_CHECK(true);
              chakra::key_value_container & tbl = entry;
              storage_string s ("first");
              tbl[s].emplace_back(storage_string("1"));

              BOOST_CHECK(tbl[s][0] == storage_string("1") );
          }else{
              BOOST_CHECK(false);
          }

      },*item);
}

BOOST_AUTO_TEST_CASE(CHACKRA_TABLE_TEST){
    chakra::storage_manager db;
    db.add_catalog_entry("test1",chakra::CATALOG_ITEM_TYPE::CATALOG_KEY_VALUE);
    chakra::storage_table st_table = db.get_table("test1");
    auto header = chakra::make_header({ {"id",storage_int(0)},{"timestamp",storage_string("")},{"temperature",storage_real()},{"message",storage_string("")}   });
    st_table.create_header(header);

    std::vector<std::pair<std::string,base_storage_object>> v = { {"id",storage_int(0)},{"timestamp",storage_string("02-06-2022 21:21:22")},{"temperature",storage_real(12.3)},{"message",storage_string("no message")}   };
    st_table.insert(v);
    std::vector<base_storage_object> res = st_table.find(storage_int(0));
    for(auto & iter : res) {
        std::visit([&](const auto &x) {
            std::cout<<x.get_value()<< " ";
        }, iter);
    }
    std::cout<<std::endl;


}

BOOST_AUTO_TEST_SUITE_END()