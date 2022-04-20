#ifndef LOCATIONS_HPP
#define LOCATIONS_HPP
/*
    contains information related to locations. The city numbers are just going to
    come from the file: we see the first city, it's 0, the next city is 1, etc.
    The zip codes will be related to the cities.
*/
class LocationRecord{
    std::string city;
    std::string state;
    std::string zipCode;
    unsigned int index;

    public:
    LocationRecord(): city(""), state(""), zipCode(""), index(0){}

    LocationRecord(std::string newCity, std::string newState,
        std::string newZip, unsigned int newIndex): city(newCity), state(newState),
        zipCode(newZip), index(newIndex){}

    unsigned int getIndex() const{
        return index;
    }
    void setIndex(const unsigned int newIndex){
        index = newIndex;
    }
    std::string getCity() const{
        return city;
    }
    void setCity(const std::string& newCity){
        city = newCity;
    }
    std::string getState() const{
        return state;
    }
    void setState(const std::string& newState){
        state = newState;
    }
    std::string getZip() const{
        return zipCode;
    }
    void setZip(const std::string& newZip){
        zipCode = newZip;
    }
};

#endif