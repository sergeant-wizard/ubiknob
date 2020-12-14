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
                case SingleKnobMode::alt:
                break;
                case SingleKnobMode::crs:
                break;
                case SingleKnobMode::hdg:
                break;
                case SingleKnobMode::vsp:
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
                case DualKnobMode::com1:
                break;
                case DualKnobMode::com2:
                break;
                case DualKnobMode::nav1:
                break;
                case DualKnobMode::nav2:
                break;
            }
        }
    };
}