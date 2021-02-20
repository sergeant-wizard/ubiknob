#pragma once

#include <usb_flightsim.h>
#include "aircrafts/Aircraft.h"
#include "Command.h"
#include "Knob.h"
#include "Publisher.h"

namespace ubiknob {
    namespace ga {
        class ValueManager {
            public:
            ValueManager(const _XpRefStr_* diff, int interval_small, int interval_large, bool is_cyclic):
            interval_small(interval_small),
            interval_large(interval_large),
            is_cyclic(is_cyclic)
            {
                sim_flt = diff;
            }
            void update(KnobDiff diff, bool is_small) {
                if (is_small) {
                    sim_flt = getUpdatedValue(diff, interval_small);
                } else {
                    sim_flt = getUpdatedValue(diff, interval_large);
                }
            }
            private:
            long getUpdatedValue(KnobDiff diff, int interval) const {
                long ret = sim_flt + diff * interval;
                ret = ret - ret % interval_small;
                if (is_cyclic) {
                    return (ret + 360) % 360;
                } else {
                    return ret;
                }
            }
            FlightSimFloat sim_flt;
            const int interval_small;
            const int interval_large;
            const bool is_cyclic;
        };

        class GAPublisher: public Publisher {
            public:
            GAPublisher():
            alt_diff(XPlaneRef("sim/cockpit/autopilot/altitude"), 100, 1000, false),
            hdg_diff(XPlaneRef("sim/cockpit/autopilot/heading_mag"), 1, 10, true),
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
                        if (is_inner) {
                            if (diff < 0) {
                                obs.runDown();
                            } else if (diff > 0) {
                                obs.runUp();
                            }
                        } else {
                            if (diff < 0) {
                                baro.runDown();
                            } else if (diff > 0) {
                                baro.runUp();
                            }
                        }
                        break;
                    case KnobMode::mode_hdg:
                        hdg_diff.update(diff, is_inner);
                        break;
                    case KnobMode::mode_vsp:
                        vsp_diff.update(diff, is_inner);
                        break;
                    case KnobMode::mode_fms:
                        if (is_inner) {
                            if (diff < 0) {
                                fms_inner.runDown();
                            } else if (diff > 0) {
                                fms_inner.runUp();
                            }
                        } else {
                            if (diff < 0) {
                                fms_outer.runDown();
                            } else if (diff > 0) {
                                fms_outer.runUp();
                            }
                        }
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
                    case KnobMode::mode_spd:
                        // noop
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
                } else if (mode == KnobMode::mode_fms) {
                    fms_cursor.run(state);
                }
            }
            const FrequencyInterface& getFrequencyInterface(KnobMode mode) const {
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
            static UpDownButtonCommand baro;
            static UpDownButtonCommand obs;
            ValueManager hdg_diff;
            ValueManager vsp_diff;
            FrequencyManager com1;
            FrequencyManager com2;
            FrequencyManager nav1;
            FrequencyManager nav2;
            static ButtonCommand alt_sync;
            static ButtonCommand obs_sync;
            static ButtonCommand hdg_sync;
            static ButtonCommand vsp_sync;
            static UpDownButtonCommand fms_outer;
            static UpDownButtonCommand fms_inner;
            static ButtonCommand fms_cursor;
        };

        UpDownButtonCommand GAPublisher::baro = UpDownButtonCommand(
            XPlaneRef("sim/GPS/g1000n1_baro_up"),
            XPlaneRef("sim/GPS/g1000n1_baro_down")
        );
        UpDownButtonCommand GAPublisher::obs = UpDownButtonCommand(
            XPlaneRef("sim/GPS/g1000n1_crs_up"),
            XPlaneRef("sim/GPS/g1000n1_crs_down")
        );
        ButtonCommand GAPublisher::alt_sync = ButtonCommand(
            XPlaneRef("sim/autopilot/altitude_sync")
        );
        ButtonCommand GAPublisher::obs_sync = ButtonCommand(
            XPlaneRef("sim/GPS/g1000n1_crs_sync")
        );
        ButtonCommand GAPublisher::hdg_sync = ButtonCommand(
            XPlaneRef("sim/autopilot/heading_sync")
        );
        ButtonCommand GAPublisher::vsp_sync = ButtonCommand(
            XPlaneRef("sim/autopilot/vertical_speed_sync")
        );
        UpDownButtonCommand GAPublisher::fms_outer = UpDownButtonCommand(
            XPlaneRef("sim/GPS/g1000n1_fms_outer_up"),
            XPlaneRef("sim/GPS/g1000n1_fms_outer_down")
        );
        UpDownButtonCommand GAPublisher::fms_inner = UpDownButtonCommand(
            XPlaneRef("sim/GPS/g1000n1_fms_inner_up"),
            XPlaneRef("sim/GPS/g1000n1_fms_inner_down")
        );
        ButtonCommand GAPublisher::fms_cursor = ButtonCommand(
            XPlaneRef("sim/GPS/g1000n1_cursor")
        );

        static const KnobMode left_modes[5] = {
            KnobMode::mode_alt,
            KnobMode::mode_obs,
            KnobMode::mode_hdg,
            KnobMode::mode_vsp,
            KnobMode::mode_fms,
        };
        static const KnobMode right_modes[4] = {
            KnobMode::mode_com1,
            KnobMode::mode_com2,
            KnobMode::mode_nav1,
            KnobMode::mode_nav2,
        };
    };

    struct GA: public Aircraft<5, 4> {
        public:
        GA():
            Aircraft(_publisher, ga::left_modes, ga::right_modes),
            _publisher(ga::GAPublisher())
        {}
        private:
            ga::GAPublisher _publisher;
    };
};