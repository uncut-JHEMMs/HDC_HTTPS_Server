#ifndef DATAENRICHER_HPP
#define DATAENRICHER_HPP
/*
    Class whose job it is to take a flat file and enrich the users, the cards
    and the merchants. Takes an un-enriched file of transactions and files 
    with users, cards and merchants, and produces an enriched file of
    transactions, as well as possibly the other files, but those aren't a
    priority right now.
*/
#include <string>
#include <set>
#include <unordered_map>
#include <fstream>
#include <memory>
#include "UserRecord.hpp"
#include "CardRecord.hpp"
#include "LocationRecord.hpp"
#include "MerchantRecord.hpp"
#include "TransactionRecord.hpp"

class DataEnricher
{
    //files and associated streams
    //std::string flatFileName;
    std::ifstream flatFileStream;
    //std::string transactionsOutFileName;
    std::ofstream transactionsOutStream;
    //std::string usersInFileName;
    std::ifstream usersInStream;
    //std::string cardsInFileName;
    std::ifstream cardsInStream;
    //std::string merchantsInFileName;
    std::ifstream merchantsInStream;
    //std::string usersOutFileName;
    std::ofstream usersOutStream;
    //std::string cardsOutFileName;
    std::ofstream cardsOutStream;
    //std::string merchantsOutFileName;
    std::ofstream merchantsOutStream;

    //the sets of things we're enriching
    std::unordered_map<unsigned int,std::shared_ptr<UserRecord>> idMap;
    std::set<std::string> merchantSet;
    public:
    //constructor opens the files
    DataEnricher();
    DataEnricher(const std::string& flatFile, const std::string& enrichedFile,
    const std::string& usersInFile, const std::string& cardsInFile, const std::string&
    merchantsInFile, const std::string& usersOutFile, const std::string& cardsOutFile,
    const std::string& merchantsOutFile);
    //destructor writes to all the output files and closes them
    ~DataEnricher(); //write all output files, close all files
    //enrich method does the work.
    void enrich();
    //need the write methods?
};

#endif