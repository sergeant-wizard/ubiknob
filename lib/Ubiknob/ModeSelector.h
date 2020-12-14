#pragma once

namespace ubiknob {
    template <class T, unsigned N>
    class ModeSelector {
        public:
        ModeSelector(T mode):
            mode(mode)
        {
        }
        void update(KnobDiff diff) {
            mode = static_cast<T>((mode + diff) % N);
        }
        T getMode() const {
            return mode;
        }
        private:
        T mode;
    };
}