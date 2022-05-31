//
// Created by cefour on 03.05.22.
//

#ifndef DATACONTAINER_MQTT_MESSENGER_H
#define DATACONTAINER_MQTT_MESSENGER_H
#ifndef MESSENGER_H
#define MESSENGER_H

#include "../exceptions/mqtt_messenger_exception.h"

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



namespace{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::mutex generator_lock;
}


template<std::size_t QOS>
class mqtt_messenger : public ::mqtt::callback {
public:
    mqtt_messenger(const std::string &broker_ip, bool handle = false, const ::mqtt::will_options& will_opt = {})
            : will_options{will_opt }
    {
        publish_counter = 0;
        std::string id = get_client_id();
        mqtt_id = id;
        lost_connection = false;
        client = std::make_shared<::mqtt::client>(prepare_addresse(broker_ip), id);
        ip = prepare_addresse(broker_ip);
        this->current_addresse = broker_ip;

        client->set_callback(*this);
        is_disconnect = false;
        if(handle){
            try{
                is_disconnect = !connect();
                has_valid_connection = true;
            }catch(...){
                has_valid_connection = false;
                lost_connection = true;
            }
        }else{

            is_disconnect = !connect();
            has_valid_connection = !is_disconnect;
        }
        if(is_disconnect){
            std::cout << create_log_prefix() << " Connection failed! ";
        }else{
            std::cout << create_log_prefix() << " { Is connected with : " << id << " }";
        }
    }


    virtual void disconnect(){
        if(client->is_connected()){
            is_disconnect = true;
            //this->client->disconnect();
        }
    }
    virtual ~mqtt_messenger() {
        if (this->client->is_connected()) {
            this->client->disconnect();
        }
    }
public:
    virtual bool connect() {
        if(!client->is_connected()){
            client->connect(create_connect_options());
            return client->is_connected();
        }
        return true;
    }

    virtual bool is_connected() const{
        return client->is_connected();
    }

    virtual void publish(const std::string & topic, const std::string &payload) {
        if(!has_valid_connection || lost_connection || is_disconnect){
            should_reconnect();
            return;
        }
        try{
            if(topic.length() == 0 || payload.length() == 0) return;

            auto published_message = ::mqtt::make_message(topic, payload);
            published_message->set_qos(0);
            //publish_mutex.lock();
            client->publish(published_message);
            //publish_mutex.unlock();
        }catch(::mqtt::exception & e){
            handle_mqtt_exception(e);
        }catch(...){

        }
    }

    virtual bool publishWithRetained(const std::string & topic, const std::string &payload) {
        if(!has_valid_connection){
            return false;
        }
        try{
            auto published_message = ::mqtt::make_message(topic, payload);
            published_message->set_qos(0);
            published_message->set_retained(true);
            client->publish(published_message);
            return true;
        }catch(::mqtt::exception & e){
            handle_mqtt_exception(e);
        }catch(...){
            std::cout <<  "unexpected exception by publish WithRetained" << std::endl;
        }
        return false;
    }

    bool is_disconnecting_by_server()const{
        return lost_connection;
    }


    virtual void subscribe(const std::string & topic) {
        auto it = std::find(topic_cache.begin(), topic_cache.end(), topic);
        if(it == topic_cache.end()) {
            topic_cache.insert(topic);
        }
        if(!has_valid_connection){
            return;
        }
        try{
            client->subscribe(topic, 0);
        }catch(::mqtt::exception & e){
            handle_mqtt_exception(e);
            if(e.get_reason_code() == 128){
                disconnect();
                reconnect();
                //throw e;
            }
        }catch(...){

        }
    }
    virtual void unsubscribe(const std::string & topic) {
        topic_cache.erase(topic);
        if(!has_valid_connection){
            return;
        }
        client->unsubscribe(topic);
    }

    virtual std::pair<std::string, std::string> get_message(const std::chrono::seconds wait){
        if(!has_valid_connection){
            return mqtt_messenger::empty_message;                }
        try{
            ::mqtt::const_message_ptr msg;

            bool message_consumed = client->try_consume_message_for(&msg, wait);
            if(!is_connected() || is_disconnect ){
                throw mqtt_disconnect_exception("Connection to broker " + get_broker_address() + "is closed by client");
            }
            if(message_consumed) {
                return std::make_pair(msg->get_topic(), msg->get_payload());
            }
            return mqtt_messenger::empty_message;

        }catch(::mqtt::exception & e){
            handle_mqtt_exception(e);
        }catch(mqtt_disconnect_exception & e){
            throw e;
        }
        catch(...){
            std::cout << create_log_prefix() << " Unknown exception in mqtt messenger";
        }

        return mqtt_messenger::empty_message;
    }

    virtual std::pair<std::string, std::string> getMessage() {
        if(!has_valid_connection){
            return mqtt_messenger::empty_message;
        }
        try{
            ::mqtt::const_message_ptr msg;
            bool message_consumed = false;
            while(!message_consumed){
                message_consumed = client->try_consume_message_for(&msg, std::chrono::seconds(1));
                if(!is_connected() || is_disconnect){
                    throw mqtt_disconnect_exception("Connection to broker is closed by client");
                }
            }
            return std::make_pair(msg->get_topic(), msg->get_payload());
        }catch(::mqtt::exception & e){
            handle_mqtt_exception(e);
        }catch(mqtt_disconnect_exception & e){
            throw e;
        }
        catch(...){
            std::cout << create_log_prefix() << " Unknown exception in mqtt messenger";
        }

        return mqtt_messenger::empty_message;
    }

