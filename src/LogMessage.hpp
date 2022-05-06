/*
    This type encapsulates a log message, used by the server and the logger to
    take certain actions based on the information in the message
*/
#ifndef LOGMESSAGE_HPP
#define LOGMESSAGE_HPP
#include <string>


enum LogMessageType{
    /*
        A status message that doesn't affect the reporting statistics. For
        instance, the messages that run at startup for the configs are type
        0 messages.
    */
    NONACCESS = 0,
    /*
        A status message that a request has been made to a resource
    */
    REQUESTLOG = 1,
    /*
        A status message that a response has been sent
    */
    RESPONSELOG = 2

    //others as needed
};

/*
    The response codes we currently support
*/
enum LogResponseCode{
    OK = 200,
    NOTFOUND = 404,
    ERROR = 500
};

class LogMessage{
    private:
        //type
        LogMessageType type;
        //string message
        std::string textMessage;
        //response size in bytes
        unsigned int responseSize; //this is technically a limitation
        //response code
        LogResponseCode responseCode;

    public:
    //default constructor assumes a type of 0, which has an empty string message
    LogMessage(): type(LogMessageType::NONACCESS), textMessage(""), 
        responseSize(0), responseCode(LogResponseCode::OK){}
    //getters/setters
    LogMessageType getMessageType() const{
        return type;
    }
    void setMessageType(const LogMessageType newMT){
        type = newMT;
    }
    
    std::string getTextMessage() const{
        return textMessage;
    }
    void setTextMessage(const std::string& newText){
        textMessage = newText;
    }

    unsigned int getResponseSize(){
        return responseSize;
    }
    void setResponseSize(const unsigned int respSize){
        responseSize = respSize;
    }

    LogResponseCode getResponseCode() const{
        return responseCode;
    }
    void setResponseCode(const LogResponseCode code){
        responseCode = code;
    }

};

#endif