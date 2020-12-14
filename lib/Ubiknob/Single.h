#pragma once

namespace ubiknob {
    enum SingleKnobMode {
        alt = 0,
        crs = 1,
        hdg = 2,
        vsp = 3,
    };

    constexpr unsigned NUM_SINGLE_KNOB_MODE = 4;
};