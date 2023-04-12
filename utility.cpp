#include "utility.h"

#define nullNumFlag -1

int main()
{
    SystemManagerClass system;
    //AllAirports* airports;
    system.load_db();
    string testAirportName = "EGKK", testCallsignSHT9X = "SHT9X";
    SingleAirport EGKK = system.getFlightsByAirportName(testAirportName);
    vector<FlightInfo*> SHT9X = system.getFlightsByCallsign(testCallsignSHT9X);
    system.regenerate_db();
    return 0;
}


//Main Functions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SystemManagerClass::regenerate_db()
{
    AllAirports* airports = this->allAirportsPtr;
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
    this->load_db();
}

void SystemManagerClass::load_db() {
    AllAirports* airports = new AllAirports();
    vector<string> paths;

    vector<SingleAirport*>& airpoprtsVector = airports->getAirportsVector();

    getAllPaths(paths);
    //for each airport there are 2 paths (apt, dpt)
    int numberOfAirports = paths.size() / 2;

    for (int i = 0; i < numberOfAirports; i++)
    {
        //the order of the paths 0 - airport1, 1 - airport1, 2 - airport2, 3 - airport2...
        SingleAirport* currentAirport = new SingleAirport(getAirportName(paths[2*i]));

        //one of calls below is for .apt and the second is .dpt - unorginized order
        updateAirportDataFlights(*currentAirport, paths[2 * i]);
        updateAirportDataFlights(*currentAirport, paths[(2 * i) + 1]);

        airpoprtsVector.push_back(currentAirport);
 
    }
    this->loaded_DB = true;
    this->allAirportsPtr = airports;
}

SingleAirport& SystemManagerClass::getFlightsByAirportName(string& airportName) {

    if (this->loaded_DB == false)
        this->load_db();
    
    vector<SingleAirport*>& allAirports = this->allAirportsPtr->getAirportsVector();

        for (auto& airport : allAirports)
    {
        if (airport->getIcaoCode() == airportName)
            return *airport;
    }

    ///////////////********what if airportName doesn't exist? error message? ********
    
}

vector<FlightInfo*> SystemManagerClass::getFlightsByCallsign(string& callsign)
{    
    vector<FlightInfo*> flightsByCallsign;

    if (this->loaded_DB == false)
        this->load_db();

    vector<SingleAirport*>& allAirports = this->allAirportsPtr->getAirportsVector();


    for (auto &airport : allAirports)
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

void SystemManagerClass::updateAirportDataFlights(SingleAirport& currentAirport, string& path)
{
    string currentLine;
    string pathType = getPathType(path);
    ifstream currentFile(path);

    if (!currentFile.is_open()) {
        cout << "wrong path was given";
    }

    // skip the first line
    getline(currentFile, currentLine);
        

    while (currentFile) {
        getline(currentFile, currentLine);
        char* currentLineAsChar = (char*)currentLine.c_str();
        FlightInfo* currentFlightInfo = getCurrentFlightInfo(currentLineAsChar, pathType);
        currentAirport.addFlightToAirport(*currentFlightInfo);

    }

    ////////////////********we need to close the file?********
    ///////********what if path doesn't lead to a wanted file? error message? ********
}



///////********what if currentLine is empty? error message? ********

//problems with nulls - inserting last flight values
FlightInfo* SystemManagerClass::getCurrentFlightInfo(char* currentLine, string& pathType) {
    const char delim[] = ", ";
    // all needed variables for FlightInfo
    string icao24, estDepartureAirport, estArrivalAirport, callsign;
    int firstSeen, lastSeen, estDptAirportHorizDistance, estDptAirportVertDistance, estArvAirportHorizDistance,
    estArvAirportVertDistance, dptAirportCandidatesCount, arvAirportCandidatesCount;

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
        case 7:
            if (strcmp(token, "null") != 0)
                estDptAirportHorizDistance = stoi(token);
            else 
                estDptAirportHorizDistance = nullNumFlag;
            break;
        case 8:
            if (strcmp(token, "null") != 0)
                estDptAirportVertDistance = stoi(token);
            else 
                estDptAirportVertDistance = nullNumFlag;
            break;
        case 9:
            if (strcmp(token, "null") != 0)
                estArvAirportHorizDistance = stoi(token);
            else 
                estArvAirportHorizDistance = nullNumFlag;
            break;
        case 10:
            if (strcmp(token, "null") != 0)
                estArvAirportVertDistance = stoi(token);
            else
                estArvAirportVertDistance = nullNumFlag;
            break;
        case 11:
            if (strcmp(token, "null") != 0)
                dptAirportCandidatesCount = stoi(token);
            else 
                dptAirportCandidatesCount - nullNumFlag;
            break;
        case 12:
            if (strcmp(token, "null") != 0)
                arvAirportCandidatesCount = stoi(token);
            else
                arvAirportCandidatesCount = nullNumFlag;  
            break;
        default:
            cout << "switch case overflow" << endl;
            break;
        }

        token = strtok(nullptr, delim);
    }
    if (pathType == "apt") {
        return new FlightInfo('a', icao24, firstSeen, estDepartureAirport, lastSeen, estArrivalAirport, callsign,
                            estDptAirportHorizDistance, estDptAirportVertDistance, estArvAirportHorizDistance,
                            estArvAirportVertDistance, dptAirportCandidatesCount, arvAirportCandidatesCount);
    } else {
        return new FlightInfo('d', icao24, firstSeen, estDepartureAirport, lastSeen, estArrivalAirport, callsign,
                            estDptAirportHorizDistance, estDptAirportVertDistance, estArvAirportHorizDistance,
                            estArvAirportVertDistance, dptAirportCandidatesCount, arvAirportCandidatesCount);
    }
}

void SystemManagerClass::getAllPaths(vector<string> & paths)
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


string SystemManagerClass::getAirportName(string& path) {
    string airportName;
    size_t lastSlashPos = path.find_last_of("/");
    airportName = path.substr(lastSlashPos + 1);
    size_t dotPos = airportName.find_last_of(".");
    airportName = airportName.substr(0, dotPos);
    return airportName;
}

string SystemManagerClass::getPathType(string& path)
{
    string pathType;
    size_t dotPos = path.find_last_of(".");
    pathType = path.substr(dotPos + 1, 3);
    return pathType;
}



   
void SystemManagerClass::getAllAirportsNames(vector<string>& airportsNamesVector) { 
    
    vector<SingleAirport*>& allAirports = this->allAirportsPtr->getAirportsVector();
    
    for (auto& airport: allAirports)
       airportsNamesVector.push_back(airport->getIcaoCode());
}