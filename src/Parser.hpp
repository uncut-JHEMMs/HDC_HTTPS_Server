/*
    This file contains definitions related to parsing data from some configuration source
*/
#ifndef PARSER_HPP
#define PARSER_HPP
#include <sstream>
#include <fstream>
/*
    Abstract base class with the common interface
*/
class Parser{
    public:
        //use the key to get the value, subclasses concretize this
        virtual std::string getValueFromKey(const std::string& keyName) = 0;
        //check that a given input is a valid number (a positive integer)
	bool isValidNumber(const std::string& input) const;
        //convert a string value into an unsigned int
        unsigned int convertToUInt(const std::string& value);
        //convert a string value to a boolean
        bool convertToBool(const std::string& value);
        virtual ~Parser();
};

/*
    Subclass that reads data from a text configuration file
*/
class ConfigFileParser: public Parser{
    private:
        //here we actually open the file and look for stuff
        std::string inFileName;
        std::ifstream inFile;
    public:
        //constructor takes a string filename and assigns the inFile object
        ConfigFileParser(std::string inFiles);
        //close the file
        virtual ~ConfigFileParser();

        //getting values from a config file
        virtual std::string getValueFromKey(const std::string& keyName);

};

class EnvParser: public Parser
{
	public:
	EnvParser();
	virtual ~EnvParser();
	virtual std::string getValueFromKey(const std::string& keyName);
};
#endif
