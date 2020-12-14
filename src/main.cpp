#include "Arduino.h"

enum SingleKnobMode {
    alt = 0,
    crs = 1,
    hdg = 2,
    vsp = 3,
};

static constexpr unsigned NUM_SINGLE_KNOB_MODE = 4;

enum DualKnobMode {
    nav1 = 0,
    nav2 = 1,
    com1 = 2,
    com2 = 3,
};

static constexpr unsigned NUM_DUAL_KNOB_MODE = 4;

template<class T, unsigned NUM>
class ModeIterator {
    public:
    ModeIterator(T mode):
    mode(mode)
    {
    }
    private:
    T mode;
};

enum KnobState {
    unchanged,
    cw,
    ccw,
};

class KnobReader {
    public:
    KnobState update() const {
        return KnobState::unchanged;
    }
};

template <class T, unsigned N>
class ModeSelector {
    public:
    ModeSelector(T mode):
        mode(mode)
    {
    }
    void update(KnobState state) {
        if (state == KnobState::cw) {
            next();
        } else if (state == KnobState::ccw) {
            prev();
        }
    }
    T getMode() const {
        return this->mode;
    }
    private:
    void next() {
        this->mode = static_cast<T>((mode + 1) % N);
    }
    void prev() {
        this->mode = static_cast<T>((mode - 1) % N);
    }
    T mode;
};

template<class T>
class Publisher {
    public:
    void update(T mode, KnobState state) const;
};

template<>
class Publisher<SingleKnobMode> {
    public:
    static void update(SingleKnobMode mode, KnobState state) {
        if (state == KnobState::unchanged) {
            return;
        }
        switch (mode) {
            // TODO
            case SingleKnobMode::alt:
            break;
            case SingleKnobMode::crs:
            break;
            case SingleKnobMode::hdg:
            break;
            case SingleKnobMode::vsp:
            break;
        }
    }
};

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