#include "State.h"

State::State(float eventTime, int availability[20], int blockedCalls, int droppedCalls)
{
    this->eventTime = eventTime;
    for (int i = 0; i < 20; i++)
        this->availability[i] = availability[i];
    this->blockedCalls = blockedCalls;
    this->droppedCalls = droppedCalls;
}

void State::print()
{

    printf("\n");
    for (int i = 0; i < 20; i++)
    {
        if (i % 5 == 0)
            printf("\n");
        printf("[ %2d %2d ]", i + 1, availability[i]);
    }
    printf("\nBlocked Calls : %d & Dropped Calls : %d", blockedCalls, droppedCalls);
}

string State::returnCSVString()
{
    string csv_string = to_string(eventTime) + ",";
    for (int i = 0; i < 20; i++)
    {
        csv_string = csv_string + to_string(availability[i]) + ',';
    }
    csv_string = csv_string + to_string(blockedCalls) + ',' + to_string(droppedCalls) + '\n';
    return csv_string;
}