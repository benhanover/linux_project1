#include "utility.h"


int main(int argc, char* argv[])
{
    System airports;
    airports.load_db();
    string airportName = argv[1];
    airports.printFullAirportSchedule(airportName);
    return 0;
}