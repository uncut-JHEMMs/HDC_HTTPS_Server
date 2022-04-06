/*
    Program that permutes all of the names, goods and businesses into a text file of 
    full business names
*/
#include "MerchantNames.hpp"
#include <fstream>
#include <iostream>
using namespace DataGenNames;

#define MCCSIZE 10

std::string getRandomMCC(){
    return MCCs[rand() % MCCSIZE];
}
int main (){
    //from the merchant names file:
        //get iterators to the start and end of every array so we have them all
        auto owners_begin = std::begin(owners);
        auto owners_end = std::end(owners);
        auto goods_begin = std::begin(goods);
        auto goods_end = std::end(goods);
        auto places_begin = std::begin(places);
        auto places_end = std::end(places);
        /*
        auto mcc_begin = std::begin(MCCs);
        auto mcc_end = std::end(MCCs);
        */
        std::ofstream outFile("MerchantNames.txt", std::ofstream::out);
        //no apparent std::permute, just do it manually
        for (auto oit = owners_begin; oit != owners_end; ++oit){
            for (auto git = goods_begin; git != goods_end; ++git){
                for (auto pit = places_begin; pit != places_end; ++pit){
                    //write each name to a file
                    outFile << *oit << " " << *git << " " << *pit << " " << getRandomMCC() << std::endl;
                }
            }
        }
    outFile.close();
    

    //now 
}