/*
    This is the program that takes the flat file and the list of user names
    and creates a data structure of UserRecords with unique UserIDs mapped 
    to names and emails. Currently it's just names, but eventually it will 
    be users, cards, merchants and states.

    Maybe just write the xml file first and the server just returns it?
*/

#include <set>
#include <string>
#include "UserRecord.hpp"
#include "MerchantRecord.hpp"
#include <tinyxml2.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory> //unique_ptr

#define USER_XML_FILENAME "users.xml"
#define MERCHANT_XML_FILENAME "merchants.xml"
#define PLACES_XML_FILENAME "places.xml"

/*
    have a function that operates on a single line: gets the user ID and the
    card ID. Does it take a stream? the stream is also in the line. See below for some ideas
*/

/*
    Write all the users from the users vector to the file
*/
bool writeUsersToXML(std::vector<std::unique_ptr<UserRecord>>& vec){
    //start the xml document
  tinyxml2::XMLDocument doc;
  //create a node
  tinyxml2::XMLNode* pRoot = doc.NewElement("Users");
  //attach the node to the document
  doc.InsertFirstChild(pRoot);
  //for every item in the vector, put in nodes for id, first name, last name, email
  tinyxml2::XMLElement* pElement;
  for (const std::unique_ptr<UserRecord>& uptr : vec){
    // make a new child for root
    tinyxml2::XMLNode* userNode = doc.NewElement("User");
    pRoot->InsertFirstChild(userNode);
    //id
    pElement = doc.NewElement("User ID");
    pElement->SetText(uptr->getUserID());
    userNode->InsertEndChild(pElement);
    //first name
    pElement = doc.NewElement("First Name");
    pElement->SetText(uptr->getFirstName().c_str());
    userNode->InsertEndChild(pElement);
    //last name
    pElement = doc.NewElement("Last Name");
    pElement->SetText(uptr->getLastName().c_str());
    userNode->InsertEndChild(pElement);
    //email
    pElement = doc.NewElement("Email");
    pElement->SetText(uptr->getEmail().c_str());
    userNode->InsertEndChild(pElement);
  }
  //save the document
  doc.SaveFile(USER_XML_FILENAME);
  return true;
}

bool writeMerchantsToXML(std::vector<std::unique_ptr<MerchantRecord>>& merchants){
tinyxml2::XMLDocument doc;
  //create a node
  tinyxml2::XMLNode* pRoot = doc.NewElement("Merchants");
  //attach the node to the document
  doc.InsertFirstChild(pRoot);
  //for every item in the vector, put in nodes for id, first name, last name, email
  tinyxml2::XMLElement* pElement;
  for (auto it = merchants.rbegin(); it != merchants.rend(); it = ++it){
    // make a new child for root
    tinyxml2::XMLNode* userNode = doc.NewElement("Merchant");
    pRoot->InsertFirstChild(userNode);
    //id
    pElement = doc.NewElement("Name");
    pElement->SetText((*it)->getName().c_str());
    userNode->InsertEndChild(pElement);
    //first name
    pElement = doc.NewElement("MCC");
    pElement->SetText((*it)->getMCC().c_str());
    userNode->InsertEndChild(pElement);
    pElement = doc.NewElement("DocCode");
    pElement->SetText((*it)->getDocCode().c_str());
    userNode->InsertEndChild(pElement);
  }
  //save the document
  doc.SaveFile(MERCHANT_XML_FILENAME);
  return true;
}
/*
    parallelization stuff someday. Remember that we can probably just have the
    main thread pull lines from the file into a stream, and then pass the streams
    to the workers? Probably do my cheap shit: main thread knows all the queues, workers
    only know their own queue, wait on a semaphore, when it's time to work, they run
    the function on the stringstream in their queue?
*/

