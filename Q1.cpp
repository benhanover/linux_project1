#include "utility.h"
/*Recieve airport ICOA code names and output(print) a table of which airports have flights
to the given airports and the flight details.*/

void notAllInDB(vector<string>& missing_names);
void printAirportArv(System& airports, string& IcoaCode);


int main(int argc, char* argv[])
{
    System airports;
    vector<string> paths;
    paths.reserve(10);
    vector<string> missing_names;
    missing_names.reserve(10);

    if (argc == 1)
    {
        cout << "No code names inserted." << endl;
        cout << "Exiting program. Try To run again with ICOA code names." << endl;
        return 0;
    }

    airports.getAllPaths(paths);
    bool allInDB = false;
    allInDB = airports.checkIfAllInDB(paths,missing_names,argc,argv);
    
    if (!allInDB)
        notAllInDB(missing_names);
    else
        cout << "All in DB. Printing as asked." << endl;
    
    airports.getAllPaths(paths);
    airports.load_db(paths);
    string curAircraftName;
    
    for(int i = 1; i < argc; i++)
    {
        curAircraftName = argv[i]; 
        printAirportArv(airports,curAircraftName);
    }
    return 0;
}


void notAllInDB(vector<string>& missing_names)
{
    int option;

    cout << "Not all ICOA code names inserted exist in current database." << endl;
    cout << "These names doesn't exist in the database:" << endl;
    for (int i = 0; i < missing_names.size(); i++)
    cout << missing_names[i] << ' ';
    cout << endl;
        
    cout << "You have 2 options" << endl;
    cout << "Press 1 to run script with these names to try download info to database." << endl;
    cout << "Press 2 to display data of airports that already exist." << endl ;
    cin >> option;

    
    fs::path currentPath = fs::current_path();
    string projectPath = currentPath;
    string airportNames = "";
    for (auto& name : missing_names) 
        airportNames += name += " ";
    
    if (option == 1)        
        system(((projectPath + "/flightScanner.sh ") += airportNames).c_str());
    else return; //will be printed by main

}

void printAirportArv(System& airports,string& IcoaCode)
{
    int airportIndex = airports.getAirportIndexByName(IcoaCode);
    vector<SingleAirport*> airportsVector = airports.getAirportsVector();
    for (auto& flightInfo: airportsVector[airportIndex]->getArivals())
    {
        cout << "Flight #" << flightInfo->getCallsign() << " arriving from " << flightInfo->getEstDepartureAirport() << ", tookoff at " << flightInfo->getFirstSeen() <<  " landed at " << flightInfo->getLastSeen() << endl;
    }
}
