/*
    Contains definitions for data manipulation: condensing the source file to a
    more compact type. I think the data mart creation goes here too.
*/
#include "DataManipulator.hpp"

//small stuff for the comparator types
MerchDocnID::MerchDocnID(): docCode(""),idNum(0){}
MerchDocnID::MerchDocnID(const std::string& newDoc, const unsigned int newID):
docCode(newDoc), idNum(newID){}
bool MerchDocnID::operator<(const MerchDocnID& other)
{
    return docCode < other.docCode;
}

CityStringnID::CityStringnID(): cityName(""), cityIDNum(0){}
CityStringnID::CityStringnID(const std::string& newName, const unsigned int newID):
    cityName(newName), cityIDNum(newID){}
bool CityStringnID::operator<(const CityStringnID& other)
{
    return cityName < other.cityName;
}

StateStringnID::StateStringnID():stateName(""), stateIDNum(0){}
StateStringnID::StateStringnID(const std::string& newName, const unsigned int newID):
    stateName(newName), stateIDNum(newID){}
bool StateStringnID::operator<(const StateStringnID& other)
{
    return stateName < other.stateName;
}


DataManipulator::DataManipulator(): nextMerchantID(0), nextTransactionID(0),
asciiFileName("lilInput.csv"), CDFFileName("lilOutput.cdf")
{
    std::ifstream inFile(asciiFileName,std::ifstream::in);
    asciiFile = std::move(inFile);
    if (!asciiFile.is_open()){
        throw("failed to open inFile in DM");
    }
    CDFFile = std::ofstream{CDFFileName, std::ofstream::out};
}

DataManipulator::DataManipulator(const std::string& inFile, const std::string& outFile):
    nextMerchantID(0), nextTransactionID(0), asciiFileName(inFile), CDFFileName(outFile)
{
    asciiFile =  std::ifstream{asciiFileName,std::ifstream::in};
    if (!asciiFile.is_open()){
        throw("failed to open inFile in DM");
    }
    CDFFile = std::ofstream{CDFFileName, std::ofstream::out};
    //don't need to check this, just create if it isn't there
}

//take a string and turn it into a 4 byte number, write it to the file
void DataManipulator::writeAs4Bytes(const std::string& numString, char* co)
{
    std::stringstream stream(numString);
    unsigned int num;
    stream >> num;
    unsigned char* c = reinterpret_cast<unsigned char*>(&num);
    for (int i = 0; i < 4; ++i, ++c)
    {
        *co = *c;
        ++co;
    }
}
//take a string and turn it into 2 byte number
void DataManipulator::writeAs2Bytes(const std::string& numString, char* co)
{
    std::stringstream stream(numString);
    unsigned int num;
    stream >> num;
    if (num > 65535) throw("number doesn't fit in 2 bytes");
    unsigned char* c = reinterpret_cast<unsigned char*>(&num);
    for (int i = 0; i < 2; ++i, ++c)
    {
        *co = *c;
        ++co;
    }
}
//take a string and turn it into 1 byte number
void DataManipulator::writeAs1Byte(const std::string& numString, char* co)
{
    std::stringstream stream(numString);
    unsigned int num;
    stream >> num;
    if (num > 255) throw("number doesn't fit in 1 byte");
    unsigned char* c = reinterpret_cast<unsigned char*>(&num);
    *co = *c;
    ++co;
}

//read 4 bytes from a file and return the int. Remember that we're little-endian
int DataManipulator::getIntFrom4Bytes(std::ifstream& fileStream)
{
    char bytes[4];
    try
    {
        for (int i = 0; i < 4; ++i)
        {
            fileStream >> bytes[i];
        }
    }
    catch(...)
    {
        std::cout << "couldn't extract 4 values from file" << std::endl;
    }
    int* i = reinterpret_cast<int*>(&bytes);
    return *i;
}
//read 2 bytes from a file and return the int
int DataManipulator::getIntFrom2Bytes(std::ifstream& fileStream)
{
    char bytes[2];
    try
    {
        for (int i = 0; i < 2; ++i)
        {
            fileStream >> bytes[i];
        }
    }
    catch(...)
    {
        std::cout << "couldn't extract 2 values from file" << std::endl;
    }
    short* s = reinterpret_cast<short*>(&bytes);
    int i = *s;
    return i;
}
//read one byte from a file and return the int
int DataManipulator::getIntFrom1Byte(std::ifstream& fileStream)
{
    int j;
    try
    {
            fileStream >> j;
    }
    catch(...)
    {
        std::cout << "couldn't extract 1 value from file" << std::endl;
    }
    return j;
}

