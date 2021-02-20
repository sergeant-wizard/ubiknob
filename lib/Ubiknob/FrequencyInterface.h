#pragma once

#include "Frequency.h"

namespace ubiknob {
    class FrequencyInterface {
        public:
        virtual Frequency getActive() const = 0;
        virtual Frequency getStandby() const = 0;
    };
};