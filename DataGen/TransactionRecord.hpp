#ifndef TRANSACTIONRECORD_HPP
#define TRANSACTIONRECORD_HPP
/*
    Definitions for Transaction record class, the in-memory version of a
    transaction. This includes the other record types as members
*/

#include "UserRecord.hpp"
#include "CardRecord.hpp"
#include "MerchantRecord.hpp"

/*
    enumeration for the transaction types
*/
enum TransactionType
{
    SWIPE_TRANSACTION = 0,
    CHIP_TRANSACTION = 1,
    ONLINE_TRANSACTION = 2
};

class TransactionRecord
{
    private:
    UserRecord user;
    CardRecord card;
    //non-records need to be replaced by record types when they're done
    unsigned int amount; //amount is in cents, converted to dollars for display
    std::string date;
    std::string time;
    MerchantRecord merchant;
    std::string city;
    std::string state; //blank if city is online
    std::string zipCode; //blank if city is online
    /*
        errors are stored in a single byte (maybe expand later): technical
        glitch is error 1, invalid pin is error 2, bad pin is error 4
    */
    std::byte errors;
    bool isFraud;

    public:
    //default constructor
    TransactionRecord():user(UserRecord()),card(CardRecord()),amount(0),
        date("1/1/2000"),time("00:00"), merchant(MerchantRecord()), city("Nowhere"),
        state("NW"),zipCode("00000"){}

    //parameterized constructor
    TransactionRecord(const UserRecord& nUser, const CardRecord& nCard,
        unsigned int nAmount, const std::string& nDate, const std::string& nTime,
        const MerchantRecord& nMerchant, const std::string& nCity, 
        const std::string& nState, const std::string& nZip): user(nUser), 
        card(nCard), amount(nAmount), date(nDate), time(nTime), 
        merchant(nMerchant), city(nCity), state(nState), zipCode(nZip){}
    //need copy/move constructor?
    
    //getters/setters
    UserRecord getUserRecord() const;
    void setUserRecord(const UserRecord& nUser);
    CardRecord getCardRecord() const;
    void setCardRecord(const CardRecord& nCard);
    unsigned int getAmount() const;
    void setAmount(unsigned int nAmount);
    std::string getDate() const;
    void setDate(const std::string& nDate);
    std::string getTime() const;
    void setTime(const std::string& nTime);
    MerchantRecord getMerchantRecord() const;
    void setMerchantRecord(const MerchantRecord& nMerchant);
    std::string getCity() const;
    void setCity(const std::string& nCity);
    std::string getState() const;
    void setState(const std::string& nState);
    std::string getZip() const;
    void setZip(const std::string& nZip);

    //utility functions?
    //cleanupZip, get rid of teh .0
    //formatTime, turn 6,12, into 06:12
    //formatDate, turn 12,23 into 12/23
};

#endif