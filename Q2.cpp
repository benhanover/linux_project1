#include "utility.h"
/*Receive airport name and output(print) the full airport schedule containing 
both arrivals and departures ordered by time.*/

string getNewNameFromDBorRerunScript(string missing_name, vector<string>& paths, System& airports);
void printFullAirportSchedule(string& IcoaCode, System& airports);
int compare2Flights(FlightInfo* f1, FlightInfo* f2);

int main(int argc, char* argv[])
{
    System airports;
    
    if (argc == 1)
    {
        cout << "No code name inserted." << endl;
        cout << "Exiting program. Try To run again with ICOA code name (only one)." << endl;
        return 0;
    }

    if (argc > 2)
    {
        cout << "You entered more than one ICOA code name." << endl;
        cout << "This program handles only one airport, so it will take the first code inserted." << endl;
    }
    
    vector<string> paths;
    paths.reserve(10);
    vector<string> missing_name;
    airports.getAllPaths(paths);
    bool wantedNameInDB = false;
    wantedNameInDB = airports.checkIfAllInDB(paths,missing_name,2,argv);
    
    string newNameFromDB = "";
    if (!wantedNameInDB)
       newNameFromDB = getNewNameFromDBorRerunScript(missing_name[0],paths, airports);
    
    if (newNameFromDB == "") //printing data of code in argv[1], if existed before or dowloaded in function getNewName(...)
    {
       airports.getAllPaths(paths);
       airports.load_db(paths);
       string airportName = argv[1];
       printFullAirportSchedule(airportName,airports);
    }
    else
    {
       airports.load_db(paths);
       printFullAirportSchedule(newNameFromDB,airports);
    }
    return 0;
}

string getNewNameFromDBorRerunScript(string missing_name, vector<string>& paths,System& airports)
{
    int option;
    string wantedName;
    cout << "The first ICOA code name inserted (" << missing_name << ") doesn't exist in current database." << endl;
     cout << "You have 2 options" << endl;
    cout << "Press 1 to run script with this name to try download info to database." << endl;
    cout << "Press 2 to choose code name that exist in the database." << endl;
    cin >> option;

    fs::path currentPath = fs::current_path();
    string projectPath = currentPath;
    string airportName = " " + missing_name;
   
    if (option == 1)        
    {
       system(((projectPath + "/flightScanner.sh ") += airportName).c_str());
       return "";
    }
    else //option == 2
    {
        cout << "You pressed 2. A list of names that exist in the database will be printed." << endl;
        cout << "You need to write one of them to continue and get the full schedule." << endl ;
        cout << "Names that exist in database: " ;
        for (int i=0; i < paths.size(); i += 2)
            cout << airports.getAirportNameFromPath(paths[i]) << "  ";
        cout << endl << "Please enter only 1 name from this list." << endl;
        cin >> wantedName;
        return wantedName;
    }
}

void printFullAirportSchedule(string& IcoaCode, System& airports)
{
    int index = airports.getAirportIndexByName(IcoaCode);
    vector<FlightInfo*> combine;
    vector<SingleAirport*> airportsVector = airports.getAirportsVector();

    combine.reserve(airportsVector[index]->getArivals().size() + airportsVector[index]->getDepartures().size());

    for(auto& flightInfo: airportsVector[index]->getArivals())
        combine.push_back(flightInfo);
        
    for(auto& flightInfo: airportsVector[index]->getDepartures())
        combine.push_back(flightInfo);

    sort(combine.begin(), combine.end(), compare2Flights);

    for (auto& flightInfo: combine)
    {
        if (flightInfo->getArvOrDpt() == 'a') //will print lastSeen - arrival time
            cout << "Flight #" << flightInfo->getCallsign() << " arriving from " << flightInfo->getEstDepartureAirport() << " at " << flightInfo->getLastSeen() << endl;
        else                                  //will print firstSeen - departure time
            cout << "Flight #" << flightInfo->getCallsign() << " departing to " << flightInfo->getEstDepartureAirport() << " at " << flightInfo->getFirstSeen() << endl;
    }
}

int compare2Flights(FlightInfo* f1, FlightInfo* f2)
{
    char f1ArvOrDpt = f1->getArvOrDpt();
    char f2ArvOrDpt = f2->getArvOrDpt();

    if (f1ArvOrDpt == 'a' && f2ArvOrDpt =='a')
        return (stoi(f1->getLastSeen()) < stoi(f2->getLastSeen())); //comparing 2 arrivals

    else if (f1ArvOrDpt == 'd' && f2ArvOrDpt =='d')
        return (stoi(f1->getFirstSeen()) < stoi(f2->getFirstSeen())); //comparing 2 departures
    
    else if (f1ArvOrDpt == 'a' && f2ArvOrDpt =='d')
        return (stoi(f1->getLastSeen()) < stoi(f2->getFirstSeen())); //comparing arrival(f1) and departure(f2)   
    
    else //(f1ArvOrDpt == 'd' && f2ArvOrDpt =='a')
        return (stoi(f1->getFirstSeen()) < stoi(f2->getLastSeen())); //comparing departure(f1) and arrival(f2)
}