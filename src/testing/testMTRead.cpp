//test whether two different threads can open the same file and read the same lines, or they both do it from the same file pointer
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <mutex>

std::mutex m;
//line reader function reads a single line and puts it on the console
/*
    if we do it this way, there's a race to get control of one of the streams, and 
    the threads only read disjoint portions of the file. This is NOT what we want.
*/
void readLine(std::ifstream& file)
{
    m.lock();
    std::string s;
    std::getline(file,s);
    std::cout << s << std::endl;
    m.unlock();
}

//when we do it this way, both threads read the whole file (the outputs are interleaved, but that's not an issue where we're going)
void readLineOpenFile(const std::string& fileName)
{
    m.lock();
    std::ifstream stream(fileName, std::ios::in);
    std::string s;
    while (std::getline(stream, s))
    {
        std::cout << s << std::endl;
    }
    m.unlock();
}
int main()
{
    std::ifstream inStream("testMTRead.txt", std::ios::in);
    std::cout << "two threads sharing reference to one stream object" << std::endl;
    std::thread t1(readLine, std::ref(inStream));
    std::thread t2(readLine, std::ref(inStream));
    //if the threads use different file pointers, we'll get two distinct lines
    t1.join();
    t2.join();
    std::cout << "each function makes it's own stream from the same file" << std::endl;
    std::string filename{"testMTRead.txt"};
    std::thread t3(readLineOpenFile,std::ref(filename));
    std::thread t4(readLineOpenFile, std::ref(filename));
    t3.join();
    t4.join();
    return 0;
}