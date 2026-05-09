#include "PluginEditor.h"

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p), 
      configWidget (p.getMapper(), p.getGateway(), p.getMidiMediator())
{
    juce::ignoreUnused (processorRef);
    
    #ifdef DEBUG
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
    #endif

    addAndMakeVisible (configWidget);

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
    
    g.setColour (juce::Colours::white);
    g.setFont (16.0f);
 }

void PluginEditor::resized()
{
    // layout the positions of your child components here
    auto area = getLocalBounds();
    
     #ifdef DEBUG
    // Ensure all components have space and are visible
    auto topArea = area.removeFromTop (50);
    inspectButton.setBounds (topArea.withSizeKeepingCentre (100, 30));
    #endif

    configWidget.setBounds (area);
}
