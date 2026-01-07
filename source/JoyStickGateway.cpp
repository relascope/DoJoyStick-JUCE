#include "JoyStickGateway.h"

#include "SDL3/SDL_init.h"
#include "SDL3/SDL.h"

#include <iostream>

JoyStickGateway::JoyStickGateway()
{
    if (SDL_Init(SDL_INIT_JOYSTICK) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }
    sdlInitialized = true;

    if (!SDL_HasJoystick())
        throw "BLA";

    // int numJoysticks = SDL_NumJoysticks();
    // for (int i = 0; i < numJoysticks; ++i)
    // {
    //     SDL_Joystick* joystick = SDL_JoystickOpen(i);
    //     if (joystick)
    //     {
    //         openJoysticks.push_back(joystick);
    //     }
    // }
}

JoyStickGateway::~JoyStickGateway()
{
    closeJoysticks();
    if (sdlInitialized)
    {
        SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
    }
}

void JoyStickGateway::closeJoysticks()
{
    for (auto* joystick : openJoysticks)
    {
    //     if (SDL_JoystickGetAttached(joystick))
    //     {
    //         SDL_JoystickClose(joystick);
    //     }
    }
    openJoysticks.clear();
}

bool JoyStickGateway::isButtonDown(int joystickIndex, int buttonIndex) const
{
    if (joystickIndex < 0 || joystickIndex >= (int)openJoysticks.size())
        return false;

    // SDL_JoystickUpdate();
    // return SDL_JoystickGetButton(openJoysticks[joystickIndex], buttonIndex) != 0;

    return true;
}

int JoyStickGateway::getNumJoysticks() const
{
    return (int)openJoysticks.size();
}

std::string JoyStickGateway::getJoystickName(int index) const
{
    if (index < 0 || index >= (int)openJoysticks.size())
        return "";

    // return SDL_JoystickName(openJoysticks[index]);

    return "HELLO";
}
