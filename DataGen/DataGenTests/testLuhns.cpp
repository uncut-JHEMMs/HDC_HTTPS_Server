/*
    Test file for Luhns Calculator
*/

#include "LuhnsCalculator.hpp"
#include <string>
#include <cassert>
#include <iostream>

int main(){
    //all of LuhnsCalculator's methods are static, don't need to make one?
    //create some number strings and verify the results are what we expect
    std::string single0 = "0";
    std::string badNumber("012a");
    try{
        LuhnsCalculator::isValid(badNumber);
    }
    catch (const char* c){
        std::cout << "caught expected exception:" << c << std::endl;
    }
    badNumber = "01a2";
    try{
        LuhnsCalculator::isValid(badNumber);
    }
    catch (const char* c){
        std::cout << "caught expected exception:" << c << std::endl;
    }
    std::string luhns1 = LuhnsCalculator::makeLuhnsValid(single0);  //should be 00
    assert(luhns1 == "00");
    std::string double0 = "00";
    std::string luhns2 = LuhnsCalculator::makeLuhnsValid(double0); //should be 000
    assert(luhns2 == "000");
    try {
        LuhnsCalculator::isValid(single0);
    }
    catch (const char* c){
        std::cout << "caught expected exception:" << c << std::endl;
    }
    assert(LuhnsCalculator::isValid(luhns2));
    std::string notValid("001");
    assert(!LuhnsCalculator::isValid(notValid));
    //now for some non-zero numbers
    std::string arg = "1";
    luhns1 = LuhnsCalculator::makeLuhnsValid(arg); //checkVal should be 8, for a total "18"
    assert(luhns1 == "18");
    assert(LuhnsCalculator::isValid(luhns1));
    arg = "11";
    luhns1 = LuhnsCalculator::makeLuhnsValid(arg);
    assert(luhns1 == "117");
    assert(LuhnsCalculator::isValid(luhns1));
    arg = "115";
    luhns1 = LuhnsCalculator::makeLuhnsValid(arg);
    assert(luhns1 == "1156");
    assert(LuhnsCalculator::isValid(luhns1));
    arg = "151";
    luhns1 = LuhnsCalculator::makeLuhnsValid(arg);
    assert(luhns1 == "1511");
    assert(LuhnsCalculator::isValid(luhns1));
    //some more realistic numbers
    arg = "151823762";
    luhns1 = LuhnsCalculator::makeLuhnsValid(arg);
    assert(luhns1 == "1518237621");
    assert(LuhnsCalculator::isValid(luhns1));
    arg = "0123456789";
    luhns1 = LuhnsCalculator::makeLuhnsValid(arg);
    assert(luhns1 == "01234567897");
    assert(LuhnsCalculator::isValid(luhns1));
    arg = "1234567890";
    luhns1 = LuhnsCalculator::makeLuhnsValid(arg);
    assert(luhns1 == "12345678903");
    assert(LuhnsCalculator::isValid(luhns1));
}