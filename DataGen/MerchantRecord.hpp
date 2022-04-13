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

    public:
    std::string getDocCode(){
        return docCode;
    }
    void setDocCode(const std::string& newDocCode){
        docCode = newDocCode;
    }
    std::string getName(){
        return name;
    }
    void setName(const std::string& newName){
        name = newName;
    }
    std::string getMCC(){
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

};
#endif