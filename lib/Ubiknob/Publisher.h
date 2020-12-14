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
            command = command + diff;
        }
        private:
        FlightSimInteger command;
    };
    template<class T>
    class Publisher {
        public:
        void update(T mode, KnobDiff state) const;
    };

    template<>
    class Publisher<SingleKnobMode> {
        public:
        static void update(SingleKnobMode mode, KnobDiff diff) {
            switch (mode) {
                // TODO
                case SingleKnobMode::mode_alt:
                alt.run(diff);
                break;
                case SingleKnobMode::mode_crs:
                crs.run(diff);
                break;
                case SingleKnobMode::mode_hdg:
                hdg.run(diff);
                break;
                case SingleKnobMode::mode_vsp:
                vsp.run(diff);
                break;
            }
        }
        private:
        static DiffCommand alt;
        static DiffCommand crs;
        static DiffCommand hdg;
        static DiffCommand vsp;
    };
    DiffCommand Publisher<SingleKnobMode>::alt = DiffCommand(
        XPlaneRef("sim/autopilot/altitude")
    );
    DiffCommand Publisher<SingleKnobMode>::crs = DiffCommand(
        XPlaneRef("sim/cockpit/gps/course")
    );
    DiffCommand Publisher<SingleKnobMode>::hdg = DiffCommand(
        XPlaneRef("sim/autopilot/heading")
    );
    DiffCommand Publisher<SingleKnobMode>::vsp = DiffCommand(
        XPlaneRef("sim/autopilot/vertical_speed")
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
    };
}