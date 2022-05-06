/*
    This file contains definitions related to parsing data from some configuration source
*/
#include <sstream>
#include <fstream>
#include "Parser.hpp"

    //check that a given input is a valid number (a positive integer)
    bool Parser::isValidNumber(const std::string& input) const{
        for (const char& c : input){
            if (!std::isdigit(c)) return false;
        }
        return true;
    }
    //convert a string value into an unsigned int
    unsigned int Parser::convertToUInt(const std::string& value){
        if (!isValidNumber(value)){
            throw("Value in config file isn't a number");
        }
        else{
            unsigned int res = stoi(value);
            return res;
        }
    }

    //convert a string value to a boolean
    bool Parser::convertToBool(const std::string& value){
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
    Parser::~Parser(){
        //empty
    }

        //constructor takes a string filename and assigns the inFile object
        ConfigFileParser::ConfigFileParser(std::string inFiles): inFileName(inFiles){
            inFile.open(inFiles);
            if (!inFile.is_open()){
                throw("Config file couldn't be opened");
            }
        }

        //destructor closes the file
        ConfigFileParser::~ConfigFileParser(){
            inFile.close();
        }

        //getting values from a config file
        std::string ConfigFileParser::getValueFromKey(const std::string& keyName){
                //reset the position of the file pointer so we always check the whole file
                inFile.clear();
                inFile.seekg(0);
                std::string line, key, value;
                std::istringstream streamLine;
                while (getline(inFile, line)){
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
                        return value;
                    }
                }
                //if we get down here, we didn't find it
                throw("Couldn't find key in file");
        }

