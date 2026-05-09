#pragma once

#include "JoyStickGateway.h"
#include "JoyStickMapper.h"
#include "KeyboardGateway.h"

class JoyStickKeyboardMediator : public JoyStickGateway::Listener
{
public:
    JoyStickKeyboardMediator (JoyStickMapper& m, KeyboardGateway& k) 
        : mapper (m), keyboard (k)
    {
    }

    void buttonChanged (int /*joystickIndex*/, int buttonIndex, bool isDown) override
    {
        if (isDown)
        {
            for (const auto& m : mapper.getMappings())
            {
                if (m.buttonIndex == buttonIndex && m.keyboardShortcut.isNotEmpty())
                {
                    keyboard.pressShortcut (m.keyboardShortcut);
                }
            }
        }
    }

    void joystickAdded (int, const std::string&) override {}
    void joystickRemoved (int) override {}

private:
    JoyStickMapper& mapper;
    KeyboardGateway& keyboard;
};
