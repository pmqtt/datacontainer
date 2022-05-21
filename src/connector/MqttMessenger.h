//
// Created by cefour on 03.05.22.
//

#ifndef DATACONTAINER_MQTTMESSENGER_H
#define DATACONTAINER_MQTTMESSENGER_H
#ifndef MESSENGER_H
#define MESSENGER_H
#include <string>
#include <mqtt/client.h>
#include <mqtt/exception.h>
#include <boost/thread.hpp>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <atomic>
#include <set>
#include <random>
#include <algorithm>
#include <iterator>
#include "../exceptions/MqttMessengerException.h"



namespace{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::mutex generatorLock;
}


template<std::size_t QOS>
class MqttMessenger : public ::mqtt::callback {
public:
    MqttMessenger(const std::string &brokerIP, bool handle = false, const ::mqtt::will_options& willOpt = {})
            : willOptions{ willOpt }
    {
        publishCounter = 0;
        std::string id = getClientID();
        mqttID = id;
        connectionLost = false;
        client = std::make_shared<::mqtt::client>(prepareAddresse(brokerIP),id);
        ip = prepareAddresse(brokerIP);
        this->currentAdr = brokerIP;

        client->set_callback(*this);
        std::cout<<createLogPrefix()<<" Try to connect... ";
        isDisconnect = false;
        if(handle){
            try{
                isDisconnect = !connect();
                hasValidConnection = true;
            }catch(...){
                hasValidConnection = false;
                connectionLost = true;
            }
        }else{

            isDisconnect = !connect();
            hasValidConnection = !isDisconnect;
        }
        if(isDisconnect){
            std::cout<<createLogPrefix()<<" Connection failed! ";
        }else{
            std::cout<<createLogPrefix()<<" { Is connected with : "<< id <<" }";
        }
    }


    virtual void disconnect(){
        if(client->is_connected()){
            isDisconnect = true;
            //this->client->disconnect();
        }
    }
    virtual ~MqttMessenger() {
        if (this->client->is_connected()) {
            this->client->disconnect();
        }
    }
public:
    virtual bool connect() {
        if(!client->is_connected()){
            client->connect(createConnectOptions());
            return client->is_connected();
        }
        return true;
    }

    virtual bool isConnected() const{
        return client->is_connected();
    }

    virtual void publish(const std::string & topic, const std::string &payload) {
        if(!hasValidConnection || connectionLost || isDisconnect){
            shouldReconnect();
            return;
        }
        try{
            if(topic.length() == 0 || payload.length() == 0) return;

            auto publishedMessage = ::mqtt::make_message(topic, payload);
            publishedMessage->set_qos(0);
            //publishMutex.lock();
            client->publish(publishedMessage);
            //publishMutex.unlock();
        }catch(::mqtt::exception & e){
            handleMqttException(e);
        }catch(...){

        }
    }

    virtual bool publishWithRetained(const std::string & topic, const std::string &payload) {
        if(!hasValidConnection){
            return false;
        }
        try{
            auto publishedMessage = ::mqtt::make_message(topic, payload);
            publishedMessage->set_qos(0);
            publishedMessage->set_retained(true);
            client->publish(publishedMessage);
            return true;
        }catch(::mqtt::exception & e){
            handleMqttException(e);
        }catch(...){
            std::cout <<  "unexpected exception by publish WithRetained" << std::endl;
        }
        return false;
    }

    bool isDisconnectingByServer()const{
        return connectionLost;
    }


    virtual void subscribe(const std::string & topic) {
        auto it = std::find(topicCache.begin(),topicCache.end(),topic);
        if(it == topicCache.end()) {
            topicCache.insert(topic);
        }
        if(!hasValidConnection){
            return;
        }
        try{
            client->subscribe(topic, 0);
        }catch(::mqtt::exception & e){
            handleMqttException(e);
            if(e.get_reason_code() == 128){
                disconnect();
                reconnect();
                //throw e;
            }
        }catch(...){

        }
    }
    virtual void unsubscribe(const std::string & topic) {
        topicCache.erase(topic);
        if(!hasValidConnection){
            return;
        }
        client->unsubscribe(topic);
    }

