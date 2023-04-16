#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <string>
#include <cstring>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

class FlightInfo
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
};

class SingleAirport
{
private:
    string icaoCode;
    vector<FlightInfo *> arrivals;
    vector<FlightInfo *> departures;

public:
    SingleAirport(string _icaoCode) : icaoCode(_icaoCode)
    {
        arrivals.reserve(200);
        departures.reserve(200);
    }
    ~SingleAirport()
    {
        for(vector<FlightInfo *>::iterator it = arrivals.begin(); it != arrivals.end(); ++it)
            delete *it;
        arrivals.clear();
        
        for(vector<FlightInfo *>::iterator it = departures.begin(); it != departures.end(); ++it)
        {
            delete *it;
        }
        departures.clear();

    }
    string getIcaoCode() const { return icaoCode;} 
    vector<FlightInfo*> getArivals() const { return arrivals; }
    vector<FlightInfo*> getDepartures() const { return departures; }

    void addFlightToAirport(FlightInfo &flight)
    {
        if (flight.getArvOrDpt() == 'a')
            arrivals.push_back(&flight);
        else // arvOrDpt == 'd'
            departures.push_back(&flight);
    }
    void setIcaoCode(const string &airportName)
    {
        icaoCode = airportName;
    }
    void updateAirportDataFlights(string &path);

};

class AllAirports
{
    vector<SingleAirport *> airportsVector;

public:
    friend SupportFunctions;
    AllAirports()
    {
        airportsVector.reserve(10);
    }
    ~AllAirports()
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
    vector<SingleAirport*> getAirportsVector() const { return airportsVector; }
    void getAllAirportsNames(vector<string> &airportNames);
    vector<FlightInfo *> getFlightsByCallsign(string &callsign);
    SingleAirport& getAirportByName(string& airportName);

};

class SupportFunctions
{
    public:
    string getPathType(string &path);
    void getAllPaths(vector<string> &paths);
    string getAirportName(string &path);
    friend void load_db();
    void regenerate_db();
};