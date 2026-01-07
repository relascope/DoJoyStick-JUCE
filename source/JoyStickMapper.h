#pragma once

#include <vector>

struct JoyStickMapping
{
    int buttonIndex;
    int midiNote;
};

class JoyStickMapper
{
public:
    JoyStickMapper();
    const std::vector<JoyStickMapping>& getMappings() const;

private:
    std::vector<JoyStickMapping> mappings;
};
