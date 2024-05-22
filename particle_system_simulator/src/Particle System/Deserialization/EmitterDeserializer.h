#pragma once

#include <memory>
#include "Particle System/Emitter/Emitter.h"

class Deserializer;

std::unique_ptr<Emitter> deserializeEmitter(const Deserializer& deserializer);
