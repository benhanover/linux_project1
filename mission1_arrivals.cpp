#include "utility.h"

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