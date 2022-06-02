/*
    Implementation file for queries
*/
#include <string>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include "Queries.hpp"
#include "tinyxml2.h"
#include <iomanip>

#define TEMPFILENAME "tempfile.xml"
#define FRAUDROWSTART 18
/*
    Respond to the "Percentage of Fraud By Year" query:
    open the stats file, navigate down to the 19th line (where the year frauds
    are), then get each year (entry 2), transactions (entry 4), and fraud 
    (entry 6). Do floating point division to get a percentage, then make and
    return the xml document.
*/
std::string getFraudByYear(const std::string statsFileName)
{
    std::vector<YearPercentFraud> vec = populateVectorFBY(statsFileName);
    std::string outFileName = buildXML(vec);
    return outFileName;
}

/*
    Respond to "Users with Insufficient Balance" stories. Navigate to line 3,
    item 2 to get the total number of users. Navigate to line 14, item 2 for 
    no insufficient balance errors, line 15, item 2 for 1 or more, line 16, item
    2 for more than 1
*/
std::string getUIB(const std::string statsFileName)
{
    std::ifstream dbFile(statsFileName.c_str(), std::ifstream::in);
    if (!dbFile.is_open()){
    std::string error = "couldn't open stats file";
    return error;
    }
    std::string inputLine, inputVal;
    std::stringstream sstream;
    //head to line 3
    std::getline(dbFile, inputLine);
    std::getline(dbFile, inputLine);
    std::getline(dbFile, inputLine);
    sstream.str(inputLine);
    sstream >> inputVal;
    sstream >> inputVal; //total users
    float totalUsers = stof(inputVal);
    //head down to the insufficient numbers
    for (int i = 0 ; i < 11; ++i)
    {
        std::getline(dbFile, inputLine);
    }
    sstream.str("");
    sstream.clear();
    sstream.str(inputLine);
    sstream >> inputVal;
    sstream >> inputVal; //0 IB
    float noIB = stof(inputVal);
    std::getline(dbFile, inputLine);
    sstream.str("");
    sstream.clear();
    sstream.str(inputLine);
    sstream >> inputVal;
    sstream >> inputVal; //1 or more IB
    float oneplusIB = stof(inputVal);
    std::getline(dbFile, inputLine);
    sstream.str("");
    sstream.clear();
    sstream.str(inputLine);
    sstream >> inputVal;
    sstream >> inputVal; //more than 1 IB
    float twoplusIB = stof(inputVal);
    float percentNo = (noIB * 100.0) / totalUsers;
    float percentOnePlus = (oneplusIB * 100.00) / totalUsers;
    float percentTwoPlus = (twoplusIB * 100.00) / totalUsers;
    sstream.str("");
    sstream.clear();
    sstream << std::fixed << std::setprecision(3) << percentNo;
    //make xml doc
    tinyxml2::XMLDocument doc;
    //create a node
    tinyxml2::XMLNode* pRoot = doc.NewElement("InsufficientBalances");
    //attach the node to the document
    doc.InsertFirstChild(pRoot);
    //now add children to root
    tinyxml2::XMLElement* pElement;
    pElement = doc.NewElement("Record");
    pElement->SetAttribute("Errors", "None");
    pElement->SetText(sstream.str().c_str());
    pRoot->InsertEndChild(pElement);
    pElement = doc.NewElement("Record");
    pElement->SetAttribute("Errors", "OnePlus");
    sstream.clear();
    sstream.str("");
    sstream << std::fixed << std::setprecision(3) << percentOnePlus;
    pElement->SetText(sstream.str().c_str());
    pRoot->InsertEndChild(pElement);
    pElement = doc.NewElement("Record");
    pElement->SetAttribute("Errors", "TwoPlus");
    sstream.clear();
    sstream.str("");
    sstream << std::fixed << std::setprecision(3) << percentTwoPlus;
    pElement->SetText(sstream.str().c_str());
    pRoot->InsertEndChild(pElement);
    //save xml document and return the filename
    doc.SaveFile(TEMPFILENAME);
    std::string tempFile{TEMPFILENAME};
    return tempFile;
}

