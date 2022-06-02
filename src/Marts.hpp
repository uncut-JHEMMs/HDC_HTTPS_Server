#ifndef MARTS_HPP
#define MARTS_HPP
/*
    Contains declarations related to organizing the big file into data marts,
    including a statistics file
*/

#include <string>
#include <fstream>
#include <sstream>
#include <set>
#include <vector>
#include <thread>
#include <algorithm>

#define FRAUD_FILE_NAME "FraudTrans.txt"
#define STATS_FILENAME "Stats.txt"
#define SOURCE_FILENAME "../../data_sources/card_transaction.v1.csv"
const std::string headerLine{"User,Card,Year,Month,Day,Time,Amount,Use Chip,Merchant Name,Merchant City,Merchant State,Zip,MCC,Errors?,Is Fraud?"};

struct MartStats
{
    std::string m_inFileName;
    unsigned int m_distinctUsers;
    unsigned int m_transactions;
    unsigned int m_distinctMerchants;
    unsigned int m_distinctCities;
    unsigned int m_distinctStates;
    unsigned int m_hasError;
    unsigned int m_distinctUserHasInsufBal;
    unsigned int m_distinctYears;
    unsigned int m_isFraud;
    unsigned int m_errors;
    unsigned int m_insufBal;
    unsigned int m_badPin;
    unsigned int m_badCardNum;
    unsigned int m_badExpy;
    unsigned int m_badCVV;
    unsigned int m_techGlitch;
    std::set<std::string> m_years;
    MartStats():m_inFileName(""),m_distinctUsers(0), m_transactions(0),
    m_distinctMerchants(0),m_isFraud(0), m_errors(0), m_insufBal(0),
    m_badPin(0),m_badCardNum(0), m_badExpy(0), m_badCVV(0), m_techGlitch(0){}
};
/*
    get statistics from the file by running each of the particular stats
    functions in different threads
*/
void getStats(MartStats& stats, const std::string& inputFile);
void printStats(MartStats& stats, const std::string& outFileName);
//getting the transaction number is part of getting the users, since it's pretty quick
void getdUsers(MartStats& stats,const std::string& fileName);
void getdMerchants(MartStats& stats,const std::string& fileName);
void getdYears(MartStats& stats, const std::string& fileName);
void getFraud(MartStats& stats, const std::string& fileName);
void create_year_marts(const std::string& inFileName, const std::string& outFileName);
void create_data_marts(const std::string& inFileName,const std::string& rootDir);

/*
    allows us to map the year string to an output stream so we can consistently add
    to the correct mart file
*/
struct YearFile
{
    std::string yearString;
    mutable std::ofstream stream;
    YearFile(std::string year)
    {
        yearString = year;
        //leave stream undefined, this is just a dummy for searching?
    }
    YearFile(std::string year, std::string fileName): yearString(year)
    {
        std::ofstream ostream{fileName, std::ios::out};
        stream = std::move(ostream);
    }
    //move constructor
    YearFile(YearFile&& yF)
    {
        yearString = yF.yearString;
        stream = std::move(yF.stream);
    }
    ~YearFile()
    {
        if (stream.is_open())
        {
        stream.close();
        }
    }
    friend bool operator<(const YearFile& a, const YearFile& b)
    {
        return a.yearString < b.yearString;
    }
};


#endif 