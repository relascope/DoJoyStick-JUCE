#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "JoyStickMapper.h"
#include "ConfigElementWidget.h"

class MappingListConfig : public juce::Component, public juce::ChangeListener
{
public:
    MappingListConfig (JoyStickMapper& m) : mapper (m)
    {
        mapper.addChangeListener (this);
        refresh();
    }

    ~MappingListConfig() override
    {
        mapper.removeChangeListener (this);
    }

    void refresh()
    {
        elements.clear();
        const auto& mappings = mapper.getMappings();
        for (int i = 0; i < (int) mappings.size(); ++i)
        {
            auto* el = new ConfigElementWidget (mappings[(size_t)i], i);
            el->onRemove = [this](int idx) { mapper.removeMapping (idx); };
            elements.add (el);
            addAndMakeVisible (el);
        }
        resized();
    }

    void changeListenerCallback (juce::ChangeBroadcaster*) override
    {
        refresh();
        if (onListChanged)
            onListChanged();
    }

    std::function<void()> onListChanged;

    void resized() override
    {
        auto area = getLocalBounds();
        for (auto* el : elements)
        {
            el->setBounds (area.removeFromTop (30));
        }
    }

private:
    JoyStickMapper& mapper;
    juce::OwnedArray<ConfigElementWidget> elements;
};
