#pragma once

namespace ubiknob {
    enum KnobState {
        unchanged,
        cw,
        ccw,
    };

    class KnobReader {
        public:
        KnobState update() const {
            return KnobState::unchanged;
        }
    };
};