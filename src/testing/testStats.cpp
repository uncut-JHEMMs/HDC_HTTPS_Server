#include "../Marts.hpp"
#include <cassert>

/*
    test by running on a small file
*/

const std::string BIGSRC{"../../../data_sources/card_transaction.v1.csv"};
const std::string SINGLESRC{"singleInput.csv"};
const std::string LILSRC{"lilInput.csv"};
const std::string outFile{"stats.txt"};

void testStats()
{
    MartStats stats;
    assert(stats.m_distinctUsers == 0);
    getStats(stats, BIGSRC);

    assert(stats.m_inFileName == BIGSRC);
    assert(stats.m_distinctUsers == 2000);
    printStats(stats,outFile);
    getStats(stats, "lilInput.csv");
}

/*
    test clean an parsing errors and fraud
*/
void testClean()
{
    std::string noErrors{",No"};
    std::string noErrors2{",Yes"};
    std::string error1{"\"Bad PIN,\",Yes"};
    std::string error2{"\"Bad expiration date,Bad PIN\",No"};
    std::string res;
}

/*
    tests for creating data marts and ensuring correctness
*/
void testMarts()
{
    std::string rootDir{"../../../data_sources/DataMartFiles/Years/"};
    create_data_marts(BIGSRC, rootDir);
}
int main()
{
    testClean();
    testStats();
    testMarts();
}