/*
    returns a string that lists all the transaction types, maybe in
    a JSON-like array
*/
std::string getTransactionTypes(const std::string& dbFileName)
{
    //open the file
    std::ifstream dbFile(dbFileName.c_str(), std::ifstream::in);
    if (!dbFile.is_open()){
    std::string error = "couldn't open transaction types input file";
    return error;
    }
    //need a set of strings
    std::set<std::string> resultSet;
    //for every row,
    std::string inputLine, val;
    while (getline(dbFile, inputLine))
    {
        std::stringstream lineStream(inputLine);
        //navigate to column 8 (the transaction types column)
        std::getline(lineStream,val,','); //user ID
        std::getline(lineStream,val,','); //card id
        std::getline(lineStream,val,','); //year
        std::getline(lineStream,val,','); //month
        std::getline(lineStream,val,','); //day
        std::getline(lineStream,val,','); //time
        std::getline(lineStream,val,','); //amount
        std::getline(lineStream,val,','); // type
        //insert every entry into the set
        resultSet.insert(val);
    }

    //when done, make a string from a stringstream of every element in the set
    std::stringstream finalResult;
    //make xml doc
    tinyxml2::XMLDocument doc;
    //create a node
    tinyxml2::XMLNode* pRoot = doc.NewElement("Transaction Types");
    //attach the node to the document
    doc.InsertFirstChild(pRoot);
    //for every item in the vector, put in nodes for id, first name, last name, email
    tinyxml2::XMLElement* pElement;
    //add the header
    pElement = doc.NewElement("Count");
    finalResult << "Unique transaction types in database: " << resultSet.size();
    pElement->SetText(finalResult.str().c_str());
    pRoot->InsertEndChild(pElement);
    for (std::string s : resultSet){
        //add each unique type as an element
        pElement = doc.NewElement("Transaction Type");
        pElement->SetText(s.c_str());
        pRoot->InsertEndChild(pElement);
    }
    //close the database file
    dbFile.close();
    //save the document and return the filename
    doc.SaveFile(TEMPFILENAME);
    std::string tempFile{TEMPFILENAME};
    return tempFile;
}

/* 
    returns the number of unique merchants in the file
*/
std::string getNumberOfMerchants(const std::string& dbFileName)
{
    //open the file
    std::ifstream dbFile(dbFileName.c_str(), std::ifstream::in);
    if (!dbFile.is_open()){
    std::string error{"couldn't open merchant number input file"};
    return error;
    }
    //need a set of strings
    std::set<std::string> resultSet;
    //for every row,
    std::string inputLine, val;
    while (getline(dbFile, inputLine))
    {
        //navigate to column 9 (merchant name)
        std::stringstream lineStream(inputLine);
        std::getline(lineStream,val,','); //user ID
        std::getline(lineStream,val,','); //card id
        std::getline(lineStream,val,','); //year
        std::getline(lineStream,val,','); //month
        std::getline(lineStream,val,','); //day
        std::getline(lineStream,val,','); //time
        std::getline(lineStream,val,','); //amount
        std::getline(lineStream,val,','); // type
        std::getline(lineStream,val,','); //merchant name
        //insert every entry into the set
        resultSet.insert(val);
    }
    //when done, count the size of the set
    //put all the merchants in a file
    dbFile.close();
    std::stringstream result;
    tinyxml2::XMLDocument doc;
    //create a node
    tinyxml2::XMLNode* pRoot = doc.NewElement("Merchant Count");
    //attach the node to the document
    doc.InsertFirstChild(pRoot);
    tinyxml2::XMLElement* pElement;
    //add the header
    result << "Unique merchants in database: " << resultSet.size();
    std::string sres = result.str();
    pElement = doc.NewElement("Count");
    pElement->SetText(sres.c_str());
    pRoot->InsertEndChild(pElement);
    for (std::string s : resultSet){
        //add each unique type as an element
        pElement = doc.NewElement("Merchant");
        pElement->SetText(s.c_str());
        pRoot->InsertEndChild(pElement);
    }
    //save the file
    doc.SaveFile(TEMPFILENAME);
    std::string tempFile{TEMPFILENAME};
    return tempFile;
}