int main(int argc, char* argv[]){ //argv[1] is the flat input file
    //open the files
    std::ifstream flatFile(argv[1], std::ifstream::in);
    if (!flatFile.is_open()){
        std::cout << "Couldn't open input file" <<std::endl;
        exit(1);
    }
    //open the user names file so we can use it
    std::ifstream nameFile("full_names.txt", std::ifstream::in);
    if (!nameFile.is_open()){
        std::cout << "Couldn't open name file" <<std::endl;
        exit(1);
    }
    //open the merchant names file
    std::ifstream merchantsInFile("MerchantNames.txt", std::ifstream::in);
    if (!merchantsInFile.is_open()){
        std::cout << "Couldn't open merchants file" <<std::endl;
        exit(1);
    }
    //std::ofstream outFile("UserRecords.txt", std::ofstream::out);
    //merchants xml is in its function

    //outFile is fine like this, no fail to read
    std::string line, userID, nameLine;
    unsigned int idNum;
    //sets for checking for uniqueness
    std::set<unsigned int> idSet;
    std::set<std::string> merchantSet;
    //vectors for holding the record type
    std::vector<std::unique_ptr<UserRecord>> users;
    std::vector<std::unique_ptr<MerchantRecord>> merchants;
    //open the output file where the user records go: serialized? HAPPENS IN WRITE METHOD
    //skip the first line because it's headers
    while(getline(flatFile, line))
    {
        getline(flatFile, line);
        if (line.empty()) continue; //we're gonne need to check for the blank fields too
        //when we get to the other fields, they just go here along with user stuff
        //make a stream from the line
        //userID = "";
        std::stringstream streamLine(line);
        std::getline(streamLine,userID,','); //this is the user ID filed
        //turn userID into an unsigned int
        idNum = stoul(userID);
        //search the set for the ID
        auto it = idSet.find(idNum);
        if (it == idSet.end())
        {
             /*
                it's not already in the set, so build a new UserRecord with
                the ID and the next name from the names file, and write it 
                out to the user record file
            */
           //put the id into the set
           idSet.insert(idNum);
           //get next name from file
           getline(nameFile, nameLine);
           std::stringstream nameStream(nameLine);
            //make new user record with all the members
           //push it to some data structure
            std::string fName, lName, eMail;
            nameStream >> fName;
            nameStream >> lName;
            nameStream >> eMail;
            std::unique_ptr<UserRecord> uptr(new UserRecord(fName,
                lName, eMail, idNum));
            users.push_back(std::move(uptr));
        }
            std::getline(streamLine,nameLine,','); //credit card
            std::getline(streamLine,nameLine,','); //year
            std::getline(streamLine,nameLine,','); //month
            std::getline(streamLine,nameLine,','); //day
            std::getline(streamLine,nameLine,','); //time
            std::getline(streamLine,nameLine,','); //amount
            std::getline(streamLine,nameLine,','); //use chip
            std::getline(streamLine,nameLine,','); //merchant name
            std::string merchantText = nameLine;
            //search the set for it
            auto mit = merchantSet.find(merchantText);
            if (mit == merchantSet.end()){
                merchantSet.insert(merchantText);
                //now we add a new merchant from the file
                getline(merchantsInFile, nameLine); //note the variable re-use
                std::stringstream merchantStream(nameLine);
                std::string name, product, place, mcc, fullName;
                merchantStream >> name;
                merchantStream >> product;
                merchantStream >> place;
                merchantStream >> mcc;
                merchantStream.str("");
                merchantStream.clear();
                merchantStream << name << " " << product << " " << place;
                fullName = merchantStream.str();
                std::unique_ptr<MerchantRecord> uptr(new MerchantRecord(fullName, mcc, merchantText));
                merchants.push_back(std::move(uptr)); //have to explicitly move uptr
            }

    }

    //dump the entire structures out to the output files
    writeUsersToXML(users);
    std::cout << merchants.size();
    writeMerchantsToXML(merchants);
    


    //close the files
    flatFile.close();
    nameFile.close();
    merchantsInFile.close();
    //xml files are closed by the library
    return 0;
}
