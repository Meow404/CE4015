#include "CSV.h"

#include <fstream>
#include <sstream>

using namespace std;

list<Event *> readCSV(string filename)
{
    list<Event *> FEL;

    printf("Read from file %s\n", filename.c_str());
    std::ifstream myFile(filename);

    // Make sure the file is open
    if (!myFile.is_open())
        throw std::runtime_error("Could not open file");

    // Helper vars
    std::string line, colname;
    string string_arrivalNumber, string_eventTime, string_baseStation, string_callDuration, string_velocity;
    int val;

    // Read the column names
    if (myFile.good())
    {
        // Extract the first line in the file
        std::getline(myFile, line);

        // Create a stringstream from line
        std::stringstream ss(line);

        // Extract each column name
        printf("columns found : ");
        while (std::getline(ss, colname, ','))
        {
            printf("[ %s ]", colname.c_str());
        }
    }

    // Read data, line by line
    while (std::getline(myFile, line))
    {
        // Create a stringstream of the current line
        std::stringstream ss(line);

        std::getline(ss, string_arrivalNumber, ',');
        std::getline(ss, string_eventTime, ',');
        float eventTime = std::stof(string_eventTime);

        std::getline(ss, string_baseStation, ',');
        float baseStation = std::stoi(string_baseStation);

        std::getline(ss, string_callDuration, ',');
        float callDuration = std::stof(string_callDuration);

        std::getline(ss, string_velocity, ',');
        float velocity = std::stof(string_velocity);

        Event *event = new CallInitiation(eventTime, velocity, baseStation, callDuration);
        FEL.push_back(event);
    }

    // Close file
    myFile.close();
    printf("\n%d Arrival Events Loaded\n", FEL.size());

    return FEL;
}


void writeCSV(string filename, list<State> ledger)
{

    std::ofstream ledger_file;
    ledger_file.open(filename);

    string header_string = "Event Time,";
    for (int i = 0; i < 20; i++)
    {
        header_string = header_string + "Station " + to_string(i + 1) + ',';
    }
    header_string = header_string + "Blocked Calls" + ',' + "Dropped Calls" + '\n';
    ledger_file << header_string;

    std::list<State>::iterator it;
    for (it = ledger.begin(); it != ledger.end(); ++it)
    {
        ledger_file << it->returnCSVString();
    }

    ledger_file.close();
}