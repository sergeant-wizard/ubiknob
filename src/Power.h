#pragma once
#include <usb_flightsim.h>
#include "Knob.h"


namespace ubiknob {
    static const _XpRefStr_* PROPELLER_UP[2] = {
         XPlaneRef("sim/engines/prop_up_1"),
         XPlaneRef("sim/engines/prop_up_2"),
    };
    static const _XpRefStr_* PROPELLER_DOWN[2] = {
         XPlaneRef("sim/engines/prop_down_1"),
         XPlaneRef("sim/engines/prop_down_2"),
    };
    static const _XpRefStr_* MIXTURE_UP[2] = {
         XPlaneRef("sim/engines/mixture_up_1"),
         XPlaneRef("sim/engines/mixture_up_2"),
    };
    static const _XpRefStr_* MIXTURE_DOWN[2] = {
         XPlaneRef("sim/engines/mixture_down_1"),
         XPlaneRef("sim/engines/mixture_down_2"),
    };
    class PowerManager {
        public:
        PowerManager(int engine_idx) {
            propeller_up = PROPELLER_UP[engine_idx];
            propeller_down = PROPELLER_DOWN[engine_idx];
            mixture_up = MIXTURE_UP[engine_idx];
            mixture_down = MIXTURE_DOWN[engine_idx];
        }
        void update(KnobDiff diff, bool is_inner) {
            for (int i = 0; i < 4; ++i) {
                if (is_inner) {
                    if (diff < 0) {
                        propeller_down.once();
                    } else if (diff > 0) {
                        propeller_up.once();
                    }
                } else {
                    if (diff < 0) {
                        mixture_down.once();
                    } else if (diff > 0) {
                        mixture_up.once();
                    }
                }
            }
        }
        private:
        FlightSimCommand mixture_up;
        FlightSimCommand mixture_down;
        FlightSimCommand propeller_up;
        FlightSimCommand propeller_down;
    };
}
