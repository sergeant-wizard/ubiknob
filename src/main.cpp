#include <Arduino.h>

#include "Display.h"
#include "Dual.h"
#include "Knob.h"
#include "ModeSelector.h"
#include "Publisher.h"
#include "Single.h"

using namespace ubiknob;

// single mode pin
static constexpr int PIN_SM1 = 0;
static constexpr int PIN_SM2 = 1;
// dual mode pin
static constexpr int PIN_DM1 = 2;
static constexpr int PIN_DM2 = 3;

// single value pin
static constexpr int PIN_SV1 = 4;
static constexpr int PIN_SV2 = 5;
// dual value inner pin
static constexpr int PIN_DVI1 = 6;
static constexpr int PIN_DVI2 = 7;
// dual value outer pin
static constexpr int PIN_DVO1 = 8;
static constexpr int PIN_DVO2 = 9;

// buttons
static constexpr int PIN_SB = 10;
static constexpr int PIN_DB = 11;

// LCD
static constexpr int PIN_LCD_RS = 12;
static constexpr int PIN_LCD_EN = 13;
static constexpr int PIN_LCD_D4 = 14;
static constexpr int PIN_LCD_D5 = 15;
static constexpr int PIN_LCD_D6 = 16;
static constexpr int PIN_LCD_D7 = 17;

// modes
static auto single_mode_knob = KnobReader(PIN_SM1, PIN_SM2);
static auto single_mode_selector = ModeSelector<SingleKnobMode, NUM_SINGLE_KNOB_MODE>(SingleKnobMode::mode_alt);
static auto dual_mode_knob = KnobReader(PIN_DM1, PIN_DM2);
static auto dual_mode_selector = ModeSelector<DualKnobMode, NUM_DUAL_KNOB_MODE>(DualKnobMode::mode_com1);

// values
static auto single_value_knob = KnobReader(PIN_SV1, PIN_SV2);
static auto single_value_button = ButtonReader(PIN_SB);
static auto dual_value_inner_knob = KnobReader(PIN_DVI1, PIN_DVI2);
static auto dual_value_outer_knob = KnobReader(PIN_DVO1, PIN_DVO2);
static auto dual_value_button = ButtonReader(PIN_DB);

// publishers
static auto single_publisher = Publisher<SingleKnobMode>();
static auto dual_publisher = Publisher<DualKnobMode>();

static auto lcd = ubiknob::LCD(
    PIN_LCD_RS,
    PIN_LCD_EN,
    PIN_LCD_D4,
    PIN_LCD_D5,
    PIN_LCD_D6,
    PIN_LCD_D7
);

void setup() {
    pinMode(PIN_SV1, INPUT);
    pinMode(PIN_SV2, INPUT);
    pinMode(PIN_DVI1, INPUT);
    pinMode(PIN_DVI2, INPUT);
    pinMode(PIN_DVO1, INPUT);
    pinMode(PIN_DVO2, INPUT);
    pinMode(PIN_SB, INPUT);
    pinMode(PIN_DB, INPUT);

    pinMode(PIN_LCD_EN, OUTPUT); // TODO: check
    pinMode(PIN_LCD_RS, OUTPUT);
    pinMode(PIN_LCD_D4, OUTPUT);
    pinMode(PIN_LCD_D5, OUTPUT);
    pinMode(PIN_LCD_D6, OUTPUT);
    pinMode(PIN_LCD_D7, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    FlightSim.update();

    // modes
    const auto single_mode_diff = single_mode_knob.update();
    single_mode_selector.update(single_mode_diff);
    const auto dual_mode_diff = dual_mode_knob.update();
    dual_mode_selector.update(dual_mode_diff);

    // values
    const auto single_value_diff = single_value_knob.update();
    const auto dual_value_inner_diff = dual_value_inner_knob.update();
    const auto dual_value_outer_diff = dual_value_outer_knob.update();

    const auto single_button_diff = single_value_button.update();
    const auto dual_button_diff = dual_value_button.update();

    // show display
    lcd.update(single_mode_selector.getMode(), dual_mode_selector.getMode());

    // publish to xplane
    single_publisher.update(single_mode_selector.getMode(), single_button_diff);
    dual_publisher.update(dual_mode_selector.getMode(), dual_button_diff);

    // for some reason only one emission is allowed per frame
    if (single_value_diff != 0) {
        single_publisher.update(single_mode_selector.getMode(), single_value_diff);
        return;
    }
    if (dual_value_inner_diff != 0) {
        dual_publisher.update(dual_mode_selector.getMode(), dual_value_inner_diff, true);
        return;
    }
    if (dual_value_outer_diff != 0) {
        dual_publisher.update(dual_mode_selector.getMode(), dual_value_outer_diff, false);
        return;
    }
}

// test connection with xplane
static int hoge = 0;
void dual() {
    FlightSim.update();
    hoge++;
    if (hoge % 3 == 0) {
        dual_publisher.update(DualKnobMode::mode_nav1, 1, false);
    } else if (hoge % 3 == 1) {
        dual_publisher.update(DualKnobMode::mode_nav1, 1, true);
    } else {
        dual_publisher.update(DualKnobMode::mode_nav1, ButtonState::falling);
    }
    delay(500);
}

void single() {
    FlightSim.update();
    hoge++;
    if (hoge % 3 == 0) {
        single_publisher.update(SingleKnobMode::mode_hdg, 1);
    } else if (hoge % 3 == 1) {
        single_publisher.update(SingleKnobMode::mode_hdg, ButtonState::rising);
    } else if (hoge % 3 == 0) {
        single_publisher.update(SingleKnobMode::mode_hdg, ButtonState::falling);
    }
    delay(500);
}