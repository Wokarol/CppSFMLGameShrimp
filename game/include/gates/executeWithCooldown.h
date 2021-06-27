#pragma once

#include <limits>

class AfterDelayGate
{
public:
    AfterDelayGate(bool executeAtFirstCall = true)
    {
        timeSinceLastExecution = executeAtFirstCall
            ? std::numeric_limits<float>::max()
            : 0;
    }

    bool executeAfter(float interval, float dt)
    {
        if (timeSinceLastExecution > interval)
        {
            timeSinceLastExecution = 0;
            return true;
        }

        timeSinceLastExecution += dt;
        return false;
    }

private:
    float timeSinceLastExecution;
};