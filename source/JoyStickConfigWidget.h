#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "JoyStickGateway.h"
#include "JoyStickMapper.h"
#include "JoyStickMidiMediator.h"
#include "MappingListConfig.h"

class JoyStickConfigWidget : public juce::Component, public JoyStickGateway::Listener
{
public:
    JoyStickConfigWidget (JoyStickMapper& mapper, JoyStickGateway& gateway, JoyStickMidiMediator& mediator);
    ~JoyStickConfigWidget() override;

    void resized() override;
    
    // JoyStickGateway::Listener
    void buttonChanged (int joystickIndex, int buttonIndex, bool isDown) override;
    void joystickAdded (int joystickIndex, const std::string& name) override { juce::ignoreUnused (joystickIndex, name); }
    void joystickRemoved (int joystickIndex) override { juce::ignoreUnused (joystickIndex); }

private:
    void startLearning();
    void stopLearning();

    JoyStickMapper& mapper;
    JoyStickGateway& gateway;
    JoyStickMidiMediator& mediator;

    juce::GroupComponent midiConfigGroup { "midiConfigGroup", "Midi Configuration" };
    juce::ToggleButton sendToSeparateDevice { "Send to Separate Device" };
    juce::ToggleButton sendToMidiPort { "Send to Midi Port" };

    juce::Viewport viewport;
    MappingListConfig mappingList;
    juce::TextButton addMappingButton { "Add Configuration" };

    bool isLearning = false;
    int learnedButton = -1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JoyStickConfigWidget)
};
