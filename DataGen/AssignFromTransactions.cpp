/*
    This is the program that takes the flat file and the list of user names
    and creates a data structure of UserRecords with unique UserIDs mapped 
    to names and emails. Currently it's just names, but eventually it will 
    be users, cards, merchants and states.

    Maybe just write the xml file first and the server just returns it?
*/

#include <set>
#include <unordered_map>
#include <string>
#include "assignFuncs.hpp"
#include <tinyxml2.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory> //unique_ptr

/*
    parallelization stuff someday. Remember that we can probably just have the
    main thread pull lines from the file into a stream, and then pass the streams
    to the workers? Probably do my cheap shit: main thread knows all the queues, workers
    only know their own queue, wait on a semaphore, when it's time to work, they run
    the function on the stringstream in their queue?
*/

int main(int argc, char* argv[]){ //argv[1] is the flat input file
    //keep track of the transaction values as we find them for use in other field
    unsigned int userKey, cardKey, idNum, nextLocationIndex = 0;
    std::string merchantNameKey, cityKey, stateKey, zipKey, mccKey;

    //open input files
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
    //open the card numbers file
    std::ifstream cardsInFile("Cards.txt", std::ifstream::in);
    if (!cardsInFile.is_open()){
        std::cout << "Couldn't open cards file" <<std::endl;
        exit(1);
    }

    std::string line, userID, nameLine;
    //sets for checking for uniqueness
    std::unordered_map<unsigned int,std::shared_ptr<UserRecord>> idMap;
    std::set<std::pair<unsigned int, unsigned int>> cardSet;
    std::set<std::string> merchantSet;
    auto locationComp = [](const LocationRecord& a, const LocationRecord& b){
        if (a.getState() < b.getState())return true;
        if (a.getCity() < b.getCity()) return true;
        return a.getZip() < b.getZip();
    };
    std::set<LocationRecord, decltype(locationComp)> locationSet(locationComp);
    //vectors for holding the record type
    std::vector<std::shared_ptr<UserRecord>> users;
    std::vector<std::unique_ptr<MerchantRecord>> merchants;
    std::vector<std::unique_ptr<CardRecord>> cards;
    std::vector<std::unique_ptr<LocationRecord>> locations;
    LocationRecord location;
    //open the output file where the user records go: serialized? HAPPENS IN WRITE METHOD
    //skip the first line because it's headers
    while(getline(flatFile, line))
    {
        getline(flatFile, line);
        if (line.empty()) continue; //we're gonne need to check for the blank fields too
        //make a stream from the line
        std::stringstream streamLine(line);
        std::getline(streamLine,userID,','); //this is the user ID filed
        //turn userID into an unsigned int
        idNum = stoul(userID);
        userKey = idNum;
        //search the set for the ID
        auto it = idMap.find(idNum);
        if (it == idMap.end())
        {
             /*
                it's not already in the set, so build a new UserRecord with
                the ID and the next name from the names file, and write it 
                out to the user record file
            */
           getline(nameFile, nameLine);
           std::stringstream nameStream(nameLine);
            //make new user record with all the members
           //push it to some data structure
            std::string fName, lName, eMail;
            nameStream >> fName;
            nameStream >> lName;
            nameStream >> eMail;
            std::shared_ptr<UserRecord> uptr(new UserRecord(fName,
                lName, eMail, idNum));
            idMap.insert({idNum, uptr});
            users.push_back(std::move(uptr));
        }
        std::getline(streamLine,nameLine,','); //credit card
        //we now have the value of the card
        cardKey = stoul(nameLine);
        //make a pair with the user and the card number
        std::pair<unsigned int, unsigned int> ucPair{userKey,cardKey};
        //search the UM for the pair key
        std::set<std::pair<unsigned int, unsigned int>>::iterator cc_it = cardSet.find(ucPair);
        if (cc_it == cardSet.end())
        {

            //add it to the set
            cardSet.insert(ucPair);
            //make a new card from the card file
            getline(cardsInFile, nameLine);
            std::stringstream cardStream(nameLine);
            //make new user record with all the members
           //push it to some data structure
            std::string cardNumber, expMonth, expYear;
            cardStream >> cardNumber;
            cardStream >> expMonth;
            cardStream >> expYear;
            std::unique_ptr<CardRecord> ccr_ptr(new CardRecord(ucPair.first,ucPair.second, cardNumber));
            //put it into the cardRecords vector
            cards.push_back(std::move(ccr_ptr));
        }

        std::getline(streamLine,nameLine,','); //year
        std::getline(streamLine,nameLine,','); //month
        std::getline(streamLine,nameLine,','); //day
        std::getline(streamLine,nameLine,','); //time
        std::getline(streamLine,nameLine,','); //amount
        std::getline(streamLine,nameLine,','); //use chip

        std::getline(streamLine,nameLine,','); //merchant name
        merchantNameKey = nameLine;
        //search the set for it
        auto mit = merchantSet.find(merchantNameKey);
        if (mit == merchantSet.end())
        {
            merchantSet.insert(merchantNameKey);
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
            std::unique_ptr<MerchantRecord> uptr(new MerchantRecord(fullName, mcc, merchantNameKey));
            merchants.push_back(std::move(uptr)); //have to explicitly move uptr
        }
        std::getline(streamLine,nameLine,','); //merchant city, can be ONLINE
        cityKey = nameLine;
        std::getline(streamLine,nameLine,','); //merchant state; if city is ONLINE, this blank
        stateKey = nameLine;
        std::getline(streamLine,nameLine,','); //merchant zip; if city is ONLINE, this is blank
        
        /* There's an error here with the comparator
        zipKey = nameLine;
        location.setCity(cityKey);
        location.setState(stateKey);
        location.setZip(zipKey);
        //check the set, insert and add it if it it's not there
        auto l_it = locationSet.find(location);
        if (l_it == locationSet.end()){
            locationSet.insert(location);
            std::unique_ptr<LocationRecord> uptr(new LocationRecord(cityKey,
                stateKey,zipKey,nextLocationIndex++));
            locations.push_back(std::move(uptr));
        }
        */
        std::getline(streamLine,nameLine,','); //merchant MCC
        std::getline(streamLine,nameLine,','); //error, blank for none
        std::getline(streamLine,nameLine,','); //fraud            
    }
    //dump the entire structures out to the output files
    //TODO these need to be written to a binary file to save space
    writeUsersToXML(users);
    //writeUsersToSerial(users);
    writeMerchantsToXML(merchants);
    //writeMerchantsToSerial(merchants);
    //writeTransactionsToSerial(transactions);
    writeCardsToXML(cards);
    //writeCardsToSerial(cards);
    writeLocationsToXML(locations);
    flatFile.close();
    nameFile.close();
    merchantsInFile.close();
    cardsInFile.close();
    //xml files are closed by the library
    return 0;
}