    virtual std::pair<std::string, std::string> getMessage(const std::chrono::seconds wait){
        if(!hasValidConnection){
            return MqttMessenger::emptyMessage;                }
        try{
            ::mqtt::const_message_ptr msg;

            bool messageConsumed = client->try_consume_message_for(&msg,wait);
            if(!isConnected() || isDisconnect ){
                throw MqttDisconnectException("Connection to broker " + getBrokerAddress() + "is closed by client");
            }
            if(messageConsumed) {
                return std::make_pair(msg->get_topic(), msg->get_payload());
            }
            return MqttMessenger::emptyMessage;

        }catch(::mqtt::exception & e){
            handleMqttException(e);
        }catch(MqttDisconnectException & e){
            throw e;
        }
        catch(...){
            std::cout<<createLogPrefix()<<" Unknown exception in mqtt messenger";
        }

        return MqttMessenger::emptyMessage;
    }

    virtual std::pair<std::string, std::string> getMessage() {
        if(!hasValidConnection){
            return MqttMessenger::emptyMessage;
        }
        try{
            ::mqtt::const_message_ptr msg;
            bool messageConsumed = false;
            while(!messageConsumed){
                messageConsumed = client->try_consume_message_for(&msg,std::chrono::seconds(1));
                if(!isConnected()|| isDisconnect){
                    throw MqttDisconnectException("Connection to broker is closed by client");
                }
            }
            std::cout<<"MESSAGE:"<<msg->get_topic()<<std::endl;
            return std::make_pair(msg->get_topic(), msg->get_payload());
        }catch(::mqtt::exception & e){
            handleMqttException(e);
        }catch(MqttDisconnectException & e){
            throw e;
        }
        catch(...){
            std::cout<<createLogPrefix()<<" Unknown exception in mqtt messenger";
        }

        return MqttMessenger::emptyMessage;
    }

    virtual std::pair<std::string, std::string> getMessageNowait()
    {
        if(!hasValidConnection){
            return MqttMessenger::emptyMessage;
        }
        ::mqtt::const_message_ptr msg;
        try{
            //Try to read the next message from the queue without blocking

            bool msgrcv = client->try_consume_message(&msg);
            if (msgrcv) {
                return std::make_pair(msg->get_topic(), msg->get_payload());
            }
        }
        catch(::mqtt::exception & e){
            handleMqttException(e);
        }catch(...){
            std::cout<<createLogPrefix()<<" Unknown exception in mqtt messenger";
        }

        return MqttMessenger::emptyMessage;
    }

    virtual void reconnect() {
        if(!client->is_connected()){
            try {
                std::cout<<createLogPrefix()<<" Try to reconnect ... ";
                connect();
                for(auto & topic : topicCache){
                    client->subscribe(topic, 0);
                }
                isDisconnect = false;
                hasValidConnection = true;
                connectionLost = false;

            }catch(...){
                std::cout<<createLogPrefix()<<" Reconnect not possible";
            }
            if(client->is_connected()){
                std::cout<<createLogPrefix()<<" Reconnect success!";
            }
        }else{
            std::cout<<createLogPrefix()<<" Client is connected";
        }
    }


public:
    virtual void connection_lost(const std::string& cause) override {
        std::cout <<createLogPrefix()<< "Connection lost for " << this->ip << " with cause:" << cause;
        isDisconnect = true;
        connectionLost = true;
    }

    virtual void connected( const std::string&  cause) override
    {
        std::cout <<createLogPrefix()<< "Connection established for " << this->ip << " with cause:" << cause;
        isDisconnect = false;
        connectionLost = false;
    }

    virtual void delivery_complete(::mqtt::delivery_token_ptr tok) override {
        do { (void)(tok); } while (0);
    }

    const std::string&  getBrokerAddress() const
    {
        return currentAdr;
    }

