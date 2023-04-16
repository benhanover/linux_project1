#include "utility.h"

#define nullNumFlag -1

int main()
{
    AllAirports airports;
    //AllAirports* airports;
    SupportFunctions::load_db(airports);
    string testAirportName = "EGKK", testCallsignSHT9X = "SHT9X";
    SingleAirport EGKK = airports.getFlightsByAirportName(testAirportName);
    vector<FlightInfo*> SHT9X = airports.getFlightsByCallsign(testCallsignSHT9X);
    airports.regenerate_db();
    return 0;
}

//test


//Main Functions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SupportFunctions::regenerate_db()
{
    AllAirports* airports = allAirportsPtr;
    vector<string> airportsNamesVector;
    getAllAirportsNames(airportsNamesVector);
    delete airports; //destructor will free all allocated memory

    fs::path currentPath = fs::current_path();
    string projectPath = currentPath;
    string airportNames = "";
    for (auto& name:airportsNamesVector) 
        airportNames += name += " ";

    //delete previous DB
    system(((projectPath + "/clean.sh ") += airportNames).c_str());
    //create data base
    system(((projectPath + "/flightScanner.sh ") += airportNames).c_str());
    load_db();
}

void SupportFunctions::load_db(&AllAirports airports) {
    airports = new AllAirports();
    vector<string> paths;

    vector<SingleAirport*> airpoprtsVector = airports->getAirportsVector();

    getAllPaths(paths);
    //for each airport there are 2 paths (apt, dpt)
    int numberOfAirports = paths.size() / 2;

    for (int i = 0; i < numberOfAirports; i++)
    {
        //the order of the paths 0 - airport1, 1 - airport1, 2 - airport2, 3 - airport2...
        SingleAirport* currentAirport = new SingleAirport(getAirportNameFromPath(paths[2*i]));

        //one of calls below is for .apt and the second is .dpt - unorginized order
        updateAirportDataFlights(*currentAirport, paths[2 * i]);
        updateAirportDataFlights(*currentAirport, paths[(2 * i) + 1]);

        airpoprtsVector.push_back(currentAirport);
 
    }
    loaded_DB = true;
    allAirportsPtr = airports;
}

SingleAirport& AllAirports::getAirportByName(string& airportName) {

    //if (loaded_DB == false)
      //  load_db();
    
    for (auto& airport : this->airportsVector)
    {
        if (airport->getIcaoCode() == airportName)
            return *airport;
    }

    ///////////////********what if airportName doesn't exist? error message? ********
    
}

vector<FlightInfo*> AllAirports::getFlightsByCallsign(string& callsign)
{    
    vector<FlightInfo*> flightsByCallsign;


    for (auto &airport : airportsVector)
    {
        // arrivals
        for (auto &flightInfo : airport->getArivals())
            if (flightInfo->getCallsign() == callsign)
                flightsByCallsign.push_back(flightInfo);
        // departures
        for (auto &flightInfo : airport->getDepartures())
            if (flightInfo->getCallsign() == callsign)
                flightsByCallsign.push_back(flightInfo);
    }
    return flightsByCallsign;

   ///////////////********what if callsign doesn't exist? error message? ********

}

//Helpers functions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SingleAirport::updateAirportDataFlights(string& path)
{
    string currentLine;
    //apt / dpt
    string pathType = SupportFunctions::getPathType(path);
    ifstream currentFile(path);

    if (!currentFile.is_open()) {
        cout << "wrong path was given";
    }

    // skip the first line
    getline(currentFile, currentLine);
        

    while (currentFile)
    {
        getline(currentFile, currentLine);
        char* currentLineAsChar = (char*)currentLine.c_str();
        FlightInfo* currentFlightInfo = FlightInfo::getCurrentFlightInfo(currentLineAsChar, pathType);
        addFlightToAirport(*currentFlightInfo);

    }

    ////////////////********we need to close the file?********
    ///////********what if path doesn't lead to a wanted file? error message? ********
}



///////********what if currentLine is empty? error message? ********

//problems with nulls - inserting last flight values
FlightInfo* FlightInfo::getCurrentFlightInfo(char* currentLine, string& pathType) {
    const char delim[] = ", ";
    // all needed variables for FlightInfo
    string icao24, estDepartureAirport, estArrivalAirport, callsign;
    int firstSeen, lastSeen;

    char* token = strtok(currentLine, delim);
    int countTokens = 0;
    while (token)
    {
        countTokens++;
        switch (countTokens)
        {
        case 1:
            icao24 = token;
            break;
        case 2:
            if (strcmp(token, "null") != 0)
                firstSeen = stoi(token);
            else
                firstSeen = nullNumFlag;
            break;
        case 3:
            estDepartureAirport = token;
            break;
        case 4:
            if (strcmp(token, "null") != 0)
                lastSeen = stoi(token);
            else
                lastSeen = nullNumFlag;
            break;
        case 5:
            estArrivalAirport = token;
            break;
        case 6:
            callsign = token;
            break;
        default:
            cout << "switch case overflow" << endl;
            break;
        }

        token = strtok(nullptr, delim);
    }
    if (pathType == "apt")
    {
        return new FlightInfo('a', icao24, firstSeen, estDepartureAirport, lastSeen, estArrivalAirport, callsign);
    } else
    {
        return new FlightInfo('d', icao24, firstSeen, estDepartureAirport, lastSeen, estArrivalAirport, callsign);
    }
}

void SupportFunctions::getAllPaths(vector<string> & paths)
{
    fs::path currentPath = fs::current_path();
    string path = currentPath;
    

    for (const auto& entry : fs::recursive_directory_iterator(path))
    {
        if (fs::is_directory(entry.path()))
        {   // Skip directories
            continue;
        }
        
        // Check if the file ends with ".apt" or ".dpt"
        if (entry.path().extension() == ".apt" || entry.path().extension() == ".dpt") 
                paths.push_back(entry.path().string());
    }
}


string SupportFunctions::getAirportNameFromPath(string& path) {
    string airportName;
    size_t lastSlashPos = path.find_last_of("/");
    airportName = path.substr(lastSlashPos + 1);
    size_t dotPos = airportName.find_last_of(".");
    airportName = airportName.substr(0, dotPos);
    return airportName;
}

string SupportFunctions::getPathType(string& path)
{
    string pathType;
    size_t dotPos = path.find_last_of(".");
    pathType = path.substr(dotPos + 1, 3);
    return pathType;
}



void AllAirports::getAllAirportsNames(vector<string>& airportsNamesVector) { 
    
    
    for (auto& airport: airportsVector)
       airportsNamesVector.push_back(airport->getIcaoCode());
}

// /home/...../EGGK/EKKL.apt