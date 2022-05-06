#ifndef MERCHANTRECORD_HPP
#define MERCHANTRECORD_HPP
#include <string>
/*
    Header for Merchant Records
*/
class MerchantRecord{
    std::string name;
    std::string mcc;
    std::string docCode; //this is for the key in the file for now
    //probably need an ID in the future
    public:
    std::string getDocCode()const {
        return docCode;
    }
    void setDocCode(const std::string& newDocCode){
        docCode = newDocCode;
    }
    std::string getName() const{
        return name;
    }
    void setName(const std::string& newName){
        name = newName;
    }
    std::string getMCC() const{
        return mcc;
    }
    void setMCC(const std::string& newMCC){
        mcc = newMCC;
    }
    //default constructor
    MerchantRecord(): name(""), mcc(""), docCode(""){}
    //parameterized constructor
    MerchantRecord(const std::string& newName, const std::string& newMCC, 
        const std::string& newDC):name(newName),mcc(newMCC),docCode(newDC){}
    //copy constructor
    MerchantRecord(const MerchantRecord& nMerchant):name(nMerchant.getName()),
    mcc(nMerchant.getMCC()),docCode(nMerchant.getDocCode()){}
};
#endif