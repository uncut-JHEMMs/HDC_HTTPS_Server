/*
    This file contains declarations and types for taking the input file and condensing it
    into a more compact format instead of ASCII for quicker searching. Has functions 
    for translating both ways.  I think the data mart creation goes here too.
*/
#ifndef DATAMANIPULATOR_HPP
#define DATAMANIPULATOR_HPP
#include <string>
#include <fstream>
#include <set>
#include <sstream>
#include <iostream>

//bit values for the different error types
enum TRErrors
{
    TECH_GLITCH = 1,
    BAD_PIN = 2,
    BAD_EXPY = 4,
    BAD_NUM = 8,
    INSUFF_BAL = 16

};
//enum for transaction type
enum TRType
{
    SWIPE = 1,
    CHIP = 2,
    ONLINE = 4
};


//Merchant DocCode to ID type for looking through the set
struct MerchDocnID
{
    std::string docCode;
    unsigned int idNum;
    //comparator for searching the set
    MerchDocnID();
    MerchDocnID(const std::string& newDoc, const unsigned int newID);
    bool operator<(const MerchDocnID& other);
};

//City string and ID type for writing out these details later?
struct CityStringnID
{
    std::string cityName;
    unsigned int cityIDNum;
    CityStringnID();
    CityStringnID(const std::string& newName, const unsigned int newID);
    //comparator for set
    bool operator<(const CityStringnID& other);
};
//State string and ID type
struct StateStringnID
{
    std::string stateName;
    unsigned int stateIDNum;
    //sets to blank and 0
    StateStringnID();
    StateStringnID(const std::string& newName, const unsigned int newID);
    //comparator for set
    bool operator<(const StateStringnID& other);
};



/*
    a data manipulator is responsible for taking the ascii file and spitting
    out the compressed file. Also has some other stuff for sorting the 
    compressed file different ways
*/
class DataManipulator{
    private:
        unsigned int nextMerchantID;
        unsigned int nextTransactionID;
        std::string asciiFileName;
        std::string CDFFileName;
        std::ifstream asciiFile; //input file
        std::ofstream CDFFile; //output file
        //we don't do card expansion here, that's done by enrichment
    public:
    ~DataManipulator()
    {
        asciiFile.close();
        CDFFile.close();
    }
    std::ifstream& getinStream()
    {
        return asciiFile;
    }
    std::ofstream& getOutStream()
    {
        return CDFFile;
    }
    unsigned int getNextMerchantID()
    {
        return nextMerchantID;
    }
    unsigned int getNextTransactionID()
    {
        return nextTransactionID;
    }
    void setNextMerchantID(unsigned int newNext)
    {
        nextMerchantID = newNext;
    }
    void setNextTransactionID(unsigned int newNext)
    {
        nextTransactionID = newNext;
    }
    //sets for elements that need to be consistently mapped
    std::set<MerchDocnID> merches;
    std::set<CityStringnID> cities;
    std::set<StateStringnID> states;
    //default sets all the nexts to 0
    DataManipulator();
    DataManipulator(const std::string& inFile, const std::string& outFile);
    static const unsigned int TR_SIZE=36; //includes 1 byte for the newline at the end of the transaction

    //take a string and turn it into a 4 byte number, put it in the buffer
    void writeAs4Bytes(const std::string& numString, char* co);
    //take a string and turn it into 2 byte number
    void writeAs2Bytes(const std::string& numString, char* co);
    //take a string and turn it into 1 byte number
    void writeAs1Byte(const std::string& numString, char* co);
    //read 4 bytes from a file and return the int
    static int getIntFrom4Bytes(std::ifstream& fileStream);
    //read 2 bytes from a file and return the int
    static int getIntFrom2Bytes(std::ifstream& fileStram);
    //read one byte from a file and return the int
    static int getIntFrom1Byte(std::ifstream& fileStream);
    //read the errors in a transaction error and produce the appropriate error byte
    static unsigned char errorsToChar(const std::string& errors);
    //read the transaction type and return the appropriate type byte
    static unsigned char typeToChar(const std::string& typeS);
    //get the integer representing the zip code from the string
    static unsigned int zipStringToUInt(const std::string& zipS);
    //get the integer representing the transaction amount from the string
    static int amountStringToInt(const std::string& amountS);
    //time string to integer
    static unsigned int timeStringToUInt(const std::string& timeS);
    //fraud to fraud byte
    static unsigned char fraudToByte(const std::string& fraudS);
    /*
        take an entire transaction record ascii string and turn it into the
        compact format, return the non-ascii char array? The array is passed
        as a parameter to the function, that's why we return void
    */
   void asciiTRtoCDF(const std::string& asciiRec, char* compactRec);

    /*
        take the transactions record file and turn it into the reduced 
        transactions record file, the files are already class members
    */
    void compactTransactions();

    /*
        we're gonne need to produce a map of the cities, states and merchants to IDs
        so we can use it to look up these values for the xmls
    */
};

/*
    gonna need some kind of CDF in-memory type so we can sort in memory with 
    different comparators. Maybe not, just a transactionRecord from CDF function
    and then transaction record gets the comparator
*/
#endif