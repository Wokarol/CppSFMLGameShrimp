#pragma once

#include <vector>
#include <iostream>
#include <cassert>

namespace wok
{
    template <class frame>
    class TwoStateFrameAnimator
    {
    public:
        TwoStateFrameAnimator(float& framerate, frame& setFrame, frame& unsetFrame, std::vector<frame>& settingFrameStrip, std::vector<frame>& resettingFrameStrip, bool startState = false) :
            setFrame(&setFrame), unsetFrame(&unsetFrame), settingFrameStrip(&settingFrameStrip), resettingFrameStrip(&resettingFrameStrip),
            framerate(&framerate),
            lastState(startState)
        { }

        frame update(float delta, bool state)
        {
            if (state == lastState && currentAnimation == nullptr)
                return getStaticStateFrame(state);

            lastState = state;

            if (currentAnimation == nullptr)
            {
                startAnimation(state);
            }
            assert(currentAnimation);

            float frameInterval = 1.f / *framerate;

            timeSinceLastFrameSwitch += delta;

            if (timeSinceLastFrameSwitch > frameInterval)
            {
                timeSinceLastFrameSwitch -= frameInterval;
                currentFrame++;

                if (currentFrame >= (*currentAnimation).size())
                {
                    currentAnimation = nullptr;
                    return getStaticStateFrame(state);
                }
            }

            return (*currentAnimation)[currentFrame];
        }

    private:
        frame getStaticStateFrame(bool state)
        {
            return *(state
                ? setFrame
                : unsetFrame);
        }

        void startAnimation(bool newState)
        {
            currentAnimation = newState
                ? settingFrameStrip
                : resettingFrameStrip;

            currentFrame = 0;
            timeSinceLastFrameSwitch = 0.f;
        }

    private:
        frame* setFrame;
        frame* unsetFrame;
        std::vector<frame>* settingFrameStrip;
        std::vector<frame>* resettingFrameStrip;

        float* framerate;

        int currentFrame = -1;
        float timeSinceLastFrameSwitch = -1;
        bool lastState;
        std::vector<frame>* currentAnimation = nullptr;
    };
}