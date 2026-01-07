#pragma once

#include "SDL3/SDL_joystick.h"

#include <string>
#include <vector>

class JoyStickGateway
{
public:
    JoyStickGateway();
    ~JoyStickGateway();

    bool isButtonDown(int joystickIndex, int buttonIndex) const;
    int getNumJoysticks() const;
    std::string getJoystickName(int index) const;

private:
    void closeJoysticks();
    std::vector<SDL_Joystick*> openJoysticks;
    bool sdlInitialized = false;
};
