#pragma once
#include "pch.h"

namespace sw {

    struct simple_window_api Event {

        struct SizeEvent
        {
            unsigned int width;
            unsigned int height;
        };

        struct KeyEvent
        {
            unsigned int code;    
            bool alt;       // is Alt pressed
            bool control;   // is Controll pressed
            bool shift;     // is Shift pressed
            bool system;    // is System pressed
        };

        struct TextEvent
        {
            std::uint32_t unicodeChar;
        };

        struct MouseButtonEvent
        {
            WPARAM code;
            int x;
            int y;
        };

        struct MouseMovedEvent
        {
            int x;
            int y;
        };

        struct MouseScrollEvent
        {
            int z;
        };

        struct ButtonEvent
        {
            HWND ID;
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
            MouseEntereOrLeft,
            ButtonClick,
            Paint,

            Count // The total number of event types
        };

        // Fields
        EventType type;

        union
        {
            MouseScrollEvent mouse_scroll;
            ButtonEvent button;
            SizeEvent size;
            KeyEvent key;
            TextEvent text;
            MouseButtonEvent mouse_click;
            MouseMovedEvent mouseMove;
        };
    };
}