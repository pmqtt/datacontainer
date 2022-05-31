#define BOOST_TEST_DYN_LINK 1
#define BOOST_TEST_MODULE TypeConfigYamlTest
#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include "../src/typeconfig/type_defintion_file.h"
/*
 * types:
  - TEMPERATURE_SENSOR_01




TEMPERATURE_SENSOR_01:
  expire_in: 20min
  key: T_S01
  name: TEMPERATURE_SENSOR
  values:
    - ID: index
    - TIMESTAMP: string
    - TEMPERATURE: real
    - MESSAGE: string
  read_event:
    type: mqtt
    broker: localhost:1883
    subscribe: SENSOR/01/TEMPERATURE
    format_def: "%r %s"
    insert:
      - ID: ink_index(0)
      - TIMESTAMP: timestamp
      - TEMPERATURE: $1
      - MESSAGE: $2
  send_event:
    type: mqtt
    broker: localhost:1883
    topic: CALCULATED
    prepare: mean(TEMPERATURE)
    when:
      - count(TEMPERATURE) >= 3
      - reset: true
 */

BOOST_AUTO_TEST_SUITE(TypeConfigYamlTestSuite)

    BOOST_AUTO_TEST_CASE(LoadFileTest){
       type_defintion_file file("typeconfigs/test_type_1.yml");
       std::vector<yaml_type_node> types = file.load();
       BOOST_CHECK_MESSAGE(types.size() == 1, "Count of readed types should 1 and not " + std::to_string(types.size()));

       yaml_type_node sensorType = types[0];

        BOOST_CHECK_MESSAGE(sensorType.type_name == "TEMPERATURE_SENSOR_01", "Type name should TEMPERATURE_SENSOR_01 and not " + sensorType.type_name);
        BOOST_CHECK(sensorType.expire_in == "20min");
        BOOST_CHECK(sensorType.key == "T_S01");

        std::vector<yaml_values_node> values = sensorType.values;
        BOOST_CHECK(values.size() == 4);
        BOOST_CHECK(values[0].key == "ID");
        BOOST_CHECK(dynamic_cast<yaml_index_type*>(values[0].value.get()) != nullptr);

        BOOST_CHECK(values[1].key == "TIMESTAMP");
        BOOST_CHECK(dynamic_cast<yaml_string_type*>(values[1].value.get()) != nullptr);

        BOOST_CHECK(values[2].key == "TEMPERATURE");
        BOOST_CHECK(dynamic_cast<yaml_real_type*>(values[2].value.get()) != nullptr);

        BOOST_CHECK(values[3].key == "MESSAGE");
        BOOST_CHECK(dynamic_cast<yaml_string_type*>(values[3].value.get()) != nullptr);

        BOOST_CHECK(sensorType.read_event.connection_type == "mqtt");
        BOOST_CHECK(sensorType.read_event.broker_adr == "localhost:1883");
        BOOST_CHECK(sensorType.read_event.subscribe == "SENSOR/01/TEMPERATURE");
        BOOST_CHECK_MESSAGE(sensorType.read_event.format_def == "%r %s", "GET " + sensorType.read_event.format_def);
        BOOST_CHECK(sensorType.read_event.insertions[values[0].key] == "ink_index(0)");
        BOOST_CHECK(sensorType.read_event.insertions[values[1].key] == "timestamp");
        BOOST_CHECK(sensorType.read_event.insertions[values[2].key] == "$1");
        BOOST_CHECK(sensorType.read_event.insertions[values[3].key] == "$2");

        BOOST_CHECK(sensorType.send_event.connection_type == "mqtt");
        BOOST_CHECK(sensorType.send_event.broker_adr == "localhost:1883");
        BOOST_CHECK(sensorType.send_event.topic == "CALCULATED");
        BOOST_CHECK(sensorType.send_event.prepare == "mean(TEMPERATURE)");


    }

BOOST_AUTO_TEST_SUITE_END()