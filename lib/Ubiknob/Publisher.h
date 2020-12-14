#pragma once
#include <usb_flightsim.h>

#include "Dual.h"
#include "Knob.h"
#include "Single.h"

namespace ubiknob {
    class UpDownCommand {
        public:
        UpDownCommand(
            const _XpRefStr_ *up,
            const _XpRefStr_ *down
        ) {
            up_command.assign(up);
            down_command.assign(down);
        }
        void run(KnobState state) {
            if (state == KnobState::cw) {
                up_command.once();
            } else if (state == KnobState::ccw) {
                down_command.once();
            }
        }
        private:
        FlightSimCommand up_command;
        FlightSimCommand down_command;
    };
    template<class T>
    class Publisher {
        public:
        void update(T mode, KnobState state) const;
    };

    template<>
    class Publisher<SingleKnobMode> {
        public:
        static void update(SingleKnobMode mode, KnobState state) {
            switch (mode) {
                // TODO
                case SingleKnobMode::mode_alt:
                alt.run(state);
                break;
                case SingleKnobMode::mode_crs:
                crs.run(state);
                break;
                case SingleKnobMode::mode_hdg:
                hdg.run(state);
                break;
                case SingleKnobMode::mode_vsp:
                vsp.run(state);
                break;
            }
        }
        private:
        static UpDownCommand alt;
        static UpDownCommand crs;
        static UpDownCommand hdg;
        static UpDownCommand vsp;
    };
    UpDownCommand Publisher<SingleKnobMode>::alt = UpDownCommand(
        XPlaneRef("sim/autopilot/altitude_up"),
        XPlaneRef("sim/autopilot/altitude_down")
    );
    UpDownCommand Publisher<SingleKnobMode>::crs = UpDownCommand(
        XPlaneRef("sim/GPS/g1000n1_crs_up"),
        XPlaneRef("sim/GPS/g1000n1_crs_down")
    );
    UpDownCommand Publisher<SingleKnobMode>::hdg = UpDownCommand(
        XPlaneRef("sim/autopilot/heading_up"),
        XPlaneRef("sim/autopilot/heading_down")
    );
    UpDownCommand Publisher<SingleKnobMode>::vsp = UpDownCommand(
        XPlaneRef("sim/autopilot/vertical_speed_up"),
        XPlaneRef("sim/autopilot/vertical_speed_down")
    );

    template<>
    class Publisher<DualKnobMode> {
        public:
        static void update(DualKnobMode mode, KnobState state, bool is_inner) {
            if (state == KnobState::unchanged) {
                return;
            }
            switch(mode) {
                // TODO
                case DualKnobMode::mode_com1:
                break;
                case DualKnobMode::mode_com2:
                break;
                case DualKnobMode::mode_nav1:
                break;
                case DualKnobMode::mode_nav2:
                break;
            }
        }
    };
}