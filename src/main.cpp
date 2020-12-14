#include <Arduino.h>

#include "Display.h"
#include "Dual.h"
#include "Knob.h"
#include "ModeSelector.h"
#include "Publisher.h"
#include "Single.h"

using namespace ubiknob;

// modes
static auto single_mode_knob = KnobReader(0, 0);
static auto single_mode_selector = ModeSelector<SingleKnobMode, NUM_SINGLE_KNOB_MODE>(SingleKnobMode::mode_alt);
static auto dual_mode_knob = KnobReader(0, 0);
static auto dual_mode_selector = ModeSelector<DualKnobMode, NUM_DUAL_KNOB_MODE>(DualKnobMode::mode_com1);

// values
static auto single_value_knob = KnobReader(0, 0);
static auto single_value_button = ButtonReader(0);
static auto dual_value_inner_knob = KnobReader(0, 0);
static auto dual_value_outer_knob = KnobReader(0, 0);
static auto dual_value_button = ButtonReader(0);

// TODO: set pins
static auto lcd = ubiknob::LCD(0, 0, 0, 0, 0, 0);

void setup() {
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

    // publish to xplane
    Publisher<SingleKnobMode>::update(single_mode_selector.getMode(), single_value_diff);
    Publisher<DualKnobMode>::update(dual_mode_selector.getMode(), dual_value_inner_diff, true);
    Publisher<DualKnobMode>::update(dual_mode_selector.getMode(), dual_value_outer_diff, false);

    Publisher<SingleKnobMode>::update(single_mode_selector.getMode(), single_button_diff);
    Publisher<DualKnobMode>::update(dual_mode_selector.getMode(), dual_button_diff);

    // show display
    lcd.update(single_mode_selector.getMode(), dual_mode_selector.getMode());
}