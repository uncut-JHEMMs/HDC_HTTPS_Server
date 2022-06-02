/*
    Test the data manipulation stuff
*/
#include "../DataManipulator.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <cassert>
#include <fstream>
#include <cassert>

#define OUTPUTFILENAME "numberfile.cdf"

void testCastToUC()
{
    //note this test shows us that we're little-endian
    //test turning a string into a unsigned int and then reading the bytes as chars
    std::string num("256");
    std::stringstream stream(num);
    unsigned int numVal;
    stream >> numVal;
    assert(numVal == 256);
    //now we want the bytes
    unsigned char* c = reinterpret_cast< unsigned char*>(&numVal);
    assert(*(c) == 0); //sign is important, shows 255 OR -1
    assert(*(c+1) == 1); // when we get past 255, we jump up to the next byte
    assert(*(c+2) == 0);
    assert(*(c + 3) == 0);
}

void writeBytesToFile()
{
    std::string num("256");
    std::stringstream stream(num);
    unsigned int numVal;
    stream >> numVal;
    unsigned char* c = reinterpret_cast< unsigned char*>(&numVal);
    //open file and put the bytes in
    std::ofstream outFile(OUTPUTFILENAME, std::ios::out);
    for (int i = 0; i < 4; ++i, ++c)
    {
        outFile << *c;
    }
    //now the 4 bytes for the number are in the file
    outFile << std::endl;
    outFile.close();

}

//read 4 bytes from a file and make them an int
unsigned int readBytesFromFile()
{
    std::ifstream inFile(OUTPUTFILENAME, std::ios::in);
    char c[4];
    char* pc = reinterpret_cast<char*>(&c);
    for (int i = 0; i < 4; ++i,++pc)
    {
        inFile >> *pc;
    }
    //all 4 bytes are there, now just reinterperet
    unsigned int* pu = reinterpret_cast<unsigned int*>(&c);
    return *pu;
}

/*
    test correctness of constructor destructor
*/
void testDMBasics()
{
    //DataManipulator dm; //tests default cons and destructor
    /*
    DataManipulator dmI("lilInput.csv", "test.cdf");
    assert(dmI.cities.size() == 0);
    dmI.writeAs4Bytes("127");
    dmI.writeAs4Bytes("256");
    dmI.writeAs4Bytes("2139062143");
    dmI.writeAs2Bytes("65535"); //this is all question marks
    dmI.writeAs2Bytes("32639");
    try
    {
        dmI.writeAs2Bytes("65536");
    }
    catch(...){
        std::cout << "overflowed 2 bytes with 65536" << std::endl;
    }
    dmI.writeAs1Byte("0");
     try
    {
        dmI.writeAs1Byte("65536");
    }
    catch(...){
        std::cout << "overflowed 1 byte with 65536" << std::endl;
    }
    */


}
/*
    test the static functions: put some values into a file, then extract them
    and make sure they're the same
*/
void testStaticFunctions()
{
  /*  {
        DataManipulator dmI("lilInput.csv", "test.cdf");
        char c[4];
        char* d = reinterpret_cast<char*>(&c);
        dmI.writeAs4Bytes("127",c); //bytes del(127) nul(0) nul nul
        std::ostream& stream = dmI.getOutStream();
        for (int i = 0; i < 4; ++i, ++d){
            stream << *d;
        }
        //dmI.writeAs4Bytes("16777472",c); //bytes  LITTLE END HERE:nul soh(1) nul soh(1)
    } //dmI goes out of scope and destructor closes the file
    */
    std::ifstream inFile("test.cdf");
    int i = DataManipulator::getIntFrom4Bytes(inFile);
    assert(i == 67305985);
    int j = DataManipulator::getIntFrom2Bytes(inFile);
    assert(j == 127);
    int k = DataManipulator::getIntFrom1Byte(inFile); //interesting thing here. Seems as though the file stream wraps around if we've hit the end of file in a previous read
    assert(k == 0);
    
}

void testConversions()
{
    std::string error1{"Bad PIN"};
    std::string error2{"Insufficient Balance"};
    std::string type1{"Swipe Transaction"};
    std::string type2{"Chip Transaction"};
    std::string type3{"Online Transaction"};
    std::string zip1{"11223.0"};
    std::string zip2{"91754.0"};
    std::string amount1{"$123.45"};
    std::string amount2{"$11.20"};
    std::string time1{"01:23"};
    std::string time2{"18:23"};
    std::string fraud1{"Yes"};
    std::string fraud2{"No"};

    unsigned char uc;
    unsigned int ui;
    int i;
    uc = DataManipulator::errorsToChar(error1);
    assert(uc == 2);
    uc = DataManipulator::errorsToChar(error2);
    assert(uc == 16);
    uc = DataManipulator::typeToChar(type1);
    assert(uc == 1);
    uc = DataManipulator::typeToChar(type2);
    assert(uc == 2);
    uc = DataManipulator::typeToChar(type3);
    assert(uc == 4);
    ui = DataManipulator::zipStringToUInt(zip1);
    assert(ui == 11223);
    ui = DataManipulator::zipStringToUInt(zip2);
    assert(ui == 91754);
    i = DataManipulator::amountStringToInt(amount1);
    assert(i == 12345);
    i = DataManipulator::amountStringToInt(amount2);
    assert(i == 1120);
    ui = DataManipulator::timeStringToUInt(time1);
    assert(ui == 83);
    ui = DataManipulator::timeStringToUInt(time2);
    assert(ui == 1103);
    uc = DataManipulator::fraudToByte(fraud1);
    assert(uc == 1);
    uc = DataManipulator::fraudToByte(fraud2);
    assert(uc == 0);
}


/*
    run the line converter on a string to make sure the values are coming out correctly
*/
void testAsciiTRToRDF(){
    DataManipulator dm;
    std::string input{"0,0,2002,9,1,06:21,$11.00,Swipe Transaction,3527213246127876953,La Verne,CA,91750.0,5300,,No"};
    char cline[35];
    //check that each field is right, we want to test every variation: different years, times, amounts, types, cities, states, zips, errors, frauds
    dm.asciiTRtoCDF(input, cline);
    //here's where we're testing this representation
}

void testSmall(){
    DataManipulator dm("Above100After8ByZip.csv", "AHAEBZ.cdf");
    dm.compactTransactions();
}
int main()
{
    //basic cast testing
    //testCastToUC();
    //now we write to a file and retrieve the bytes from that file
    //writeBytesToFile(); //works exactly as I imagined it, we get 4 bytes in little-endian order
    //assert(readBytesFromFile() == 256);

    //now we start testing the class members of DM
    //testDMBasics();
    //testStaticFunctions();
    //testConversions();
    
    //testSmall();
    return 0;
}

