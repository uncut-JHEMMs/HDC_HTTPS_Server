/*
    A class thats stores all the server statistics for reporting
*/
#ifndef SERVERSTATS_HPP
#define SERVERSTATS_HPP
class ServerStats{

    private:
    unsigned int totalRequests;
    unsigned int totalResponses;
    unsigned int responsePerSecond; //tracks response throughput as bytes per second
    unsigned int requestsPerSecond;
    unsigned int okCount; //count of 200 response codes
    unsigned int okPerSecond;
    unsigned int notFoundCount; // count of 404 response codes
    unsigned int notFoundPerSecond;
    unsigned int errorCount; // 500 count
    unsigned int errorPerSecond; 
    unsigned int otherCount; //any code besides the above
    unsigned int otherPerSecond;

    public:
    //default constructor just sets all the members to 0
    ServerStats(): totalRequests(0), totalResponses(0), requestsPerSecond(0),
        okCount(0), okPerSecond(0), notFoundCount(0), notFoundPerSecond(0),
        errorCount(0), errorPerSecond(0), otherCount(0), otherPerSecond(0){}

    //getters and setters
    //totalRequests
    void setTotReq(const unsigned int newTR){
        totalRequests = newTR;
    }
    unsigned int getTotReq() const{
        return totalRequests;
    }
    //totalResponses
    void setTotResp(const unsigned int newTR){
        totalResponses = newTR;
    }
    unsigned int getTotResp() const{
        return totalResponses;
    }
    //response throughput
    void setRespPerSec(const unsigned int newRPS){
        responsePerSecond = newRPS;
    }
    unsigned int getRespPerSec() const{
        return responsePerSecond;
    }
    //requestsPerSecond
    void setReqPerSec(const unsigned int newRPS){
        requestsPerSecond = newRPS;
    }
    unsigned int getReqPerSec() const{
        return requestsPerSecond;
    }
    //okCount
    void setOkCount(const unsigned int newOK){
        okCount = newOK;
    }
    unsigned int getOKCount() const{
        return okCount;
    }
    //okPerSecond
    void setOKPerSec(const unsigned int newOK){
        okPerSecond = newOK;
    }
    unsigned int getOKPerSec() const{
        return okPerSecond;
    }
    //notFoundCount
    void setNotFoundCount(const unsigned int newNF){
        notFoundCount = newNF;
    }
    unsigned int getNotFoundCount() const{
        return notFoundCount;
    }
    //notFoundPerSecond
    void setNFPerSec(const unsigned int newNF){
        notFoundPerSecond = newNF;
    }
    unsigned int getNFPerSec() const{
        return notFoundPerSecond;
    }
    //errorCount
    void setErrorCount(const unsigned int newErr){
        errorCount = newErr;
    }
    unsigned int getErrorCount() const{
        return errorCount;
    }
    //errorPerSecond
    void setErrorPerSec(const unsigned int newErr){
        errorPerSecond = newErr;
    }
    unsigned int getErrorPerSec() const{
        return errorPerSecond;
    }
    //otherCount
    void setOtherCount(const unsigned int newOther){
        otherCount = newOther;
    }
    unsigned int getOtherCount() const{
        return otherCount;
    }
    //otherPerSecond
    void setOtherPerSec(const unsigned int newOther){
        otherPerSecond = newOther;
    }
    unsigned int getOtherPerSec() const{
        return otherPerSecond;
    }

    //reset all the per-second counts when we've taken the metric
    void resetPerSec(){
        setReqPerSec(0);
        setRespPerSec(0);
        setOKPerSec(0);
        setNFPerSec(0);
        setErrorPerSec(0);
        setOtherPerSec(0);
    }
};
#endif