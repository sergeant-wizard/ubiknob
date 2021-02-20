#pragma once

#include "aircrafts/Aircraft.h"
#include "Knob.h"
#include "Publisher.h"

namespace ubiknob {
    namespace a320 {
        class A320Publisher: public Publisher {
            void update(KnobMode mode, KnobDiff diff, bool is_inner) {
            }
            void update(KnobMode mode, ButtonState state) {
            }
            const FrequencyManager& getFrequencyManager(KnobMode mode) const {
            }
        };
    };
    struct GA: public Aircraft<5, 4> {
        public:
        GA():
            Aircraft(publisher, left_modes, right_modes),
            _publisher(a320::A320Publisher())
        {}
        private:
            a320::A320Publisher _publisher;
    };
};