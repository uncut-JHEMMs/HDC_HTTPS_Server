/*
    We're gonna use this to just clean up this names file
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>


int main(int argc, char* argv[]){
    //open the input file
    std::ifstream inFile(argv[1],std::ifstream::in);
    std::ofstream outFile("full_names.txt",std::ofstream::out);
    //get the first line and discard it
    std::string firstName, lastName;
    std::stringstream emailLine;
    std::vector<std::shared_ptr<UserRecord>> vec;
    //getline(inFile, line); // discard
    //from the second line on:
    //get the line
    while (getline(inFile, firstName))
    //for (int i = 0; i < 200; ++i)
    {   
      //  getline(inFile, firstName);
        if(getline(inFile, lastName))
        {
            std::shared_ptr<UserRecord> rec{new UserRecord()};
            rec->setFirstName(firstName);
            //std::cout << rec->getFirstName() << std::endl;
            rec->setLastName(lastName);
            //set email
            emailLine << firstName << "." << lastName << "@smoothceeplusplus.com";
            rec->setEmail(emailLine.str());
            //if we don't reset the emailLine destination here, it concatenates multiple
            //records and becomes fucking huge
            emailLine.str("");
            outFile << rec->getFirstName() << " " << rec->getLastName() << " " << rec->getEmail() << std::endl;
        }
        //there is no last name
        else
        {
            //break;
        }
    }
    //std::cout << "Vec size: " << vec.size() << std::endl;
    //cut the list in half, make the second half last names, and pair them up, then build emails
    //dump this back out to a csv, we'll use this in our data generation

    //print out the vector
    //close the input file
    inFile.close();
    outFile.close();    
    return 0;
}