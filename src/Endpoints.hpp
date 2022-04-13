//This file contains the various endpoints of the websites and how they 
//reply
#ifndef ENDPOINTS_HPP
#define ENDPOINTS_HPP
#include <httpserver.hpp>
#include <chrono>
#include <sstream>
#include <thread>
#include "Logger.hpp"
#include "LogMessage.hpp"

extern Logger logger;
/*
    I'm not sure if I need a class that inherits httpserver::http_resource.
    I'll know if there are functionalities that are common to each of these
    endpoints besides needing to report the 
*/

//used for digest
#define MY_OPAQUE "11733b200778ce33060f31c9af70a870ba96ddd4"

class hello_world_resource : public httpserver::http_resource {
    const std::shared_ptr<httpserver::http_response> render_GET(const httpserver::http_request& req) {
        std::string body_text("Hello World");
        unsigned int response_size=body_text.length();
        LogResponseCode response_code = LogResponseCode::OK;
        std::string mime_type = "text/plain";
        //send the type 2 message to the logger
        LogMessage message;
        message.setResponseCode(response_code);
        message.setResponseSize(response_size);
        message.setMessageType(LogMessageType::RESPONSELOG);
        logger.postMessage(message);
        //httpserver::string_response response(body_text, response_code, mime_type);
        const std::shared_ptr<httpserver::http_response> responsePtr(new httpserver::string_response(body_text, response_code, mime_type));
        return responsePtr;
        //return std::shared_ptr<httpserver::string_response>(new httpserver::string_response("Hello World", 200, "text/plain"));
     }
};
//servers the user documents
class docs_resource : public httpserver::http_resource {
public:
//digest authentication part
    const std::shared_ptr<httpserver::http_response> render_GET(const httpserver::http_request& req) {
	if (req.get_digested_user() == "") {
            return std::shared_ptr<httpserver::digest_auth_fail_response>(new httpserver::digest_auth_fail_response("FAIL", "test@example.com", MY_OPAQUE, true));
         } else {
            bool reload_nonce = false;
            if (!req.check_digest_auth("test@example.com", "mypass", 300, &reload_nonce)) {
                return std::shared_ptr<httpserver::digest_auth_fail_response>(new httpserver::digest_auth_fail_response("FAIL", "test@example.com", MY_OPAQUE, reload_nonce));
            }
        }
    //check the argument if it matches users or merchants and return the appropriate file
    std::string arg = req.get_arg("type");
    if (arg == "users"){
        //return users
        return std::shared_ptr<httpserver::file_response>(new httpserver::file_response("xmlfiles/users.xml", 200, "text/plain"));
    }
    else if (arg == "merchants"){
        //return merchants
        return std::shared_ptr<httpserver::file_response>(new httpserver::file_response("xmlfiles/merchants.xml", 200, "text/plain"));
    }
    else{
        //return an error
        return std::shared_ptr<httpserver::string_response>(new httpserver::string_response("FAIL", 500, "text/plain"));
    }
    //TODO: Get this endpoint to post stats

    }
};

//all our endpoints should be a digest resource
class digest_resource : public httpserver::http_resource {
 public:
    const std::shared_ptr<httpserver::http_response> render_GET(const httpserver::http_request& req) {
	if (req.get_digested_user() == "") {
            return std::shared_ptr<httpserver::digest_auth_fail_response>(new httpserver::digest_auth_fail_response("FAIL", "test@example.com", MY_OPAQUE, true));
         } else {
            bool reload_nonce = false;
            if (!req.check_digest_auth("test@example.com", "mypass", 300, &reload_nonce)) {
                return std::shared_ptr<httpserver::digest_auth_fail_response>(new httpserver::digest_auth_fail_response("FAIL", "test@example.com", MY_OPAQUE, reload_nonce));
            }
        }
        /*
            Get this endpoint to post stats
        */


       ////////////////////////////////////
        return std::shared_ptr<httpserver::file_response>(new httpserver::file_response("digest.html", 200, "text/html"));
    }
};

class image_resource : public httpserver::http_resource {
 public:
    const std::shared_ptr<httpserver::http_response> render_GET(const httpserver::http_request& req) {
        /*
            Get this endpoint to post stats
        */


       //////////////////
        return std::shared_ptr<httpserver::file_response>(new httpserver::file_response("SSLogo.png", 200, "text/plain"));
    }
};

class latency_resource : public httpserver::http_resource {
 public:
	const std::shared_ptr<httpserver::http_response> render_GET(const httpserver::http_request& req) {
	//calculate the time
	std::string stamp = req.get_arg("time");
	std::stringstream stream(stamp);
	long long istamp = 0, diff = 0;
	stream >> istamp;
	//std::cout << "istamp is " << istamp<<std::endl;
	//get the current time
	auto s1 = std::chrono::system_clock::now();
	auto s1_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(s1);
	auto epoch = s1_ms.time_since_epoch();
	long long timeval = epoch.count();
	diff = timeval - istamp;
	//std::cout<<"difference between "<<timeval<< " and "<< istamp << " is " << diff << std::endl;
	//turn it back into a string
	stream = std::stringstream();
	stream << diff;
	stamp = stream.str();
        return std::shared_ptr<httpserver::string_response>(new httpserver::string_response(stamp, 200, "text/plain"));
    }

};
#endif