    virtual std::pair<std::string, std::string> getMessageNowait()
    {
        if(!has_valid_connection){
            return mqtt_messenger::empty_message;
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
            handle_mqtt_exception(e);
        }catch(...){
            std::cout << create_log_prefix() << " Unknown exception in mqtt messenger";
        }

        return mqtt_messenger::empty_message;
    }

    virtual void reconnect() {
        if(!client->is_connected()){
            try {
                std::cout << create_log_prefix() << " Try to reconnect ... ";
                connect();
                for(auto & topic : topic_cache){
                    client->subscribe(topic, 0);
                }
                is_disconnect = false;
                has_valid_connection = true;
                lost_connection = false;

            }catch(...){
                std::cout << create_log_prefix() << " Reconnect not possible";
            }
            if(client->is_connected()){
                std::cout << create_log_prefix() << " Reconnect success!";
            }
        }else{
            std::cout << create_log_prefix() << " Client is connected";
        }
    }


public:
    virtual void connection_lost(const std::string& cause) override {
        std::cout << create_log_prefix() << "Connection lost for " << this->ip << " with cause:" << cause;
        is_disconnect = true;
        lost_connection = true;
    }

    virtual void connected( const std::string&  cause) override
    {
        std::cout << create_log_prefix() << "Connection established for " << this->ip << " with cause:" << cause;
        is_disconnect = false;
        lost_connection = false;
    }

    virtual void delivery_complete(::mqtt::delivery_token_ptr tok) override {
        do { (void)(tok); } while (0);
    }

    const std::string&  get_broker_address() const
    {
        return current_addresse;
    }

    static const std::pair<std::string, std::string>& get_empty_message()
    {
        return mqtt_messenger::empty_message;
    }


private:
    virtual void should_reconnect(){
        publish_counter++;
        if(publish_counter > 3){
            reconnect();
            publish_counter = 0;
        }
    }

    ::mqtt::connect_options create_connect_options()const {
        ::mqtt::connect_options connection_options;
        connection_options.set_keep_alive_interval(120);
        connection_options.set_clean_session(true);
        connection_options.set_automatic_reconnect(false);

        if (!sslCA.empty()){
            ::mqtt::ssl_options sslopts;
            sslopts.set_trust_store(sslCA);
            sslopts.set_enable_server_cert_auth(false);
            connection_options.set_ssl(sslopts);
        }

#ifdef _SET_MQTT_VERSION_5_

        connection_options.set_mqtt_version(MQTTVERSION_5);
#endif
        return connection_options;
    }
    static std::string prepare_addresse(const std::string & adr) {
        std::string result = "";
        for (std::size_t i = 0; i < adr.length(); ++i) {
            result += adr[i];
            if (result[i] == '/') {
                result[i] = ':';
            }
        }
        return result;
    }

    void handle_mqtt_exception(::mqtt::exception & e){
        int reason_code = -e.get_reason_code();
        std::vector<std::string> exception_text;
        exception_text.push_back("NO FAILURE"); // reasoncode 0
        exception_text.push_back("MQTT client operation failed. Connected to broker "); // reasondcode -1
        exception_text.push_back("No error text"); // reasondcode -2
        exception_text.push_back("Connection to broker is broken"); // reasondcode -3
        exception_text.push_back("The maximum number of messages allowed to be simultaneously in-flight has been reached"); // reasondcode -4
        exception_text.push_back("An invalid UTF-8 string has been detected."); // reasondcode -6

        if(reason_code < (int)exception_text.size() && reason_code >= 0){
            std::cout << create_log_prefix() << exception_text[reason_code] << " | " << e.to_string();
        }else{
            std::cout << create_log_prefix() << e.to_string();
        }

    }

private:
    std::shared_ptr<::mqtt::client> client;
    boost::mutex publish_mutex;
    std::set<std::string> topic_cache;
    std::string mqtt_id;
    std::string ip;
    std::string sslCA;
    std::atomic_bool is_disconnect;
    std::atomic_bool has_valid_connection;
    std::atomic_bool lost_connection;
    std::string current_addresse;
    std::atomic_uint publish_counter;
    ::mqtt::will_options will_options;
private:
    static std::string allowed_chars;
    static std::string prefix;
    static std::pair<std::string,std::string> empty_message;
public:

    std::string create_log_prefix()const{
        return "[ MQTT BROKER " + this->current_addresse + " ] ";
    }

    static void setClientIdPrefix(const std::string & pre){
        prefix = pre;
    }
    static const std::string get_client_id()
    {
        const std::lock_guard<std::mutex> guard(generator_lock);
        std::string private_allowed_chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        std::shuffle(private_allowed_chars.begin(), private_allowed_chars.end(), generator);
        if (prefix.size() > 17){
            prefix = prefix.substr(0,17);
        }

        srand(time(NULL));
        std::string output_string("");
        //MQTT max client id length is 23 Byte
        for (size_t i = 0; i < 23 - prefix.length(); ++i){
            output_string += private_allowed_chars[rand() % private_allowed_chars.length()];
        }

        if (output_string.empty()){
            return get_client_id();
        }
        else{
            return prefix + output_string;
        }
    }
};

bool waitUntilBrokerIsReachable(mqtt_messenger<2> & messenger);


template<std::size_t QOS> std::string mqtt_messenger<QOS>::allowed_chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
template<std::size_t QOS> std::string mqtt_messenger<QOS>::prefix = "ID-";
template<std::size_t QOS> std::pair<std::string,std::string> mqtt_messenger<QOS>::empty_message = std::make_pair<std::string,std::string>("", "");

#endif
#endif //DATACONTAINER_MQTT_MESSENGER_H
