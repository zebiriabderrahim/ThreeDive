//
// Created by ABDERRAHIM ZEBIRI on 2024-07-14.
//

#ifndef THREEDIVE_EVENT_TYPE_H
#define THREEDIVE_EVENT_TYPE_H

#include <bitset>
#include <sstream>

#include "key_codes.h"


namespace s3Dive
{

    enum class EventType : uint16_t {
        None = 0,
        WindowResize, WindowClose,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased, KeyTyped,
        MouseMoved, MouseButtonPressed, MouseButtonReleased, MouseScrolled,
        Count
    };

    using EventCategory = std::bitset<5>;
    inline constexpr EventCategory EventCategoryApplication(1 << 0);
    inline constexpr EventCategory EventCategoryInput(1 << 1);
    inline constexpr EventCategory EventCategoryKeyboard(1 << 2);
    inline constexpr EventCategory EventCategoryMouse(1 << 3);
    inline constexpr EventCategory EventCategoryMouseButton(1 << 4);

// Mouse code enum
    enum class MouseCode : uint16_t {
        ButtonLeft = 0,
        ButtonRight = 1,
        ButtonMiddle = 2,
        ButtonLast = ButtonMiddle
    };
    // Event structures
    struct WindowResizeEvent {
        int width;
        int height;
        static EventType getEventType() { return EventType::WindowResize; }
        static EventCategory getCategoryFlags() { return EventCategoryApplication; }
        [[nodiscard]] std::string toString() const {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << width << ", " << height;
            return ss.str();
        }
    };

    struct WindowCloseEvent {
        static EventType getEventType() { return EventType::WindowClose; }
        static EventCategory getCategoryFlags() { return EventCategoryApplication; }
        static std::string toString() { return "WindowCloseEvent"; }
    };

    struct AppTickEvent {
        static EventType getEventType() { return EventType::AppTick; }
        static EventCategory getCategoryFlags() { return EventCategoryApplication; }
        static std::string toString() { return "AppTickEvent"; }
    };

    struct AppUpdateEvent {
        static EventType getEventType() { return EventType::AppUpdate; }
        static EventCategory getCategoryFlags() { return EventCategoryApplication; }
        static std::string toString() { return "AppUpdateEvent"; }
    };

    struct AppRenderEvent {
        static EventType getEventType() { return EventType::AppRender; }
        static EventCategory getCategoryFlags() { return EventCategoryApplication; }
        static std::string toString() { return "AppRenderEvent"; }
    };

    struct KeyPressedEvent {
        Key::KeyCode keyCode;
        bool isRepeat;
        static EventType getEventType() { return EventType::KeyPressed; }
        static EventCategory getCategoryFlags() { return EventCategoryKeyboard | EventCategoryInput; }
        [[nodiscard]] std::string toString() const {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << static_cast<int>(keyCode) << " (repeat = " << isRepeat << ")";
            return ss.str();
        }
    };

    struct KeyReleasedEvent {
        Key::KeyCode keyCode;
        static EventType getEventType() { return EventType::KeyReleased; }
        static EventCategory getCategoryFlags() { return EventCategoryKeyboard | EventCategoryInput; }
        [[nodiscard]] std::string toString() const {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << static_cast<int>(keyCode);
            return ss.str();
        }
    };

    struct KeyTypedEvent {
        Key::KeyCode keyCode;
        static EventType getEventType() { return EventType::KeyTyped; }
        static EventCategory getCategoryFlags() { return EventCategoryKeyboard | EventCategoryInput; }
        [[nodiscard]] std::string toString() const {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << static_cast<int>(keyCode);
            return ss.str();
        }
    };

    struct MouseMovedEvent {
        float x;
        float y;
        static EventType getEventType() { return EventType::MouseMoved; }
        static EventCategory getCategoryFlags() { return EventCategoryMouse | EventCategoryInput; }
        [[nodiscard]] std::string toString() const {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << x << ", " << y;
            return ss.str();
        }
    };

    struct MouseScrolledEvent {
        float xOffset;
        float yOffset;
        static EventType getEventType() { return EventType::MouseScrolled; }
        static EventCategory getCategoryFlags() { return EventCategoryMouse | EventCategoryInput; }
        [[nodiscard]] std::string toString() const {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << xOffset << ", " << yOffset;
            return ss.str();
        }
    };

    struct MouseButtonPressedEvent {
        MouseCode button;
        float x;
        float y;
        static EventType getEventType() { return EventType::MouseButtonPressed; }
        static EventCategory getCategoryFlags() { return EventCategoryMouse | EventCategoryInput; }
        [[nodiscard]] std::string toString() const {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << static_cast<int>(button);
            return ss.str();
        }
    };

    struct MouseButtonReleasedEvent {
        MouseCode button;
        float x;
        float y;
        static EventType getEventType() { return EventType::MouseButtonReleased; }
        static EventCategory getCategoryFlags() { return EventCategoryMouse | EventCategoryInput; }
        [[nodiscard]] std::string toString() const {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << static_cast<int>(button);
            return ss.str();
        }
    };
}


#endif //THREEDIVE_EVENT_TYPE_H
