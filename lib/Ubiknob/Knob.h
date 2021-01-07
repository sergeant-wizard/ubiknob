#pragma once
#include <Bounce.h>
#include <Encoder.h>

namespace ubiknob {
    typedef int KnobDiff;
    class KnobReader {
        public:
        KnobReader(int pin1, int pin2):
            enc(pin1, pin2),
            prev(0),
            prev_tmp(0),
            consecutive(0)
        {
        }
        KnobDiff update() {
            const auto current = enc.read() / 2;
            if (current == prev_tmp) {
                ++consecutive;
            } else {
                consecutive = 0;
                prev_tmp = current;
            }
            if (consecutive == 10) {
                consecutive = 0;
                const auto diff = current - prev;
                prev = current;
                return diff / abs(diff);
            } else {
                return 0;
            }
        }
        private:
        Encoder enc;
        int prev;
        int prev_tmp;
        int consecutive;
    };
    enum ButtonState {
        unchanged,
        rising,
        falling,
    };
    static constexpr int interval = 5;
    class ButtonReader {
        public:
        ButtonReader(int pin): button(pin, interval) {
        }
        ButtonState update() {
            button.update();
            if (button.fallingEdge()) {
                return ButtonState::falling;
            } else if (button.risingEdge()) {
                return ButtonState::rising;
            } else {
                return ButtonState::unchanged;
            }
        }
        private:
        Bounce button;
    };

    enum KnobMode {
        mode_alt = 0,
        mode_obs = 1,
        mode_hdg = 2,
        mode_vsp = 3,
        mode_fms = 4,
        mode_com1 = 5,
        mode_com2 = 6,
        mode_nav1 = 7,
        mode_nav2 = 8,
    };
};
