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
                command = 1;
            } else if (state == ButtonState::rising) {
                command = 0;
            }
        }
        private:
        FlightSimCommand command;
    };

    class ValueManager {
        public:
        ValueManager(const _XpRefStr_* diff, int interval_small, int interval_large, bool is_cyclic):
        interval_small(interval_small),
        interval_large(interval_large),
        is_cyclic(is_cyclic)
        {
            sim_int = diff;
        }
        void update(KnobDiff diff, bool is_small) {
            if (is_small) {
                sim_int = getUpdatedValue(diff, interval_small);
            } else {
                sim_int = getUpdatedValue(diff, interval_large);
            }
        }
        private:
        long getUpdatedValue(KnobDiff diff, int interval) const {
            long ret = sim_int + diff * interval;
            ret = ret - ret % interval_small;
            if (is_cyclic) {
                return (ret + 360) % 360;
            } else {
                return ret;
            }
        }
        FlightSimInteger sim_int;
        const int interval_small;
        const int interval_large;
        const bool is_cyclic;
    };

    class Publisher {
        public:
        Publisher():
        alt_diff(XPlaneRef("sim/cockpit/autopilot/altitude"), 100, 1000, false),
        obs_diff(XPlaneRef("sim/cockpit/radios/nav1_obs_degm"), 1, 10, true),
        hdg_diff(XPlaneRef("sim/cockpit/autopilot/heading"), 1, 10, true),
        vsp_diff(XPlaneRef("sim/cockpit/autopilot/vertical_velocity"), 100, 1000, false),
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
                    alt_diff.update(diff, is_inner);
                    break;
                case KnobMode::mode_obs:
                    obs_diff.update(diff, is_inner);
                    break;
                case KnobMode::mode_hdg:
                    hdg_diff.update(diff, is_inner);
                    break;
                case KnobMode::mode_vsp:
                    vsp_diff.update(diff, is_inner);
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
            if (state == ButtonState::falling) {
                if (mode == KnobMode::mode_com1) {
                    com1.swap();
                } else if (mode == KnobMode::mode_com2) {
                    com2.swap();
                } else if (mode == KnobMode::mode_nav1) {
                    nav1.swap();
                } else if (mode == KnobMode::mode_nav2) {
                    nav2.swap();
                }
            }
            
            if (mode == KnobMode::mode_alt) {
                alt_sync.run(state);
            } else if (mode == KnobMode::mode_obs) {
                obs_sync.run(state);
            } else if (mode == KnobMode::mode_hdg) {
                hdg_sync.run(state);
            } else if (mode == KnobMode::mode_vsp) {
                vsp_sync.run(state);
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
        ValueManager obs_diff;
        ValueManager hdg_diff;
        ValueManager vsp_diff;
        static ButtonCommand alt_sync;
        static ButtonCommand obs_sync;
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
    ButtonCommand Publisher::obs_sync = ButtonCommand(
        XPlaneRef("sim/GPS/g1000n1_crs_sync")
    );
    ButtonCommand Publisher::hdg_sync = ButtonCommand(
        XPlaneRef("sim/autopilot/heading_sync")
    );
    ButtonCommand Publisher::vsp_sync = ButtonCommand(
        XPlaneRef("sim/autopilot/vertical_speed_sync")
    );
}