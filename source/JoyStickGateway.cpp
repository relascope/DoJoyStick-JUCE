#include "JoyStickGateway.h"
#include "SDL3/SDL.h"

JoyStickGateway::JoyStickGateway()
{
    if (SDL_Init (SDL_INIT_JOYSTICK | SDL_INIT_EVENTS))
        sdlInitialized = true;

    startTimer (5); // Poll every 5ms on the Main/Message thread
}

JoyStickGateway::~JoyStickGateway()
{
    stopTimer();
    closeJoysticks();
    if (sdlInitialized)
        SDL_QuitSubSystem (SDL_INIT_JOYSTICK | SDL_INIT_EVENTS);
}

void JoyStickGateway::timerCallback()
{
    SDL_Event event;
    while (SDL_PollEvent (&event))
    {
        if (event.type == SDL_EVENT_JOYSTICK_ADDED)
        {
            if (auto* joystick = SDL_OpenJoystick (event.jdevice.which))
            {
                openJoysticks.push_back (joystick);
                listeners.call (&Listener::joystickAdded, (int) openJoysticks.size() - 1, SDL_GetJoystickName (joystick));
            }
        }
        else if (event.type == SDL_EVENT_JOYSTICK_REMOVED)
        {
            if (auto* removed = SDL_GetJoystickFromID (event.jdevice.which))
            {
                auto it = std::find (openJoysticks.begin(), openJoysticks.end(), removed);
                if (it != openJoysticks.end())
                {
                    int index = (int) std::distance (openJoysticks.begin(), it);
                    SDL_CloseJoystick (*it);
                    openJoysticks.erase (it);
                    listeners.call (&Listener::joystickRemoved, index);
                }
            }
        }
        else if (event.type == SDL_EVENT_JOYSTICK_BUTTON_DOWN || event.type == SDL_EVENT_JOYSTICK_BUTTON_UP)
        {
            bool isDown = (event.type == SDL_EVENT_JOYSTICK_BUTTON_DOWN);
            listeners.call (&Listener::buttonChanged, 0, (int) event.jbutton.button, isDown);
        }
    }
}

void JoyStickGateway::closeJoysticks()
{
    for (auto* joystick : openJoysticks)
        SDL_CloseJoystick(joystick);
    
    openJoysticks.clear();
}

int JoyStickGateway::getNumJoysticks() const
{
    return (int)openJoysticks.size();
}

std::string JoyStickGateway::getJoystickName(int index) const
{
    if (index >= 0 && index < (int)openJoysticks.size())
        return SDL_GetJoystickName(openJoysticks[index]);
    else
        return "Invalid joystick index";
}