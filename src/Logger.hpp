/*
  This file contains defintions related to the logging functionality of the
  project. Right now it's just a simple function that checks a shared vector
  of messages, eventually it will be an actual logging library.
*/
#ifndef LOGGER_HPP
#define LOGGER_HPP
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
  std::mutex statsGuard; //this is for statistics, shared between the 
  std::mutex queueGuard;
  bool quitFlag;
  std::ofstream statsStream;
  std::ofstream accessStream;
  //default constructor lets us initialize the semaphore
  Logger(std::string filename){
    try{
      accessStream.open(filename.c_str());
      //TODO fix this hard-coded value
      statsStream.open("serverStats.txt");
      nameStatColumns();
    }
    catch(...){
      std::cout<<"Failed to open log files"<<std::endl;
      exit(1);
    }
    sem_init(&(this->queueSize),0,0);
    quitFlag = false;
  }
    
  /*
    write one message from the access queue to the underlying file and dupe it
    to stdout. ProcessMessage calls this when it pops a message
    off of the messageQueue.
  */ 
  void writeMessage(std::string& message){
    accessStream << message << std::endl;
    std::cout << message << std::endl;
  }

  //post a message to the access queue
  void postMessage(LogMessage& message){
    queueGuard.lock();
    queue.push(message);
    queueGuard.unlock();
    sem_post(&queueSize);
  }

  //post a type 0 messsage by providing the message text
  void postType0Message(const std::string& messageText){
    //make a new type 0 message
    LogMessage message;
    message.setMessageType(LogMessageType::NONACCESS);
    message.setTextMessage(messageText);
    postMessage(message);
  }

  //overload for c-string 
  void postType0Message(const char* messageText){
    LogMessage message;
    std::string mText{messageText};
    message.setTextMessage(mText);
    message.setMessageType(LogMessageType::NONACCESS);
    postMessage(message);
  }

  //post a type 0 message by providing the message text
  void postType1Message(const std::string& messageText){
    //set the URL text part
    LogMessage message;
    message.setMessageType(LogMessageType::REQUESTLOG);
    message.setResponseCode(LogResponseCode::OK);
    message.setTextMessage(messageText);
    postMessage(message);
  }

  /*
    process a message from the queue, which includes adjusting any stats and
    writing messages to files if necessary
  */

  //write all the messages in the queue to the file and exit, used when we're quitting
  void quit(){
    //we lock just in case, but no one should be logging anymore
    //queueGuard.lock();
    while (!queue.empty()){
      LogMessage front = queue.front();
      queue.pop();
      processMessage(front);
    }
    //one final stats dump
    writeStatsToFile();
    //log a quit message. This is done in the signal handler currently
    //accessStream << "Quit signal received. Shutting down" <<std::endl;
    //close all files so stuff is saved.
    statsStream.close();
    accessStream.close();
  }
    
  //here's the thread that logs the config and all the accesses detected
  void accessThread(){
    while(true){
      sem_wait(&queueSize);
      if(quitFlag){
        quit();
        break;
      }
      else{
        //get the message from the front
        queueGuard.lock();
        LogMessage message = queue.front();
        queue.pop();
        //we're done with the queue so unlock it
        queueGuard.unlock();
        //process the message
        processMessage(message);
      }
    }
  }

  /*
    here's the stats thread. Every second, we write the per seconds out to 
    files, then we reset them all
    */
  void statsThread(){
    while(true){
      std::this_thread::sleep_for(1000ms);
      writeStatsToFile();
      if (quitFlag){
        //because the accessLog does all the quitting stuff, we can just leave
        break;
      }
      //in steady state, we just dump all the perseconds to a file, and reset them

      //check the quit flag after we've dumped so we don't miss anything
    }
  }

  //get the stats and write them to the file, used in steady state and by quit
  void writeStatsToFile(){
    statsGuard.lock();
      unsigned int reqPerSec = stats.getReqPerSec();
      unsigned int okPerSec = stats.getOKPerSec();
      unsigned int nfPerSec = stats.getNFPerSec();
      unsigned int errorPerSec = stats.getErrorPerSec();
      unsigned int otherPerSec = stats.getOtherPerSec();
      unsigned int respThrough = stats.getRespPerSec();
      stats.resetPerSec();
      statsGuard.unlock();
      //no one else writes to the stats file except this thread
      statsStream << reqPerSec <<"\t\t"<< okPerSec << "\t\t"<< nfPerSec << "\t\t" <<
        errorPerSec << "\t\t" << otherPerSec << "\t\t"<< respThrough << std::endl;
  }

  /*
    Process any message (calls the other process methods, or report an error 
    message)
  */
 void processMessage(LogMessage& message){
   LogMessageType type = message.getMessageType();
   switch(type){
    case LogMessageType::NONACCESS: 
       process0message(message);
       break;
    case LogMessageType::REQUESTLOG: 
      process1message(message);
      break;
    case LogMessageType::RESPONSELOG:
      process2message(message);
   }
 }
  /*
    Process a type-0 log message: Write it to the file, dupe it to stdout.
    Return an int as a status?
  */
 void process0message(LogMessage& message){
   std::string text = message.getTextMessage();
   writeMessage(text);
 }

  /*
    Process a type-1 log message: Write it to the file and dupe it to stdout,
    increment the total request count and the requests per second count
  */
 void process1message(LogMessage& message){
   statsGuard.lock();
   stats.setTotReq(stats.getTotReq() + 1);
   stats.setReqPerSec(stats.getReqPerSec() + 1);
   statsGuard.unlock();
   std::string text = message.getTextMessage();
   writeMessage(text);
 }

  /*
    Process a type-2 message: increase the response throughput size by the
    size of the response
  */
 void process2message(LogMessage& message){
   //get status code before we grab the lock
   LogResponseCode rc = message.getResponseCode();
   //make sure this doesn't break anything
   std::lock_guard<std::mutex> guard(statsGuard);
   stats.setTotResp(stats.getTotResp() + 1);
   switch(rc){
      case LogResponseCode::OK : 
        stats.setOkCount(stats.getOKCount() + 1);
        stats.setOKPerSec(stats.getOKPerSec() + 1);
        break;
      case LogResponseCode::NOTFOUND :
        stats.setNotFoundCount(stats.getNotFoundCount() + 1);
        stats.setNFPerSec(stats.getNFPerSec() + 1);
        break;
      case LogResponseCode::ERROR :
        stats.setErrorCount(stats.getErrorCount() + 1);
        stats.setErrorPerSec(stats.getErrorPerSec() + 1);
        break;
      //in any other case, increment other count
      default:
        stats.setOtherCount(stats.getOtherCount() + 1);
        stats.setOtherPerSec(stats.getOtherPerSec() + 1);
   }
   //in any case, adjust the response throughtput by the size of the message body
   stats.setRespPerSec(stats.getRespPerSec() + message.getResponseSize());
 }

 //set the column names for the stats log file
 void nameStatColumns(){
   statsStream<<"ReqPerSec\tOKPerSec\tNotFoundPerSec\tErrorPerSec\tOtherPerSec\tResponseThroughPutBytes"<<std::endl;
 }

};

#endif
