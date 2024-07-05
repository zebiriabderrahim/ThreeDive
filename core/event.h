//
// Created by ABDERRAHIM ZEBIRI on 2024-07-04.
//

#ifndef THREEDIVE_EVENT_H
#define THREEDIVE_EVENT_H

#include <functional>
#include <string>
#include <string_view>
#include <type_traits>

namespace s3Dive {

        enum class EventType {
            None = 0,
            MouseMovedEvent, MouseScrolledEvent, MouseButtonPressedEvent, MouseButtonReleasedEvent,
            KeyPressedEvent, KeyReleasedEvent,
            WindowResizeEvent, WindowCloseEvent,
            AppTickEvent, AppUpdateEvent, AppRenderEvent,
        };

        enum class EventCategory {
            None            = 0,
            Application     = 1 << 0,
            Input           = 1 << 1,
            Keyboard        = 1 << 2,
            Mouse           = 1 << 3,
            MouseButton     = 1 << 4
        };

        // Helper function to combine event categories
        constexpr int CombineEventCategories(EventCategory category) {
            return static_cast<int>(category);
        }

        template<typename... Categories>
        constexpr int CombineEventCategories(EventCategory first, Categories... rest) {
            return static_cast<int>(first) | CombineEventCategories(rest...);
        }

        // Improved macros with better naming and type safety
#define EVENT_CLASS_TYPE(type) \
        static constexpr EventType getStaticType() { return EventType::type; } \
        virtual EventType getEventType() const override { return getStaticType(); } \
        virtual std::string_view getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(...) \
        virtual int getCategoryFlags() const override { return CombineEventCategories(__VA_ARGS__); }

        class Event {
        public:
            friend class EventDispatcher;

            virtual ~Event() = default;

            [[nodiscard]] virtual EventType getEventType() const = 0;
            [[nodiscard]] virtual std::string_view getName() const = 0;
            [[nodiscard]] virtual int getCategoryFlags() const = 0;
            [[nodiscard]] virtual std::string toString() const { return std::string(getName()); }

            [[nodiscard]] bool isInCategory(EventCategory category) const {
                return getCategoryFlags() & static_cast<int>(category);
            }
            [[nodiscard]] bool isHandled() const { return isHandled_; }
            void setIsHandled(bool isHandled) { isHandled_ = isHandled; }

        private:
            bool isHandled_ = false;
        };

        class EventDispatcher {
            template<typename T>
            using EventFn = std::function<bool(T&)>;

        public:
            explicit EventDispatcher(Event& event) : m_Event(event) {}

            template<typename T>
            bool dispatch(EventFn<T> func) {
                if (m_Event.getEventType() == T::getStaticType()) {
                    m_Event.setIsHandled(func(static_cast<T&>(m_Event)));
                    return true;
                }
                return false;
            }

        private:
            Event& m_Event;
        };

        inline std::ostream& operator<<(std::ostream& os, const Event& e) {
            return os << e.toString();
        }

} // s3Dive

#endif //THREEDIVE_EVENT_H
