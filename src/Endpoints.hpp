//This file contains the various endpoints of the websites and how they 
//reply
#include <httpserver.hpp>
#include <chrono>
#include <sstream>
#include <thread>

//used for digest
#define MY_OPAQUE "11733b200778ce33060f31c9af70a870ba96ddd4"

class hello_world_resource : public httpserver::http_resource {
     const std::shared_ptr<httpserver::http_response> render_GET(const httpserver::http_request& req) {
        /*
         if (req.get_digested_user() == "") {
             return std::shared_ptr<httpserver::digest_auth_fail_response>(new httpserver::digest_auth_fail_response("FAIL", "test@example.com", MY_OPAQUE, true));
         } else {
             bool reload_nonce = false;
             if (!req.check_digest_auth("test@example.com", "mypass", 300, &reload_nonce)) {
                 return std::shared_ptr<httpserver::digest_auth_fail_response>(new httpserver::digest_auth_fail_response("FAIL", "test@example.com", MY_OPAQUE, reload_nonce));
             }
         }
         */
         return std::shared_ptr<httpserver::string_response>(new httpserver::string_response("Hello World", 200, "text/plain"));
     }
     //for HW example to get data passed in the url
     //void set_some_data(const std::string &s) {data = s;}
     //std::string data;
};

//big file response


/* Used for the HW resource to check the data from the url, used with set_some_data above
// Using the render method you are able to catch each type of request you receive
const std::shared_ptr<httpserver::http_response> hello_world_resource::render(const httpserver::http_request& req) {
    // It is possible to store data inside the resource object that can be altered through the requests
    std::cout << "Data was: " << data << std::endl;
    std::string datapar = req.get_arg("data");
    set_some_data(datapar == "" ? "no data passed!!!" : datapar);
    std::cout << "Now data is:" << data << std::endl;

    // It is possible to send a response initializing an http_string_response that reads the content to send in response from a string.
    return std::shared_ptr<httpserver::http_response>(new httpserver::string_response("Hello World!!!", 200));
}
*/

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
         return std::shared_ptr<httpserver::file_response>(new httpserver::file_response("test_file", 200, "text/plain"));
     }
};

class image_resource : public httpserver::http_resource {
 public:
     const std::shared_ptr<httpserver::http_response> render_GET(const httpserver::http_request& req) {
         return std::shared_ptr<httpserver::file_response>(new httpserver::file_response("shop.png", 200, "text/plain"));
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
