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

    Keyboard::Event Keyboard::ReadKey()
    {
        if (!_keyBuffer.empty())
        {
            auto keyboardEvent = _keyBuffer.front();
            _keyBuffer.pop();
            return keyboardEvent;
        }
        else
        {
            return {};
        }
    }

    bool Keyboard::IsKeyPressed(unsigned char keycode) const
    {
        return _keyStateSet[keycode];
    }

    bool Keyboard::IsKeyEmpty() const
    {
        return _keyBuffer.empty();
    }

    void Keyboard::ClearKey()
    {
        while (!_keyBuffer.empty())
        {
            _keyBuffer.pop();
        }
    }

    wchar_t Keyboard::ReadCharW()
    {
        if (!_charBuffer.empty())
        {
            wchar_t top = _charBuffer.front();
            _charBuffer.pop();
            return top;
        }

        return 0;
    }

    bool Keyboard::IsCharEmpty() const
    {
        return _charBuffer.empty();
    }

    void Keyboard::ClearChar()
    {
        while (!_charBuffer.empty())
        {
            _charBuffer.pop();
        }
    }

    void Keyboard::Clear()
    {
        ClearKey();
        ClearChar();
    }

    void Keyboard::EnableAutoRepeat()
    {
        _autoRepeat = true;
    }

    void Keyboard::DisableAutoRepeat()
    {
        _autoRepeat = false;
    }

    bool Keyboard::IsAutoRepeatEnabled() const
    {
        return _autoRepeat;
    }

#pragma endregion

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