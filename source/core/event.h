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

#include "event_type.h"

namespace s3Dive {

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