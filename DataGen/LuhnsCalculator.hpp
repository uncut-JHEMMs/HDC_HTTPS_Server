/*
    Header file for Luhn's calculator, methods allow me to find the Luhn's 
    result of a variety of formats (start with a string of numbers)
*/
#ifndef LUHNSCALCULATOR_HPP
#define LUHNSCACLULATOR_HPP
#include <string>
#include <ctype.h> //isdigit

class LuhnsCalculator
{
    private:
    /*
        return the sum of the digits, used by the other functions to get the
        total sum piece of the checksum bit
    */
    static unsigned int returnDigitSum(unsigned int digit)
    {
        
        if (digit > 9)
        {
            return digit - 9;
        }
        else
        {
            return digit;
        }
    }
    /*
        return the sum of the digit sums (above) in the number provided. We go
        through the number in reverse (starting from the rightmost digit) so
        we don't have to look ahead and determine even or odd case
    */
    static unsigned int sumAllDigits(const std::string& rawNumber)
    {
        unsigned int digitSum = 0;
        bool doubleVal = true;
        for (int size = rawNumber.size() - 1; size >= 0; --size)
            {
                if (!isdigit(rawNumber[size])) throw("sumAllDigits");
                if (doubleVal){
                    digitSum += returnDigitSum((rawNumber[size] - '0') * 2);
                }
                else{
                    digitSum += returnDigitSum(rawNumber[size] - '0');
                }
                doubleVal = !doubleVal;
            }
            
            return digitSum;
    }

    /*
        Using the sum of all of the non-checksum digits, find the checksum value
    */
    static unsigned int calculateCSFromSum(unsigned int& sum){
        unsigned int checkDigit = 10 - (sum % 10);
        if (checkDigit == 10) checkDigit = 0; // is this okay?
        return checkDigit;
    }
    public:
    /*
        take a string of numbers, and return the character representing the
        Luhns checksum. Propagates exception from sum method.
    */
   static char calculateCheckValue(const std::string& rawNumber)
   {
       unsigned int num;
       try
       {
        num = sumAllDigits(rawNumber);
       }
       catch(const char* c){
        throw(c);
       }
       num = calculateCSFromSum(num);
       char checkVal = num + '0'; //this is a problem if the number is all 0s
       return checkVal;
   }

   /*
        Give a string of numbers, verify that the number is valid according
        to the algorithm (the check digit at the end is correct): Get the last
        character, calculate the check for the rest of the string, make sure
        that the value we calculate is the value already there.
   */
    static bool isValid(const std::string& rawNumber)
    {
      if (rawNumber.size() < 2) throw("isValid size");
      char checkVal = rawNumber[rawNumber.size() - 1];
      if (!isdigit(checkVal)) throw("isValid checkVal");
      //get a sub-string without the check value
      std::string sub = rawNumber.substr(0,rawNumber.size() - 1);
      //calculate the check value on the substring
      char calcVal;
      try
      {
        calcVal = calculateCheckValue(sub);
      }
      catch (const char * c){
          throw(c);
      }
      //check that the calculated value is the one we got earlier
      return calcVal == checkVal;
    }

    /*
        Given a string representing a number, make it compliant with
        Luhns Algorithm by calculating the check value and returning
        a string that consists of the raw value appended with the check
        value
    */
    static std::string makeLuhnsValid(const std::string& rawNumber){
        //calculate the check value from the number
        char check = calculateCheckValue(rawNumber);
        //return a string with the raw number appended with the check value
        std::string copy(rawNumber);
        return copy + check;
    }
};
#endif