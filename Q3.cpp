#include "utility.h"
//Receive list of aircraft names(icao24) and output(print) all flights (departures and arrivals) that it did.

int main(int argc, char* argv[])
{
    System airports;
    airports.load_db();
    string curAircraft;
    for(int i = 1; i < argc; i++)
    {
        curAircraft = argv[i];
        airports.printAircraftFlights(curAircraft);
    }
    return 0;
}