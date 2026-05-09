#include "JoyStickMapper.h"

JoyStickMapper::JoyStickMapper()
{
}

const std::vector<JoyStickMapping>& JoyStickMapper::getMappings() const
{
    return mappings;
}

void JoyStickMapper::addMapping (JoyStickMapping mapping)
{
    mappings.push_back (mapping);
    sendChangeMessage();
}

void JoyStickMapper::removeMapping (int index)
{
    if (index >= 0 && index < (int) mappings.size())
    {
        mappings.erase (mappings.begin() + index);
        sendChangeMessage();
    }
}

void JoyStickMapper::clearMappings()
{
    mappings.clear();
    sendChangeMessage();
}

juce::ValueTree JoyStickMapper::toValueTree() const
{
    juce::ValueTree v ("Mappings");
    for (const auto& m : mappings)
    {
        juce::ValueTree child ("Mapping");
        child.setProperty ("buttonIndex", m.buttonIndex, nullptr);
        child.setProperty ("midiNote", m.midiNote, nullptr);
        v.addChild (child, -1, nullptr);
    }
    return v;
}

void JoyStickMapper::fromValueTree (const juce::ValueTree& v)
{
    mappings.clear();
    for (int i = 0; i < v.getNumChildren(); ++i)
    {
        auto child = v.getChild (i);
        if (child.hasType ("Mapping"))
        {
            mappings.push_back ({ (int) child.getProperty ("buttonIndex"), (int) child.getProperty ("midiNote") });
        }
    }
    sendChangeMessage();
}
