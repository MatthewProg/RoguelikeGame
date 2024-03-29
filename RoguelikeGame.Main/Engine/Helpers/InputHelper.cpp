#include "InputHelper.h"

std::string InputHelper::DecodeKey(sf::Keyboard::Key key)
{
	switch (key)
	{
	case sf::Keyboard::Unknown: return "Unknown";
	case sf::Keyboard::A: return "A";
	case sf::Keyboard::B: return "B";
	case sf::Keyboard::C: return "C";
	case sf::Keyboard::D: return "D";
	case sf::Keyboard::E: return "E";
	case sf::Keyboard::F: return "F";
	case sf::Keyboard::G: return "G";
	case sf::Keyboard::H: return "H";
	case sf::Keyboard::I: return "I";
	case sf::Keyboard::J: return "J";
	case sf::Keyboard::K: return "K";
	case sf::Keyboard::L: return "L";
	case sf::Keyboard::M: return "M";
	case sf::Keyboard::N: return "N";
	case sf::Keyboard::O: return "O";
	case sf::Keyboard::P: return "P";
	case sf::Keyboard::Q: return "Q";
	case sf::Keyboard::R: return "R";
	case sf::Keyboard::S: return "S";
	case sf::Keyboard::T: return "T";
	case sf::Keyboard::U: return "U";
	case sf::Keyboard::V: return "V";
	case sf::Keyboard::W: return "W";
	case sf::Keyboard::X: return "X";
	case sf::Keyboard::Y: return "Y";
	case sf::Keyboard::Z: return "Z";
	case sf::Keyboard::Num0: return "0";
	case sf::Keyboard::Num1: return "1";
	case sf::Keyboard::Num2: return "2";
	case sf::Keyboard::Num3: return "3";
	case sf::Keyboard::Num4: return "4";
	case sf::Keyboard::Num5: return "5";
	case sf::Keyboard::Num6: return "6";
	case sf::Keyboard::Num7: return "7";
	case sf::Keyboard::Num8: return "8";
	case sf::Keyboard::Num9: return "9";
	case sf::Keyboard::Escape: return "Esc";
	case sf::Keyboard::LControl: return "Left Ctrl";
	case sf::Keyboard::LShift: return "Left Shift";
	case sf::Keyboard::LAlt: return "Left Alt";
	case sf::Keyboard::LSystem: return "Left System";
	case sf::Keyboard::RControl: return "Right Ctrl";
	case sf::Keyboard::RShift: return "Right Shift";
	case sf::Keyboard::RAlt: return "Right Alt";
	case sf::Keyboard::RSystem: return "Right System";
	case sf::Keyboard::Menu: return "Menu";
	case sf::Keyboard::LBracket: return "[";
	case sf::Keyboard::RBracket: return "]";
	case sf::Keyboard::Semicolon: return ";";
	case sf::Keyboard::Comma: return ",";
	case sf::Keyboard::Period: return ".";
	case sf::Keyboard::Quote: return "'";
	case sf::Keyboard::Slash: return "/";
	case sf::Keyboard::Backslash: return "\\";
	case sf::Keyboard::Tilde: return "`";
	case sf::Keyboard::Equal: return "=";
	case sf::Keyboard::Dash: return "-";
	case sf::Keyboard::Space: return "Space";
	case sf::Keyboard::Enter: return "Enter";
	case sf::Keyboard::Backspace: return "Backspace";
	case sf::Keyboard::Tab: return "Tab";
	case sf::Keyboard::PageUp: return "Page Up";
	case sf::Keyboard::PageDown: return "Page Down";
	case sf::Keyboard::End: return "End";
	case sf::Keyboard::Home: return "Home";
	case sf::Keyboard::Insert: return "Insert";
	case sf::Keyboard::Delete: return "Delete";
	case sf::Keyboard::Add: return "NumPad +";
	case sf::Keyboard::Subtract: return "NumPad -";
	case sf::Keyboard::Multiply: return "NumPad *";
	case sf::Keyboard::Divide: return "NumPad /";
	case sf::Keyboard::Left: return "Left";
	case sf::Keyboard::Right: return "Right";
	case sf::Keyboard::Up: return "Up";
	case sf::Keyboard::Down: return "Down";
	case sf::Keyboard::Numpad0: return "NumPad 0";
	case sf::Keyboard::Numpad1: return "NumPad 1";
	case sf::Keyboard::Numpad2: return "NumPad 2";
	case sf::Keyboard::Numpad3: return "NumPad 3";
	case sf::Keyboard::Numpad4: return "NumPad 4";
	case sf::Keyboard::Numpad5: return "NumPad 5";
	case sf::Keyboard::Numpad6: return "NumPad 6";
	case sf::Keyboard::Numpad7: return "NumPad 7";
	case sf::Keyboard::Numpad8: return "NumPad 8";
	case sf::Keyboard::Numpad9: return "NumPad 9";
	case sf::Keyboard::F1: return "F1";
	case sf::Keyboard::F2: return "F2";
	case sf::Keyboard::F3: return "F3";
	case sf::Keyboard::F4: return "F4";
	case sf::Keyboard::F5: return "F5";
	case sf::Keyboard::F6: return "F6";
	case sf::Keyboard::F7: return "F7";
	case sf::Keyboard::F8: return "F8";
	case sf::Keyboard::F9: return "F9";
	case sf::Keyboard::F10: return "F10";
	case sf::Keyboard::F11: return "F11";
	case sf::Keyboard::F12: return "F12";
	case sf::Keyboard::F13: return "F13";
	case sf::Keyboard::F14: return "F14";
	case sf::Keyboard::F15: return "F15";
	case sf::Keyboard::Pause: return "Pause";
	case sf::Keyboard::KeyCount: return "Key Count";
	default: return std::string();
	}
}

