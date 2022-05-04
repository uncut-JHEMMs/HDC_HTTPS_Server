/*
    This program contains the logic for generating credit card numbers and 
    expiration dates. We'll try 30,000 cards (10,000 of each type) and adjust
    from there.
*/
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "LuhnsCalculator.hpp"

#define NUMBEROFCARDS 30000

unsigned int getExpirationMonth(){
    return (rand() % 12) + 1;
}
unsigned int getExpirationYear(){
    return (rand() % 4) + 2023;
}

int main()
{
    unsigned int nextVisaNum = 567898765; //just fill it like below?
    unsigned int nextMCNum = 171202335;
    unsigned int nextAENum = 618115202;
    /*
        Just a loop where we make 10000 of each card. To get the number, we put a prefix down,
        then we put down the next available number padded with a padding character out to the 
        appropriate length.
    */
    std::ofstream outFile("Cards.txt", std::ofstream::out);
    std::stringstream cardString;
    for (unsigned int i = 0; i < NUMBEROFCARDS; ++i)
    {
        if (i % 3 == 0)
        {
            //visa
            cardString << "412";
            //put 14 digits of next visa in place
            //use Luhn to find the 16th digit and put it in place
            cardString << std::setfill('2') << std::setw(12) << nextVisaNum++;
            //now we assign the check bit
            std::string numString = LuhnsCalculator::makeLuhnsValid(cardString.str());
            //put it back in the string stream
            cardString.str(numString);
            cardString << numString << " " << getExpirationMonth() << " " <<
             getExpirationYear() << std::endl;
            outFile << cardString.str();
            cardString.str("");
            cardString.clear();
        }
        
        else if (i % 3 == 1)
        {
            //mastercard
            cardString << "553";
            cardString << std::setfill('3') << std::setw(12) << nextMCNum++;
            std::string numString = LuhnsCalculator::makeLuhnsValid(cardString.str());
            cardString.str(numString);
            cardString << numString << " " << getExpirationMonth() << " " << 
             getExpirationYear() << std::endl;
            outFile << cardString.str();
            cardString.str("");
            cardString.clear();
        }
        else
        {
            //Amex
            cardString << "373";
            cardString << std::setfill('5') << std::setw(12) << nextAENum++;
            std::string numString = LuhnsCalculator::makeLuhnsValid(cardString.str());
            cardString.str(numString);
            cardString << numString << " " << getExpirationMonth() << " " << 
             getExpirationYear() << std::endl;
            outFile << cardString.str();
            cardString.str("");
            cardString.clear();
        }
    }
    outFile.close();
}