#include "utility.h"
/*Receive airport name and output(print) the full airport schedule containing 
both arrivals and departures ordered by time.*/

int main(int argc, char* argv[])
{
    System airports;
    airports.load_db();
    string airportName = argv[1];
    airports.printFullAirportSchedule(airportName);
    return 0;
}