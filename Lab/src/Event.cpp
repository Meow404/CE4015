#include "Event.h"

Event::Event(float eventTime)
{
    this->eventTime = eventTime;
}

void Event::seteventTime(float eventTime)
{
    this->eventTime = eventTime;
}

float Event::geteventTime()
{
    return eventTime;
}

void Event::insertEvent(list<Event *> *FEL, Event *event)
{
    if (!FEL->empty())
    {
        if (FEL->back()->geteventTime() < event->geteventTime())
        {
            FEL->push_back(event);
        }
        else
        {

            list<Event *>::iterator it;
            // int index = 0;
            for (it = FEL->begin(); it != FEL->end(); it++)
            {
                if ((*it)->geteventTime() > event->geteventTime())
                {
                    FEL->insert(it, event);
                    break;
                }
                // index++;
            }
            // advance(it, index);
        }
    }
    else
    {
        FEL->push_front(event);
    }
}

void Event::printEvents(list<Event *> FEL, int max = 5)
{
    list<Event *>::iterator it;
    int index = 0;
    printf("\nPrint Event ... ");
    for (it = FEL.begin(); it != FEL.end(); it++)
    {
        if (index > max)
            break;
        printf("%.3f |", (*it)->geteventTime());
        ++index;
    }
    printf(" .... %.3f |", FEL.back()->geteventTime());
}

CallTermination::CallTermination(float time, int station) : Event(time)
{
    this->station = station;
}

void CallTermination::print()
{
    printf("\n\nCALL TERMINATION EVENT\nTime : %.3f\nStation : %d", geteventTime(), station);
}

void CallTermination::handleEvent(list<Event *> *FEL, int channelAvailability[], int *blockedCalls, int *droppedCalls, bool print = false)
{
    // printf("\nHi from CallTermination handleEvent");
    if (station == 0)
        channelAvailability[0]++;
    else if (station == 21)
        channelAvailability[19]++;
    else
        channelAvailability[station - 1]++;

    if (print)
        if (station >= 1 && station <= 20)
            printf("\nCall Terminated in Station %d @ %.3f", station, geteventTime());
        else
            printf("\nCall Terminated outside range (%d) @ %.3f", station, geteventTime());

    // printf("\n Next Event @ %.3f", FEL->front()->geteventTime());
}

CallHandover::CallHandover(float time, float speed, int station, float duration, int direction) : Event(time)
{
    this->speed = speed;
    this->station = station;
    this->duration = duration;
    this->direction = direction;
}

void CallHandover::print()
{
    printf("\n\nCALL HANDOVER EVENT\nTime : %.3f\nSpeed : %.3f\nDuration : %.3f\nStation : %d\nDirection : %d\n", geteventTime(), speed, duration, station, direction);
}

void CallHandover::handleEvent(list<Event *> *FEL, int channelAvailability[], int *blockedCalls, int *droppedCalls, bool print = false)
{
    if (channelAvailability[station - 1] > 0)
    {
        int oldStation = direction ? station + 1 : station - 1;
        channelAvailability[oldStation - 1]++;
        channelAvailability[station - 1]--;
    }
    else
    {
        (*droppedCalls)++;
        if (print)
            printf("\nCall Dropped");

        return;
    }

    // Assuming position is calculated left to right (direction : 0)
    float callTime = 0.0;

    float remainingDistance = (duration * speed / 360) - 2.0;
    int newStation = direction ? station - 1 : station + 1;

    if (remainingDistance > 0)
    {
        callTime = 2 * 360 / speed;
        float handoverTime = geteventTime() + callTime;
        float remainingDuration = duration - callTime;
        if (newStation == 0 || newStation == 21)
        {
            if (print)
                printf("\nNew Termination Event @ %.3f station %d call time %.3f", handoverTime, newStation, callTime);

            CallTermination *event = new CallTermination(handoverTime, newStation);
            insertEvent(FEL, event);
        }
        else
        {
            if (print)
                printf("\nNew Handover Event @ %.3f station %d \nRemaining distance %.3f call time %.3f \nRemaining Duration %.3f", handoverTime, newStation, remainingDistance, callTime, remainingDuration);

            CallHandover *event = new CallHandover(handoverTime, speed, newStation, remainingDuration, direction);
            insertEvent(FEL, event);
        }
    }
    else
    {
        float terminationTime = geteventTime() + duration;
        if (print)
            printf("\nNew Termination Event @ %.3f station %d call time %.3f", terminationTime, station, duration);

        CallTermination *event = new CallTermination(terminationTime, station);
        insertEvent(FEL, event);
    }
}

CallInitiation::CallInitiation(float time, float speed, int station, float duration) : Event(time)
{
    this->speed = speed;
    this->station = station;
    this->duration = duration;
    position = (((float)rand()) / (float)RAND_MAX) * 2;
    direction = rand() % 2;
}

void CallInitiation::print()
{
    printf("\n\nCALL INITIATION EVENT\nTime : %.3f\nSpeed : %.3f\nDuration : %.3f\nStation : %d\nPosition : %.3f\nDirection : %d\n", geteventTime(), speed, duration, station, position, direction);
}

void CallInitiation::handleEvent(list<Event *> *FEL, int channelAvailability[], int *blockedCalls, int *droppedCalls, bool print = false)
{
    // printf("\nHi from CallInitiation handleEvent");
    // Assuming position is calculated left to right (direction : 0)
    if (channelAvailability[station - 1] > 1)
        channelAvailability[station - 1]--;
    else
    {
        (*blockedCalls)++;
        if (print)
            printf("\nCall Blocked");

        return;
    }

    float remainingDistance = 0.0;
    float callTime = 0.0;

    if (direction == 0)
    {
        remainingDistance = (duration * speed / 360) - 2.0 + position;
        callTime = (2.0 - position) * 360 / speed;
    }
    else
    {
        remainingDistance = (duration * speed / 360) - position;
        callTime = position * 360 / speed;
    }

    int newStation = direction ? station - 1 : station + 1;

    if (remainingDistance > 0)
    {
        float handoverTime = geteventTime() + callTime;
        float remainingDuration = duration - callTime;

        if (newStation == 0 || newStation == 21)
        {
            if (print)
                printf("\nNew Termination Event @ %.3f station %d call time %.3f", handoverTime, newStation, callTime);

            CallTermination *event = new CallTermination(handoverTime, newStation);
            insertEvent(FEL, event);
        }
        else
        {
            if (print)
                printf("\nNew Handover Event @ %.3f station %d \nRemaining distance %.3f call time %.3f \nRemaining Duration %.3f", handoverTime, newStation, remainingDistance, callTime, remainingDuration);

            CallHandover *event = new CallHandover(handoverTime, speed, newStation, remainingDuration, direction);
            insertEvent(FEL, event);
        }
    }
    else
    {
        float terminationTime = geteventTime() + callTime;

        if (print)
            printf("\nNew Termination Event @ %.3f station %d call time %.3f", terminationTime, newStation, callTime);

        CallTermination *event = new CallTermination(terminationTime, newStation);
        insertEvent(FEL, event);
    }
}