#pragma once
#include <Bounce.h>
#include <Encoder.h>

namespace ubiknob {
    typedef int KnobDiff;
    class KnobReader {
        public:
        KnobReader(int pin1, int pin2): enc(pin1, pin2), prev(0) {
        }
        KnobDiff update() {
            const auto current = enc.read();
            const auto diff = current - prev;
            prev = current;
            return diff / abs(diff);
        }
        private:
        Encoder enc;
        int prev;
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
        mode_com1 = 4,
        mode_com2 = 5,
        mode_nav1 = 6,
        mode_nav2 = 7,
    };
    static constexpr int NUB_KNOB_MODES = 8;
};
