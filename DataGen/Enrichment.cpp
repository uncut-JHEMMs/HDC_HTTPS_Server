/*
    Driver for data enrichment
*/

#include "DataEnricher.hpp"

int main(){
    DataEnricher enricher("../../data_sources/card_transaction.v1.csv", "../../data_sources/bigOutput.fsv","full_names.txt",
    "Cards.txt","MerchantNames.txt","farusertable.fsv", "farcardtable.fsv","farmerchanttable.fsv");
    enricher.enrich();
    //destructor takes care of everything else
}