/*
    read the errors in a transaction error and produce the appropriate error byte
    Assume we've stripped the "" already, we'll do that in the parsing
*/
unsigned char DataManipulator::errorsToChar(const std::string& errors)
{
    std::stringstream stream(errors);
    std::string error;
    unsigned char res = 0;
    while (std::getline(stream,error,','))
    {
        if(error == "Bad PIN")
        {
            res += TRErrors::BAD_PIN;
        }
        else if(error == "Insufficient Balance")
        {
            res += TRErrors::INSUFF_BAL;
        }
    }
    return res;
}
//read the transaction type and return the appropriate type byte
unsigned char DataManipulator::typeToChar(const std::string& typeS)
{
    if (typeS == "Swipe Transaction")
    {
        return TRType::SWIPE;
    }
    else if (typeS == "Chip Transaction")
    {
        return TRType::CHIP;
    }
    else return TRType::ONLINE;
}
//get the integer representing the zip code from the string
unsigned int DataManipulator::zipStringToUInt(const std::string& zipS)
{
    std::stringstream stream(zipS);
    float f;
    stream >> f;
    unsigned int i;
    i = f;
    return i;

}
//get the integer representing the transaction amount from the string
int DataManipulator::amountStringToInt(const std::string& amountS)
{
    std::string dollarsS, centsS;
    int dollars, cents;
    //strip the dollar sign
    std::stringstream stream(amountS);
    std::getline(stream,dollarsS,'$');
    //get whole dollars amount
    std::getline(stream,dollarsS,'.');
    //get cents amount
    std::getline(stream,centsS);
    dollars = stoi(dollarsS);
    cents = stoi(centsS);
    //return dollars * 100 + cents
    return dollars * 100 + cents;
}
//time string to integer
unsigned int DataManipulator::timeStringToUInt(const std::string& timeS)
{
    std::string hoursS, minutesS;
    unsigned int hours, minutes;
    std::stringstream stream(timeS);
    std::getline(stream,hoursS,':');
    std::getline(stream, minutesS);
    hours = stoi(hoursS);
    minutes = stoi(minutesS);
    return hours * 60 + minutes;
}

//fraud to fraud byte
unsigned char DataManipulator::fraudToByte(const std::string& fraudS)
{
    if(fraudS == "Yes"){
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
    assume compactRec is always of the appropriate size (35 bytes without newline)
*/
void DataManipulator::asciiTRtoCDF(const std::string& asciiRec, char* compactLine)
{
    //break up the string by commas
    unsigned int pos = 0;
    std::string userIDS, cardIDS, yearS, monthS, dayS, timeS, amountS, typeS, merchantS,
    cityS, stateS, zipS, mccS, errorsS, fraudS; 
    std::stringstream stream(asciiRec);
    std::getline(stream,userIDS,',');
    std::getline(stream,cardIDS,',');
    std::getline(stream,yearS,',');
    std::getline(stream,monthS,',');
    std::getline(stream,dayS,',');
    std::getline(stream,timeS,',');
    std::getline(stream,amountS,',');
    std::getline(stream,typeS,',');
    std::getline(stream, merchantS,',');
    std::getline(stream,cityS, ',');
    std::getline(stream,stateS,',');
    std::getline(stream,zipS,',');
    std::getline(stream,mccS,',');
    std::getline(stream,errorsS,',');
    std::getline(stream,fraudS);
    //perform conversions where necessary and write to file
    writeAs2Bytes(userIDS,compactLine); //now 2 bytes
    pos += 2;
    writeAs1Byte(cardIDS,compactLine+pos);// pos is 3
    pos += 1;
    writeAs2Bytes(yearS,compactLine+pos);//pos is 5
    pos += 2;
    writeAs1Byte(monthS,compactLine + pos); //pos is 6
    pos += 1;
    writeAs1Byte(dayS,compactLine + pos); //pos is 7
    pos += 1;
    //TODO this is very sus, have to look at this later
    unsigned int ui = DataManipulator::timeStringToUInt(timeS);
    std::string sres = std::to_string(ui);
    writeAs2Bytes(sres,compactLine + pos);// pos is 9
    pos += 2;
    int i = DataManipulator::amountStringToInt(amountS);
    sres = std::to_string(i);
    writeAs4Bytes(sres,compactLine + pos);//pos is 13
    unsigned char uc = DataManipulator::typeToChar(typeS);
    sres = std::to_string(uc);
    writeAs1Byte(sres, compactLine + pos); //pos is 14
    sres = "1111"; // merchant ID
    writeAs4Bytes(sres, compactLine + pos);// pos is 18
    sres = "1234"; //city id
    writeAs4Bytes(sres, compactLine + pos); // pos is 22
    sres = "2"; //state id
    writeAs1Byte(sres, compactLine + pos); // pos is 23
    ui = DataManipulator::zipStringToUInt(zipS);
    sres = std::to_string(ui);
    writeAs4Bytes(sres, compactLine + pos); // pos is 27
    writeAs2Bytes(mccS,compactLine + pos); // pos is 29
    uc = errorsToChar(errorsS);
    sres = std::to_string(uc);
    writeAs1Byte(sres, compactLine + pos); // pos is 30
    uc = fraudToByte(fraudS);
    sres = std::to_string(uc);
    writeAs1Byte(sres,compactLine + pos); // pos is 31
}

void DataManipulator::compactTransactions(){
    //input file is already opened by the constructor
    //need a char array
    std::string sline;
    char cline[35];
    //skip first line, it's headers
    std::getline(asciiFile,sline);
    while (std::getline(asciiFile, sline))
    {
        if (sline.empty()) break;
        asciiTRtoCDF(sline, cline);
        for (int i = 0; i < 35; ++i)
        {
        CDFFile << cline + i;
        }
        CDFFile << "\n";
    }
}
