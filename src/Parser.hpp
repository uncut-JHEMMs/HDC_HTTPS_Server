/*
    This file contains definitions related to parsing data from a config file
*/
#ifndef PARSER_HPP
#define PARSER_HPP
#include <sstream>
#include <fstream>
#include <utility> //pair
#include <list> //list

/*
    Abstract base class with the common interface
*/
class Parser{
    public:
        //use the key to get the value, subclasses concretize this
        virtual std::string getValueFromKey(const std::string& keyName) = 0;
        //virtual 
        std::list<std::pair<std::string, std::string>> keyVals;
        //check that a given input is a valid number (a positive integer)
	bool isValidNumber(const std::string& input) const{
            for (char c : input){
                if (!std::isdigit(c)) return false;
            }
            return true;
        }

        //convert a string value into an unsigned int
        unsigned int convertToUInt(const std::string& value){
            if (!isValidNumber(value)){
                throw("Value in config file isn't a number");
            }
            else{
                unsigned int res = stoi(value);
                return res;
            }
        }

        //convert a string value to a boolean
        bool convertToBool(const std::string& value){
            if (!isValidNumber(value)){
                throw("Value in config file isn't a number");
            }
            else{
                if (value == "0"){
                    return false;
                }
                else{
                    return true;
                }
            }
        }
        virtual ~Parser(){
            //empty, need this for the unique_ptr for a virtual destructor
        }
};

/*
    Subclass that reads data from a config file and parses it into some data structure
    (a list of pairs?)
*/
class ConfigFileParser: public Parser{
    private:
        //here we actually open the file and look for stuff
        std::string inFileName;
    public:
        //constructor takes a string filename and assigns the inFile object
        ConfigFileParser(std::string inFile): inFileName(inFile){
            //empty
        }
        virtual ~ConfigFileParser(){
            //empty
        }
        //getting values from a config file
        virtual std::string getValueFromKey(const std::string& keyName){
            std::ifstream inFile;
            inFile.open(inFileName);
            if (!inFile.is_open()){
                throw("Config file couldn't be opened");
            }
            else{
                //we need the line, the key and the value
                std::string line, key, value;
                std::istringstream streamLine;
                while (getline(inFile, line)){
                    //work on the line
                    streamLine = std::istringstream(line);
                    //get the key
                    std::getline(streamLine, key, '=');
                    if (key == ""){
                        throw("Key is blank in config file");
                    }
                    if (key != keyName){
                        continue;
                    }
                    else{
                        //this is the one we want, return it
                        std::getline(streamLine,value);
                        if (value == ""){
                            throw("Value is blank in config file");
                        }
                        inFile.close();
                        return value;
                    }
                }
                //if we get down here, we didn't find it
                inFile.close();
                throw("Couldn't find key in file");
            }
        }
        

};

class EnvVarParser : public Parser{
    //needs the array of key names
    //then looks very similar to configFileParser
};

#endif
