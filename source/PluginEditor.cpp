#include "PluginEditor.h"

void PluginEditor::addMidiCondiguration()
{
    addAndMakeVisible (midiConfigGroup);
    addAndMakeVisible (sendToSeparateDevice);
    addAndMakeVisible (sendToMidiPort);

    auto& mediator = processorRef.getMidiMediator();

    sendToSeparateDevice.setToggleState (mediator.isSendingToSeparateDevice(), juce::dontSendNotification);
    sendToSeparateDevice.onClick = [this, &mediator] {
        mediator.setSendToSeparateDevice (sendToSeparateDevice.getToggleState());
    };

    sendToMidiPort.setToggleState (mediator.isSendingToMidiPort(), juce::dontSendNotification);
    sendToMidiPort.onClick = [this, &mediator] {
        mediator.setSendToMidiPort (sendToMidiPort.getToggleState());
    };
    
    #if JUCE_WINDOWS
    sendToSeparateDevice.setEnabled (false);
    sendToSeparateDevice.setTooltip ("This is at the moment not available on Windows");
    #endif    
}
PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);

    addAndMakeVisible (inspectButton);

    // this chunk of code instantiates and opens the melatonin inspector
    inspectButton.onClick = [&] {
        if (!inspector)
        {
            inspector = std::make_unique<melatonin::Inspector> (*this);
            inspector->onClose = [this]() { inspector.reset(); };
        }

        inspector->setVisible (true);
    };

    addMidiCondiguration();

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 350);
}

PluginEditor::~PluginEditor()
{
}

void PluginEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    auto area = getLocalBounds();
    g.setColour (juce::Colours::white);
    g.setFont (16.0f);
    auto helloWorld = juce::String ("Hello from ") + PRODUCT_NAME_WITHOUT_VERSION + " v" VERSION + " running in " + CMAKE_BUILD_TYPE;
    // Removed removeFromTop(150) here as it conflicts with resized() layout
    g.drawText (helloWorld, area.removeFromTop(50), juce::Justification::centred, false);
}

void PluginEditor::resized()
{
    // layout the positions of your child components here
    auto area = getLocalBounds();
    
    // Ensure all components have space and are visible
    auto topArea = area.removeFromTop (100);
    inspectButton.setBounds (topArea.withSizeKeepingCentre (100, 50));

    // The midi configuration group
    auto configArea = area.removeFromTop (150).reduced (20, 10);
    midiConfigGroup.setBounds (configArea);
    
    auto innerConfig = configArea.reduced (10, 20);
    sendToSeparateDevice.setBounds (innerConfig.removeFromTop (30));
    sendToMidiPort.setBounds (innerConfig.removeFromTop (30));
}
