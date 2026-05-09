#pragma once

#include "JoyStickGateway.h"
#include "JoyStickMapper.h"
#include <juce_audio_devices/juce_audio_devices.h>

class JoyStickMidiMediator : public JoyStickGateway::Listener
{
public:
    JoyStickMidiMediator (JoyStickMapper& m, JoyStickGateway& g) : mapper (m), gateway (g)
    {
        // Create a virtual MIDI out port that other apps can see
        // This works on macOS. On Windows, it returns nullptr (requires a driver like loopMIDI)
        virtualMidiOutput = juce::MidiOutput::createNewDevice ("DoJoyStick Port");
    }

    void addDefaultMappings()
    {
        if (! mapper.getMappings().empty())
            return;

        int numJoysticks = gateway.getNumJoysticks();
        if (numJoysticks == 0)
            return;

        // We add default mappings for the first joystick found
        int numButtons = gateway.getNumButtons (0);
        // Limit to 21 as in original code
        int buttonsToAdd = std::min (numButtons, 21);

        for (int i = 0; i < buttonsToAdd; ++i)
        {
            mapper.addMapping ({ i, 36 + i, "" });
        }
    }

    void sendMidiMessage(juce::MidiMessage msg)
    {
        // 1. Send to the Plugin/Standalone buffer (for the current host)
        if (sendToMidiPort)
            messageCollector.addMessageToQueue(msg);

        // 2. Send to the Virtual System Port (for MuseScore/Logic External)
        if (sendToSeparateDevice && virtualMidiOutput != nullptr)
            virtualMidiOutput->sendMessageNow(msg);
    }
    void buttonChanged (int /*jid*/, int buttonIndex, bool isDown) override
    {
        for (const auto& m : mapper.getMappings())
        {
            if (m.buttonIndex == buttonIndex && m.midiNote > 0)
            {
                auto msg = isDown ? juce::MidiMessage::noteOn (1, m.midiNote, (juce::uint8)127)
                                  : juce::MidiMessage::noteOff (1, m.midiNote, (juce::uint8)0);
                
                sendMidiMessage(msg);
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

    void setSendToMidiPort (bool shouldSend) { sendToMidiPort = shouldSend; }
    bool isSendingToMidiPort() const { return sendToMidiPort; }

    void setSendToSeparateDevice (bool shouldSend) { sendToSeparateDevice = shouldSend; }
    bool isSendingToSeparateDevice() const { return sendToSeparateDevice; }

private:
    JoyStickMapper& mapper;
    JoyStickGateway& gateway;
    juce::MidiMessageCollector messageCollector;
    std::unique_ptr<juce::MidiOutput> virtualMidiOutput;
    bool sendToMidiPort = true;
    bool sendToSeparateDevice = true;
};
