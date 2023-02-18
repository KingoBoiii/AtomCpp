#pragma once

namespace Atom
{
	using KeyCode = uint16_t;

	namespace Key
	{
		enum : KeyCode
		{
			// From glfw3.h
			Space = 32,
			Apostrophe = 39, /* ' */
			Comma = 44, /* , */
			Minus = 45, /* - */
			Period = 46, /* . */
			Slash = 47, /* / */

			D0 = 48, /* 0 */
			D1 = 49, /* 1 */
			D2 = 50, /* 2 */
			D3 = 51, /* 3 */
			D4 = 52, /* 4 */
			D5 = 53, /* 5 */
			D6 = 54, /* 6 */
			D7 = 55, /* 7 */
			D8 = 56, /* 8 */
			D9 = 57, /* 9 */

			Semicolon = 59, /* ; */
			Equal = 61, /* = */

			A = 65,
			B = 66,
			C = 67,
			D = 68,
			E = 69,
			F = 70,
			G = 71,
			H = 72,
			I = 73,
			J = 74,
			K = 75,
			L = 76,
			M = 77,
			N = 78,
			O = 79,
			P = 80,
			Q = 81,
			R = 82,
			S = 83,
			T = 84,
			U = 85,
			V = 86,
			W = 87,
			X = 88,
			Y = 89,
			Z = 90,

			LeftBracket = 91,  /* [ */
			Backslash = 92,  /* \ */
			RightBracket = 93,  /* ] */
			GraveAccent = 96,  /* ` */

			World1 = 161, /* non-US #1 */
			World2 = 162, /* non-US #2 */

			/* Function keys */
			Escape = 256,
			Enter = 257,
			Tab = 258,
			Backspace = 259,
			Insert = 260,
			Delete = 261,
			Right = 262,
			Left = 263,
			Down = 264,
			Up = 265,
			PageUp = 266,
			PageDown = 267,
			Home = 268,
			End = 269,
			CapsLock = 280,
			ScrollLock = 281,
			NumLock = 282,
			PrintScreen = 283,
			Pause = 284,
			F1 = 290,
			F2 = 291,
			F3 = 292,
			F4 = 293,
			F5 = 294,
			F6 = 295,
			F7 = 296,
			F8 = 297,
			F9 = 298,
			F10 = 299,
			F11 = 300,
			F12 = 301,
			F13 = 302,
			F14 = 303,
			F15 = 304,
			F16 = 305,
			F17 = 306,
			F18 = 307,
			F19 = 308,
			F20 = 309,
			F21 = 310,
			F22 = 311,
			F23 = 312,
			F24 = 313,
			F25 = 314,

			/* Keypad */
			KP0 = 320,
			KP1 = 321,
			KP2 = 322,
			KP3 = 323,
			KP4 = 324,
			KP5 = 325,
			KP6 = 326,
			KP7 = 327,
			KP8 = 328,
			KP9 = 329,
			KPDecimal = 330,
			KPDivide = 331,
			KPMultiply = 332,
			KPSubtract = 333,
			KPAdd = 334,
			KPEnter = 335,
			KPEqual = 336,