std::string InputHelper::DecodeButton(sf::Mouse::Button button)
{
	switch (button)
	{
	case sf::Mouse::Left: return "LMB";
	case sf::Mouse::Right: return "RMB";
	case sf::Mouse::Middle: return "Mouse 3";
	case sf::Mouse::XButton1: return "Mouse 4";
	case sf::Mouse::XButton2: return "Mouse 5";
	case sf::Mouse::ButtonCount: return "Button Count";
	default: return std::string();
	}
}

sf::Keyboard::Key InputHelper::EncodeKey(const std::string& key)
{
	if (key == "A") return sf::Keyboard::A;
	else if (key == "B") return sf::Keyboard::B;
	else if (key == "C") return sf::Keyboard::C;
	else if (key == "D") return sf::Keyboard::D;
	else if (key == "E") return sf::Keyboard::E;
	else if (key == "F") return sf::Keyboard::F;
	else if (key == "G") return sf::Keyboard::G;
	else if (key == "H") return sf::Keyboard::H;
	else if (key == "I") return sf::Keyboard::I;
	else if (key == "J") return sf::Keyboard::J;
	else if (key == "K") return sf::Keyboard::K;
	else if (key == "L") return sf::Keyboard::L;
	else if (key == "M") return sf::Keyboard::M;
	else if (key == "N") return sf::Keyboard::N;
	else if (key == "O") return sf::Keyboard::O;
	else if (key == "P") return sf::Keyboard::P;
	else if (key == "Q") return sf::Keyboard::Q;
	else if (key == "R") return sf::Keyboard::R;
	else if (key == "S") return sf::Keyboard::S;
	else if (key == "T") return sf::Keyboard::T;
	else if (key == "U") return sf::Keyboard::U;
	else if (key == "V") return sf::Keyboard::V;
	else if (key == "W") return sf::Keyboard::W;
	else if (key == "X") return sf::Keyboard::X;
	else if (key == "Y") return sf::Keyboard::Y;
	else if (key == "Z") return sf::Keyboard::Z;
	else if (key == "0") return sf::Keyboard::Num0;
	else if (key == "1") return sf::Keyboard::Num1;
	else if (key == "2") return sf::Keyboard::Num2;
	else if (key == "3") return sf::Keyboard::Num3;
	else if (key == "4") return sf::Keyboard::Num4;
	else if (key == "5") return sf::Keyboard::Num5;
	else if (key == "6") return sf::Keyboard::Num6;
	else if (key == "7") return sf::Keyboard::Num7;
	else if (key == "8") return sf::Keyboard::Num8;
	else if (key == "9") return sf::Keyboard::Num9;
	else if (key == "Esc") return sf::Keyboard::Escape;
	else if (key == "Left Ctrl") return sf::Keyboard::LControl;
	else if (key == "Left Shift") return sf::Keyboard::LShift;
	else if (key == "Left Alt") return sf::Keyboard::LAlt;
	else if (key == "Left System") return sf::Keyboard::LSystem;
	else if (key == "Right Ctrl") return sf::Keyboard::RControl;
	else if (key == "Right Shift") return sf::Keyboard::RShift;
	else if (key == "Right Alt") return sf::Keyboard::RAlt;
	else if (key == "Right System") return sf::Keyboard::RSystem;
	else if (key == "Menu") return sf::Keyboard::Menu;
	else if (key == "[") return sf::Keyboard::LBracket;
	else if (key == "]") return sf::Keyboard::RBracket;
	else if (key == ";") return sf::Keyboard::Semicolon;
	else if (key == ",") return sf::Keyboard::Comma;
	else if (key == ".") return sf::Keyboard::Period;
	else if (key == "'") return sf::Keyboard::Quote;
	else if (key == "/") return sf::Keyboard::Slash;
	else if (key == "\\") return sf::Keyboard::Backslash;
	else if (key == "`") return sf::Keyboard::Tilde;
	else if (key == "=") return sf::Keyboard::Equal;
	else if (key == "-") return sf::Keyboard::Dash;
	else if (key == "Space") return sf::Keyboard::Space;
	else if (key == "Enter") return sf::Keyboard::Enter;
	else if (key == "Backspace") return sf::Keyboard::Backspace;
	else if (key == "Tab") return sf::Keyboard::Tab;
	else if (key == "Page Up") return sf::Keyboard::PageUp;
	else if (key == "Page Down") return sf::Keyboard::PageDown;
	else if (key == "End") return sf::Keyboard::End;
	else if (key == "Home") return sf::Keyboard::Home;
	else if (key == "Insert") return sf::Keyboard::Insert;
	else if (key == "Delete") return sf::Keyboard::Delete;
	else if (key == "NumPad +") return sf::Keyboard::Add;
	else if (key == "NumPad -") return sf::Keyboard::Subtract;
	else if (key == "NumPad *") return sf::Keyboard::Multiply;
	else if (key == "NumPad /") return sf::Keyboard::Divide;
	else if (key == "Left") return sf::Keyboard::Left;
	else if (key == "Right") return sf::Keyboard::Right;
	else if (key == "Up") return sf::Keyboard::Up;
	else if (key == "Down") return sf::Keyboard::Down;
	else if (key == "NumPad 0") return sf::Keyboard::Numpad0;
	else if (key == "NumPad 1") return sf::Keyboard::Numpad1;
	else if (key == "NumPad 2") return sf::Keyboard::Numpad2;
	else if (key == "NumPad 3") return sf::Keyboard::Numpad3;
	else if (key == "NumPad 4") return sf::Keyboard::Numpad4;
	else if (key == "NumPad 5") return sf::Keyboard::Numpad5;
	else if (key == "NumPad 6") return sf::Keyboard::Numpad6;
	else if (key == "NumPad 7") return sf::Keyboard::Numpad7;
	else if (key == "NumPad 8") return sf::Keyboard::Numpad8;
	else if (key == "NumPad 9") return sf::Keyboard::Numpad9;
	else if (key == "F1") return sf::Keyboard::F1;
	else if (key == "F2") return sf::Keyboard::F2;
	else if (key == "F3") return sf::Keyboard::F3;
	else if (key == "F4") return sf::Keyboard::F4;
	else if (key == "F5") return sf::Keyboard::F5;
	else if (key == "F6") return sf::Keyboard::F6;
	else if (key == "F7") return sf::Keyboard::F7;
	else if (key == "F8") return sf::Keyboard::F8;
	else if (key == "F9") return sf::Keyboard::F9;
	else if (key == "F10") return sf::Keyboard::F10;
	else if (key == "F11") return sf::Keyboard::F11;
	else if (key == "F12") return sf::Keyboard::F12;
	else if (key == "F13") return sf::Keyboard::F13;
	else if (key == "F14") return sf::Keyboard::F14;
	else if (key == "F15") return sf::Keyboard::F15;
	else if (key == "Pause") return sf::Keyboard::Pause;
	else if (key == "Key Count") return sf::Keyboard::KeyCount;
	else return sf::Keyboard::Unknown;
}

sf::Mouse::Button InputHelper::EncodeButton(const std::string& button)
{
	if (button == "LMB") return sf::Mouse::Left;
	else if (button == "RMB") return sf::Mouse::Right;
	else if (button == "Mouse 3") return sf::Mouse::Middle;
	else if (button == "Mouse 4") return sf::Mouse::XButton1;
	else if (button == "Mouse 5") return sf::Mouse::XButton2;
	else if (button == "Button Count") return sf::Mouse::ButtonCount;
	
	return sf::Mouse::Left;
}
