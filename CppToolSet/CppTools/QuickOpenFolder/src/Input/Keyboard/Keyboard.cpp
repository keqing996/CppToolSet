#include "Keyboard.h"
#include "Util/Util.hpp"

namespace Input
{

#pragma region [Event]

    Keyboard::Event::Event()
            : _type(Type::Invalid), _code(0)
    {
    }

    Keyboard::Event::Event(Type type, unsigned char code)
            : _type(type), _code(code)
    {
    }

    bool Keyboard::Event::IsPressed() const
    {
        return _type == Type::Press;
    }

    bool Keyboard::Event::IsReleased() const
    {
        return _type == Type::Release;
    }

    bool Keyboard::Event::IsValid() const
    {
        return _type != Type::Invalid;
    }

#pragma endregion

#pragma region [Accessor]

    Keyboard::Accessor::Accessor(Keyboard* pKeyboard)
    {
        _pKeyboard = pKeyboard;
    }

    Keyboard::Event Keyboard::Accessor::ReadKey()
    {
        if (!_pKeyboard->_keyBuffer.empty())
        {
            auto keyboardEvent = _pKeyboard->_keyBuffer.front();
            _pKeyboard->_keyBuffer.pop();
            return keyboardEvent;
        }
        else
        {
            return {};
        }
    }

    bool Keyboard::Accessor::IsKeyPressed(unsigned char keycode) const
    {
        return _pKeyboard->_keyStateSet[keycode];
    }

    bool Keyboard::Accessor::IsKeyEmpty() const
    {
        return _pKeyboard->_keyBuffer.empty();
    }

    void Keyboard::Accessor::ClearKey()
    {
        while (!_pKeyboard->_keyBuffer.empty())
        {
            _pKeyboard->_keyBuffer.pop();
        }
    }

    wchar_t Keyboard::Accessor::ReadCharW()
    {
        if (!_pKeyboard->_charBuffer.empty())
        {
            wchar_t top = _pKeyboard->_charBuffer.front();
            _pKeyboard->_charBuffer.pop();
            return top;
        }

        return 0;
    }

    bool Keyboard::Accessor::IsCharEmpty() const
    {
        return _pKeyboard->_charBuffer.empty();
    }

    void Keyboard::Accessor::ClearChar()
    {
        while (!_pKeyboard->_charBuffer.empty())
        {
            _pKeyboard->_charBuffer.pop();
        }
    }

    void Keyboard::Accessor::Clear()
    {
        ClearKey();
        ClearChar();
    }

    void Keyboard::Accessor::EnableAutoRepeat()
    {
        _pKeyboard->_autoRepeat = true;
    }

    void Keyboard::Accessor::DisableAutoRepeat()
    {
        _pKeyboard->_autoRepeat = false;
    }

    bool Keyboard::Accessor::IsAutoRepeatEnabled() const
    {
        return _pKeyboard->_autoRepeat;
    }

#pragma endregion

    Keyboard::Accessor Keyboard::GetAccessor()
    {
        return Keyboard::Accessor{ this };
    }

    void Keyboard::OnKeyPressed(unsigned char keycode)
    {
        _keyStateSet[keycode] = true;
        _keyBuffer.emplace(Event::Type::Press, keycode);
        Util::TrimQueue(_keyBuffer, QUEUE_SIZE);
    }

    void Keyboard::OnKeyReleased(unsigned char keycode)
    {
        _keyStateSet[keycode] = false;
        _keyBuffer.emplace(Event::Type::Release, keycode);
        Util::TrimQueue(_keyBuffer, QUEUE_SIZE);
    }

    void Keyboard::OnCharW(wchar_t c)
    {
        _charBuffer.push(c);
        Util::TrimQueue(_charBuffer, QUEUE_SIZE);
    }

    void Keyboard::ClearState()
    {
        _keyStateSet.reset();
    }


}