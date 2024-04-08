#pragma once

#include <vector>
#include "Input Management/Data/KeyCode.h"
#include "Input Management/Data/MouseButton.h"

uint32_t fromKeyCode(KeyCode keycode);
KeyCode toKeyCode(uint32_t code);
std::vector<KeyCode> getAllKeyCodes();

uint32_t fromMouseButton(MouseButton button);
MouseButton toMouseButton(uint32_t button);
std::vector<MouseButton> getAllMouseButtons();
