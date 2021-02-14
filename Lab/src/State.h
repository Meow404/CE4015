#ifndef STATE_H
#define STATE_H 

#include <stdlib.h>
#include <stdio.h>
#include <string>

using namespace std;

class State
{
    float eventTime;
    int availability[20], blockedCalls, droppedCalls;

public:
    State(float eventTime, int availability[20], int blockedCalls, int droppedCalls);
    void print();
    string returnCSVString();
};

#endif