			LeftShift = 340,
			LeftControl = 341,
			LeftAlt = 342,
			LeftSuper = 343,
			RightShift = 344,
			RightControl = 345,
			RightAlt = 346,
			RightSuper = 347,
			Menu = 348
		};
	}

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

#if 0
// From glfw3.h
#define AT_KEY_SPACE           ::Atom::Key::Space
#define AT_KEY_APOSTROPHE      ::Atom::Key::Apostrophe    /* ' */
#define AT_KEY_COMMA           ::Atom::Key::Comma         /* , */
#define AT_KEY_MINUS           ::Atom::Key::Minus         /* - */
#define AT_KEY_PERIOD          ::Atom::Key::Period        /* . */
#define AT_KEY_SLASH           ::Atom::Key::Slash         /* / */
#define AT_KEY_0               ::Atom::Key::D0
#define AT_KEY_1               ::Atom::Key::D1
#define AT_KEY_2               ::Atom::Key::D2
#define AT_KEY_3               ::Atom::Key::D3
#define AT_KEY_4               ::Atom::Key::D4
#define AT_KEY_5               ::Atom::Key::D5
#define AT_KEY_6               ::Atom::Key::D6
#define AT_KEY_7               ::Atom::Key::D7
#define AT_KEY_8               ::Atom::Key::D8
#define AT_KEY_9               ::Atom::Key::D9
#define AT_KEY_SEMICOLON       ::Atom::Key::Semicolon     /* ; */
#define AT_KEY_EQUAL           ::Atom::Key::Equal         /* = */
#define AT_KEY_A               ::Atom::Key::A
#define AT_KEY_B               ::Atom::Key::B
#define AT_KEY_C               ::Atom::Key::C
#define AT_KEY_D               ::Atom::Key::D
#define AT_KEY_E               ::Atom::Key::E
#define AT_KEY_F               ::Atom::Key::F
#define AT_KEY_G               ::Atom::Key::G
#define AT_KEY_H               ::Atom::Key::H
#define AT_KEY_I               ::Atom::Key::I
#define AT_KEY_J               ::Atom::Key::J
#define AT_KEY_K               ::Atom::Key::K
#define AT_KEY_L               ::Atom::Key::L
#define AT_KEY_M               ::Atom::Key::M
#define AT_KEY_N               ::Atom::Key::N
#define AT_KEY_O               ::Atom::Key::O
#define AT_KEY_P               ::Atom::Key::P
#define AT_KEY_Q               ::Atom::Key::Q
#define AT_KEY_R               ::Atom::Key::R
#define AT_KEY_S               ::Atom::Key::S
#define AT_KEY_T               ::Atom::Key::T
#define AT_KEY_U               ::Atom::Key::U
#define AT_KEY_V               ::Atom::Key::V
#define AT_KEY_W               ::Atom::Key::W
#define AT_KEY_X               ::Atom::Key::X
#define AT_KEY_Y               ::Atom::Key::Y
#define AT_KEY_Z               ::Atom::Key::Z
#define AT_KEY_LEFT_BRACKET    ::Atom::Key::LeftBracket   /* [ */
#define AT_KEY_BACKSLASH       ::Atom::Key::Backslash     /* \ */
#define AT_KEY_RIGHT_BRACKET   ::Atom::Key::RightBracket  /* ] */
#define AT_KEY_GRAVE_ACCENT    ::Atom::Key::GraveAccent   /* ` */
#define AT_KEY_WORLD_1         ::Atom::Key::World1        /* non-US #1 */
#define AT_KEY_WORLD_2         ::Atom::Key::World2        /* non-US #2 */

/* Function keys */
#define AT_KEY_ESCAPE          ::Atom::Key::Escape
#define AT_KEY_ENTER           ::Atom::Key::Enter
#define AT_KEY_TAB             ::Atom::Key::Tab
#define AT_KEY_BACKSPACE       ::Atom::Key::Backspace
#define AT_KEY_INSERT          ::Atom::Key::Insert
#define AT_KEY_DELETE          ::Atom::Key::Delete
#define AT_KEY_RIGHT           ::Atom::Key::Right
#define AT_KEY_LEFT            ::Atom::Key::Left
#define AT_KEY_DOWN            ::Atom::Key::Down
#define AT_KEY_UP              ::Atom::Key::Up
#define AT_KEY_PAGE_UP         ::Atom::Key::PageUp
#define AT_KEY_PAGE_DOWN       ::Atom::Key::PageDown
#define AT_KEY_HOME            ::Atom::Key::Home
#define AT_KEY_END             ::Atom::Key::End
#define AT_KEY_CAPS_LOCK       ::Atom::Key::CapsLock
#define AT_KEY_SCROLL_LOCK     ::Atom::Key::ScrollLock
#define AT_KEY_NUM_LOCK        ::Atom::Key::NumLock
#define AT_KEY_PRINT_SCREEN    ::Atom::Key::PrintScreen
#define AT_KEY_PAUSE           ::Atom::Key::Pause
#define AT_KEY_F1              ::Atom::Key::F1
#define AT_KEY_F2              ::Atom::Key::F2
#define AT_KEY_F3              ::Atom::Key::F3
#define AT_KEY_F4              ::Atom::Key::F4
#define AT_KEY_F5              ::Atom::Key::F5
#define AT_KEY_F6              ::Atom::Key::F6
#define AT_KEY_F7              ::Atom::Key::F7
#define AT_KEY_F8              ::Atom::Key::F8
#define AT_KEY_F9              ::Atom::Key::F9
#define AT_KEY_F10             ::Atom::Key::F10
#define AT_KEY_F11             ::Atom::Key::F11
#define AT_KEY_F12             ::Atom::Key::F12
#define AT_KEY_F13             ::Atom::Key::F13
#define AT_KEY_F14             ::Atom::Key::F14
#define AT_KEY_F15             ::Atom::Key::F15
#define AT_KEY_F16             ::Atom::Key::F16
#define AT_KEY_F17             ::Atom::Key::F17
#define AT_KEY_F18             ::Atom::Key::F18
#define AT_KEY_F19             ::Atom::Key::F19
#define AT_KEY_F20             ::Atom::Key::F20
#define AT_KEY_F21             ::Atom::Key::F21
#define AT_KEY_F22             ::Atom::Key::F22
#define AT_KEY_F23             ::Atom::Key::F23
#define AT_KEY_F24             ::Atom::Key::F24
#define AT_KEY_F25             ::Atom::Key::F25

/* Keypad */
#define AT_KEY_KP_0            ::Atom::Key::KP0
#define AT_KEY_KP_1            ::Atom::Key::KP1
#define AT_KEY_KP_2            ::Atom::Key::KP2
#define AT_KEY_KP_3            ::Atom::Key::KP3
#define AT_KEY_KP_4            ::Atom::Key::KP4
#define AT_KEY_KP_5            ::Atom::Key::KP5
#define AT_KEY_KP_6            ::Atom::Key::KP6
#define AT_KEY_KP_7            ::Atom::Key::KP7
#define AT_KEY_KP_8            ::Atom::Key::KP8
#define AT_KEY_KP_9            ::Atom::Key::KP9
#define AT_KEY_KP_DECIMAL      ::Atom::Key::KPDecimal
#define AT_KEY_KP_DIVIDE       ::Atom::Key::KPDivide
#define AT_KEY_KP_MULTIPLY     ::Atom::Key::KPMultiply
#define AT_KEY_KP_SUBTRACT     ::Atom::Key::KPSubtract
#define AT_KEY_KP_ADD          ::Atom::Key::KPAdd
#define AT_KEY_KP_ENTER        ::Atom::Key::KPEnter
#define AT_KEY_KP_EQUAL        ::Atom::Key::KPEqual

#define AT_KEY_LEFT_SHIFT      ::Atom::Key::LeftShift
#define AT_KEY_LEFT_CONTROL    ::Atom::Key::LeftControl
#define AT_KEY_LEFT_ALT        ::Atom::Key::LeftAlt
#define AT_KEY_LEFT_SUPER      ::Atom::Key::LeftSuper
#define AT_KEY_RIGHT_SHIFT     ::Atom::Key::RightShift
#define AT_KEY_RIGHT_CONTROL   ::Atom::Key::RightControl
#define AT_KEY_RIGHT_ALT       ::Atom::Key::RightAlt
#define AT_KEY_RIGHT_SUPER     ::Atom::Key::RightSuper
#define AT_KEY_MENU            ::Atom::Key::Menu
#endif