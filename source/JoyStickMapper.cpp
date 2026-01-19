#include "JoyStickMapper.h"

JoyStickMapper::JoyStickMapper()
{
    // Map first 12 buttons to MIDI notes 60-71 (Middle C octave)
    for (int i = 0; i < 21; ++i)
    {
        mappings.push_back({ i, 60 + i });
    }
}

const std::vector<JoyStickMapping>& JoyStickMapper::getMappings() const
{
    return mappings;
}
