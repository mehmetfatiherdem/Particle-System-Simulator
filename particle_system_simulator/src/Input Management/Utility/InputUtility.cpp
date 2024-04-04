#include "InputUtility.h"

unsigned int fromKeyCode(KeyCode keycode)
{
	return static_cast<unsigned int>(keycode);
}

KeyCode toKeyCode(unsigned int code)
{
	switch(KeyCode keyCode = static_cast<KeyCode>(code))
	{
	case KeyCode::KEY_W:
	case KeyCode::KEY_A:
	case KeyCode::KEY_S:
	case KeyCode::KEY_D:
	case KeyCode::KEY_Q:
	case KeyCode::KEY_E:
	case KeyCode::KEY_R:
	case KeyCode::KEY_T:
	case KeyCode::KEY_Y:
	case KeyCode::KEY_U:
	case KeyCode::KEY_F:
	case KeyCode::KEY_Z:
	case KeyCode::KEY_X:
	case KeyCode::KEY_C:
	case KeyCode::KEY_0:
	case KeyCode::KEY_1:
	case KeyCode::KEY_2:
	case KeyCode::KEY_3:
	case KeyCode::KEY_4:
	case KeyCode::KEY_5:
	case KeyCode::KEY_6:
	case KeyCode::KEY_7:
	case KeyCode::KEY_8:
	case KeyCode::KEY_9:
	case KeyCode::ARROW_UP:
	case KeyCode::ARROW_LEFT:
	case KeyCode::ARROW_BACK:
	case KeyCode::ARROW_RIGHT:
	case KeyCode::SHIFT:
	case KeyCode::CTRL:
	case KeyCode::ESCAPE:
	case KeyCode::SPACE:
		return keyCode;
	default:
		return KeyCode::UNDEFINED;
	}
}

std::vector<KeyCode> getAllKeyCodes()
{
	std::vector<KeyCode> keyCodes;
	keyCodes.reserve(32);

	keyCodes.push_back(KeyCode::KEY_W);
	keyCodes.push_back(KeyCode::KEY_A);
	keyCodes.push_back(KeyCode::KEY_S);
	keyCodes.push_back(KeyCode::KEY_D);
	keyCodes.push_back(KeyCode::KEY_Q);
	keyCodes.push_back(KeyCode::KEY_E);
	keyCodes.push_back(KeyCode::KEY_R);
	keyCodes.push_back(KeyCode::KEY_T);
	keyCodes.push_back(KeyCode::KEY_Y);
	keyCodes.push_back(KeyCode::KEY_U);
	keyCodes.push_back(KeyCode::KEY_F);
	keyCodes.push_back(KeyCode::KEY_Z);
	keyCodes.push_back(KeyCode::KEY_X);
	keyCodes.push_back(KeyCode::KEY_C);
	keyCodes.push_back(KeyCode::KEY_0);
	keyCodes.push_back(KeyCode::KEY_1);
	keyCodes.push_back(KeyCode::KEY_2);
	keyCodes.push_back(KeyCode::KEY_3);
	keyCodes.push_back(KeyCode::KEY_4);
	keyCodes.push_back(KeyCode::KEY_5);
	keyCodes.push_back(KeyCode::KEY_6);
	keyCodes.push_back(KeyCode::KEY_7);
	keyCodes.push_back(KeyCode::KEY_8);
	keyCodes.push_back(KeyCode::KEY_9);
	keyCodes.push_back(KeyCode::ARROW_UP);
	keyCodes.push_back(KeyCode::ARROW_LEFT);
	keyCodes.push_back(KeyCode::ARROW_BACK);
	keyCodes.push_back(KeyCode::ARROW_RIGHT);
	keyCodes.push_back(KeyCode::SHIFT);
	keyCodes.push_back(KeyCode::CTRL);
	keyCodes.push_back(KeyCode::ESCAPE);
	keyCodes.push_back(KeyCode::SPACE);
	return keyCodes;
}

unsigned int fromMouseButton(MouseButton button)
{
	return static_cast<unsigned int>(button);
}

MouseButton toMouseButton(unsigned int button)
{
	switch(auto mouseButton = static_cast<MouseButton>(button))
	{
	case MouseButton::MOUSE_LEFT:
	case MouseButton::MOUSE_RIGHT:
	case MouseButton::MOUSE_MIDDLE:
		return mouseButton;
	default:
		return MouseButton::UNDEFINED;
	}
}

std::vector<MouseButton> getAllMouseButtons()
{
	std::vector<MouseButton> mouseButtons;
	mouseButtons.reserve(3);

	mouseButtons.push_back(MouseButton::MOUSE_LEFT);
	mouseButtons.push_back(MouseButton::MOUSE_RIGHT);
	mouseButtons.push_back(MouseButton::MOUSE_MIDDLE);
	return mouseButtons;
}