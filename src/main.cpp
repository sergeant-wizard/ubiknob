#include "Arduino.h"

#include "Display.h"
#include "Dual.h"
#include "Knob.h"
#include "ModeSelector.h"
#include "Publisher.h"
#include "Single.h"

using namespace ubiknob;

// modes
static const auto single_mode_knob = KnobReader();
static auto single_mode_selector = ModeSelector<SingleKnobMode, NUM_SINGLE_KNOB_MODE>(SingleKnobMode::mode_alt);
static const auto dual_mode_knob = KnobReader();
static auto dual_mode_selector = ModeSelector<DualKnobMode, NUM_DUAL_KNOB_MODE>(DualKnobMode::mode_com1);

// values
static const auto single_value_knob = KnobReader();
static const auto dual_value_inner_knob = KnobReader();
static const auto dual_value_outer_knob = KnobReader();

// TODO: set pins
static auto lcd = LCD(0, 0, 0, 0, 0, 0);

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    // modes
    const auto single_mode_state = single_mode_knob.update();
    single_mode_selector.update(single_mode_state);
    const auto dual_mode_state = dual_mode_knob.update();
    dual_mode_selector.update(dual_mode_state);

    // values
    const auto single_value_state = single_value_knob.update();
    const auto dual_value_inner_state = dual_value_inner_knob.update();
    const auto dual_value_outer_state = dual_value_outer_knob.update();

    // publish to xplane
    Publisher<SingleKnobMode>::update(single_mode_selector.getMode(), single_value_state);
    Publisher<DualKnobMode>::update(dual_mode_selector.getMode(), dual_value_inner_state, true);
    Publisher<DualKnobMode>::update(dual_mode_selector.getMode(), dual_value_outer_state, false);

    // show display
    lcd.update(single_mode_selector.getMode(), dual_mode_selector.getMode());
}