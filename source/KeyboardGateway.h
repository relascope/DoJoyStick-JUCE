#pragma once

#include "keyboard-auto-type.h"
#include <juce_core/juce_core.h>

class KeyboardGateway
{
public:
    KeyboardGateway() = default;
    
    void typeText (const juce::String& text)
    {
        std::u32string u32str;
        auto utf32 = text.toUTF32();
        for (auto* p = utf32.getAddress(); *p != 0; ++p)
            u32str += (char32_t)*p;

        autoType.text (u32str);
    }
    
    void pressShortcut (const juce::String& shortcut)
    {
        auto tokens = juce::StringArray::fromTokens (shortcut.toLowerCase(), "+", "");
        
        keyboard_auto_type::Modifier modifiers = keyboard_auto_type::Modifier::None;
        keyboard_auto_type::KeyCode keyCode = keyboard_auto_type::KeyCode::Undefined;
        
        for (const auto& token : tokens)
        {
            auto t = token.trim();
            if (t == "cmd" || t == "command" || t == "win" || t == "meta")
                modifiers = modifiers | keyboard_auto_type::Modifier::Meta;
            else if (t == "ctrl" || t == "control")
                modifiers = modifiers | keyboard_auto_type::Modifier::Ctrl;
            else if (t == "alt" || t == "option")
                modifiers = modifiers | keyboard_auto_type::Modifier::Alt;
            else if (t == "shift")
                modifiers = modifiers | keyboard_auto_type::Modifier::Shift;
            else
            {
                // It's a key
                if (t == "space") keyCode = keyboard_auto_type::KeyCode::Space;
                else if (t == "pos1" || t == "home") keyCode = keyboard_auto_type::KeyCode::Home;
                else if (t == "end") keyCode = keyboard_auto_type::KeyCode::End;
                else if (t == "enter" || t == "return") keyCode = keyboard_auto_type::KeyCode::Return;
                else if (t == "esc" || t == "escape") keyCode = keyboard_auto_type::KeyCode::Escape;
                else if (t == "tab") keyCode = keyboard_auto_type::KeyCode::Tab;
                else if (t == "backspace") keyCode = keyboard_auto_type::KeyCode::Backspace;
                else if (t == "delete") keyCode = keyboard_auto_type::KeyCode::ForwardDelete;
                else if (t == "up") keyCode = keyboard_auto_type::KeyCode::UpArrow;
                else if (t == "down") keyCode = keyboard_auto_type::KeyCode::DownArrow;
                else if (t == "left") keyCode = keyboard_auto_type::KeyCode::LeftArrow;
                else if (t == "right") keyCode = keyboard_auto_type::KeyCode::RightArrow;
                else if (t == "pgup" || t == "pageup") keyCode = keyboard_auto_type::KeyCode::PageUp;
                else if (t == "pgdn" || t == "pagedown") keyCode = keyboard_auto_type::KeyCode::PageDown;
                else if (t == "ins" || t == "insert") keyCode = keyboard_auto_type::KeyCode::Insert;
                else if (t == "home") keyCode = keyboard_auto_type::KeyCode::Home;
                else if (t.length() == 1)
                {
                    char32_t c = (char32_t)t[0];
                    if (c >= 'a' && c <= 'z') keyCode = (keyboard_auto_type::KeyCode)((int)keyboard_auto_type::KeyCode::A + (c - 'a'));
                    else if (c >= '0' && c <= '9') keyCode = (keyboard_auto_type::KeyCode)((int)keyboard_auto_type::KeyCode::D0 + (c - '0'));
                }
                else if (t.startsWith("f") && t.length() > 1)
                {
                    int fNum = t.substring(1).getIntValue();
                    if (fNum >= 1 && fNum <= 24)
                        keyCode = (keyboard_auto_type::KeyCode)((int)keyboard_auto_type::KeyCode::F1 + (fNum - 1));
                }
            }
        }
        
        if (keyCode != keyboard_auto_type::KeyCode::Undefined)
        {
            autoType.key_press (keyCode, modifiers);
        }
        else if (modifiers != keyboard_auto_type::Modifier::None)
        {
             // Only modifiers? probably not what user wants but let's see.
             // keyboard-auto-type key_press requires a KeyCode.
        }
        else
        {
            typeText (shortcut);
        }
    }

private:
    keyboard_auto_type::AutoType autoType;
};
