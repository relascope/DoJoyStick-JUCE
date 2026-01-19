#pragma once

#include "JoyStickGateway.h"
#include "JoyStickMapper.h"
#include <juce_audio_devices/juce_audio_devices.h>

class JoyStickMidiMediator : public JoyStickGateway::Listener
{
public:
    JoyStickMidiMediator (JoyStickMapper& m) : mapper (m)
    {
        // Create a virtual MIDI out port that other apps can see
        // This works on macOS. On Windows, it returns nullptr (requires a driver like loopMIDI)
        virtualMidiOutput = juce::MidiOutput::createNewDevice ("DoJoyStick Port");
    }

    void buttonChanged (int /*jid*/, int buttonIndex, bool isDown) override
    {
        for (const auto& m : mapper.getMappings())
        {
            if (m.buttonIndex == buttonIndex)
            {
                auto msg = isDown ? juce::MidiMessage::noteOn (1, m.midiNote, (juce::uint8)127)
                                  : juce::MidiMessage::noteOff (1, m.midiNote, (juce::uint8)0);
                
                // 1. Send to the Plugin/Standalone buffer (for the current host)
                messageCollector.addMessageToQueue (msg);

                // 2. Send to the Virtual System Port (for MuseScore/Logic External)
                if (virtualMidiOutput != nullptr)
                    virtualMidiOutput->sendMessageNow (msg);
            }
        }
    }

    void joystickAdded (int, const std::string&) override {}
    void joystickRemoved (int) override {}

    // Called in prepareToPlay
    void prepare (double sampleRate)
    {
        messageCollector.reset (sampleRate);
    }

    // Called in processBlock
    void collectPendingMessages (juce::MidiBuffer& dest, int numSamples)
    {
        messageCollector.removeNextBlockOfMessages (dest, numSamples);
    }

private:
    JoyStickMapper& mapper;
    juce::MidiMessageCollector messageCollector;
    std::unique_ptr<juce::MidiOutput> virtualMidiOutput;
};
