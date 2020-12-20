#pragma once
#include "LiquidCrystal.h"

#include "Dual.h"
#include "Single.h"


namespace ubiknob {
    static const char alt[4] = "ALT";
    static const char crs[4] = "CRS";
    static const char hdg[4] = "HDG";
    static const char vsp[4] = "VSP";
    static const char* format_single_mode(SingleKnobMode mode) {
        switch(mode) {
            case SingleKnobMode::mode_alt:
            return alt;
            case SingleKnobMode::mode_crs:
            return crs;
            case SingleKnobMode::mode_hdg:
            return hdg;
            case SingleKnobMode::mode_vsp:
            return vsp;
        }
        __builtin_unreachable();
    }
    static const char com1[5] = "COM1";
    static const char com2[5] = "COM2";
    static const char nav1[5] = "NAV1";
    static const char nav2[5] = "NAV2";
    static const char* format_dual_mode(DualKnobMode mode) {
        switch(mode) {
            case DualKnobMode::mode_com1:
            return com1;
            case DualKnobMode::mode_com2:
            return com2;
            case DualKnobMode::mode_nav1:
            return nav1;
            case DualKnobMode::mode_nav2:
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
        void update(SingleKnobMode single_mode, DualKnobMode dual_mode) {
            lcd.begin(16, 2);
            lcd.print(format_single_mode(single_mode));
            lcd.setCursor(0, 1);
            lcd.print(format_dual_mode(dual_mode));
        }
        private:
        LiquidCrystal lcd;
    };
}
