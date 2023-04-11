#include "utility.h"


void updateAirportDataFlights(SingleAirport& currentAirport, string& path)
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
}


//problems with nulls - inserting last flight values
FlightInfo* getCurrentFlightInfo(char* currentLine, string& pathType) {
    const char delim[] = ", ";
    // all needed variables for FlightInfo
    string icao24, estDptIcao, estArvIcao, callsign;
    int dptTime, arvTime, estDptAirportHorizDistance, estDptAirportVertDistance, estArvAirportHorizDistance, estArvAirportVertDistance, dptAirportCandidatesCount, arvAirportCandidatesCount;

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
            {
                dptTime = stoi(token);
            }
            break;
        case 3:
            estDptIcao = token;
            break;
        case 4:
            if (strcmp(token, "null") != 0)
            {
                arvTime = stoi(token);
            }
            break;
        case 5:
            estArvIcao = token;
            break;
        case 6:
            callsign = token;
            break;
        case 7:
            if (strcmp(token, "null") != 0)
            {
                estDptAirportHorizDistance = stoi(token);
            }
            break;
        case 8:
            if (strcmp(token, "null") != 0)
            {
                estDptAirportVertDistance = stoi(token);
            }
            break;
        case 9:
            if (strcmp(token, "null") != 0)
            {
                estArvAirportHorizDistance = stoi(token);
            }
            break;
        case 10:
            if (strcmp(token, "null") != 0)
            {
                estArvAirportVertDistance = stoi(token);
            }
            break;
        case 11:
            if (strcmp(token, "null") != 0)
            {
                dptAirportCandidatesCount = stoi(token);
            }
            break;
        case 12:
            if (strcmp(token, "null") != 0)
            {
                arvAirportCandidatesCount = stoi(token);
            }
            break;
        default:
            cout << "switch case overflow" << endl;
            break;
        }

        token = strtok(nullptr, delim);
    }
    if (pathType == "apt") {
        return new FlightInfo('a', icao24, dptTime, estDptIcao, arvTime, estArvIcao, callsign, estDptAirportHorizDistance, estDptAirportVertDistance, estArvAirportHorizDistance, estArvAirportVertDistance, dptAirportCandidatesCount, arvAirportCandidatesCount);
    } else {
        return new FlightInfo('d', icao24, dptTime, estDptIcao, arvTime, estArvIcao, callsign, estDptAirportHorizDistance, estDptAirportVertDistance, estArvAirportHorizDistance, estArvAirportVertDistance, dptAirportCandidatesCount, arvAirportCandidatesCount);
    }
}

void getAllPaths(vector<string> & paths)
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
                // std::cout << "Found file: " << entry.path() << endl;
                paths.push_back(entry.path().string());
    }
}

void load_db() {
    AllAirports airports;
    vector<string> paths;
    
    getAllPaths(paths);
    //for each airport there are 2 paths (apt, dpt)
    int numberOfAirports = paths.size() / 2;
    airports.getAirportsVector().reserve(numberOfAirports);
//   port0          port1           port2         path3
//path0  path1   path2 path3    path4 path5   path6  path7



    for (int i = 0; i < numberOfAirports; i++)
    {
        SingleAirport* currentAirport = new SingleAirport(getAirportName(paths[2*i]));

        updateAirportDataFlights(*currentAirport, paths[2 * i]);
        updateAirportDataFlights(*currentAirport, paths[(2 * i) + 1]);

        airports.getAirportsVector().push_back(currentAirport);
 
    }
        cout << "gg";
}

string getAirportName(string& path) {
    string airportName;
    size_t lastSlashPos = path.find_last_of("/");
    airportName = path.substr(lastSlashPos + 1);
    size_t dotPos = airportName.find_last_of(".");
    airportName = airportName.substr(0, dotPos);
    return airportName;
}

string getPathType(string& path)
{
    string pathType;
    size_t dotPos = path.find_last_of(".");
    pathType = path.substr(dotPos + 1, 3);
    return pathType;
}


int main()
{
    load_db();
    return 0;
}


// int main()
// {
// FlightInfo f1('a', "40750c", 1680990127, "LPPT", 1680997781, "EGGW", "WUK28FL", 391, 23,1470,68,2,5);
// FlightInfo f2('d',"406752", 1680984976, "null", 1680997594, "EGGW", "EZY38UV", 0,0, 1535,61,0,5);

// Airport EGKK("EGKK");
// EGKK.addFlightToAirport(f1);
// EGKK.addFlightToAirport(f2);

// Airports airports; 
// airports.addAirport(EGKK);



// return 0;
// }


//



   