#include "utility.h"

#define nullNumFlag -1

//----------------------------------------Main Functions-----------------------------------------

void System::deleteAll()
{
    for (auto& airport : airportsVector)
    {
        delete airport;

    }
    airportsVector.clear();
}

int System::getAirportIndexByName(string& airportName)
{
    while(true)
    {
        int index = 0;
        for (auto& airport : airportsVector)
        {
            if (airport->getIcaoCode() == airportName)
                return index;
            else
                index++;
        }
        cout << "Airport name doesn't exist in the database." << endl;
        cout << "Please enter new name." << endl;
        cin >> airportName;
    }
}

void System::regenerate_db()
{
    vector<string> airportsNamesVector;
    getAllAirportsNames(airportsNamesVector);

    fs::path currentPath = fs::current_path();
    string projectPath = currentPath;
    string airportNames = "";
    for (auto& name:airportsNamesVector) 
        airportNames += name += " ";

    //delete previous DB
    system(((projectPath + "/clean.sh ") += airportNames).c_str());
    deleteAll();
    //create data base
    system(((projectPath + "/flightScanner.sh ") += airportNames).c_str());
    vector<string> paths;
    paths.reserve(10);
    getAllPaths(paths);
    load_db(paths);
}

void System::load_db(vector<string>& paths)
{
    //for each airport there are 2 paths (apt, dpt)
    int numberOfAirports = paths.size() / 2;

    for (int i = 0; i < numberOfAirports; i++)
    {
        SingleAirport* currentAirport = new SingleAirport(getAirportNameFromPath(paths[2*i]));

        //one of calls below is for .apt and the second is .dpt - unorginized order
        currentAirport->updateAirportDataFlights(paths[2 * i]);
        currentAirport->updateAirportDataFlights(paths[(2 * i) + 1]);

        airportsVector.push_back(currentAirport);
    }
}

vector<FlightInfo*> System::getFlightsByCallsign(string& callsign)
{    
    vector<FlightInfo*> flightsByCallsign;
    while(flightsByCallsign.size() == 0)
    {
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
        
        if(flightsByCallsign.size() == 0)
        {
            cout << "Callsign not found!" << endl;
            cout << "Please enter new callsign.";
            cin >> callsign;
        }
    }
    return flightsByCallsign;

   ///////////////********what if callsign doesn't exist? error message? ********

}

//--------------------------------------Helpers functions-----------------------------------------
void SingleAirport::updateAirportDataFlights(string& path)
{
    string currentLine;
    //Apt / Dpt
    string pathType = getPathType(path);
    ifstream currentFile(path);

    if (!currentFile.is_open())
    {
        cout << "wrong path was given";
    }

    // Skip the first line
    getline(currentFile, currentLine); 

    while (currentFile)
    {
        getline(currentFile, currentLine);
        char* currentLineAsChar = (char*)currentLine.c_str();
        FlightInfo* currentFlightInfo = currentFlightInfo->getCurrentFlightInfo(currentLineAsChar, pathType);
        currentFlightInfo->addFlightToAirport(*this);
    }
    currentFile.close();
}

 FlightInfo* FlightInfo::getCurrentFlightInfo(char* currentLine, string& pathType) 
 {
    const char delim[] = ", ";
    string icao24, estDepartureAirport, estArrivalAirport, callsign;
    int firstSeen, lastSeen;

    char* token = strtok(currentLine, delim);
    int countTokens = 0;

    //Taking only the six arguments from the file.
    while (token && countTokens < 6)
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
        return  new FlightInfo('a', icao24, firstSeen, estDepartureAirport, lastSeen, estArrivalAirport, callsign);
    } else
    {
        return new FlightInfo('d', icao24, firstSeen, estDepartureAirport, lastSeen, estArrivalAirport, callsign);
    }
}

void System::getAllPaths(vector<string>& paths)
{
    fs::path currentPath = fs::current_path();
    string path = currentPath;
    
    for (const auto& entry : fs::recursive_directory_iterator(path))
    {
        if (fs::is_directory(entry.path()))
        {   // Skip directories
            continue;
        }
        
        if (entry.path().extension() == ".apt" || entry.path().extension() == ".dpt") 
                paths.push_back(entry.path().string());
    }
}

string System::getAirportNameFromPath(string& path)
{
    string airportName;
    size_t lastSlashPos = path.find_last_of("/");
    airportName = path.substr(lastSlashPos + 1);
    size_t dotPos = airportName.find_last_of(".");
    airportName = airportName.substr(0, dotPos);
    return airportName;
}

string System::getPathType(string& path)
{
    string pathType;
    size_t dotPos = path.find_last_of(".");
    pathType = path.substr(dotPos + 1, 3);
    return pathType;
}

void System::getAllAirportsNames(vector<string>& airportsNamesVector) { 
    
    
    for (auto& airport: airportsVector)
       airportsNamesVector.push_back(airport->getIcaoCode());
}


bool System::checkIfAllInDB(vector<string>& paths, vector<string>& missing_names, int numOfCodesRecieved, char** codesRecievedArr)
{
    int pathsIndex,namesIndex,numberOfAirports;
    vector<string> airportsNamesVector;
    
    numberOfAirports = paths.size() / 2;
    pathsIndex = 0;

    for (int namesIndex = 0; namesIndex < numberOfAirports ; namesIndex++)
    {
        string curName = getAirportNameFromPath(paths[pathsIndex]);
        airportsNamesVector.push_back(curName); 
        pathsIndex += 2; //different name every 2 paths (1 for arv file and 1 for dpt file - always adjacent)

    }

    // Iterate over the array of arguments recieved.
    for (int i = 1; i < numOfCodesRecieved; i++)
    {
        string codeString = codesRecievedArr[i];
        // Check if the current code (argument) is in the vector.
        if (find(airportsNamesVector.begin(), airportsNamesVector.end(), codeString) == airportsNamesVector.end())
            missing_names.push_back(codeString);
            // The current name is not in the vector.

    }

    if (missing_names.empty())
        return true; //no missing names, all arguments in DB
    else 
        return false;

}

bool System::isAircraftInDB(string code)
{
    for (auto airport : airportsVector)
    {
        vector<FlightInfo*> arrivals = airport->getArivals();
        vector<FlightInfo*> departures = airport->getDepartures();
        for (auto flightInfo : arrivals)
            if (flightInfo->getIcao24() == code)   
                return true;
        for (auto flightInfo : departures)
            if (flightInfo->getIcao24() == code)   
                return true;
    }

    return false;
}