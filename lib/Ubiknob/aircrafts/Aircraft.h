#pragma once

#include "Knob.h"
#include "Publisher.h"

namespace ubiknob {
    template<int NUM_LEFT, int NUM_RIGHT>
    struct Aircraft {
    public:
        Publisher& publisher;
        const KnobMode* left_modes;
        const KnobMode* right_modes;
        static constexpr int NUM_LEFT_MODES = NUM_LEFT;
        static constexpr int NUM_RIGHT_MODES = NUM_RIGHT;
    protected:
        Aircraft(Publisher& publisher, const KnobMode left_modes[NUM_LEFT], const KnobMode right_modes[NUM_RIGHT]):
            publisher(publisher),
            left_modes(left_modes),
            right_modes(right_modes)
        {}
    };
};