#include "Marts.hpp"

/*
    dump stats to an output file, first item is the stat, second is the number
*/
void printStats(MartStats& stats, const std::string& outFileName)
{
    std::ofstream outFile{outFileName, std::ios::out};
    //source file
    outFile << "Source_File: " << stats.m_inFileName << std::endl;
    //transaction count
    outFile << "Transaction_Count: " << stats.m_transactions << std::endl;
    //distinct users
    outFile << "Distinct_Users: " << stats.m_distinctUsers <<std::endl;
    //Merchants
    outFile << "Distinct_Merchants: " << stats.m_distinctMerchants <<std::endl;
    //years
    outFile << "Distinct_Years: " << stats.m_distinctYears << std::endl;
    outFile << "Years: ";
    for (const std::string& year : stats.m_years)
    {
        outFile << year << " ";
    }
    outFile << std::endl;
    outFile << "Error_Transactions: " << stats.m_errors << std::endl;
    outFile << "CardNumber_Errors: " << stats.m_badCardNum << std::endl;
    outFile << "BadPin_Errors: " << stats.m_badPin << std::endl;
    outFile << "BadExpy_Errors: " << stats.m_badExpy << std::endl;
    outFile << "BadCVV_Errors: " << stats.m_badCVV << std::endl;
    outFile << "TechGlitch_Errors: " << stats.m_techGlitch << std::endl;
    outFile << "Lowbalance_Errors: " << stats.m_insufBal << std::endl;
    outFile << "Fraud_Transactions: "<< stats.m_isFraud << std::endl;
    outFile.close();
}
/*
    get all the stats in the file. Expand with more functions
*/ 
void getStats(MartStats& stats, const std::string& inputFile)
{
    stats.m_inFileName = inputFile;
    std::vector<std::thread> threads;
    std::thread t1{getdUsers,std::ref(stats), std::ref(inputFile)};
    threads.push_back(std::move(t1));
    std::thread t2{getdMerchants,std::ref(stats), std::ref(inputFile)};
    threads.push_back(std::move(t2));
    std::thread t3{getdYears,std::ref(stats), std::ref(inputFile)};
    threads.push_back(std::move(t3));
    std::thread t4{getFraud,std::ref(stats), std::ref(inputFile)};
    threads.push_back(std::move(t4));
    for (unsigned int i = 0; i < threads.size(); i++)
    {
        threads[i].join();
    }
}
/*
    look through file and get the number of distinct users
*/
void getdUsers(MartStats& stats,const std::string& fileName)
{
    unsigned int trans_count;
    std::set<std::string> dUsers;
    std::ifstream inFile{fileName,std::ios::in};
    std::stringstream sstream;
    std::string inputLine, userString;
    //get first line and throw it away
    std::getline(inFile, inputLine);
    while (std::getline(inFile, inputLine))
    {
     trans_count++;
     sstream.str(inputLine);
     getline(sstream, userString, ',');
     dUsers.insert(userString);
    }
    inFile.close();
    stats.m_transactions = trans_count;
    stats.m_distinctUsers = dUsers.size();
}

void getdMerchants(MartStats& stats,const std::string& fileName)
{
 std::set<std::string> dmerch;
 std::ifstream inFile{fileName,std::ios::in};
 std::stringstream sstream;
 std::string inputLine, itemString;
 //get first line and throw it away
 std::getline(inFile, inputLine);
 while (std::getline(inFile, inputLine))
 {
    sstream.str(inputLine);
    getline(sstream, itemString, ','); // user
    getline(sstream, itemString, ',');//card
    getline(sstream, itemString, ',');//year
    getline(sstream, itemString, ',');//month
    getline(sstream, itemString, ',');//day
    getline(sstream, itemString, ',');//time
    getline(sstream, itemString, ',');//amount
    getline(sstream, itemString, ',');//type
    getline(sstream, itemString, ',');//merchant
    dmerch.insert(itemString);
 }
 inFile.close();
 stats.m_distinctMerchants = dmerch.size();   
}

void getdYears(MartStats& stats, const std::string& fileName)
{
    std::set<std::string> dYears;
    std::ifstream inFile{fileName,std::ios::in};
    std::stringstream sstream;
    std::string inputLine, itemString;
    //get first line and throw it away
    std::getline(inFile, inputLine);
    while (std::getline(inFile, inputLine))
    {
     sstream.str(inputLine);
     getline(sstream, itemString, ','); // user
     getline(sstream, itemString, ',');//card
     getline(sstream, itemString, ',');//year
     dYears.insert(itemString);
    }
    inFile.close();
    stats.m_distinctYears = dYears.size();  
    stats.m_years = std::move(dYears);
}

