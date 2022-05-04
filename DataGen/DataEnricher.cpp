/*
    Implementation file for data enricher
*/
#include "DataEnricher.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

DataEnricher::DataEnricher()
{
    //default doesn't do anything, here just to check make
}

/*
    parameterized opens all the files and checks for errrors
*/
DataEnricher::DataEnricher(const std::string& flatFile, const std::string& enrichedFile,
    const std::string& usersInFile, const std::string& cardsInFile, const std::string&
    merchantsInFile, const std::string& usersOutFile, const std::string& cardsOutFile,
    const std::string& merchantsOutFile): flatFileStream(flatFile), 
    transactionsOutStream(enrichedFile), usersInStream(usersInFile), cardsInStream(cardsInFile),
    merchantsInStream(merchantsInFile), usersOutStream(usersOutFile), cardsOutStream(cardsOutFile),
    merchantsOutStream(merchantsOutFile)
{
    //check that all files are open
    if (!flatFileStream.is_open()){
        std::cout << "Couldn't open flat file" <<std::endl;
        exit(1);
    }
    if (!transactionsOutStream.is_open()){
        std::cout << "Couldn't open transactions out file" <<std::endl;
        exit(1);
    }
    if (!usersInStream.is_open()){
        std::cout << "Couldn't open users in file" <<std::endl;
        exit(1);
    }
    if (!usersOutStream.is_open()){
        std::cout << "Couldn't open users out file" <<std::endl;
        exit(1);
    }
    if (!cardsInStream.is_open()){
        std::cout << "Couldn't open cards in file" <<std::endl;
        exit(1);
    }
    if (!cardsOutStream.is_open()){
        std::cout << "Couldn't open cards out file" <<std::endl;
        exit(1);
    }
    if (!merchantsInStream.is_open()){
        std::cout << "Couldn't open merchants in file" <<std::endl;
        exit(1);
    }
    if (!merchantsOutStream.is_open()){
        std::cout << "Couldn't open merchants out file" <<std::endl;
        exit(1);
    }
}

/*
    write to all the files and close them
*/
DataEnricher::~DataEnricher()
{
    //need writing here?

    usersInStream.close();
    cardsInStream.close();
    merchantsInStream.close();
    flatFileStream.close();
    usersOutStream.close();
    cardsOutStream.close();
    merchantsOutStream.close();
    transactionsOutStream.close();
}

