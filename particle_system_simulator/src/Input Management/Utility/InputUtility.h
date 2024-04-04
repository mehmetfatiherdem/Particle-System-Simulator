#pragma once

#include <vector>
#include "Input Management/Data/KeyCode.h"
#include "Input Management/Data/MouseButton.h"

unsigned int fromKeyCode(KeyCode keycode);
KeyCode toKeyCode(unsigned int code);
std::vector<KeyCode> getAllKeyCodes();

unsigned int fromMouseButton(MouseButton button);
MouseButton toMouseButton(unsigned int button);
std::vector<MouseButton> getAllMouseButtons();
