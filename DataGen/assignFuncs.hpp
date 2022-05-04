/*
    header file with definitions of functions used in assignTransactions,
    mostly for the writing of output files or the parsing of transaction lines
*/
#ifndef ASSIGNFUNCS_HPP
#define ASSIGNFUNCS_HPP
#include "UserRecord.hpp"
#include "CardRecord.hpp"
#include "LocationRecord.hpp"
#include "MerchantRecord.hpp"
#include <tinyxml2.h>
#include <vector>
#include <memory>

#define USER_XML_FILENAME "users.xml"
#define MERCHANT_XML_FILENAME "merchants.xml"
#define PLACES_XML_FILENAME "places.xml"
#define CARD_XML_FILENAME "cards.xml"
#define LOCATION_XML_FILENAME "locations.xml"

/*
    Write all the users from the users vector to the file
*/
bool writeUsersToXML(std::vector<std::shared_ptr<UserRecord>>& vec){
    //start the xml document
  tinyxml2::XMLDocument doc;
  //create a node
  tinyxml2::XMLNode* pRoot = doc.NewElement("Users");
  //attach the node to the document
  doc.InsertFirstChild(pRoot);
  //for every item in the vector, put in nodes for id, first name, last name, email
  tinyxml2::XMLElement* pElement;
  for (const std::shared_ptr<UserRecord>& uptr : vec){
    // make a new child for root
    tinyxml2::XMLNode* userNode = doc.NewElement("User");
    pRoot->InsertFirstChild(userNode);
    //id
    pElement = doc.NewElement("UserID");
    pElement->SetText(uptr->getUserID());
    userNode->InsertEndChild(pElement);
    //first name
    pElement = doc.NewElement("FirstName");
    pElement->SetText(uptr->getFirstName().c_str());
    userNode->InsertEndChild(pElement);
    //last name
    pElement = doc.NewElement("LastName");
    pElement->SetText(uptr->getLastName().c_str());
    userNode->InsertEndChild(pElement);
    //email
    pElement = doc.NewElement("Email");
    pElement->SetText(uptr->getEmail().c_str());
    userNode->InsertEndChild(pElement);
  }
  //save the document
  doc.SaveFile(USER_XML_FILENAME);
  return true;
}

bool writeMerchantsToXML(std::vector<std::unique_ptr<MerchantRecord>>& merchants){
tinyxml2::XMLDocument doc;
  //create a node
  tinyxml2::XMLNode* pRoot = doc.NewElement("Merchants");
  //attach the node to the document
  doc.InsertFirstChild(pRoot);
  //for every item in the vector, put in nodes for id, first name, last name, email
  tinyxml2::XMLElement* pElement;
  for (auto it = merchants.rbegin(); it != merchants.rend(); it = ++it){
    // make a new child for root
    tinyxml2::XMLNode* userNode = doc.NewElement("Merchant");
    pRoot->InsertFirstChild(userNode);
    //Merchant Name
    pElement = doc.NewElement("Name");
    pElement->SetText((*it)->getName().c_str());
    userNode->InsertEndChild(pElement);
    //MCC
    pElement = doc.NewElement("MCC");
    pElement->SetText((*it)->getMCC().c_str());
    userNode->InsertEndChild(pElement);
  }
  //save the document
  doc.SaveFile(MERCHANT_XML_FILENAME);
  return true;
}

/*
    Write all the cards from the cards vector to the file
*/
bool writeCardsToXML(std::vector<std::unique_ptr<CardRecord>>& vec){
    //start the xml document
  tinyxml2::XMLDocument doc;
  //create a node
  tinyxml2::XMLNode* pRoot = doc.NewElement("Cards");
  //attach the node to the document
  doc.InsertFirstChild(pRoot);
  //for every item in the vector, put in nodes for id, first name, last name, email
  tinyxml2::XMLElement* pElement;
  for (const std::unique_ptr<CardRecord>& uptr : vec){
    // make a new child for root
    tinyxml2::XMLNode* cardNode = doc.NewElement("Card");
    pRoot->InsertFirstChild(cardNode);
    //user index
    pElement = doc.NewElement("User_Key");
    pElement->SetText(uptr->getUserKey());
    cardNode->InsertEndChild(pElement);
    //card index for user
    pElement = doc.NewElement("Card_Key");
    pElement->SetText(uptr->getCardKey());
    cardNode->InsertEndChild(pElement);
    //card number
    pElement = doc.NewElement("Card_Number");
    pElement->SetText(uptr->getCardNumber().c_str());
    cardNode->InsertEndChild(pElement);
  }
  //save the document
  doc.SaveFile(CARD_XML_FILENAME);
  return true;
}

/*
    Write all the cards from the cards vector to the file
*/
bool writeLocationsToXML(std::vector<std::unique_ptr<LocationRecord>>& vec){
    //start the xml document
  tinyxml2::XMLDocument doc;
  //create a node
  tinyxml2::XMLNode* pRoot = doc.NewElement("Locations");
  //attach the node to the document
  doc.InsertFirstChild(pRoot);
  //for every item in the vector, put in nodes for id, first name, last name, email
  tinyxml2::XMLElement* pElement;
  for (auto it = vec.rbegin(); it != vec.rend(); it = ++it){
    // make a new child for root
    tinyxml2::XMLNode* cardNode = doc.NewElement("Location");
    pRoot->InsertFirstChild(cardNode);
    //Index
    pElement = doc.NewElement("Index");
    pElement->SetText((*it)->getIndex());
    cardNode->InsertEndChild(pElement);
    //state
    pElement = doc.NewElement("State");
    pElement->SetText((*it)->getState().c_str());
    cardNode->InsertEndChild(pElement);
    //city
    pElement = doc.NewElement("City");
    pElement->SetText((*it)->getCity().c_str());
    cardNode->InsertEndChild(pElement);
    //zip code
    pElement = doc.NewElement("Zip_Code");
    pElement->SetText((*it)->getZip().c_str());
    cardNode->InsertEndChild(pElement);
  }
  //save the document
  doc.SaveFile(LOCATION_XML_FILENAME);
  return true;
}

/*
  take a string representing an amount in dollars (with leading dollar sign),
  and return an unsigned int that is the amount in cents
   
*/

#endif