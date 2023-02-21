#pragma once

namespace Atom
{

	using MouseButtonCode = uint16_t;

	namespace MouseButton
	{
		
		enum : MouseButtonCode
		{
			Button0 = 0,
			Button1 = 1,
			Button2 = 2,
			Button3 = 3,
			Button4 = 4,
			Button5 = 5,
			
			Left = Button0,
			Right = Button1,
			Middle = Button2
		};

	}

	//inline std::ostream& operator<<(std::ostream& os, MouseButtonCode button)
	//{
	//	os << static_cast<int32_t>(button);
	//	return os;
	//}
	
}

#if 0
// Mouse
#define AT_MOUSE_BUTTON_LEFT    ::Atom::Button::Left
#define AT_MOUSE_BUTTON_RIGHT   ::Atom::Button::Right
#define AT_MOUSE_BUTTON_MIDDLE  ::Atom::Button::Middle
#endif
