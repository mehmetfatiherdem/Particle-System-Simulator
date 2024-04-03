#pragma once

#include <vector>
#include "Input Management/Data/KeyCode.h"
#include "Input Management/Data/MouseButton.h"

constexpr unsigned int fromKeyCode(KeyCode keycode);
constexpr KeyCode toKeyCode(unsigned int code);
std::vector<KeyCode> getAllKeyCodes();

constexpr unsigned int fromMouseButton(MouseButton button);
constexpr MouseButton toMouseButton(unsigned int button);
std::vector<MouseButton> getAllMouseButtons();
