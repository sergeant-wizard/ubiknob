#pragma once

namespace ubiknob {
    template <unsigned N>
    class ModeSelector {
        public:
        ModeSelector(const KnobMode* modes):
            mode_idx(0),
            modes(modes)
        {
        }
        void update(KnobDiff diff) {
            mode_idx = (mode_idx + diff) % N;
        }
        KnobMode getMode() const {
            return modes[mode_idx];
        }
        private:
        unsigned mode_idx;
        const KnobMode* modes;
    };
}