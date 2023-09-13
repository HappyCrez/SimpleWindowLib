#pragma once
#include "pch.h"

namespace sw {

    extern "C++"
    struct Event {

        struct SizeEvent {
            unsigned int width;
            unsigned int height;
        };

        struct KeyEvent {
            WPARAM keyCode;    
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
    
    // Event queue
    static std::queue<Event> event_queue;

    extern "C++" simple_window_api
    bool pollEvent(Event & event);

    extern "C++" simple_window_api
    Event popEvent();

    extern "C++" simple_window_api
    void pushEvent(Event & event);
    
}