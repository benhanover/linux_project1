#include "utility.h"
/*Recieve airport ICOA code names and output(print) a table of which airports have flights
to the given airports and the flight details.*/

int main(int argc, char* argv[])
{
    System airports;
    airports.load_db();
    string curAirport;
    
    for(int i = 1; i < argc; i++)
    {
        curAirport = argv[i]; 
        airports.printAirportArv(curAirport);
    }
    return 0;
}