#include <SFML/Graphics.hpp>

using namespace sf;

inline int keyboardInput() {
	bool shift = false;
	if (Keyboard::isKeyPressed(Keyboard::LShift) || Keyboard::isKeyPressed(Keyboard::RShift))shift = true;

	if (Keyboard::isKeyPressed(Keyboard::A)) {
		if (shift)return 65;
		else return 97;
	}
	if (Keyboard::isKeyPressed(Keyboard::B)) {
		if (shift)return 66;
		else return 98;
	}
	if (Keyboard::isKeyPressed(Keyboard::C)) {
		if (shift)return 67;
		else return 99;
	}
	if (Keyboard::isKeyPressed(Keyboard::D)) {
		if (shift)return 68;
		else return 100;
	}
	if (Keyboard::isKeyPressed(Keyboard::E)) {
		if (shift)return 69;
		else return 101;
	}
	if (Keyboard::isKeyPressed(Keyboard::F)) {
		if (shift)return 70;
		else return 102;
	}
	if (Keyboard::isKeyPressed(Keyboard::G)) {
		if (shift)return 71;
		else return 103;
	}
	if (Keyboard::isKeyPressed(Keyboard::H)) {
		if (shift)return 72;
		else return 104;
	}
	if (Keyboard::isKeyPressed(Keyboard::I)) {
		if (shift)return 73;
		else return 105;
	}
	if (Keyboard::isKeyPressed(Keyboard::J)) {
		if (shift)return 74;
		else return 106;
	}
	if (Keyboard::isKeyPressed(Keyboard::K)) {
		if (shift)return 75;
		else return 107;
	}
	if (Keyboard::isKeyPressed(Keyboard::L)) {
		if (shift)return 76;
		else return 108;
	}
	if (Keyboard::isKeyPressed(Keyboard::M)) {
		if (shift)return 77;
		else return 109;
	}
	if (Keyboard::isKeyPressed(Keyboard::N)) {
		if (shift)return 78;
		else return 110;
	}
	if (Keyboard::isKeyPressed(Keyboard::O)) {
		if (shift)return 79;
		else return 111;
	}
	if (Keyboard::isKeyPressed(Keyboard::P)) {
		if (shift)return 80;
		else return 112;
	}
	if (Keyboard::isKeyPressed(Keyboard::Q)) {
		if (shift)return 81;
		else return 113;
	}
	if (Keyboard::isKeyPressed(Keyboard::R)) {
		if (shift)return 82;
		else return 114;
	}
	if (Keyboard::isKeyPressed(Keyboard::S)) {
		if (shift)return 83;
		else return 115;
	}
	if (Keyboard::isKeyPressed(Keyboard::T)) {
		if (shift)return 84;
		else return 116;
	}
	if (Keyboard::isKeyPressed(Keyboard::U)) {
		if (shift)return 85;
		else return 117;
	}
	if (Keyboard::isKeyPressed(Keyboard::V)) {
		if (shift)return 86;
		else return 118;
	}
	if (Keyboard::isKeyPressed(Keyboard::W)) {
		if (shift)return 87;
		else return 119;
	}
	if (Keyboard::isKeyPressed(Keyboard::X)) {
		if (shift)return 88;
		else return 120;
	}
	if (Keyboard::isKeyPressed(Keyboard::Y)) {
		if (shift)return 89;
		else return 121;
	}
	if (Keyboard::isKeyPressed(Keyboard::Z)) {
		if (shift)return 90;
		else return 122;
	}
	if (Keyboard::isKeyPressed(Keyboard::Num0) || Keyboard::isKeyPressed(Keyboard::Numpad0)) {
		return 48;
	}
	if (Keyboard::isKeyPressed(Keyboard::Num1) || Keyboard::isKeyPressed(Keyboard::Numpad1)) {
		return 49;
	}
	if (Keyboard::isKeyPressed(Keyboard::Num2) || Keyboard::isKeyPressed(Keyboard::Numpad2)) {
		return 50;
	}
	if (Keyboard::isKeyPressed(Keyboard::Num3) || Keyboard::isKeyPressed(Keyboard::Numpad3)) {
		return 51;
	}
	if (Keyboard::isKeyPressed(Keyboard::Num4) || Keyboard::isKeyPressed(Keyboard::Numpad4)) {
		return 52;
	}
	if (Keyboard::isKeyPressed(Keyboard::Num5) || Keyboard::isKeyPressed(Keyboard::Numpad5)) {
		return 53;
	}
	if (Keyboard::isKeyPressed(Keyboard::Num6) || Keyboard::isKeyPressed(Keyboard::Numpad6)) {
		return 54;
	}
	if (Keyboard::isKeyPressed(Keyboard::Num7) || Keyboard::isKeyPressed(Keyboard::Numpad7)) {
		return 55;
	}
	if (Keyboard::isKeyPressed(Keyboard::Num8) || Keyboard::isKeyPressed(Keyboard::Numpad8)) {
		return 56;
	}
	if (Keyboard::isKeyPressed(Keyboard::Num9) || Keyboard::isKeyPressed(Keyboard::Numpad9)) {
		return 57;
	}
	if (Keyboard::isKeyPressed(Keyboard::Space)) {
		return 32;
	}
	if (Keyboard::isKeyPressed(Keyboard::Key::Enter) || Keyboard::isKeyPressed(Keyboard::Enter)) {
		return 2;
	}
	if (Keyboard::isKeyPressed(Keyboard::Slash)) {
		return 47;
	}
	if (Keyboard::isKeyPressed(Keyboard::BackSlash)) {
		return 92;
	}
	if (Keyboard::isKeyPressed(Keyboard::Key::SemiColon) || Keyboard::isKeyPressed(Keyboard::Semicolon)) {
		if (shift) return 58;
		else return 59;
	}
	if (Keyboard::isKeyPressed(Keyboard::Period)) {
		return 46;
	}
	if (Keyboard::isKeyPressed(Keyboard::Comma)) {
		return 44;
	}
	if (Keyboard::isKeyPressed(Keyboard::Key::Add) || Keyboard::isKeyPressed(Keyboard::Add)) {
		return 43;
	}
	if (Keyboard::isKeyPressed(Keyboard::Key::Subtract) || Keyboard::isKeyPressed(Keyboard::Subtract)) {
		return 45;
	}
	if (Keyboard::isKeyPressed(Keyboard::Key::Equal) || Keyboard::isKeyPressed(Keyboard::Equal)) {
		if (shift)return 43;
		else return 95;
	}
	if (Keyboard::isKeyPressed(Keyboard::Key::Hyphen) || Keyboard::isKeyPressed(Keyboard::Hyphen)) {
		if (shift)return 45;
		else return 95;
	}
	if (Keyboard::isKeyPressed(Keyboard::Enter)) {
		return 2;
	}
	if (Keyboard::isKeyPressed(Keyboard::BackSpace)) {
		return 0;
	}
	return 1;
}