#pragma once

namespace ubiknob {
    enum DualKnobMode {
        mode_com1 = 0,
        mode_com2 = 1,
        mode_nav1 = 2,
        mode_nav2 = 3,
    };

    static constexpr unsigned NUM_DUAL_KNOB_MODE = 4;
};