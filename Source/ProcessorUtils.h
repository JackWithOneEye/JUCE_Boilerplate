#pragma once

constexpr float SMOOTHED_VAL_RAMP_LEN_SEC = 0.1f;

using SmoothedValL = SmoothedValue<float, ValueSmoothingTypes::Linear>;
using SmoothedValM = SmoothedValue<float, ValueSmoothingTypes::Multiplicative>;