/*
    returns a string that has all of the transactions by a given user
    in a file
*/
std::string getUserTransactions(const std::string& dbFileName, const std::string& userName)
{
    //open the file
    std::ifstream dbFile(dbFileName.c_str(), std::ifstream::in);
    if (!dbFile.is_open()){
    std::string error = "couldn't open user transaction input file";
    return error;
    }
    //need a set of strings
    std::set<std::string> resultSet;
    std::string inputLine, val;
    int count = 0;
    tinyxml2::XMLDocument doc;
    //create a node
    tinyxml2::XMLNode* pRoot = doc.NewElement("User Transactions");
    //attach the node to the document
    doc.InsertFirstChild(pRoot);
    tinyxml2::XMLElement* pElement;
    std::stringstream result;
    //add the header
    result << "Transactions by User: " << userName;
    std::string sres = result.str();
    pElement = doc.NewElement("Title");
    pElement->SetText(sres.c_str());
    pRoot->InsertEndChild(pElement);
    while (getline(dbFile, inputLine))
    {
        //check column 1
        std::stringstream lineStream(inputLine);
        std::string val;
        std::getline(lineStream,val,','); //user ID
        //if it matches username, add the row to a stringstream
        if (val == userName){
            result << inputLine << std::endl;
            tinyxml2::XMLNode* pNode = doc.NewElement("Transaction");
            //credit card
            std::getline(lineStream,val,',');
            pElement = doc.NewElement("Credit Card");
            pElement->SetText(val.c_str());
            pNode->InsertEndChild(pElement);
            //year
            std::getline(lineStream,val,',');
            pElement = doc.NewElement("Year");
            pElement->SetText(val.c_str());
            pNode->InsertEndChild(pElement);
            //month
            std::getline(lineStream,val,',');
            pElement = doc.NewElement("Month");
            pElement->SetText(val.c_str());
            pNode->InsertEndChild(pElement);
            //day
            std::getline(lineStream,val,',');
            pElement = doc.NewElement("Day");
            pElement->SetText(val.c_str());
            pNode->InsertEndChild(pElement);
            //time
            std::getline(lineStream,val,',');
            pElement = doc.NewElement("Time");
            pElement->SetText(val.c_str());
            pNode->InsertEndChild(pElement);
            //amount
            std::getline(lineStream,val,',');
            pElement = doc.NewElement("Amount");
            pElement->SetText(val.c_str());
            pNode->InsertEndChild(pElement);
            //type
            std::getline(lineStream,val,',');
            pElement = doc.NewElement("Transaction Type");
            pElement->SetText(val.c_str());
            pNode->InsertEndChild(pElement);
            //merchant name
            std::getline(lineStream,val,',');
            pElement = doc.NewElement("Merchant");
            pElement->SetText(val.c_str());
            pNode->InsertEndChild(pElement);
            //merchant city
            std::getline(lineStream,val,',');
            pElement = doc.NewElement("City");
            if (val.empty()) val = "";
            pElement->SetText(val.c_str());
            pNode->InsertEndChild(pElement);
            //merchant state
            std::getline(lineStream,val,',');
            pElement = doc.NewElement("State");
            if (val.empty()) val = "";
            pElement->SetText(val.c_str());
            pNode->InsertEndChild(pElement);
            //merchant zip
            std::getline(lineStream,val,',');
            pElement = doc.NewElement("Zip Code");
            if (val.empty()) val = "";
            pElement->SetText(val.c_str());
            pNode->InsertEndChild(pElement);
            //mcc
            std::getline(lineStream,val,',');
            pElement = doc.NewElement("MCC");
            pElement->SetText(val.c_str());
            pNode->InsertEndChild(pElement);
            //errors
            std::getline(lineStream,val,',');
            pElement = doc.NewElement("Errors");
            pElement->SetText(val.c_str());
            pNode->InsertEndChild(pElement);
            //fraud
            std::getline(lineStream,val,',');
            pElement = doc.NewElement("Fraud");
            pElement->SetText(val.c_str());
            pNode->InsertEndChild(pElement);
            count++;
            pRoot->InsertEndChild(pNode);
        }
    }
    //when done, return the string version of the stream
    //count
    pElement = doc.NewElement("Count");
    result.str("");
    result.clear();
    result << "Total number of transactions by user: " << count;
    sres = result.str();
    pElement->SetText(sres.c_str());
    pRoot->InsertEndChild(pElement);
    dbFile.close();
    doc.SaveFile(TEMPFILENAME);
    std::string tempFile{TEMPFILENAME};
    return tempFile;
}
std::vector<YearPercentFraud> populateVectorFBY(const std::string statsFileName)
{
    std::vector<YearPercentFraud> vec;
    std::ifstream dbFile(statsFileName.c_str(), std::ifstream::in);
    /* TODO This has to be fixed, there's no error checking now
    if (!dbFile.is_open()){
    std::string error = "couldn't open stats file";
    return error;
    }
    */

    std::string inputLine, inputVal;
    std::stringstream sstream;
    //navigate to line 19
    for (int i = 0; i < FRAUDROWSTART; ++i) //only go to 18 so we start looping at the next one
    {
        std::getline(dbFile,inputLine);
    }
    //get the values and populate vector: 2 for year, 4 for total, 6 for fraud
    while (std::getline(dbFile, inputLine))
    {
     sstream.str("");
     sstream.clear();
     sstream.str(inputLine);
     sstream >> inputVal;
     sstream >> inputVal; //year
     //yPF.yearString = inputVal;
     std::string yearString = inputVal;
     sstream >> inputVal;
     sstream >> inputVal; //total transactions
     float totalTrans = std::stof(inputVal);
     sstream >> inputVal;
     sstream >> inputVal;
     float totalFraud = std::stof(inputVal);
     float percentFraud = (totalFraud * 100) / totalTrans;
     sstream.str("");
     sstream.clear();
     sstream << std::fixed << std::setprecision(3) << percentFraud << "%";
     sstream >> inputVal;
     vec.emplace_back(yearString, inputVal);
    }
    return vec;
}

