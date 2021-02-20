#pragma once
#include <usb_flightsim.h>

#include "Knob.h"

namespace ubiknob {
    class ButtonCommand {
        public:
        ButtonCommand(
            const _XpRefStr_ *reference
        ) {
            command = reference;
        }
        void run(ButtonState state) {
            if (state == ButtonState::falling) {
                command = 1;
            } else if (state == ButtonState::rising) {
                command = 0;
            }
        }
        void runOnce() {
            command.once();
        }
        private:
        FlightSimCommand command;
    };
    class UpDownButtonCommand {
        public:
        UpDownButtonCommand(
            const _XpRefStr_ *up,
            const _XpRefStr_ *down
        ):
        up(ButtonCommand(up)),
        down(ButtonCommand(down))
        {
        }
        void runUp() {
            up.runOnce();
        }
        void runDown() {
            down.runOnce();
        }
        private:
        ButtonCommand up;
        ButtonCommand down;
    };
};