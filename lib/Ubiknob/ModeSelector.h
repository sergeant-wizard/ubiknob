#pragma once

namespace ubiknob {
    template <class T, unsigned N>
    class ModeSelector {
        public:
        ModeSelector(T mode):
            mode(mode)
        {
        }
        void update(KnobState state) {
            if (state == KnobState::cw) {
                next();
            } else if (state == KnobState::ccw) {
                prev();
            }
        }
        T getMode() const {
            return this->mode;
        }
        private:
        void next() {
            this->mode = static_cast<T>((mode + 1) % N);
        }
        void prev() {
            this->mode = static_cast<T>((mode - 1) % N);
        }
        T mode;
    };
}