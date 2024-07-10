#pragma once

#ifndef THREEDIVE_EVENT_H
#define THREEDIVE_EVENT_H

#include <array>
#include <functional>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>
#include <iostream>
#include <bitset>
#include <sstream>

#include "key_codes.h"


namespace s3Dive {

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
        [[nodiscard]] EventType getEventType() const { return EventType::WindowResize; }
        [[nodiscard]] EventCategory getCategoryFlags() const { return EventCategoryApplication; }
        [[nodiscard]] std::string toString() const {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << width << ", " << height;
            return ss.str();
        }
    };

    struct WindowCloseEvent {
       [[nodiscard]] EventType getEventType() const { return EventType::WindowClose; }
       [[nodiscard]] EventCategory getCategoryFlags() const { return EventCategoryApplication; }
       [[nodiscard]] std::string toString() const { return "WindowCloseEvent"; }
    };

    struct AppTickEvent {
        [[nodiscard]] EventType getEventType() const { return EventType::AppTick; }
        [[nodiscard]] EventCategory getCategoryFlags() const { return EventCategoryApplication; }
        [[nodiscard]] std::string toString() const { return "AppTickEvent"; }
    };

    struct AppUpdateEvent {
        [[nodiscard]] EventType getEventType() const { return EventType::AppUpdate; }
        [[nodiscard]] EventCategory getCategoryFlags() const { return EventCategoryApplication; }
        [[nodiscard]] std::string toString() const { return "AppUpdateEvent"; }
    };

    struct AppRenderEvent {
        [[nodiscard]] EventType getEventType() const { return EventType::AppRender; }
        [[nodiscard]] EventCategory getCategoryFlags() const { return EventCategoryApplication; }
        [[nodiscard]] std::string toString() const { return "AppRenderEvent"; }
    };

    struct KeyPressedEvent {
        Key::KeyCode keyCode;
        bool isRepeat;
        [[nodiscard]] EventType getEventType() const { return EventType::KeyPressed; }
        [[nodiscard]] EventCategory getCategoryFlags() const { return EventCategoryKeyboard | EventCategoryInput; }
        [[nodiscard]] std::string toString() const {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << static_cast<int>(keyCode) << " (repeat = " << isRepeat << ")";
            return ss.str();
        }
    };

    struct KeyReleasedEvent {
        Key::KeyCode keyCode;
        [[nodiscard]] EventType getEventType() const { return EventType::KeyReleased; }
        [[nodiscard]] EventCategory getCategoryFlags() const { return EventCategoryKeyboard | EventCategoryInput; }
        [[nodiscard]] std::string toString() const {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << static_cast<int>(keyCode);
            return ss.str();
        }
    };

    struct KeyTypedEvent {
        Key::KeyCode keyCode;
        [[nodiscard]] EventType getEventType() const { return EventType::KeyTyped; }
        [[nodiscard]] EventCategory getCategoryFlags() const { return EventCategoryKeyboard | EventCategoryInput; }
        [[nodiscard]] std::string toString() const {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << static_cast<int>(keyCode);
            return ss.str();
        }
    };

    struct MouseMovedEvent {
        float x;
        float y;
        [[nodiscard]] EventType getEventType() const { return EventType::MouseMoved; }
        [[nodiscard]] EventCategory getCategoryFlags() const { return EventCategoryMouse | EventCategoryInput; }
        [[nodiscard]] std::string toString() const {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << x << ", " << y;
            return ss.str();
        }
    };

    struct MouseScrolledEvent {
        float xOffset;
        float yOffset;
        [[nodiscard]] EventType getEventType() const { return EventType::MouseScrolled; }
        [[nodiscard]] EventCategory getCategoryFlags() const { return EventCategoryMouse | EventCategoryInput; }
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
        [[nodiscard]] EventType getEventType() const { return EventType::MouseButtonPressed; }
        [[nodiscard]] EventCategory getCategoryFlags() const { return EventCategoryMouse | EventCategoryInput; }
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
       [[nodiscard]] EventType getEventType() const { return EventType::MouseButtonReleased; }
       [[nodiscard]] EventCategory getCategoryFlags() const { return EventCategoryMouse | EventCategoryInput; }
       [[nodiscard]] std::string toString() const {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << static_cast<int>(button);
            return ss.str();
        }
    };

    using EventVariant = std::variant<
            WindowResizeEvent,
            WindowCloseEvent,
            AppTickEvent,
            AppUpdateEvent,
            AppRenderEvent,
            KeyPressedEvent,
            KeyReleasedEvent,
            KeyTypedEvent,
            MouseMovedEvent,
            MouseScrolledEvent,
            MouseButtonPressedEvent,
            MouseButtonReleasedEvent
    >;

    class EventDispatcher {
    public:
        using EventCallback = std::function<void(const EventVariant&)>;

        void dispatch(const EventVariant& event) {
            const auto visitor = [this](const auto& e) {
                auto type = e.getEventType();
                for (const auto& callback : listeners_[type]) {
                    callback(EventVariant(e));
                }
            };
            std::visit(visitor, event);
        }

        void addEventListener(EventType type, EventCallback callback) {
            listeners_[type].push_back(std::move(callback));
        }

        void removeEventListener(EventType type, const EventCallback& callback) {
            auto& listeners = listeners_[type];
            const auto isMatchingCallback = [&callback](const EventCallback& cb) {
                return cb.target_type() == callback.target_type();
            };
            const auto firstIt = std::remove_if(listeners.begin(), listeners.end(), isMatchingCallback);
            listeners.erase(firstIt, listeners.end());
        }

    private:
        std::unordered_map<EventType, std::vector<EventCallback>> listeners_;
    };

    class EventQueue {
    public:
        template<typename T>
        void enqueueEvent(T&& event) {
            events_.emplace_back(std::forward<T>(event));
        }

        void processEvents(EventDispatcher& dispatcher) {
            const auto processEvent = [&dispatcher](const EventVariant& event) {
                dispatcher.dispatch(event);
            };
            for (const auto& event : events_) {
                processEvent(event);
            }
            events_.clear();
        }

    private:
        std::vector<EventVariant> events_;
    };

// Helper function to get event name
    inline std::string_view getEventName(const EventVariant& event) {
        const auto visitor = [](const auto& e) -> std::string_view {
            return typeid(e).name();
        };
        return std::visit(visitor, event);
    }

// Helper function to check if an event is in a category
    inline bool isInCategory(const EventVariant& event, EventCategory category) {
        const auto visitor = [category](const auto& e) {
            return (e.getCategoryFlags() & category) == category;
        };
        return std::visit(visitor, event);
    }

// Overload operator<< for EventVariant
    inline std::ostream& operator<<(std::ostream& os, const EventVariant& e) {
        const auto visitor = [&os](const auto& event) {
            os << event.toString();
        };
        std::visit(visitor, e);
        return os;
    }

} // namespace s3Dive

#endif //THREEDIVE_EVENT_H