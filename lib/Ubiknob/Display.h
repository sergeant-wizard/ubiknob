#pragma once
#include <stdio.h>
#include <LiquidCrystal.h>

#include "FrequencyManager.h"
#include "Knob.h"


namespace ubiknob {
    static const char alt[4] = "ALT";
    static const char obs[4] = "OBS";
    static const char hdg[4] = "HDG";
    static const char vsp[4] = "VSP";
    static const char fms[4] = "FMS";
    static const char com1[5] = "COM1";
    static const char com2[5] = "COM2";
    static const char nav1[5] = "NAV1";
    static const char nav2[5] = "NAV2";
    static const char* format_mode(KnobMode mode) {
        switch(mode) {
            case KnobMode::mode_alt:
            return alt;
            case KnobMode::mode_obs:
            return obs;
            case KnobMode::mode_hdg:
            return hdg;
            case KnobMode::mode_vsp:
            return vsp;
            case KnobMode::mode_fms:
            return fms;
            case KnobMode::mode_com1:
            return com1;
            case KnobMode::mode_com2:
            return com2;
            case KnobMode::mode_nav1:
            return nav1;
            case KnobMode::mode_nav2:
            return nav2;
        }
        __builtin_unreachable();
    }
    class LCD {
        public:
        LCD(int rs, int en, int d4, int d5, int d6, int d7):
        lcd(rs, en, d4, d5, d6, d7)
        {
        }
        // 0123456789012345
        // ALT         NAV1
        // 114.000  109.050
        void update(KnobMode left_mode, KnobMode right_mode, const FrequencyManager& manager) {
            lcd.begin(16, 2);
            lcd.setCursor(0, 0);
            lcd.print(format_mode(left_mode));
            lcd.setCursor(12, 0);
            lcd.print(format_mode(right_mode));

            char frequency_output[17];
            const auto active = manager.getActive();
            const auto standby = manager.getStandby();
            snprintf(
                frequency_output, 17, "%03d.%03d  %03d.%03d",
                active.getMhz(), active.getKhz(), standby.getMhz(), standby.getKhz()
            );
            lcd.setCursor(0, 1);
            lcd.print(frequency_output);
        }
        private:
        LiquidCrystal lcd;
    };
}
