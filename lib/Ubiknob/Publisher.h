#pragma once
#include <usb_flightsim.h>

#include "Frequency.h"
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

    template<class T>
    class Publisher {
        public:
        void update(T mode, KnobDiff diff) const;
    };

    template<>
    class Publisher<LeftKnobMode> {
        public:
        Publisher():
        alt_diff(XPlaneRef("sim/cockpit/autopilot/altitude"), 100),
        crs_diff(XPlaneRef("sim/cockpit/gps/course"), 1),
        obs_diff(XPlaneRef("sim/cockpit/radios/nav1_obs_degm"), 1),
        hdg_diff(XPlaneRef("sim/cockpit/autopilot/heading"), 1),
        vsp_diff(XPlaneRef("sim/cockpit/autopilot/vertical_velocity"), 100)
        {}
        void update(LeftKnobMode mode, KnobDiff diff) {
            switch(mode) {
                case LeftKnobMode::mode_alt:
                alt_diff.update(diff);
                break;
                case LeftKnobMode::mode_crs:
                crs_diff.update(diff);
                break;
                case LeftKnobMode::mode_hdg:
                hdg_diff.update(diff);
                break;
                case LeftKnobMode::mode_vsp:
                vsp_diff.update(diff);
                break;
            }
        }
        void update(LeftKnobMode mode, ButtonState state) {
            switch(mode) {
                case LeftKnobMode::mode_alt:
                alt_sync.run(state);
                break;
                case LeftKnobMode::mode_crs:
                crs_sync.run(state);
                break;
                case LeftKnobMode::mode_hdg:
                hdg_sync.run(state);
                break;
                case LeftKnobMode::mode_vsp:
                vsp_sync.run(state);
                break;
            }
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
    };

    ButtonCommand Publisher<LeftKnobMode>::alt_sync = ButtonCommand(
        XPlaneRef("sim/autopilot/altitude_sync")
    );
    ButtonCommand Publisher<LeftKnobMode>::crs_sync = ButtonCommand(
        XPlaneRef("sim/GPS/g1000n1_crs_sync")
    );
    ButtonCommand Publisher<LeftKnobMode>::hdg_sync = ButtonCommand(
        XPlaneRef("sim/autopilot/heading_sync")
    );
    ButtonCommand Publisher<LeftKnobMode>::vsp_sync = ButtonCommand(
        XPlaneRef("sim/autopilot/vertical_speed_sync")
    );

    static Frequency make_frequency(bool is_com, FlightSimInteger fsi) {
        const auto val = fsi.read();
        return Frequency(is_com, val / 100, val % 100);
    }
    class FrequencyManager {
        public:
        FrequencyManager(const _XpRefStr_* active_ref, const _XpRefStr_* standby_ref, bool is_com):
        is_com(is_com)
        {
            active_value = active_ref;
            standby_value = standby_ref;
            if (is_com) {
                active_value = 11800;
                standby_value = 11800;
            } else {
                active_value = 10800;
                standby_value = 10800;
            }
        }
        void update(KnobDiff diff, bool is_inner) {
            auto frequency = make_frequency(is_com, standby_value);
            if (is_inner) {
                frequency.change_khz(diff);
            } else {
                frequency.change_mhz(diff);
            }
            standby_value = frequency.to_long();
        }
        void swap() {
            const auto new_active = standby_value.read();
            const auto new_standby = active_value.read();
            active_value = new_active;
            standby_value = new_standby;
        }
        private:
        const bool is_com;
        FlightSimInteger active_value;
        FlightSimInteger standby_value;
    };

    template<>
    class Publisher<RightKnobMode> {
        public:
        Publisher():
        com1(FrequencyManager(
            XPlaneRef("sim/cockpit/radios/com1_freq_hz"),
            XPlaneRef("sim/cockpit/radios/com1_stdby_freq_hz"),
            true
        )),
        nav1(FrequencyManager(
            XPlaneRef("sim/cockpit/radios/nav1_freq_hz"),
            XPlaneRef("sim/cockpit/radios/nav1_stdby_freq_hz"),
            false
        ))
        {
        }
        void update(RightKnobMode mode, KnobDiff diff, bool is_inner) {
            switch(mode) {
                case RightKnobMode::mode_com1:
                    com1.update(diff, is_inner);
                    break;
                case RightKnobMode::mode_com2:
                break;
                case RightKnobMode::mode_nav1:
                    nav1.update(diff, is_inner);
                    break;
                case RightKnobMode::mode_nav2:
                break;
            }
        }
        void update(RightKnobMode mode, ButtonState state) {
            switch(mode) {
                case RightKnobMode::mode_nav1:
                if (state == ButtonState::falling) {
                    nav1.swap();
                }
                break;
                default:
                break;
            }
        }
        private:
        FrequencyManager com1;
        FrequencyManager nav1;
    };
}