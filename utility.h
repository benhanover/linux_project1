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
    char arvOrDpt; //'a' for arrival, 'd' for departure
    string icao24;
    int dptTime;
    string estDptIcao;
    int arvTime;
    string estArvIcao;
    string callsign;
    int estDptAirportHorizDistance;
    int estDptAirportVertDistance;
    int estArvAirportHorizDistance;
    int estArvAirportVertDistance;
    int dptAirportCandidatesCount;
    int arvAirportCandidatesCount;

    public:
    //ctor
        FlightInfo(char _arvOrDpt, string _icao24, int _dptTime, string _estDptIcao, int _arvTime,
                   string _estArvIcao, string _callsign, int _estDptAirportHorizDistance, int _estDptAirportVertDistance,
                   int _estArvAirportHorizDistance, int _estArvAirportVertDistance, int _dptAirportCandidatesCount, int _arvAirportCandidatesCount)
                {
                    arvOrDpt = _arvOrDpt;
                    icao24 = _icao24;
                    dptTime = _dptTime;
                    estDptIcao = _estDptIcao;
                    arvTime = _arvTime;
                    estArvIcao = _estArvIcao;
                    callsign = _callsign;
                    estDptAirportHorizDistance = _estDptAirportHorizDistance;
                    estDptAirportVertDistance = _estDptAirportVertDistance;
                    estArvAirportHorizDistance = _estArvAirportHorizDistance;
                    estArvAirportVertDistance = _estArvAirportVertDistance;
                    dptAirportCandidatesCount = _dptAirportCandidatesCount;
                    arvAirportCandidatesCount = _arvAirportCandidatesCount;
                }

    char getArvOrDpt() {return arvOrDpt;}
    string getCallsign() {return this->callsign;}

};

class SingleAirport
{
private:
    string icaoCode;
    vector<FlightInfo*> arrivals;
    vector<FlightInfo*> departures;

public:
    SingleAirport(string _icaoCode):icaoCode(_icaoCode)
    {
        arrivals.reserve(100);
        departures.reserve(100);

    }
    string getIcaoCode() {return this->icaoCode;}
    vector<FlightInfo*> getArivals(){return this->arrivals;}
    vector<FlightInfo*> getDepartures(){return this->departures;}

    void addFlightToAirport(FlightInfo& flight)
    {
        if (flight.getArvOrDpt() == 'a')
         arrivals.push_back(&flight);
         else //arvOrDpt == 'd'
         departures.push_back(&flight);
    }
    void setIcaoCode(const string& airportName) {
        this->icaoCode = airportName;
        }
};

class AllAirports
{
    vector<SingleAirport*> airportsVector;
    
    public:
    AllAirports() {
        airportsVector.reserve(10);
    }
    void addAirport(SingleAirport* airport) {
        airportsVector.push_back(airport);
    }
    vector<SingleAirport*>& getAirportsVector() {return this->airportsVector;}

};

//Main Functions
AllAirports* load_db();
SingleAirport& getFlightsByAirportName(AllAirports& airports, string& airportName);
vector<FlightInfo*> getFlightsByCallsign(AllAirports& airports, string& callsign);
void regenerate_db(AllAirports** airportsPtr);

//Helpers
void updateAirportDataFlights(SingleAirport& currentAirport, string& path);
void getAllPaths(vector<string> & paths);
string getAirportName(string& path);
string getPathType(string& path);
FlightInfo* getCurrentFlightInfo(char* currentLine, string& pathType);
