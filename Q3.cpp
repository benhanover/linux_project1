#include "utility.h"
//Receive list of aircraft names(icao24) and output(print) all flights (departures and arrivals) that it did.

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