#pragma once
#include <usb_flightsim.h>

#include "Frequency.h"
#include "FrequencyManager.h"
#include "Knob.h"

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

    class ValueManager {
        public:
        ValueManager(const _XpRefStr_* diff, int interval):
        interval(interval)
        {
            sim_int = diff;
        }
        void update(KnobDiff diff) {
            sim_int = sim_int + diff * interval;
        }
        private:
        FlightSimInteger sim_int;
        const int interval;
    };

    class Publisher {
        public:
        Publisher():
        alt_diff(XPlaneRef("sim/cockpit/autopilot/altitude"), 100),
        crs_diff(XPlaneRef("sim/cockpit/gps/course"), 1),
        obs_diff(XPlaneRef("sim/cockpit/radios/nav1_obs_degm"), 1),
        hdg_diff(XPlaneRef("sim/cockpit/autopilot/heading"), 1),
        vsp_diff(XPlaneRef("sim/cockpit/autopilot/vertical_velocity"), 100),
        com1(FrequencyManager(
            XPlaneRef("sim/cockpit/radios/com1_freq_hz"),
            XPlaneRef("sim/cockpit/radios/com1_stdby_freq_hz"),
            true
        )),
        com2(FrequencyManager(
            XPlaneRef("sim/cockpit/radios/com2_freq_hz"),
            XPlaneRef("sim/cockpit/radios/com2_stdby_freq_hz"),
            true
        )),
        nav1(FrequencyManager(
            XPlaneRef("sim/cockpit/radios/nav1_freq_hz"),
            XPlaneRef("sim/cockpit/radios/nav1_stdby_freq_hz"),
            false
        )),
        nav2(FrequencyManager(
            XPlaneRef("sim/cockpit/radios/nav2_freq_hz"),
            XPlaneRef("sim/cockpit/radios/nav2_stdby_freq_hz"),
            false
        ))
        {}
        void update(KnobMode mode, KnobDiff diff, bool is_inner) {
            switch(mode) {
                case KnobMode::mode_alt:
                    alt_diff.update(diff);
                    break;
                case KnobMode::mode_crs:
                    crs_diff.update(diff);
                    break;
                case KnobMode::mode_hdg:
                    hdg_diff.update(diff);
                    break;
                case KnobMode::mode_vsp:
                    vsp_diff.update(diff);
                    break;
                case KnobMode::mode_com1:
                    com1.update(diff, is_inner);
                    break;
                case KnobMode::mode_com2:
                    com2.update(diff, is_inner);
                    break;
                case KnobMode::mode_nav1:
                    nav1.update(diff, is_inner);
                    break;
                case KnobMode::mode_nav2:
                    nav2.update(diff, is_inner);
                    break;
            }
        }
        void update(KnobMode mode, ButtonState state) {
            if (state != ButtonState::falling) {
                return;
            }
            switch(mode) {
                case KnobMode::mode_alt:
                    alt_sync.run(state);
                    break;
                case KnobMode::mode_crs:
                    crs_sync.run(state);
                    break;
                case KnobMode::mode_hdg:
                    hdg_sync.run(state);
                    break;
                case KnobMode::mode_vsp:
                    vsp_sync.run(state);
                    break;
                case KnobMode::mode_com1:
                    com1.swap();
                    break;
                case KnobMode::mode_com2:
                    com2.swap();
                    break;
                case KnobMode::mode_nav1:
                    nav1.swap();
                    break;
                case KnobMode::mode_nav2:
                    nav2.swap();
                    break;
            }
        }
        const FrequencyManager& getFrequencyManager(KnobMode mode) const {
            if (mode == KnobMode::mode_com1) {
                return com1;
            } else if (mode == KnobMode::mode_com2) {
                return com2;
            } else if (mode == KnobMode::mode_nav1) {
                return nav1;
            } else if (mode == KnobMode::mode_nav2) {
                return nav2;

            }
            __builtin_unreachable();
        }
        private:
        ValueManager alt_diff;
        ValueManager crs_diff;
        ValueManager obs_diff;
        ValueManager hdg_diff;
        ValueManager vsp_diff;
        static ButtonCommand alt_sync;
        static ButtonCommand crs_sync;
        static ButtonCommand hdg_sync;
        static ButtonCommand vsp_sync;
        FrequencyManager com1;
        FrequencyManager com2;
        FrequencyManager nav1;
        FrequencyManager nav2;
    };

    ButtonCommand Publisher::alt_sync = ButtonCommand(
        XPlaneRef("sim/autopilot/altitude_sync")
    );
    ButtonCommand Publisher::crs_sync = ButtonCommand(
        XPlaneRef("sim/GPS/g1000n1_crs_sync")
    );
    ButtonCommand Publisher::hdg_sync = ButtonCommand(
        XPlaneRef("sim/autopilot/heading_sync")
    );
    ButtonCommand Publisher::vsp_sync = ButtonCommand(
        XPlaneRef("sim/autopilot/vertical_speed_sync")
    );
}