#include <Arduino.h>

#include "Display.h"
#include "Knob.h"
#include "ModeSelector.h"
#include "Pins.h"
#include "Publisher.h"

using namespace ubiknob;

// modes
static const KnobMode left_modes[5] = {
    KnobMode::mode_alt,
    KnobMode::mode_obs,
    KnobMode::mode_hdg,
    KnobMode::mode_vsp,
    KnobMode::mode_fms,
};
static const KnobMode right_modes[4] = {
    KnobMode::mode_com1,
    KnobMode::mode_com2,
    KnobMode::mode_nav1,
    KnobMode::mode_nav2,
};

static auto left_mode_knob = KnobReader(PIN_MIDDLE_INNER1, PIN_MIDDLE_INNER2);
static auto left_mode_selector = ModeSelector<5>(left_modes);
static auto right_mode_knob = KnobReader(PIN_MIDDLE_OUTER1, PIN_MIDDLE_OUTER2);
static auto right_mode_selector = ModeSelector<4>(right_modes);

// values
static auto left_inner_knob = KnobReader(PIN_LEFT_INNER1, PIN_LEFT_INNER2);
static auto left_outer_knob = KnobReader(PIN_LEFT_OUTER1, PIN_LEFT_OUTER2);
static auto left_button = ButtonReader(PIN_LEFT_BUTTON);
static auto right_inner_knob = KnobReader(PIN_RIGHT_INNER1, PIN_RIGHT_INNER2);
static auto right_outer_knob = KnobReader(PIN_RIGHT_OUTER1, PIN_RIGHT_OUTER2);
static auto right_button = ButtonReader(PIN_RIGHT_BUTTON);

// publishers
static auto publisher = Publisher();

static auto lcd = ubiknob::LCD(
    PIN_LCD_RS,
    PIN_LCD_EN,
    PIN_LCD_D4,
    PIN_LCD_D5,
    PIN_LCD_D6,
    PIN_LCD_D7
);

void setup() {
    pinMode(PIN_LEFT_BUTTON, INPUT_PULLUP);
    pinMode(PIN_RIGHT_BUTTON, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    FlightSim.update();

    // modes
    const auto left_mode_diff = left_mode_knob.update();
    left_mode_selector.update(left_mode_diff);
    const auto right_mode_diff = right_mode_knob.update();
    right_mode_selector.update(right_mode_diff);

    // values
    const auto left_inner_diff = left_inner_knob.update();
    const auto left_outer_diff = left_outer_knob.update();
    const auto right_inner_diff = right_inner_knob.update();
    const auto right_outer_diff = right_outer_knob.update();

    const auto left_button_diff = left_button.update();
    const auto right_button_diff = right_button.update();

    // publish to xplane
    publisher.update(left_mode_selector.getMode(), left_button_diff);
    publisher.update(right_mode_selector.getMode(), right_button_diff);

    // for some reason only one emission is allowed per frame
    if (left_inner_diff != 0) {
        publisher.update(left_mode_selector.getMode(), left_inner_diff, true);
    } else if (left_outer_diff != 0) {
        publisher.update(left_mode_selector.getMode(), left_outer_diff, false);
    } else if (right_inner_diff != 0) {
        publisher.update(right_mode_selector.getMode(), right_inner_diff, true);
    } else if (right_outer_diff != 0) {
        publisher.update(right_mode_selector.getMode(), right_outer_diff, false);
    }

    // show display
    if (
        left_mode_diff != 0 ||
        right_mode_diff != 0 ||
        right_button_diff != 0 ||
        right_inner_diff != 0 ||
        right_outer_diff != 0
    ) {
        lcd.update(
            left_mode_selector.getMode(),
            right_mode_selector.getMode(),
            publisher.getFrequencyManager(right_mode_selector.getMode())
        );
    } else {
        // without the delay, duplicate commands are sent
        delay(250);
    }

}

// test rotary encoder input
void rotary() {
    /* int a = digitalRead(PIN_SM1); */
    /* int b = digitalRead(PIN_SM2); */
    /* Serial.println(a); */
    /* Serial.println(b); */
    /* Serial.println("done"); */

    const auto left_mode_diff = left_mode_knob.update();
    Serial.println(left_mode_diff);
    delay(100);
}

// test connection with xplane
static int hoge = 0;
void right() {
    FlightSim.update();
    hoge++;
    if (hoge % 3 == 0) {
        publisher.update(KnobMode::mode_nav1, 1, false);
    } else if (hoge % 3 == 1) {
        publisher.update(KnobMode::mode_nav1, 1, true);
    } else {
        publisher.update(KnobMode::mode_nav1, ButtonState::falling);
    }
    delay(500);
}

void left() {
    FlightSim.update();
    hoge++;
    if (hoge % 3 == 0) {
        publisher.update(KnobMode::mode_hdg, 1, true);
    } else if (hoge % 3 == 1) {
        publisher.update(KnobMode::mode_hdg, ButtonState::rising);
    } else if (hoge % 3 == 0) {
        publisher.update(KnobMode::mode_hdg, ButtonState::falling);
    }
    delay(500);
}
