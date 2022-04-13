/*
    We'll try to get some working code here for starting an xml file and putting some data in it
*/
#include "tinyxml2.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "UserRecord.hpp"
int main(){
  /*
  tinyxml2::XMLDocument doc;
  //create a node
  tinyxml2::XMLNode* pRoot = doc.NewElement("Root");
  //attach the node to the document
  doc.InsertFirstChild(pRoot);
  //now that we have a root node, we can start putting data in it
  tinyxml2::XMLElement* pElement = doc.NewElement("IntValue");
  pElement->SetText(10);
  pRoot->InsertEndChild(pElement);
  pElement = doc.NewElement("String");
  pElement->SetText("Penis");
  pRoot->InsertEndChild(pElement);
  //save the file
  doc.SaveFile("test.xml");
  */
  /*
    look at test.xml. The basic idea for the stories is that our user file will
    be an xml document with a root, and then we add all the users as children 
    of the root. Every user node will have first and last name, email and id.
  */

  //Let's try opening the file and writing an xml document with it
  std::ifstream iFile("full_names.txt",std::ifstream::in);
  if (!iFile.is_open()){
    std::cout <<"couldn't open input" << std::endl;
    exit(1);
  }
  //start a new xml document
  tinyxml2::XMLDocument doc;
  //create a node
  tinyxml2::XMLNode* pRoot = doc.NewElement("Root");
  //attach the node to the document
  doc.InsertFirstChild(pRoot);
  /*
  //now that we have a root node, we can start putting data in it
  tinyxml2::XMLElement* pElement = doc.NewElement("IntValue");
  pElement->SetText(10);
  pRoot->InsertEndChild(pElement);
  */
  tinyxml2::XMLElement* usersNode, *pElement;
  //put the initial stuff for root's kids here
  usersNode = doc.NewElement("Users");
  pRoot->InsertEndChild(usersNode);
  std::string line, item;
  std::stringstream streamLine;
  for (int i = 0; i < 10; ++i){
    getline(iFile, line);
    //set the stream
    streamLine.str(line);
    streamLine >> item;
    //populate the user record
    pElement = doc.NewElement("FirstName");
    pElement->SetText(item.c_str());
    usersNode->InsertEndChild(pElement);
  }
  //write to the document
  doc.SaveFile("test.xml");
  return 0;
}