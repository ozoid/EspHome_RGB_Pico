#pragma once
#include "esphome/core/component.h"
#include "esphome/components/number/number.h"

namespace esphome{
namespace rgb_led{
    class RGBLEDOutput : public number::Number, public Component {
        public:
            void setup() override;
            //light::LightTraits get_traits() override;
            void set_output(number::Number *output) { output_ = output; }
            void set_pattern(number::Number *pattern) { pattern_ = pattern; }
            void set_speed(number::Number *speed) { speed_ = speed; }
            void set_repeat(number::Number *repeat) { repeat_ = repeat; }
            void set_color(number::Number *color) { color_ = color; }
            void set_brightness(int bright) { bright_ = bright; }
            void pushCurrent();
            void dump_config() override;
            void loop() override;
 
        protected:
            number::Number *output_;
            number::Number *pattern_;
            number::Number *speed_;
            number::Number *repeat_;
            number::Number *color_;
            int bright_ = 30;
    };
}
}