/*
    do the work of enriching the data. The files are already open and the sets
    and stuff exist, just go through the lines of the flat file and assign all
    the members of things
*/
void DataEnricher::enrich()
{
    // members of the transaction record
    unsigned int userKey, cardKey;
    std::string userID, merchantNameKey, cityKey, stateKey, zipKey, mccKey, line, nameLine, fullName;
    std::string year, month, day, time, amount, type, errors, fraud;
    std::string ccNum, merchantFullName;
    CardRecord cardRec;
    //need these to write to file later
    std::vector<std::shared_ptr<UserRecord>> users;
    std::vector<std::unique_ptr<MerchantRecord>> merchants;
    std::vector<std::unique_ptr<CardRecord>> cards;
    //comparator for card set, as well as card set
    /*
    auto cardComp = [](const CardRecord& a, const CardRecord& b){
        if (a.getUserKey() < b.getUserKey()) return true;
        return a.getCardKey() < b.getCardKey();
    };
    */
    std::set<CardRecord/*, decltype(cardComp)*/> cardSet/*(cardComp)*/;

    //actually iterate through the file and start assigning things
    getline(flatFileStream, line);
    while(getline(flatFileStream, line))
    {
        if (line.empty()) break; //we're gonne need to check for the blank fields too
        //make a stream from the line
        std::stringstream streamLine(line);
        std::getline(streamLine,userID,','); //this is the user ID field
        //turn userID into an unsigned int
        userKey = stoul(userID);
        //userKey = idNum;
        //search the set for the ID
        auto it = idMap.find(userKey);
        if (it == idMap.end())
        {
             /*
                it's not already in the set, so build a new UserRecord with
                the ID and the next name from the names file, and write it 
                out to the user record file
            */
           getline(usersInStream, nameLine);
           std::stringstream nameStream(nameLine);
            //make new user record with all the members
           //push it to some data structure
            std::string fName, lName, eMail;
            nameStream >> fName;
            nameStream >> lName;
            nameStream >> eMail;
            std::shared_ptr<UserRecord> uptr(new UserRecord(fName,
                lName, eMail, userKey));
            idMap.insert({userKey, uptr});
            users.push_back(std::move(uptr));
            //gett the full user name here
            nameStream.str("");
            nameStream.clear();
            nameStream << fName << " " << lName;
            fullName = nameStream.str(); // now fullname has what we want for transaction
        }
        else
        {
            //search the vector of users by id and set full name from the return of find_if
            auto usersIt = std::find_if(users.begin(), users.end(), 
                [&it](const std::shared_ptr<UserRecord>& uptr)
                {
                    return uptr->getUserID() == it ->first;
                });
            std::stringstream nameStream;
            nameStream << usersIt->get()->getFirstName() << " " << usersIt->get()->getLastName();
            fullName = nameStream.str(); 
        }
        std::getline(streamLine,nameLine,','); //credit card
        //we now have the value of the card
        cardKey = stoul(nameLine);
        //make a CardRecord with the user and the card number
        cardRec.setUserKey(userKey);
        cardRec.setCardKey(cardKey);
        //search the UM for the pair key
        auto cc_it = cardSet.find(cardRec);
        /*
            TODO there's a bug in the comparator: if userID is above 0, the below
            comparison returns true always. Just hide it for now. Need to figure out
            these comparator issues.
        */
        if (cc_it == cardSet.end())
        {
            //add it to the set
            cardSet.insert(cardRec);
            //make a new card from the card file
            getline(cardsInStream, nameLine);
            std::stringstream cardStream(nameLine);
            //make new user record with all the members
           //push it to some data structure
            std::string cardNumber, expMonth, expYear;
            cardStream >> cardNumber;
            cardStream >> expMonth;
            cardStream >> expYear;
            std::unique_ptr<CardRecord> ccr_ptr(new CardRecord(userKey, cardKey, cardNumber));
            //put it into the cardRecords vector
            cards.push_back(std::move(ccr_ptr));
            ccNum = cardNumber;
        }
        else
        {
            auto cardsIt = std::find_if(cards.begin(), cards.end(), 
                [&cc_it](const std::unique_ptr<CardRecord>& uptr)
                {
                    return (uptr->getCardKey() == cc_it->getCardKey() && 
                    uptr->getUserKey() == cc_it->getUserKey());
                });
            ccNum = cardsIt->get()->getCardNumber();
        }

        std::getline(streamLine,nameLine,','); //year
        year = nameLine;
        std::getline(streamLine,nameLine,','); //month
        month = nameLine;
        std::getline(streamLine,nameLine,','); //day
        day = nameLine;
        std::getline(streamLine,nameLine,','); //time
        time = nameLine;
        std::getline(streamLine,nameLine,','); //amount
        amount = nameLine;
        std::getline(streamLine,nameLine,','); //use chip
        type = nameLine;

        std::getline(streamLine,nameLine,','); //merchant name
        merchantNameKey = nameLine;
        //search the set for it
        auto mit = merchantSet.find(merchantNameKey);
        if (mit == merchantSet.end())
        {
            merchantSet.insert(merchantNameKey);
            //now we add a new merchant from the file
            std::string merchantName;
            getline(merchantsInStream, merchantName); //no variable re-use allowed
            std::stringstream merchantStream(merchantName);
            std::string name, product, place, mcc, fullName;
            merchantStream >> name;
            merchantStream >> product;
            merchantStream >> place;
            merchantStream >> mcc;
            merchantStream.str("");
            merchantStream.clear();
            merchantStream << name << " " << product << " " << place;
            fullName = merchantStream.str();
            merchantFullName = fullName;
            std::unique_ptr<MerchantRecord> uptr(new MerchantRecord(fullName, mcc, merchantNameKey));
            merchants.push_back(std::move(uptr)); //have to explicitly move uptr
        }
        else
        {
            auto mnit = std::find_if(merchants.begin(), merchants.end(), 
            [&mit](const std::unique_ptr<MerchantRecord>& uptr)
                {
                    return (uptr->getDocCode() == *mit);
                });
            merchantFullName = mnit->get()->getName();
        }
        std::getline(streamLine,nameLine,','); //merchant city, can be ONLINE
        cityKey = nameLine;
        std::getline(streamLine,nameLine,','); //merchant state; if city is ONLINE, this blank
        stateKey = nameLine;
        if (stateKey.empty()) stateKey = "";
        std::getline(streamLine,nameLine,','); //merchant zip; if city is ONLINE, this is blank
        zipKey = nameLine;
        if (zipKey.empty())
        { 
            zipKey = "";
        }
        else
        {
            zipKey = nameLine.substr(0,nameLine.length() - 2);
        }
        
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
        mccKey = nameLine;
        std::getline(streamLine,nameLine,','); //error, blank for none
        errors = nameLine;
        if (errors.empty()) errors = "";
        std::getline(streamLine,nameLine,','); //fraud
        fraud = nameLine;

        /*
            now we've got everything, build a transaction record or just write
            it out to the transaction file?
        */
       transactionsOutStream << fullName << "," << ccNum << "," << year <<","<<
       month<<","<<day<<","<<time<<","<<amount<<","<<type<<","<<merchantFullName<<
       ","<<cityKey<<","<<stateKey<<","<<zipKey<<","<<mccKey<<","<<errors<<","<<fraud<<
       std::endl;
    }
}