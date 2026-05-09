#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "JoyStickMapper.h"

class ConfigElementWidget : public juce::Component
{
public:
    ConfigElementWidget (const JoyStickMapping& m, int idx) : mapping (m), index (idx)
    {
        buttonLabel.setText ("Btn " + juce::String (mapping.buttonIndex), juce::dontSendNotification);
        addAndMakeVisible (buttonLabel);

        noteLabel.setText ("Note " + juce::String (mapping.midiNote) + " (" + juce::MidiMessage::getMidiNoteName (mapping.midiNote, true, true, 3) + ")", juce::dontSendNotification);
        addAndMakeVisible (noteLabel);

        removeButton.setButtonText ("X");
        removeButton.setTooltip ("Remove this mapping");
        addAndMakeVisible (removeButton);
        
        removeButton.onClick = [this] { if (onRemove) onRemove (index); };
    }

    void resized() override
    {
        auto area = getLocalBounds();
        buttonLabel.setBounds (area.removeFromLeft (60));
        removeButton.setBounds (area.removeFromRight (30).reduced (2));
        noteLabel.setBounds (area.reduced (5, 0));
    }

    std::function<void(int)> onRemove;

private:
    JoyStickMapping mapping;
    int index;
    juce::Label buttonLabel;
    juce::Label noteLabel;
    juce::TextButton removeButton;
};
