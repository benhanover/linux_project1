#include "utility.h"
//Program that updates the existing airports in the DB with recent data(rerun BASH script).

int main(int argc, char* argv[])
{
    System airports;
    airports.load_db();
    airports.regenerate_db();
    return 0;
}