/*
    get the total number of transactions per year, used for percentageFraud 
    story
*/

/*
    get the number of fraud transactions in the file
*/
void getFraud(MartStats& stats, const std::string& fileName)
{
    std::stringstream sstream;
    std::string inputLine, itemString, cleanErrors;
    std::ifstream inFile{fileName,std::ios::in};
    //get first line and throw it away
    std::getline(inFile, inputLine);
    while (std::getline(inFile, inputLine))
    {
     sstream.str(inputLine);
     getline(sstream, itemString, ','); // user
     getline(sstream, itemString, ',');//card
     getline(sstream, itemString, ',');//year
     getline(sstream, itemString, ',');//month
     getline(sstream, itemString, ',');//day
     getline(sstream, itemString, ',');//time
     getline(sstream, itemString, ',');//amount
     getline(sstream, itemString, ',');//type
     getline(sstream, itemString, ',');//merchant
     getline(sstream, itemString, ',');//city, can be ONLINE
     getline(sstream, itemString, ',');//state, can be blank
     getline(sstream, itemString, ',');//zip, can be blank
     getline(sstream, itemString, ',');//mcc
     getline(sstream, itemString, ',');//errors, need to look at the parsing issue here: if there's a "", we can't break on ,
     if (!itemString.empty()) stats.m_errors++;
     while (!itemString.empty() && itemString != "\""){
         //get errors
         if (itemString == "Technical Glitch" || itemString == "\"Technical Glitch") stats.m_techGlitch++;
         else if (itemString == "Bad CVV" || itemString == "\"Bad CVV") stats.m_badCVV++;
         else if (itemString == "Insufficient Balance" || itemString == "\"Insufficient Balance") stats.m_insufBal++;
         else if (itemString == "Bad PIN" || itemString == "\"Bad PIN") stats.m_badPin++;
         else if (itemString == "Bad Card Number" || itemString == "\"Bad Card Number") stats.m_badCardNum++;
         else if (itemString == "Bad Expiration" || itemString == "\"Bad Expiration") stats.m_badExpy++;
         getline(sstream, itemString, ',');
     }
     getline(sstream, itemString, ',');//fraud
     if (itemString == "Yes")
     {
         stats.m_isFraud++;
     }
     sstream.str("");
     sstream.clear();

    }

}

/*
    create all the data marts in a file tree rooted at rootDir in a
    multithreaded fashion, one thread per task
*/
void create_data_marts(const std::string& inFileName, const std::string& rootDir)
{
    create_year_marts(inFileName, rootDir);
}
/*
    sort the transactions by years into marts
*/
void create_year_marts(const std::string& inFileName, const std::string& rootDir)
{
    std::ifstream inFile{inFileName,std::ios::in};
    std::set<YearFile> outs;
    std::stringstream sstream;
    std::string inputLine, itemString, cleanErrors;
    //get first line and throw it away
    std::getline(inFile, inputLine);
    while (std::getline(inFile, inputLine))
    {
     sstream.str(inputLine);
     getline(sstream, itemString, ','); // user
     getline(sstream, itemString, ',');//card
     getline(sstream, itemString, ',');//year
     YearFile file(itemString);
     auto it = outs.find(itemString);
     if (it != outs.end())
     {
         it->stream << inputLine << std::endl; 
     }
     else{
         //make a new yearFile, add it to the set, and push the entry
         std::string wholeName = rootDir + itemString + ".txt";
         //YearFile yF{itemString,wholeName};
         //yF.stream << inputLine << std::endl;
         outs.insert(YearFile(itemString, wholeName));
         auto it = outs.find(itemString);
         //put the required header for uniform file structure
         it->stream << headerLine << std::endl;
         it->stream << inputLine << std::endl;
         //put a filler line in there so all the files look the same?
     }
     getline(sstream, itemString, ',');//month
     getline(sstream, itemString, ',');//day
     getline(sstream, itemString, ',');//time
     getline(sstream, itemString, ',');//amount
     getline(sstream, itemString, ',');//type
     getline(sstream, itemString, ',');//merchant
     getline(sstream, itemString, ',');//city, can be ONLINE
     getline(sstream, itemString, ',');//state, can be blank
     getline(sstream, itemString, ',');//zip, can be blank
     getline(sstream, itemString, ',');//mcc
     while (!itemString.empty() && itemString != "\""){
         //get through all the errors
         getline(sstream, itemString, ',');
     }
     getline(sstream, itemString, ',');//fraud
     if (itemString == "Yes")
     {
        //we put it in the fraud file
     }
     sstream.str("");
     sstream.clear();

    }
    inFile.close();
    for (const YearFile& yF : outs)
    {
        yF.stream.close();
    }
}
