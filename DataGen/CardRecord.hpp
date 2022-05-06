/*
    Header file for credit card record. Credit cards have a user key, a card key, a number
    and and expiration date. I totally forgot about the luhn algorithm stuff, that needs to be 
    implmented.
*/
#ifndef CARDRECORD_HPP
#define CARDRECORD_HPP
class CardRecord{
    unsigned int userKey;
    unsigned int cardKey;
    std::string cardNumber;
    //need expiration date
    public:
    CardRecord():userKey(0),cardKey(0), cardNumber(""){}
    CardRecord(unsigned int user, unsigned int card, std::string number): userKey(user),
    cardKey(card), cardNumber(number){}
    CardRecord(const CardRecord& nCard){
        userKey = nCard.getUserKey();
        cardKey = nCard.getCardKey();
        cardNumber = nCard.getCardNumber();
    }
    unsigned int getUserKey() const{
        return userKey;
    }
    void setUserKey(const unsigned int& user){
        userKey = user;
    }
    unsigned int getCardKey() const{
        return cardKey;
    }
    void setCardKey(const unsigned int& card){
        cardKey = card;
    }
    std::string getCardNumber() const{
        return cardNumber;
    }
    void setCardNumber(const std::string& number){
        cardNumber = number;
    }

    bool operator<(const CardRecord& other) const{
        if (getCardKey() < other.getCardKey()) return true;
        return getUserKey() < other.getUserKey();
    }
};
#endif