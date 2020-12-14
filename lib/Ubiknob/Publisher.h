#pragma once
#include <usb_flightsim.h>

#include "Dual.h"
#include "Knob.h"
#include "Single.h"

namespace ubiknob {
    class DiffCommand {
        public:
        DiffCommand(
            const _XpRefStr_ *reference
        ) {
            command = reference;
        }
        void run(KnobDiff diff) {
            if (diff != 0) {
                command = command + diff;
            }
        }
        private:
        FlightSimInteger command;
    };

    class ButtonCommand {
        public:
        ButtonCommand(
            const _XpRefStr_ *reference
        ) {
            command = reference;
        }
        void run(ButtonState state) {
            if (state == ButtonState::falling) {
                command = 0;
            } else if (state == ButtonState::rising) {
                command = 1;
            }
        }
        private:
        FlightSimCommand command;
    };

    template<class T>
    class Publisher {
        public:
        void update(T mode, KnobDiff diff) const;
    };

    template<>
    class Publisher<SingleKnobMode> {
        public:
        static void update(SingleKnobMode mode, KnobDiff diff) {
            switch(mode) {
                case SingleKnobMode::mode_alt:
                alt_diff.run(diff);
                break;
                case SingleKnobMode::mode_crs:
                crs_diff.run(diff);
                break;
                case SingleKnobMode::mode_hdg:
                hdg_diff.run(diff);
                break;
                case SingleKnobMode::mode_vsp:
                vsp_diff.run(diff);
                break;
            }
        }
        static void update(SingleKnobMode mode, ButtonState state) {
            switch(mode) {
                case SingleKnobMode::mode_alt:
                alt_sync.run(state);
                break;
                case SingleKnobMode::mode_crs:
                crs_sync.run(state);
                break;
                case SingleKnobMode::mode_hdg:
                hdg_sync.run(state);
                break;
                case SingleKnobMode::mode_vsp:
                vsp_sync.run(state);
                break;
            }
        }
        private:
        static DiffCommand alt_diff;
        static DiffCommand crs_diff;
        static DiffCommand hdg_diff;
        static DiffCommand vsp_diff;
        static ButtonCommand alt_sync;
        static ButtonCommand crs_sync;
        static ButtonCommand hdg_sync;
        static ButtonCommand vsp_sync;
    };
    DiffCommand Publisher<SingleKnobMode>::alt_diff = DiffCommand(
        XPlaneRef("sim/autopilot/altitude")
    );
    DiffCommand Publisher<SingleKnobMode>::crs_diff = DiffCommand(
        XPlaneRef("sim/cockpit/gps/course")
    );
    DiffCommand Publisher<SingleKnobMode>::hdg_diff = DiffCommand(
        XPlaneRef("sim/autopilot/heading")
    );
    DiffCommand Publisher<SingleKnobMode>::vsp_diff = DiffCommand(
        XPlaneRef("sim/autopilot/vertical_speed")
    );

    ButtonCommand Publisher<SingleKnobMode>::alt_sync = ButtonCommand(
        XPlaneRef("sim/autopilot/altitude_sync")
    );
    ButtonCommand Publisher<SingleKnobMode>::crs_sync = ButtonCommand(
        XPlaneRef("sim/GPS/g1000n1_crs_sync")
    );
    ButtonCommand Publisher<SingleKnobMode>::hdg_sync = ButtonCommand(
        XPlaneRef("sim/autopilot/heading_sync")
    );
    ButtonCommand Publisher<SingleKnobMode>::vsp_sync = ButtonCommand(
        XPlaneRef("sim/autopilot/vertical_speed_sync")
    );

    template<>
    class Publisher<DualKnobMode> {
        public:
        static void update(DualKnobMode mode, KnobDiff diff, bool is_inner) {
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
        static void update(DualKnobMode mode, ButtonState state) {
        }
    };
}