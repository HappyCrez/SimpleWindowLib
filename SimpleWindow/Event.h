#pragma once
#include "pch.h"

namespace sw {

    struct simple_window_api Event {

        struct SizeEvent {
            unsigned int width;
            unsigned int height;
        };

        struct KeyEvent {
            WPARAM code;    
            bool alt;       // is Alt pressed
            bool control;   // is Controll pressed
            bool shift;     // is Shift pressed
            bool system;    // is System pressed
        };

        struct TextEvent {
            std::uint32_t unicodeChar;
        };

        struct mouseButtonEvent {
            WPARAM code;
            int x;
            int y;
        };

        struct mouseMovedEvent {
            int x;
            int y;
        };

        enum EventType
        {
            Closed,                 
            Resized,                
            LostFocus,              
            GainedFocus,            
            TextEntered,            
            KeyPressed,             
            KeyReleased,            
            MouseWheelScrolled,     
            MouseButtonPressed,     
            MouseButtonReleased,    
            MouseMoved,             
            MouseEntered,           
            MouseLeft,              

            Count // The total number of event types
        };

        // Fields
        EventType type;

        union {
            SizeEvent size;
            KeyEvent key;
            TextEvent text;
            mouseButtonEvent mouseBtn;
            mouseMovedEvent mouseMoved;
        };
    };
}