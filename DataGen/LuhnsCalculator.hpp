/*
    Header file for Luhn's calculator, methods allow me to find the Luhn's 
    result of a variety of formats (start with a string of numbers)
*/
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
        
        if (digit > 10)
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
        for (std::string::reverse_iterator rev_it = rawNumber.rbegin(); rev_it
            != rawNumber.rend(); ++rev_it)
            {
                if (!isdigit(*rev_it)) throw("sumAllDigits");
                if (doubleVal){
                    digitSum += returnDigitSum(((*rev_it) - '0') * 2);
                }
                else{
                    digitSum += returnDigitSum(((*rev_it) - '0'));
                }
                doubleVal = !doubleVal;
            }
    }

    /*
        Using the sum of all of the non-checksum digits, find the checksum value
    */
    static unsigned int calculateCSFromSum(unsigned int& sum){
        unsigned int checkDigit = 10 - (sum % 10);
        return checkDigit;
    }
    public:
    /*
        take a string of numbers, and return the character representing the
        Luhns checksum. Propagates exception from sum method.
    */
   char calculateCheckValue(const std::string& rawNumber)
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
       char checkVal = num + '0';
       return checkVal;
   }

   /*
        Give a string of numbers, verify that the number is valid according
        to the algorithm (the check digit at the end is correct): Get the last
        character, calculate the check for the rest of the string, make sure
        that the value we calculate is the value already there.
   */
  bool isValid(const std::string& rawNumber){
      if (rawNumber.size() == 1) throw("isValid");
      char check = rawNumber[rawNumber.size() - 1];
      
  }
};