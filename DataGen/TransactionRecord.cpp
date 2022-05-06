/*
    Implementation file for transaction record
*/
#include "TransactionRecord.hpp"

    UserRecord TransactionRecord::getUserRecord() const
    {
        return user;
    }
    
    void TransactionRecord::setUserRecord(const UserRecord& nUser)
    {
        user = nUser;
    }

    CardRecord TransactionRecord::getCardRecord() const
    {
        return card;
    }
    
    void TransactionRecord::setCardRecord(const CardRecord& nCard)
    {
        card = nCard;
    }
    unsigned int TransactionRecord::getAmount() const
    {
        return amount;
    }

    void TransactionRecord::setAmount(unsigned int nAmount)
    {
        amount = nAmount;
    }
    
    std::string TransactionRecord::getDate() const
    {
        return date;
    }
    
    void TransactionRecord::setDate(const std::string& nDate)
    {
        date = nDate;
    }
    
    std::string TransactionRecord::getTime() const
    {
        return time;
    }
    
    void TransactionRecord::setTime(const std::string& nTime)
    {
        time = nTime;
    }
    
    MerchantRecord TransactionRecord::getMerchantRecord() const
    {
        return merchant;
    }
    
    void TransactionRecord::setMerchantRecord(const MerchantRecord& nMerchant)
    {
        merchant = nMerchant;
    }
    std::string TransactionRecord::getCity() const
    {
        return city;
    }
    void TransactionRecord::setCity(const std::string& nCity)
    {
        city = nCity;
    }
    std::string TransactionRecord::getState() const
    {
        return state;
    }
    void TransactionRecord::setState(const std::string& nState)
    {
        state = nState;
    }
    std::string TransactionRecord::getZip() const
    {
        return zipCode;
    }
    void TransactionRecord::setZip(const std::string& nZip)
    {
        zipCode = nZip;
    }