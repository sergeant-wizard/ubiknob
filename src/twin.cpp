#include "Pins.h"
#include "Power.h"

using namespace ubiknob;

static auto left_inner_knob = KnobReader(PIN_LEFT_INNER1, PIN_LEFT_INNER2);
static auto left_outer_knob = KnobReader(PIN_LEFT_OUTER1, PIN_LEFT_OUTER2);
static auto right_inner_knob = KnobReader(PIN_RIGHT_INNER1, PIN_RIGHT_INNER2);
static auto right_outer_knob = KnobReader(PIN_RIGHT_OUTER1, PIN_RIGHT_OUTER2);

static auto pml = PowerManager(0);
static auto pmr = PowerManager(1);

void _setup() {
    pinMode(LED_BUILTIN, OUTPUT);
}

void _loop() {
    FlightSim.update();
    const auto right_inner_diff = right_inner_knob.update();
    if (right_inner_diff != 0) {
        pmr.update(right_inner_diff, true);
    }
    const auto right_outer_diff = right_outer_knob.update();
    if (right_outer_diff != 0) {
        pmr.update(right_outer_diff, false);
    }
    const auto left_inner_diff = left_inner_knob.update();
    if (left_inner_diff != 0) {
        pml.update(left_inner_diff, true);
    }
    const auto left_outer_diff = left_outer_knob.update();
    if (left_outer_diff != 0) {
        pml.update(left_outer_diff, false);
    }
}
