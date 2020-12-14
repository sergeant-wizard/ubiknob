#pragma once
#include <Encoder.h>

namespace ubiknob {
    typedef int KnobDiff;
    class KnobReader {
        public:
        KnobReader(int pin1, int pin2): enc(pin1, pin2), prev(0) {
        }
        KnobDiff update() {
            const auto current = enc.read();
            const auto diff = current - prev;
            prev = current;
            return diff;
        }
        private:
        Encoder enc;
        int prev;
    };
};