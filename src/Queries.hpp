#ifndef QUERIES_HPP
#define QUERIES_HPP
/*
    header files for query functions
*/
#include <fstream>
#include <string>
#include <iostream>

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
#endif