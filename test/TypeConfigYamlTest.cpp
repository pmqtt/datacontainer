#define BOOST_TEST_DYN_LINK 1
#define BOOST_TEST_MODULE TypeConfigYamlTest
#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include "../src/typeconfig/ConfigurationFile.h"
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
    format: "%r %s"
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
       ConfigurationFile file("typeconfigs/test_type_1.yml");
       std::vector<YamlTypeNode> types = file.load();
       BOOST_CHECK_MESSAGE(types.size() == 1, "Count of readed types should 1 and not " + std::to_string(types.size()));

       YamlTypeNode sensorType = types[0];

        BOOST_CHECK_MESSAGE(sensorType.typeName == "TEMPERATURE_SENSOR_01", "Type name should TEMPERATURE_SENSOR_01 and not " + sensorType.typeName);
        BOOST_CHECK(sensorType.expire_in == "20min");
        BOOST_CHECK(sensorType.key == "T_S01");

        std::vector<YamlValuesNode> values = sensorType.values;
        BOOST_CHECK(values.size() == 4);
        BOOST_CHECK(values[0].key == "ID");
        BOOST_CHECK(dynamic_cast<YamlIndexType*>(values[0].value.get()) != nullptr);

        BOOST_CHECK(values[1].key == "TIMESTAMP");
        BOOST_CHECK(dynamic_cast<YamlStringType*>(values[1].value.get()) != nullptr);

        BOOST_CHECK(values[2].key == "TEMPERATURE");
        BOOST_CHECK(dynamic_cast<YamlRealType*>(values[2].value.get()) != nullptr);

        BOOST_CHECK(values[3].key == "MESSAGE");
        BOOST_CHECK(dynamic_cast<YamlStringType*>(values[3].value.get()) != nullptr);

        BOOST_CHECK(sensorType.readEvent.connectionType == "mqtt");
        BOOST_CHECK(sensorType.readEvent.brokerAdr == "localhost:1883");
        BOOST_CHECK(sensorType.readEvent.subscribe == "SENSOR/01/TEMPERATURE");
        BOOST_CHECK_MESSAGE(sensorType.readEvent.format == "%r %s", "GET " + sensorType.readEvent.format);
        BOOST_CHECK(sensorType.readEvent.insertions[values[0].key] == "ink_index(0)");
        BOOST_CHECK(sensorType.readEvent.insertions[values[1].key] == "timestamp");
        BOOST_CHECK(sensorType.readEvent.insertions[values[2].key] == "$1");
        BOOST_CHECK(sensorType.readEvent.insertions[values[3].key] == "$2");

        BOOST_CHECK(sensorType.sendEvent.connectionType == "mqtt");
        BOOST_CHECK(sensorType.sendEvent.brokerAdr == "localhost:1883");
        BOOST_CHECK(sensorType.sendEvent.topic == "CALCULATED");
        BOOST_CHECK(sensorType.sendEvent.prepare == "mean(TEMPERATURE)");


    }

BOOST_AUTO_TEST_SUITE_END()