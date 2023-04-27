#include "utility.h"
//Receive list of aircraft names(icao24) and output(print) all flights (departures and arrivals) that it did.

bool checkIfAllAircraftsInDB(System& airports, vector<string>& missing_names, int numOfCodesRecieved, char** codesRecievedArr);
void printAircraftFlights(string& icao24, System& airports);

int main(int argc, char* argv[])
{
    System airports;
    vector<string> paths;
    paths.reserve(10);
    vector<string> missing_names;
    missing_names.reserve(argc);

    if (argc == 1)
    {
        cout << "No code names inserted." << endl;
        cout << "Exiting program. Try To run again with ICOA code names." << endl;
        return 0;
    }

    airports.getAllPaths(paths);
    bool allInDB = false;
    airports.load_db(paths);

    allInDB = checkIfAllAircraftsInDB(airports,missing_names,argc,argv);
    
    if (!allInDB)
        cout << "Not all aircrafts in database. Printing only those that exist." << endl;
    airports.load_db(paths);
    
    string curAircraft;
    for(int i = 1; i < argc; i++)
    {
       curAircraft = argv[i];
       if (find(missing_names.begin(), missing_names.end(), curAircraft) != missing_names.end())
            continue;// The current name missing.
       else 
        printAircraftFlights(curAircraft,airports);
    }


    return 0;
}

bool checkIfAllAircraftsInDB(System& airports, vector<string>& missing_names, int numOfCodesRecieved, char** codesRecievedArr)
{
    for (int i=1; i< numOfCodesRecieved; i++)
    {   
        string aircraft = codesRecievedArr[i];
        bool existInDB = airports.isAircraftInDB(aircraft);
        if(!existInDB)
            missing_names.push_back(aircraft);
    }

    if (missing_names.empty())
        return true; //no missing names, all arguments in DB
    else 
        return false;

}



void printAircraftFlights(string& icao24, System& airports)
{
    string aircraftName;
    vector<SingleAirport*> airportsVector = airports.getAirportsVector();

    for(auto& airport: airportsVector)
    {
        for (auto& flightInfo: airport->getArivals())
        {
            if(flightInfo->getAircraftName() == icao24)
            {
                cout << icao24 << " departed from  " << flightInfo->getEstDepartureAirport() << " at " << flightInfo->getFirstSeen() <<  " arrived in " 
                  << flightInfo->getEstArrivalAirport() << " at " <<flightInfo->getLastSeen() << endl;
            }
        }
        for (auto& flightInfo: airport->getDepartures())
        {
            if(flightInfo->getAircraftName() == icao24)
            {
                cout << flightInfo->getAircraftName() << " departed from  " << flightInfo->getEstDepartureAirport() << " at " << flightInfo->getFirstSeen() <<  " arrived in " 
                    << flightInfo->getEstArrivalAirport() << " at " <<flightInfo->getLastSeen() << endl;
            }
        }
    }
}