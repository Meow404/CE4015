#ifndef CSV_H
#define CSV_H 

#include "Event.h"
#include "State.h"

using namespace std;

list<Event *> readCSV(string filename);
void writeCSV(string filename, list<State> ledger);

#endif