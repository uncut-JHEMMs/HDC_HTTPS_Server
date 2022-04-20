/*
    Source file for user records
*/
#include "UserRecord.hpp"
#include "tinyxml2.h" //need this for "print to.."
/*
    default constructor is blank strings and ID 0
*/
UserRecord::UserRecord(){
    firstName = "";
    lastName = "";
    emailAddress = "";
    userID = 0;
}

/*
    overloaded takes a first name, a last name, an email and an ID
*/
UserRecord::UserRecord(const std::string& first, const std::string& last, 
    const std::string& em, const unsigned int& id): firstName(first),
    lastName(last), emailAddress(em), userID(id){/*empty*/}

std::string UserRecord::toString() const{
    std::stringstream res;
    res << userID;
    res << " " << firstName;
    res << " " << lastName;
    res << " " << emailAddress;
    //RVO takes care of this to prevent copies
    return res.str();
}

//all the getters and setters can just stay in the header file, they're small
//remember we're 3 strings and an unsigned int for ID

/*
    Gonna need "serialize into file" here, just use ostream and the fields, or 
    maybe just return a string/xml whatever
*/


/*
    Gonna need "return this object as an xml node": populate all the fields, and then 
    return the xml node so the document does the inserting?
*/

/*
    also going to need "populate object from file" here, this should basically
    be the deserialize from above: think about Jordan's technique: four bytes int,
    one byte size, string, one byte size, string, one byte size, string (or just derive
    from firstName, lastName) 
*/

//need a comparator function for when we're sorting, maybe use static functions instead?
//comparator by ID
/*
static bool IDComp(const UserRecord& a, const UserRecord& b){
    return a.getUserID() < b.getUserID();
}
//comparator by email
static bool EmailComp(const UserRecord& a, const UserRecord& b){
    return a.getEmail() < b.getEmail();
}
//comparator by first/last name
static bool LastNameComp(const UserRecord& a, const UserRecord& b){
    return a.getLastName() < b.getLastName();
}
static bool FirstNameComp(const UserRecord& a, const UserRecord& b){
    return a.getFirstName() < b.getFirstName();
}
*/

