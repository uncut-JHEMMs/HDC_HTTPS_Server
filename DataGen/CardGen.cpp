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

unsigned int getExpirationMonth(){
    return (rand() % 12) + 1;
}
unsigned int getExpirationYear(){
    return (rand() % 4) + 2023;
}
/*
    Takes a string of numbers and returns the final digit according to Luhn 
    Algorithm.The length of the string can be anything. This needs to be put 
    into an object:
    1) If it's in an object, I can ask for the Luhn's result on any string of
    numbers
    2) If it isn't in an object and it's just in main, I can't test the methods 
*/
char getLuhnsResult(const std::string& num)
{

}

int main()
{
    unsigned int nextVisaNum = 0;
    unsigned int nextMCNum = 0;
    unsigned int nextAENum = 0;
    /*
        Just a loop where we make 10000 of each card. To get the number, we put a prefix down,
        then we put down the next available number padded with a padding character out to the 
        appropriate length.
    */
    std::ofstream outFile("Cards.txt", std::ofstream::out);
    std::stringstream cardString;
    for (unsigned int i = 0; i < 30000; ++i){
        if (i % 3 == 0){
            //visa
            cardString << "4";
            //put 14 digits of next visa in place
            //use Luhn to find the 16th digit and put it in place
            cardString << std::setfill('2') << std::setw(15) << nextVisaNum++;
            cardString << " " << getExpirationMonth() << " " << getExpirationYear() << std::endl;
            outFile << cardString.str();
            cardString.str("");
            cardString.clear();
        }
        else if (i % 3 == 1){
            //mastercard
            cardString << "5";
            cardString << std::setfill('3') << std::setw(15) << nextMCNum++;
            cardString << " " << getExpirationMonth() << " " << getExpirationYear() << std::endl;
            outFile << cardString.str();
            cardString.str("");
            cardString.clear();
        }
        else{
            //Amex
            cardString << "37";
            cardString << std::setfill('5') << std::setw(13) << nextAENum++;
            cardString << " " << getExpirationMonth() << " " << getExpirationYear() << std::endl;
            outFile << cardString.str();
            cardString.str("");
            cardString.clear();
        }
    }

    outFile.close();
}