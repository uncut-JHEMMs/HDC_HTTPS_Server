/*
  This file contains defintions related to the logging functionality of the
  project. Right now it's just a simple function that checks a shared vector
  of messages, eventually it will be an actual logging library.
*/
#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <thread>
#include <fstream>
#include <semaphore.h>
#include <chrono>
#include <mutex>
#include "ServerStats.hpp" //stats
#include "LogMessage.hpp" //message types

//for sleep for
using namespace std::chrono_literals;
//define the shared buffer of messages and the semaphore that guards them

struct Logger{
  std::queue<LogMessage> queue; /*they give us deque as a default container type, it's the second template arg*/
  sem_t queueSize;
  ServerStats stats;
  std::mutex statsGuard; //this is for statistics, shared between the endpoints
  std::mutex queueGuard; //this is for the message queue
  bool quitFlag;
  std::ofstream statsStream;
  std::ofstream accessStream;
  //default constructor lets us initialize the semaphore
  Logger(std::string filename);
  /*
    write one message from the access queue to the underlying file and dupe it
    to stdout. ProcessMessage calls this when it pops a message
    off of the messageQueue.
  */ 
  void writeMessage(std::string& message);
  //post a message to the access queue
  void postMessage(LogMessage& message);
  //post a type 0 messsage by providing the message text
  void postType0Message(const std::string& messageText);
  //overload for c-string 
  void postType0Message(const char* messageText);
  //post a type 0 message by providing the message text
  void postType1Message(const std::string& messageText);
  //write all the messages in the queue to the file and exit, used when we're quitting
  void quit();
  //here's the thread that logs the config and all the accesses detected
  void accessThread();
  /*
    here's the stats thread. Every second, we write the per seconds out to 
    files, then we reset them all
    */
  void statsThread();
  //get the stats and write them to the file, used in steady state and by quit
  void writeStatsToFile();
  /*
    Process any message (calls the other process methods, or report an error 
    message)
  */
 void processMessage(LogMessage& message);
  /*
    Process a type-0 log message: Write it to the file, dupe it to stdout.
    Return an int as a status?
  */
 void process0message(LogMessage& message);
  /*
    Process a type-1 log message: Write it to the file and dupe it to stdout,
    increment the total request count and the requests per second count
  */
 void process1message(LogMessage& message);
  /*
    Process a type-2 message: increase the response throughput size by the
    size of the response
  */
 void process2message(LogMessage& message);
 //set the column names for the stats log file
 void nameStatColumns();
};

#endif
