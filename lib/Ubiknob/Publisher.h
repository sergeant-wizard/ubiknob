#pragma once
#include <usb_flightsim.h>

#include "Command.h"
#include "FrequencyManager.h"
#include "Knob.h"

namespace ubiknob {
    class Publisher {
        public:
        virtual void update(KnobMode mode, KnobDiff diff, bool is_inner) = 0;
        virtual void update(KnobMode mode, ButtonState state) = 0;
        virtual const FrequencyManager& getFrequencyManager(KnobMode mode) const = 0;
    };
}