/*
    build an xml file from the fby vector, return the filename
*/
std::string buildXML(const std::vector<YearPercentFraud>& vec)
{
    //make xml doc
    tinyxml2::XMLDocument doc;
    //create a node
    tinyxml2::XMLNode* pRoot = doc.NewElement("PercentFraudByYear");
    //attach the node to the document
    doc.InsertFirstChild(pRoot);
    //now add children to root
    tinyxml2::XMLElement* pElement;
    for (const YearPercentFraud& yPF : vec)
    {
        pElement = doc.NewElement("Record");
        pElement->SetAttribute("Year",yPF.yearString.c_str());
        pElement->SetText(yPF.percentFraud.c_str());
        pRoot->InsertEndChild(pElement);
    }
    //save xml document and return the filename
    doc.SaveFile(TEMPFILENAME);
    std::string tempFile{TEMPFILENAME};
    return tempFile;
}

/*
    Method that services the "Top 10 largest transactions" query. Returns a vector
    of transactions that gets turned into an xml document later
*/
/*
void top10largestTransactions(std::vector<TransactionRecord>& vec, const std::string& dbFileName)
{
    //look through the input file
     //open the file
    std::ifstream dbFile(dbFileName.c_str(), std::ifstream::in);
    if (!dbFile.is_open()){
        //make this more compatible
        std::string error{"couldn't open largest transactions input file"};
        throw error;
    }
    //for every row,
    std::string inputLine, val;
    TransactionRecord tRec;
    while (getline(dbFile, inputLine))
    {
        std::stringstream lineStream(inputLine);
        //check that the amount in the one I'm looking at is larger than at least one of the ones in the vector
        std::getline(lineStream,val,','); //user ID
        std::getline(lineStream,val,','); //card id
        std::getline(lineStream,val,','); //year
        std::getline(lineStream,val,','); //month
        std::getline(lineStream,val,','); //day
        std::getline(lineStream,val,','); //time
        std::getline(lineStream,val,','); //amount
        std::getline(lineStream,val,','); // type
        
    }

        //update the element that it's greater than with the current transaction record
    }
    //after I've looked through the entire file, then we're done
}
*/