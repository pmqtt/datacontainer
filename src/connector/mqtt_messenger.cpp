//
// Created by cefour on 03.05.22.
//
#include "mqtt_messenger.h"


bool waitUntilBrokerIsReachable(mqtt_messenger<2> & messenger){
    std::size_t loopTry = 0;
    while(messenger.is_disconnecting_by_server()){
        boost::this_thread::sleep_for(boost::chrono::seconds(5));
        messenger.reconnect();
        loopTry++;
        if( loopTry > 1000){
            throw mqtt_messenger_exception(
                    messenger.create_log_prefix() + " All attempts to connecting to broker were in vain");
        }
    }
    return true;
}

