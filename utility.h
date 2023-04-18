#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <string>
#include <cstring>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

class SingleAirport;
class FlightInfo;

class System
{
    vector<SingleAirport*> airportsVector;

public:
    System()
    {
        airportsVector.reserve(10);
    }
    ~System()
    {
         for(vector<SingleAirport *>::iterator it = airportsVector.begin(); it != airportsVector.end(); ++it)
        {
            delete *it;
        }
        airportsVector.clear();
    }
    void addAirport(SingleAirport* airport)
    {
        airportsVector.push_back(airport);
    }
    vector<SingleAirport *> getAirportsVector() const { return airportsVector; }
    void getAllAirportsNames(vector<string> &airportNames);
    vector<FlightInfo *> getFlightsByCallsign(string &callsign);
    SingleAirport& getAirportByName(string& airportName);
    bool loaded_DB;
    static string getPathType(string &path);
    void getAllPaths(vector<string> &paths);
    string getAirportNameFromPath(string &path);
    void load_db();
    void regenerate_db();
};


class SingleAirport : public System
{
private:
    string icaoCode;
    vector<FlightInfo*> arrivals;
    vector<FlightInfo*> departures;

public:
    friend FlightInfo;
    SingleAirport() = default;
    SingleAirport(string _icaoCode) : icaoCode(_icaoCode)
    {
        arrivals.reserve(200);
        departures.reserve(200);
    }
    ~SingleAirport()
    {
        for(vector<FlightInfo*>::iterator it = arrivals.begin(); it != arrivals.end(); ++it)
            delete *it;
        arrivals.clear();
        
        for(vector<FlightInfo*>::iterator it = departures.begin(); it != departures.end(); ++it)
        {
            delete *it;
        }
        departures.clear();

    }
    string getIcaoCode() const { return icaoCode;} 
    vector<FlightInfo*> getArivals() const { return arrivals; }
    vector<FlightInfo*> getDepartures() const { return departures; }

    void setIcaoCode(const string &airportName)
    {
        icaoCode = airportName;
    }
    void updateAirportDataFlights(string &path);

};

class FlightInfo : public SingleAirport
{
    char arvOrDpt;
    string icao24;
    int firstSeen;
    string estDepartureAirport;
    int lastSeen;              
    string estArrivalAirport;  
    string callsign;           

public:
    FlightInfo(char _arvOrDpt, string _icao24, int _firstSeen, string _estDepartureAirport, int _lastSeen,
               string _estArrivalAirport, string _callsign) 
    {
        arvOrDpt = _arvOrDpt;   
        icao24 = _icao24;
        firstSeen = _firstSeen;
        estDepartureAirport = _estDepartureAirport;
        lastSeen = _lastSeen;
        estArrivalAirport = _estArrivalAirport;
        callsign = _callsign;
    }
    char getArvOrDpt() const {return arvOrDpt; }
    string getCallsign()const { return callsign; }
    FlightInfo* getCurrentFlightInfo(char* currentLine, string& pathType);
    void addFlightToAirport(SingleAirport& airport)
    {
        if (getArvOrDpt() == 'a')
            airport.arrivals.push_back(this);
        else // arvOrDpt == 'd'
            airport.departures.push_back(this);
    }
};