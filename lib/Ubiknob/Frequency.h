#pragma once
#include <usb_flightsim.h>
#include "Knob.h"

namespace ubiknob {
    class Frequency {
        public:
        Frequency(bool is_com, int mhz, int khz):
        mhz(mhz),
        khz(khz),
        mhz_min(is_com?118:108),
        mhz_max(is_com?137:118)
        {
        }
        void change_mhz(KnobDiff diff) {
            mhz = normalize(mhz, diff, mhz_min, mhz_max);
        }
        void change_khz(KnobDiff diff) {
            khz = normalize(khz, diff, 0, 100);
        }
        long to_long() const {
            return mhz * 100 + khz;
        }
        private:
        long normalize(long current, int diff, int min, int max) {
            int period = max - min;
            return min + (current - min + diff) % period;
        }
        int mhz;
        int khz;
        const int mhz_min;
        const int mhz_max;
    };
}