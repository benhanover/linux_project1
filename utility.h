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
    char arvOrDpt;                        //'a' for arrival, 'd' for departure
    string icao24;                        // Unique ICAO 24-bit address of the transponder in hex string representation
    int firstSeen;                        // Estimated time of departure for the flight as Unix time (seconds since epoch).
    string estDepartureAirport;           // ICAO code of the estimated departure airport. Can be null if the airport could not be identified.
    int lastSeen;                         // Estimated time of arrival for the flight as Unix time (seconds since epoch)
    string estArrivalAirport;             // ICAO code of the estimated arrival airport. Can be null if the airport could not be identified.
    string callsign;                      // Callsign of the vehicle (8 chars). Can be null if no callsign has been received. If the vehicle transmits multiple callsigns during the flight, we take the one seen most frequently
    int estDepartureAirportHorizDistance; // Horizontal distance of the last received airborne position to the estimated departure airport in meters
    int estDepartureAirportVertDistance;  // Vertical distance of the last received airborne position to the estimated departure airport in meters
    int estArrivalAirportHorizDistance;   // Horizontal distance of the last received airborne position to the estimated arrival airport in meters
    int estArrivalAirportVertDistance;    // Vertical distance of the last received airborne position to the estimated arrival airport in meters
    int departureAirportCandidatesCount;  // Number of other possible departure airports. These are airports in short distance to estDepartureAirport.
    int arrivalAirportCandidatesCount;    // Number of other possible departure airports. These are airports in short distance to estArrivalAirport.

public:
    // ctor
    FlightInfo(char _arvOrDpt, string _icao24, int _firstSeen, string _estDepartureAirport, int _lastSeen,
               string _estArrivalAirport, string _callsign, int _estDptAirportHorizDistance, int _estDptAirportVertDistance,
               int _estArvAirportHorizDistance, int _estArvAirportVertDistance, int _dptAirportCandidatesCount, int _arvAirportCandidatesCount)
    {
        arvOrDpt = _arvOrDpt;
        icao24 = _icao24;
        firstSeen = _firstSeen;
        estDepartureAirport = _estDepartureAirport;
        lastSeen = _lastSeen;
        estArrivalAirport = _estArrivalAirport;
        callsign = _callsign;
        estDepartureAirportHorizDistance = _estDptAirportHorizDistance;
        estDepartureAirportVertDistance = _estDptAirportVertDistance;
        estArrivalAirportHorizDistance = _estArvAirportHorizDistance;
        estArrivalAirportVertDistance = _estArvAirportVertDistance;
        departureAirportCandidatesCount = _dptAirportCandidatesCount;
        arrivalAirportCandidatesCount = _arvAirportCandidatesCount;
    }

    char getArvOrDpt() { return arvOrDpt; }
    string getCallsign() { return this->callsign; }
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
        arrivals.reserve(100);
        departures.reserve(100);
    }
    ~SingleAirport()
    {
        FlightInfo * flightInfoPtr = nullptr;
        for(vector<FlightInfo *>::iterator it = arrivals.begin(); it != arrivals.end(); ++it)
        {
            flightInfoPtr = *it;
            delete flightInfoPtr;
        }
        arrivals.clear();
        
        for(vector<FlightInfo *>::iterator it = departures.begin(); it != departures.end(); ++it)
        {
            flightInfoPtr = *it;
            delete flightInfoPtr;
        }
        departures.clear();

    }
    string getIcaoCode() { return this->icaoCode; }
    vector<FlightInfo *> getArivals() { return this->arrivals; }
    vector<FlightInfo *> getDepartures() { return this->departures; }

    void addFlightToAirport(FlightInfo &flight)
    {
        if (flight.getArvOrDpt() == 'a')
            arrivals.push_back(&flight);
        else // arvOrDpt == 'd'
            departures.push_back(&flight);
    }
    void setIcaoCode(const string &airportName)
    {
        this->icaoCode = airportName;
    }
};

class AllAirports
{
    vector<SingleAirport *> airportsVector;

public:
    AllAirports()
    {
        airportsVector.reserve(10);
    }
    ~AllAirports()
    {
        SingleAirport * singleAirportPtr;
         for(vector<SingleAirport *>::iterator it = airportsVector.begin(); it != airportsVector.end(); ++it)
        {
            singleAirportPtr = *it;
            delete singleAirportPtr;
        }
        airportsVector.clear();
    }
    void addAirport(SingleAirport* airport)
    {
        airportsVector.push_back(airport);
    }
    vector<SingleAirport*>& getAirportsVector() { return this->airportsVector; }

};

class SystemManagerClass
{
    AllAirports* allAirportsPtr;
    bool loaded_DB;

public:
    SystemManagerClass()
    {
        allAirportsPtr = nullptr;
        loaded_DB = false;        
    }
    ~SystemManagerClass(){ delete allAirportsPtr;} //deletes all allocated memory

    // Main Functions
    void load_db();
    SingleAirport & getFlightsByAirportName(string &airportName);
    vector<FlightInfo *> getFlightsByCallsign(string &callsign);
    void regenerate_db();

    // Helpers
    void updateAirportDataFlights(SingleAirport &currentAirport, string &path);
    void getAllPaths(vector<string> &paths);
    string getAirportName(string &path);
    string getPathType(string &path);
    FlightInfo *getCurrentFlightInfo(char *currentLine, string &pathType);
    void getAllAirportsNames(vector<string> &airportNames);
    
};