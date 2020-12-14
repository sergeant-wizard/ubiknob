#pragma once

namespace ubiknob {
    enum SingleKnobMode {
        mode_alt = 0,
        mode_crs = 1,
        mode_hdg = 2,
        mode_vsp = 3,
    };

    constexpr unsigned NUM_SINGLE_KNOB_MODE = 4;
};