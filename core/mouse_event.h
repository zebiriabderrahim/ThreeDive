//
// Created by ABDERRAHIM ZEBIRI on 2024-07-04.
//

#ifndef THREEDIVE_MOUSE_EVENT_H
#define THREEDIVE_MOUSE_EVENT_H

#include <sstream>
#include <cstdint>
#include "event.h"

namespace s3Dive {

        enum class MouseCode : uint16_t {
            ButtonLeft = 0,
            ButtonRight = 1,
            ButtonMiddle = 2,
            ButtonLast = ButtonMiddle
        };

        constexpr int CombineCategories(EventCategory cat1, EventCategory cat2) {
            return static_cast<int>(cat1) | static_cast<int>(cat2);
        }

        class MouseMovedEvent : public Event {

        public:
            MouseMovedEvent(float x, float y) : mouseX_(x), mouseY_(y) {}

            EVENT_CLASS_TYPE(MouseMovedEvent);
            EVENT_CLASS_CATEGORY(EventCategory::Input, EventCategory::Mouse);

            [[nodiscard]] std::string toString() const override {
                std::stringstream ss;
                ss << "MouseMovedEvent: " << mouseX_ << ", " << mouseY_;
                return ss.str();
            }

            [[nodiscard]] float getX() const { return mouseX_; }
            [[nodiscard]] float getY() const { return mouseY_; }

        private:
            float mouseX_;
            float mouseY_;
        };

        class MouseScrolledEvent : public Event {

        public:
            MouseScrolledEvent(float xOffset, float yOffset) : xOffset_(xOffset), yOffset_(yOffset) {}

            EVENT_CLASS_TYPE(MouseScrolledEvent);
            EVENT_CLASS_CATEGORY(EventCategory::Input, EventCategory::Mouse);

            [[nodiscard]] float getXOffset() const { return xOffset_; }
            [[nodiscard]] float getYOffset() const { return yOffset_; }

            [[nodiscard]] std::string toString() const override {
                std::stringstream ss;
                ss << "MouseScrolledEvent: " << getXOffset() << ", " << getYOffset();
                return ss.str();
            }

        private:
            float xOffset_;
            float yOffset_;
        };

        class MouseButtonEvent : public Event {
        public:
            [[nodiscard]] MouseCode getMouseButton() const { return button; }
            EVENT_CLASS_CATEGORY(EventCategory::Input, EventCategory::Mouse);

        protected:
            explicit MouseButtonEvent(MouseCode button) : button(button) {}
            MouseCode button;
        };

        class MouseButtonPressedEvent : public MouseButtonEvent {
        public:
            explicit MouseButtonPressedEvent(MouseCode button) : MouseButtonEvent(button) {}

            EVENT_CLASS_TYPE(MouseButtonPressedEvent)

            [[nodiscard]] std::string toString() const override {
                std::stringstream ss;
                ss << "MouseButtonPressedEvent: " << static_cast<int>(button);
                return ss.str();
            }
        };

        class MouseButtonReleasedEvent : public MouseButtonEvent {
        public:
            explicit MouseButtonReleasedEvent(MouseCode button) : MouseButtonEvent(button) {}

            EVENT_CLASS_TYPE(MouseButtonReleasedEvent)

            [[nodiscard]] std::string toString() const override {
                std::stringstream ss;
                ss << "MouseButtonReleasedEvent: " << static_cast<int>(button);
                return ss.str();
            }
        };

} // s3Dive

#endif //THREEDIVE_MOUSE_EVENT_H
