#include "utility.h"
/*Recieve airport ICOA code names and output(print) a table of which airports have flights
to the given airports and the flight details.*/

int main(int argc, char* argv[])
{
    System airports;
    airports.load_db();
    string curAircraftName;
    
    for(int i = 1; i < argc; i++)
    {
        curAircraftName = argv[i]; 
        airports.printAirportArv(curAircraftName);
    }
    return 0;
}