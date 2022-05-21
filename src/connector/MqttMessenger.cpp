//
// Created by cefour on 03.05.22.
//
#include "MqttMessenger.h"


bool waitUntilBrokerIsReachable(MqttMessenger<2> & messenger){
    std::size_t loopTry = 0;
    while(messenger.isDisconnectingByServer()){
        boost::this_thread::sleep_for(boost::chrono::seconds(5));
        messenger.reconnect();
        loopTry++;
        if( loopTry > 1000){
            throw MqttMessengerException(messenger.createLogPrefix() + " All attempts to connecting to broker were in vain");
        }
    }
    return true;
}

