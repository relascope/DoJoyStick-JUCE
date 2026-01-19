#pragma once

#include "JoyStickGateway.h"
#include "JoyStickMapper.h"
#include <juce_audio_devices/juce_audio_devices.h>

class JoyStickMidiMediator : public JoyStickGateway::Listener
{
public:
    JoyStickMidiMediator (JoyStickMapper& m) : mapper (m) {}

    void buttonChanged (int /*jid*/, int buttonIndex, bool isDown) override
    {
        for (const auto& m : mapper.getMappings())
        {
            if (m.buttonIndex == buttonIndex)
            {
                auto msg = isDown ? juce::MidiMessage::noteOn (1, m.midiNote, (juce::uint8)127)
                                  : juce::MidiMessage::noteOff (1, m.midiNote, (juce::uint8)0);
                
                // Real-time safe: pushes to an internal FIFO
                messageCollector.addMessageToQueue (msg);
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
};
