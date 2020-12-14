#pragma once
#include <Dual.h>
#include <Knob.h>
#include <Single.h>

namespace ubiknob {
    template<class T>
    class Publisher {
        public:
        void update(T mode, KnobState state) const;
    };

    template<>
    class Publisher<SingleKnobMode> {
        public:
        static void update(SingleKnobMode mode, KnobState state) {
            if (state == KnobState::unchanged) {
                return;
            }
            switch (mode) {
                // TODO
                case SingleKnobMode::mode_alt:
                break;
                case SingleKnobMode::mode_crs:
                break;
                case SingleKnobMode::mode_hdg:
                break;
                case SingleKnobMode::mode_vsp:
                break;
            }
        }
    };

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