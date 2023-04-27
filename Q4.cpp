#include "utility.h"
//Program that updates the existing airports in the DB with recent data(rerun BASH script).

int main(int argc, char* argv[])
{
    System airports;
    vector<string> paths;
    paths.reserve(10);
    airports.getAllPaths(paths);
    airports.load_db(paths);
    airports.regenerate_db();
    return 0;
}