#include "JoyStickConfigWidget.h"

JoyStickConfigWidget::JoyStickConfigWidget (JoyStickMapper& m, JoyStickGateway& g, JoyStickMidiMediator& med)
    : mapper (m), gateway (g), mediator (med), mappingList (m)
{
    gateway.addListener (this);

    addAndMakeVisible (midiConfigGroup);
    addAndMakeVisible (sendToSeparateDevice);
    addAndMakeVisible (sendToMidiPort);

    addAndMakeVisible (viewport);
    viewport.setViewedComponent (&mappingList, false);
    addAndMakeVisible (addMappingButton);

    addMappingButton.onClick = [this] { startLearning(); };

    mappingList.onListChanged = [this] { resized(); };

    sendToSeparateDevice.setToggleState (mediator.isSendingToSeparateDevice(), juce::dontSendNotification);
    sendToSeparateDevice.onClick = [this] {
        mediator.setSendToSeparateDevice (sendToSeparateDevice.getToggleState());
    };

    sendToMidiPort.setToggleState (mediator.isSendingToMidiPort(), juce::dontSendNotification);
    sendToMidiPort.onClick = [this] {
        mediator.setSendToMidiPort (sendToMidiPort.getToggleState());
    };

    #if JUCE_WINDOWS
    sendToSeparateDevice.setEnabled (false);
    sendToSeparateDevice.setTooltip ("This is at the moment not available on Windows");
    #endif
}

JoyStickConfigWidget::~JoyStickConfigWidget()
{
    gateway.removeListener (this);
}

void JoyStickConfigWidget::resized()
{
    auto area = getLocalBounds();

    // The midi configuration group
    auto configArea = area.removeFromTop (100).reduced (20, 5);
    midiConfigGroup.setBounds (configArea);
    
    auto innerConfig = configArea.reduced (10, 15);
    sendToSeparateDevice.setBounds (innerConfig.removeFromTop (25));
    sendToMidiPort.setBounds (innerConfig.removeFromTop (25));

    addMappingButton.setBounds (area.removeFromBottom (40).reduced (20, 5));
    viewport.setBounds (area.reduced (20, 5));
    mappingList.setSize (viewport.getWidth() - viewport.getScrollBarThickness(), 
                         (int)mapper.getMappings().size() * 30);
}

void JoyStickConfigWidget::startLearning()
{
    isLearning = true;
    learnedButton = -1;
    addMappingButton.setButtonText ("Press Joystick Button...");
    addMappingButton.setEnabled (false);
}

void JoyStickConfigWidget::stopLearning()
{
    isLearning = false;
    addMappingButton.setButtonText ("Add Configuration");
    addMappingButton.setEnabled (true);
}

void JoyStickConfigWidget::buttonChanged (int /*joystickIndex*/, int buttonIndex, bool isDown)
{
    if (isLearning && isDown && learnedButton == -1)
    {
        learnedButton = buttonIndex;
        
        juce::MessageManager::callAsync ([this, buttonIndex] {
            addMappingButton.setButtonText ("Select Midi Note for Button " + juce::String(buttonIndex));
            
            auto* menu = new juce::PopupMenu();
            for (int i = 0; i < 128; ++i)
            {
                menu->addItem (i + 1, juce::String(i) + " (" + juce::MidiMessage::getMidiNoteName(i, true, true, 3) + ")");
            }
            
            menu->showMenuAsync (juce::PopupMenu::Options(), [this, buttonIndex](int result) {
                if (result > 0)
                {
                    mapper.addMapping ({ buttonIndex, result - 1 });
                }
                stopLearning();
            });
        });
    }
}
