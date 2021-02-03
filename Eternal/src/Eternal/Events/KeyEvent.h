#pragma once

#include "Event.h"

namespace Eternal {

	class ETERNAL_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(int keyCode)
			: m_KeyCode(keyCode) {}

		int m_KeyCode;
	};

	class ETERNAL_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keyCode, int repeatCount)
			: KeyEvent(keyCode), m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream sstring;
			sstring << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return sstring.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int m_RepeatCount;
	};

	class ETERNAL_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode)
			: KeyEvent(keyCode) {}

		std::string ToString() const override
		{
			std::stringstream sstring;
			sstring << "KeyReleasedEvent: " << m_KeyCode;
			return sstring.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class ETERNAL_API KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keyCode)
			: KeyEvent(keyCode) {}

		std::string ToString() const override
		{
			std::stringstream sstring;
			sstring << "KeyTypedEvent: " << m_KeyCode;
			return sstring.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}