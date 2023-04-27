#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <string>
#include <cstring>
#include <filesystem>
#include <algorithm>

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
       deleteAll(); 
    }    
    void deleteAll();
    void addAirport(SingleAirport* airport)
    {
        airportsVector.push_back(airport);
    }

    vector<SingleAirport *> getAirportsVector() const { return airportsVector; }
    void getAllAirportsNames(vector<string> &airportNames);
    vector<FlightInfo *> getFlightsByCallsign(string &callsign);
    int getAirportIndexByName(string& airportName);
    static string getPathType(string &path);
    void getAllPaths(vector<string> &paths);
    string getAirportNameFromPath(string &path);
    bool checkIfAllInDB(vector<string>& paths, vector<string>& missing_names, int numOfCodesRecieved, char** codesRecievedArr);
    void load_db(vector<string>& paths);
    void regenerate_db();
    bool isAircraftInDB(string code);
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
          {
            delete *it;
          } 
        arrivals.clear();
        
        for(vector<FlightInfo*>::iterator it = departures.begin(); it != departures.end(); ++it)
        {
            delete *it;
        }
        departures.clear();
        cout << "delete single airport" << endl;
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
    string getIcao24() const {return icao24; }
    string getCallsign()const { return callsign; }
    FlightInfo* getCurrentFlightInfo(char* currentLine, string& pathType);
    string getEstArrivalAirport() const {return estArrivalAirport;}
    string getEstDepartureAirport() const {return estDepartureAirport;}
    string getFirstSeen() const { return to_string(firstSeen); }
    string getLastSeen() const { return to_string(lastSeen); }
    string getAircraftName() const{return icao24;}
    void addFlightToAirport(SingleAirport& airport)
    {
        if (getArvOrDpt() == 'a')
            airport.arrivals.push_back(this);
        else // arvOrDpt == 'd'
            airport.departures.push_back(this);
    }
};