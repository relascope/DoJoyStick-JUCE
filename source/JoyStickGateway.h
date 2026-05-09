#pragma once

#include "SDL3/SDL_joystick.h"
#include <string>
#include <vector>
#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>

class JoyStickGateway : private juce::Timer // Changed from juce::Thread
{
public:
    class Listener
    {
    public:
        virtual ~Listener() = default;
        virtual void buttonChanged (int joystickIndex, int buttonIndex, bool isDown) = 0;
        virtual void joystickAdded (int joystickIndex, const std::string& name) = 0;
        virtual void joystickRemoved (int joystickIndex) = 0;
    };

    JoyStickGateway();
    ~JoyStickGateway() override;

    void addListener (Listener* l)    { listeners.add (l); }
    void removeListener (Listener* l) { listeners.remove (l); }

    int getNumJoysticks() const;
    std::string getJoystickName (int index) const;
    int getNumButtons (int index) const;

private:
    void timerCallback() override; // Replaces run()
    void closeJoysticks();
    
    std::vector<SDL_Joystick*> openJoysticks;
    bool sdlInitialized = false;

    juce::ListenerList<Listener> listeners;
};
