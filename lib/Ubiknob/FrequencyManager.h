#pragma once
#include <usb_flightsim.h>

#include "Frequency.h"
#include "FrequencyInterface.h"
#include "Knob.h"

namespace ubiknob {
    static Frequency make_frequency(bool is_com, FlightSimInteger fsi) {
        const auto val = fsi.read();
        return Frequency(is_com, val / 100, val % 100);
    }
    class FrequencyManager: public FrequencyInterface {
        public:
        FrequencyManager(const _XpRefStr_* active_ref, const _XpRefStr_* standby_ref, bool is_com):
        is_com(is_com)
        {
            active_value = active_ref;
            standby_value = standby_ref;
            if (is_com) {
                active_value = 11800;
                standby_value = 11800;
            } else {
                active_value = 10800;
                standby_value = 10800;
            }
        }
        void update(KnobDiff diff, bool is_inner) {
            auto frequency = make_frequency(is_com, standby_value);
            if (is_inner) {
                frequency.change_khz(diff);
            } else {
                frequency.change_mhz(diff);
            }
            standby_value = frequency.to_long();
        }
        void swap() {
            const auto new_active = standby_value.read();
            const auto new_standby = active_value.read();
            active_value = new_active;
            standby_value = new_standby;
        }
        Frequency getActive() const {
            return make_frequency(is_com, active_value);
        }
        Frequency getStandby() const {
            return make_frequency(is_com, standby_value);
        }
        private:
        const bool is_com;
        FlightSimInteger active_value;
        FlightSimInteger standby_value;
    };
}