#ifndef USERRECORD_HPP
#define USERRECORD_HPP
/*
    Header file for user Records
*/
#include <string>
#include <sstream>

//Users have a name, an ID and an email address (derived from their name)
class UserRecord{
    private:
        std::string firstName;
        std::string lastName;
        std::string emailAddress;
        unsigned int userID;
    public:

    std::string getFirstName() const{
        return firstName;
    }

    void setFirstName(const std::string& name){
        firstName = name;
    }

    std::string getLastName() const{
        return lastName;
    }

    void setLastName(const std::string& name){
        lastName = name;
    }

    std::string getEmail() const{
        return emailAddress;
    }
    
    //this will be a little more complicated where we concatenate the names for the first part
    void setEmail(const std::string& address){
        emailAddress = address;
    }

    unsigned int getUserID() const{
        return userID;
    }

    void setUserID(const unsigned int& id){
        userID = id;
    }
    UserRecord();
    UserRecord(const std::string& first, const std::string& last, 
    const std::string& em, const unsigned int& id);
    //need a comparator function for when we're sorting
    //comparator by ID
    
    static bool IDComp(const UserRecord& a, const UserRecord& b);
    //comparator by email
    bool EmailComp(const UserRecord& other);
    //comparator by first/last name
    bool LastNameComp(const UserRecord& other);
    bool FirstNameComp(const UserRecord& other);
    //populate from file, the steady state file, not the "assign IDs to names" file
    std::string toString() const;
    //write into serial file
    //write into xml file, or return a populated node or something
};

#endif