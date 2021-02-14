#ifndef EVENT_H
#define EVENT_H 

#include <list>
#include <iterator>

using namespace std;

class Event
{
    float eventTime;

public:
    Event(float eventTime);
    void seteventTime(float eventTime);
    float geteventTime();

    void insertEvent(list<Event *> *FEL, Event *event);
    void printEvents(list<Event *> FEL, int max);
    virtual void handleEvent(list<Event *> *FEL, int channelAvailability[], int *blockedCalls, int *droppedCalls, bool print = false) = 0;
    virtual void print() = 0;
};

class CallTermination : public Event
{
    int station;

public:
    CallTermination(float time, int station);
    void print();
    void handleEvent(list<Event *> *FEL, int channelAvailability[], int *blockedCalls, int *droppedCalls, bool print);
};

class CallHandover : public Event
{
    float speed;
    int station;
    float duration;
    int direction;

public:
    CallHandover(float time, float speed, int station, float duration, int direction);
    void print();
    void handleEvent(list<Event *> *FEL, int channelAvailability[], int *blockedCalls, int *droppedCalls, bool print);
};

class CallInitiation : public Event
{
    float speed;
    int station;
    float position;
    float duration;
    int direction;

public:
    CallInitiation(float time, float speed, int station, float duration);
    void print();
    void handleEvent(list<Event *> *FEL, int channelAvailability[], int *blockedCalls, int *droppedCalls, bool print);
};

#endif