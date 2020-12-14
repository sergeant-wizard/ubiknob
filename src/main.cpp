#include "Arduino.h"

#include "Knob.h"
#include "ModeSelector.h"
#include "Publisher.h"
#include "Single.h"

using namespace ubiknob;

static const auto single_mode_knob = KnobReader();
static auto single_mode_selector = ModeSelector<SingleKnobMode, NUM_SINGLE_KNOB_MODE>(SingleKnobMode::alt);

static const auto single_value_knob = KnobReader();

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    const auto single_mode_state = single_mode_knob.update();
    single_mode_selector.update(single_mode_state);

    const auto single_value_state = single_value_knob.update();
    Publisher<SingleKnobMode>::update(single_mode_selector.getMode(), single_value_state);
}