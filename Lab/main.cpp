#include <time.h>
#include "src/Event.h"
#include "src/State.h"
#include "src/CSV.h"

using namespace std;


int main()
{
    srand(time(NULL));

    list<Event *> FEL = readCSV("input\\PCS_TEST_DETERMINSTIC.csv");
    list<State> ledger;
    int channelAvailability[20], blockedCalls = 0, droppedCalls = 0;
    for (int i = 0; i < 20; i++)
        channelAvailability[i] = 10;

    printf("\nBeginning simulation ... \n");
    float numOfCalls = (float)FEL.size();
    int index = 0;

    while (!FEL.empty())
    // for(int i = 0; i < 20; i++)
    {
        Event *event = FEL.front();
        event->print();
        FEL.pop_front();
        event->handleEvent(&FEL, channelAvailability, &blockedCalls, &droppedCalls);

        State currentState = State(event->geteventTime(), channelAvailability, blockedCalls, droppedCalls);
        currentState.print();
        ledger.push_back(currentState);

        delete event;
    }

    printf("\n\nBlocked Calls : %d & Dropped Calls : %d", blockedCalls, droppedCalls);
    writeCSV("output\\ledger.csv", ledger);
}