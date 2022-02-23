/*
  This file contains defintions related to the logging functionality of the
  project. Right now it's just a simple function that checks a shared vector
  of messages, eventually it will be an actual logging library.
*/
#ifndef LOGGING_HPP
#define LOGGING_HPP
#include <string>
#include <vector>
#include <thread>
#include <fstream>
#include <semaphore.h>
#include <mutex>
//define the shared buffer of messages and the semaphore that guards them
struct Logger{
    std::vector<std::string> messageQueue;
    sem_t queueSize;
    std::mutex queueGuard;
    bool quitFlag;
    std::ofstream outStream;
    //default constructor lets us initialize the semaphore
    Logger(std::string filename){
        try{
          outStream.open(filename.c_str());
        }
        catch(...){
          std::cout<<"Failted to open log file"<<std::endl;
          exit(1);
        }
        sem_init(&(this->queueSize),0,0);
        quitFlag = false;
    }
    //write one message from the queue to the underlying file
    void writeMessage(){
      queueGuard.lock();
      std::string front = messageQueue.front();
      outStream << front <<std::endl;
      messageQueue.erase(messageQueue.begin());
      queueGuard.unlock();
    }

    //post a message to the queue
    void postMessage(std::string& message){
      queueGuard.lock();
      messageQueue.push_back(message);
      sem_post(&queueSize);
      queueGuard.unlock();
    }

    //write all the messages in the queue to the file and exit, used when we're quitting
    void quit(){
      //we lock just in case, but no one should be logging anymore
      queueGuard.lock();
      for (int i = 0; i < messageQueue.size();i++){
        outStream << messageQueue[i] <<std::endl;
      }
      queueGuard.unlock();
      //log a quit message
      outStream << "Quit signal received. Shutting down" <<std::endl;
      //close the file so stuff is saved.
      outStream.close();
    }
    
    //here's the function that runs in the logging thread
    void threadFunc(){
      while(true){
        sem_wait(&queueSize);
        if(quitFlag){
          quit();
        }
        else{
          writeMessage();
        }
      }
      
    }

  
};

//also deine the error logger function that gets passed to the server's log_error method



#endif