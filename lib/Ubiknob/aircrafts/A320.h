#pragma once

#include <usb_flightsim.h>
#include "aircrafts/Aircraft.h"
#include "Command.h"
#include "Knob.h"
#include "Publisher.h"

namespace ubiknob {
    namespace a320 {
        class PushPullSelector {
            public:
            PushPullSelector(const _XpRefStr_* diff):
                is_push(false)
            {
                sim_flt = diff;
            }
            void update(ButtonState state) {
                if (state == ButtonState::falling) {
                    sim_flt = is_push ? -1.0 : 1.0;
                    is_push = !is_push;
                } else if (state == ButtonState::rising) {
                    sim_flt = 0.0;
                }
            }
            private:
            FlightSimFloat sim_flt;
            bool is_push;
        };
        class FrequencyManager: public FrequencyInterface {
            // FFA320 COM READ is not supported :(
            public:
            FrequencyManager(
                const _XpRefStr_ *inner_up,
                const _XpRefStr_ *inner_down,
                const _XpRefStr_ *outer_up,
                const _XpRefStr_ *outer_down,
                const _XpRefStr_ *swap
            ):
                inner(UpDownButtonCommand(inner_up, inner_down)),
                outer(UpDownButtonCommand(outer_up, outer_down)),
                swapCommand(ButtonCommand(swap))
            {}
            Frequency getActive() const {
                return Frequency(true, 0, 0);
            }
            Frequency getStandby() const {
                return Frequency(true, 0, 0);
            }
            void update(KnobDiff diff, bool is_inner) {
                if (diff < 0) {
                    if (is_inner) {
                        inner.runDown();
                    } else {
                        outer.runDown();
                    }
                } else if (diff > 0) {
                    if (is_inner) {
                        inner.runUp();
                    } else {
                        outer.runUp();
                    }
                }
            }
            void swap(ButtonState state) {
                swapCommand.run(state);
            }
            private:
            UpDownButtonCommand inner;
            UpDownButtonCommand outer;
            ButtonCommand swapCommand;
        };
        class A320Publisher: public Publisher {
            public:
            A320Publisher():
                alt(
                    XPlaneRef("a320/Panel/FCU_Altitude_switch+"),
                    XPlaneRef("a320/Panel/FCU_Altitude_switch-")
                ),
                hdg(
                    XPlaneRef("a320/Panel/FCU_Lateral_switch+"),
                    XPlaneRef("a320/Panel/FCU_Lateral_switch-")
                ),
                spd(
                    XPlaneRef("a320/Panel/FCU_Speed_switch+"),
                    XPlaneRef("a320/Panel/FCU_Speed_switch-")
                ),
                alt_mode(XPlaneRef("a320/Panel/FCU_AltitudeMode")),
                hdg_mode(XPlaneRef("a320/Panel/FCU_LateralMode")),
                spd_mode(XPlaneRef("a320/Panel/FCU_SpeedMode")),
                alt_step(
                    XPlaneRef("a320/Panel/FCU_AltitudeStep_switch+"),
                    XPlaneRef("a320/Panel/FCU_AltitudeStep_switch+")
                ),
                com1(
                    XPlaneRef("a320/RMP1/FreqInner_switch+"),
                    XPlaneRef("a320/RMP1/FreqInner_switch-"),
                    XPlaneRef("a320/RMP1/FreqOuter_switch+"),
                    XPlaneRef("a320/RMP1/FreqOuter_switch-"),
                    XPlaneRef("a320/RMP1/Exchange_button")
                ),
                com2(
                    XPlaneRef("a320/RMP2/FreqInner_switch+"),
                    XPlaneRef("a320/RMP2/FreqInner_switch-"),
                    XPlaneRef("a320/RMP2/FreqOuter_switch+"),
                    XPlaneRef("a320/RMP2/FreqOuter_switch-"),
                    XPlaneRef("a320/RMP2/Exchange_button")
                )
            {}
            void update(KnobMode mode, KnobDiff diff, bool is_inner) {
                if (mode == KnobMode::mode_alt) {
                    if (is_inner) {
                        if (diff < 0) {
                            alt.runDown();
                        } else if (diff > 0) {
                            alt.runUp();
                        }
                    } else {
                        if (diff < 0) {
                            alt_step.runDown();
                        } else if (diff > 0) {
                            alt_step.runUp();
                        }
                    }
                } else if (mode == KnobMode::mode_hdg) {
                    if (diff < 0) {
                        hdg.runDown();
                    } else if (diff > 0) {
                        hdg.runUp();
                    }
                } else if (mode == KnobMode::mode_spd) {
                    if (diff < 0) {
                        spd.runDown();
                    } else if (diff > 0) {
                        spd.runUp();
                    }
                } else if (mode == KnobMode::mode_com1) {
                    com1.update(diff, is_inner);
                } else if (mode == KnobMode::mode_com2) {
                    com2.update(diff, is_inner);
                }
            }
            void update(KnobMode mode, ButtonState state) {
                if (mode == KnobMode::mode_alt) {
                    alt_mode.update(state);
                } else if (mode == KnobMode::mode_hdg) {
                    hdg_mode.update(state);
                } else if (mode == KnobMode::mode_spd) {
                    spd_mode.update(state);
                } else if (mode == KnobMode::mode_com1) {
                    com1.swap(state);
                } else if (mode == KnobMode::mode_com2) {
                    com2.swap(state);
                }
            }
            const FrequencyInterface& getFrequencyInterface(KnobMode mode) const {
                return com1;
            }
            private:
            UpDownButtonCommand alt;
            UpDownButtonCommand hdg;
            UpDownButtonCommand spd;
            PushPullSelector alt_mode;
            PushPullSelector hdg_mode;
            PushPullSelector spd_mode;
            UpDownButtonCommand alt_step;
            FrequencyManager com1;
            FrequencyManager com2;
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
            Aircraft(_publisher, a320::left_modes, a320::right_modes),
            _publisher(a320::A320Publisher())
        {}
        private:
            a320::A320Publisher _publisher;
    };
};