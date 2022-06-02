#ifndef QUERIES_HPP
#define QUERIES_HPP
/*
    header files for query functions
*/
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
//#include "../DataGen/TransactionRecord.hpp"

/*
    type for fraud by year story
*/
struct YearPercentFraud
{
    std::string yearString;
    std::string percentFraud;
    YearPercentFraud(const std::string& string, const std::string& perc):
     yearString(string), percentFraud(perc){}
};

/*
    respond to "fraud by year" query
*/
std::string getFraudByYear(const std::string statsFileName);
/*
    respond to "Users with insufficient balance" query
*/
std::string getUIB(const std::string statsFileName);
/*
    returns a string that lists all the transaction types, maybe in
    a JSON-like array
*/
std::string getTransactionTypes(const std::string& dbFileName);

/* 
    returns the number of unique merchants in the file
*/
std::string getNumberOfMerchants(const std::string& dbFileName);

/*
    returns a string that has all of the transactions by a given user
    in a file
*/
std::string getUserTransactions(const std::string& dbFileName, const std::string& userName);

/*
    Populate FraudByYear's vector and return it
*/
std::vector<YearPercentFraud> populateVectorFBY(const std::string statsFileName);

/*
    build the xml document for reporting yearPercentFraud
*/
std::string buildXML(const std::vector<YearPercentFraud>& vec);
//void top10largestTransactions(std::vector<TransactionRecord>& vec, const std::string& dbFileName);
#endif

