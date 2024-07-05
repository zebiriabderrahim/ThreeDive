//
// Created by ABDERRAHIM ZEBIRI on 2024-07-04.
//

#ifndef THREEDIVE_KEY_EVENT_H
#define THREEDIVE_KEY_EVENT_H

#include <sstream>
#include "event.h"
#include "key_codes.h"

namespace s3Dive {

        class KeyEvent : public Event {

        public:
            [[nodiscard]] Key::KeyCode getKeyCode() const { return keyCode_; }

            EVENT_CLASS_CATEGORY(EventCategory::Input, EventCategory::Keyboard);

        protected:
            explicit KeyEvent(const Key::KeyCode keycode) : keyCode_(keycode) {}

            Key::KeyCode keyCode_;
        };


        class KeyPressedEvent : public KeyEvent {
        public:
            explicit KeyPressedEvent(const Key::KeyCode keycode, bool isRepeat = false) : KeyEvent(keycode),
                                                                                          isRepeat_(isRepeat) {}

            [[nodiscard]] bool IsRepeat() const { return isRepeat_; }

            EVENT_CLASS_TYPE(KeyPressedEvent);

            [[nodiscard]] std::string toString() const override {
                std::stringstream ss;
                ss << "KeyPressedEvent: " << static_cast<int>(keyCode_) << " (repeat = " << isRepeat_ << ")";
                return ss.str();
            }

        private:
            bool isRepeat_;
        };


        class KeyReleasedEvent : public KeyEvent {
        public:
            explicit KeyReleasedEvent(const Key::KeyCode keycode) : KeyEvent(keycode) {}

            EVENT_CLASS_TYPE(KeyReleasedEvent);

            [[nodiscard]] std::string toString() const override {
                std::stringstream ss;
                ss << "KeyReleasedEvent: " << static_cast<int>(keyCode_);
                return ss.str();
            }

        };

} // s3Dive

#endif //THREEDIVE_KEY_EVENT_H