    static const std::pair<std::string, std::string>& getEmptyMessage()
    {
        return MqttMessenger::emptyMessage;
    }


private:
    virtual void shouldReconnect(){
        publishCounter++;
        if(publishCounter > 3){
            reconnect();
            publishCounter = 0;
        }
    }

    ::mqtt::connect_options createConnectOptions()const {
        ::mqtt::connect_options connectionOptions;
        connectionOptions.set_keep_alive_interval(120);
        connectionOptions.set_clean_session(true);
        connectionOptions.set_automatic_reconnect(false);

        if (!sslCA.empty()){
            ::mqtt::ssl_options sslopts;
            sslopts.set_trust_store(sslCA);
            sslopts.set_enable_server_cert_auth(false);
            connectionOptions.set_ssl(sslopts);
        }

#ifdef _SET_MQTT_VERSION_5_

        connectionOptions.set_mqtt_version(MQTTVERSION_5);
#endif
        return connectionOptions;
    }
    static std::string prepareAddresse(const std::string & adr) {
        std::string result = "";
        for (std::size_t i = 0; i < adr.length(); ++i) {
            result += adr[i];
            if (result[i] == '/') {
                result[i] = ':';
            }
        }
        return result;
    }

    void handleMqttException(::mqtt::exception & e){
        int reasonCode = -e.get_reason_code();
        std::vector<std::string> exceptionText;
        exceptionText.push_back("NO FAILURE"); // reasoncode 0
        exceptionText.push_back("MQTT client operation failed. Connected to broker "); // reasondcode -1
        exceptionText.push_back("No error text"); // reasondcode -2
        exceptionText.push_back("Connection to broker is broken"); // reasondcode -3
        exceptionText.push_back("The maximum number of messages allowed to be simultaneously in-flight has been reached"); // reasondcode -4
        exceptionText.push_back("An invalid UTF-8 string has been detected."); // reasondcode -6

        if(reasonCode < (int)exceptionText.size() && reasonCode >= 0){
            std::cout<<createLogPrefix()<<exceptionText[reasonCode]<< " | "<<e.to_string();
        }else{
            std::cout<<createLogPrefix()<<e.to_string();
        }

    }

private:
    std::shared_ptr<::mqtt::client> client;
    boost::mutex publishMutex;
    std::set<std::string> topicCache;
    std::string mqttID;
    std::string ip;
    std::string sslCA;
    std::atomic_bool isDisconnect;
    std::atomic_bool hasValidConnection;
    std::atomic_bool connectionLost;
    std::string currentAdr;
    std::atomic_uint publishCounter;
    ::mqtt::will_options willOptions;
private:
    static std::string allowedChars;
    static std::string prefix;
    static std::pair<std::string,std::string> emptyMessage;
public:

    std::string createLogPrefix()const{
        return "[ MQTT BROKER " + this->currentAdr +" ] ";
    }

    static void setClientIdPrefix(const std::string & pre){
        prefix = pre;
    }
    static const std::string getClientID()
    {
        const std::lock_guard<std::mutex> guard(generatorLock);
        std::string privateAllowedChars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        std::shuffle(privateAllowedChars.begin(), privateAllowedChars.end(), generator);
        if (prefix.size() > 17){
            prefix = prefix.substr(0,17);
        }

        srand(time(NULL));
        std::string outputString("");
        //MQTT max client id length is 23 Byte
        for (size_t i = 0; i < 23 - prefix.length(); ++i){
            outputString += privateAllowedChars[rand() % privateAllowedChars.length()];
        }

        if (outputString.empty()){
            return getClientID();
        }
        else{
            return prefix + outputString;
        }
    }
};

bool waitUntilBrokerIsReachable(MqttMessenger<2> & messenger);


template<std::size_t QOS> std::string MqttMessenger<QOS>::allowedChars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
template<std::size_t QOS> std::string MqttMessenger<QOS>::prefix = "ID-";
template<std::size_t QOS> std::pair<std::string,std::string> MqttMessenger<QOS>::emptyMessage = std::make_pair<std::string,std::string>("","");

#endif
#endif //DATACONTAINER_MQTTMESSENGER_H
