#pragma once

#include <juce_data_structures/juce_data_structures.h>
#include <vector>

struct JoyStickMapping
{
    int buttonIndex;
    int midiNote;

    bool operator==(const JoyStickMapping& other) const
    {
        return buttonIndex == other.buttonIndex && midiNote == other.midiNote;
    }
};

class JoyStickMapper : public juce::ChangeBroadcaster
{
public:
    JoyStickMapper();
    const std::vector<JoyStickMapping>& getMappings() const;

    void addMapping (JoyStickMapping mapping);
    void removeMapping (int index);
    void clearMappings();

    juce::ValueTree toValueTree() const;
    void fromValueTree (const juce::ValueTree& v);

private:
    std::vector<JoyStickMapping> mappings;
};
