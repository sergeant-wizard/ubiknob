#pragma once

#include <usb_flightsim.h>
#include "aircrafts/Aircraft.h"
#include "Knob.h"
#include "Publisher.h"

namespace ubiknob {
    namespace a320 {
        class ValueManager {
            public:
            ValueManager(const _XpRefStr_* diff):
                cycle(0)
            {
                sim_flt = diff;
            }
            void update(KnobDiff diff) {
                if (diff < 0) {
                    --cycle;
                } else if (diff > 0) {
                    ++cycle;
                }
                cycle %= 64;
                sim_flt = cycle / 64.;
            }
            private:
            int cycle;
            FlightSimFloat sim_flt;
        };
        class AltStepSelector {
            public:
            AltStepSelector() {
                sim_flt = XPlaneRef("model/controls/fcu_altitude_step");
            }
            void update(KnobDiff diff) {
                if (diff < 0) {
                    sim_flt = 0;
                } else if (diff > 0) {
                    sim_flt = 1;
                }
            }
            private:
            FlightSimFloat sim_flt;
        };
        class PushPullSelector {
            public:
            PushPullSelector(const _XpRefStr_* diff):
                is_push(false)
            {
                sim_flt = diff;
            }
            void update(ButtonState state) {
                if (state == ButtonState::falling) {
                    sim_flt = is_push ? 0.0 : 1.0;
                } else if (state == ButtonState::rising) {
                    sim_flt = 0.5;
                }
            }
            private:
            FlightSimFloat sim_flt;
            bool is_push;
        };
        class A320Publisher: public Publisher {
            public:
            A320Publisher():
                alt(XPlaneRef("model/controls/fcu_altitude")),
                hdg(XPlaneRef("model/controls/fcu_lateral")),
                spd(XPlaneRef("model/controls/fcu_speed")),
                alt_mode(XPlaneRef("model/controls/fcu_altitude_mode")),
                hdg_mode(XPlaneRef("model/controls/fcu_lateral_mode")),
                spd_mode(XPlaneRef("model/controls/fcu_speed_mode"))
            {}
            void update(KnobMode mode, KnobDiff diff, bool is_inner) {
                if (mode == KnobMode::mode_alt) {
                    if (is_inner) {
                        alt.update(diff);
                    } else {
                        alt_step.update(diff);
                    }
                } else if (mode == KnobMode::mode_hdg) {
                    hdg.update(diff);
                } else if (mode == KnobMode::mode_spd) {
                    spd.update(diff);
                }
            }
            void update(KnobMode mode, ButtonState state) {
                if (mode == KnobMode::mode_alt) {
                    alt_mode.update(state);
                } else if (mode == KnobMode::mode_hdg) {
                    hdg_mode.update(state);
                } else if (mode == KnobMode::mode_spd) {
                    spd_mode.update(state);
                }
            }
            const FrequencyManager& getFrequencyManager(KnobMode mode) const {
            }
            private:
                ValueManager alt;
                ValueManager hdg;
                ValueManager spd;
                AltStepSelector alt_step;
                PushPullSelector alt_mode;
                PushPullSelector hdg_mode;
                PushPullSelector spd_mode;
        };
        static const KnobMode left_modes[3] = {
            KnobMode::mode_alt,
            KnobMode::mode_hdg,
            KnobMode::mode_spd,
        };
        static const KnobMode right_modes[2] = {
            KnobMode::mode_com1,
            KnobMode::mode_com2,
        };
    };
    struct A320: public Aircraft<3, 2> {
        public:
        A320():
            Aircraft(publisher, left_modes, right_modes),
            _publisher(a320::A320Publisher())
        {}
        private:
            a320::A320Publisher _publisher;
    };
};