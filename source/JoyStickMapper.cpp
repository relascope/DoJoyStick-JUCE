#include "JoyStickMapper.h"

JoyStickMapper::JoyStickMapper()
{
    // Hardcoded mapping: button 3 handles MIDI Note 77
    mappings.push_back({3, 77});
}

const std::vector<JoyStickMapping>& JoyStickMapper::getMappings() const
{
    return mappings;
}
