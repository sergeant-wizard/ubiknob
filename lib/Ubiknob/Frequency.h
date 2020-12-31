#pragma once
#include <usb_flightsim.h>
#include "Knob.h"

namespace ubiknob {
    static bool get_even_increment(int value) {
        return (value == 0) || (value == 5);
    }
    class Frequency {
        public:
        Frequency(bool is_com, int mhz, int khz):
        mhz(mhz),
        khz(khz),
        mhz_min(is_com?118:108),
        mhz_max(is_com?137:118),
        is_com(is_com)
        {
        }
        void change_mhz(KnobDiff diff) {
            mhz = normalize(mhz, diff, mhz_min, mhz_max);
        }
        void change_khz(KnobDiff diff) {
            if (is_com) {
                bool even_increment = get_even_increment(khz % 10);
                if (diff < 0) {
                    even_increment = !even_increment;
                }
                const int sign = (diff > 0) ? 1 : -1;
                int sum = 0;
                for (int idx = 0; idx < abs(diff); idx++) {
                    sum += sign * (even_increment ? 2 : 3);
                }
                khz = normalize(khz, sum, 0, 100);
            } else {
                khz = normalize(khz, diff * 5, 0, 100);
            }
        }
        long to_long() const {
            return mhz * 100 + khz;
        }
        int getMhz() const {
            return mhz;
        }
        int getKhz() const {
            // XPlane frequency representation
            // 13542 = 135.425
            // 13545 = 135.450
            // 13547 = 135.475
            // 13550 = 135.500
            if (khz % 5 == 0) {
                return khz * 10;
            } else {
                return khz * 10 + 5;
            }
        }
        private:
        long normalize(long current, int diff, int min, int max) {
            int period = max - min;
            // diff could be negative
            return min + (current - min + diff + period) % period;
        }
        int mhz;
        int khz;
        const int mhz_min;
        const int mhz_max;
        const bool is_com;
    };
}