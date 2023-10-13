
#include <Windows.h>
#include "Mouse.h"
#include "Util/Util.hpp"

namespace Input
{

#pragma region [Event]

    Mouse::Event::Event()
            : _type(Type::Invalid), _x(0), _y(0)
    {
    }

    Mouse::Event::Event(Type t, int x, int y)
            : _type(t), _x(x), _y(y)
    {
    }

    bool Mouse::Event::IsValid() const
    {
        return _type != Type::Invalid;
    }

    Mouse::Event::Type Mouse::Event::GetType() const
    {
        return _type;
    }

    std::pair<int, int> Mouse::Event::GetPosition() const
    {
        return {_x, _y};
    }

    int Mouse::Event::GetPositionX() const
    {
        return _x;
    }

    int Mouse::Event::GetPositionY() const
    {
        return _y;
    }

#pragma endregion

#pragma region [Accessor]

    Mouse::Accessor::Accessor(Mouse* pMouse)
    {
        _pMouse = pMouse;
    }

    std::pair<int, int> Mouse::Accessor::GetPosition() const
    {
        return {_pMouse->_x, _pMouse->_y};
    }

    int Mouse::Accessor::GetPositionX() const
    {
        return _pMouse->_x;
    }

    int Mouse::Accessor::GetPositionY() const
    {
        return _pMouse->_y;
    }

    bool Mouse::Accessor::IsLeftPressed() const
    {
        return _pMouse->_leftPressed;
    }

    bool Mouse::Accessor::IsMiddlePressed() const
    {
        return _pMouse->_middlePressed;
    }

    bool Mouse::Accessor::IsRightPressed() const
    {
        return _pMouse->_rightPressed;
    }

    bool Mouse::Accessor::IsInWindow() const
    {
        return _pMouse->_isMouseInWindow;
    }

    Mouse::Event Mouse::Accessor::RaiseEvent()
    {
        if (!_pMouse->_buffer.empty())
        {
            auto evt = _pMouse->_buffer.front();
            _pMouse->_buffer.pop();
            return evt;
        }
        else
        {
            return {};
        }
    }

    bool Mouse::Accessor::IsEmpty() const
    {
        return _pMouse->_buffer.empty();
    }

    void Mouse::Accessor::Clear()
    {
        while (!_pMouse->_buffer.empty())
            _pMouse->_buffer.pop();
    }

#pragma endregion

    Mouse::Accessor Mouse::GetAccessor()
    {
        return Mouse::Accessor{ this };
    }

    void Mouse::OnMouseMove(int x, int y)
    {
        _x = x;
        _y = y;

        _buffer.emplace(Event::Type::Move, x, y);
        Util::TrimQueue(_buffer, QUEUE_SIZE);
    }

    void Mouse::OnLeftMousePressed(int x, int y)
    {
        _x = x;
        _y = y;
        _leftPressed = true;

        _buffer.emplace(Event::Type::LeftPress, x, y);
        Util::TrimQueue(_buffer, QUEUE_SIZE);
    }

    void Mouse::OnLeftMouseReleased(int x, int y)
    {
        _x = x;
        _y = y;
        _leftPressed = false;

        _buffer.emplace(Event::Type::LeftRelease, x, y);
        Util::TrimQueue(_buffer, QUEUE_SIZE);
    }

    void Mouse::OnMiddleMousePressed(int x, int y)
    {
        _x = x;
        _y = y;
        _middlePressed = true;

        _buffer.emplace(Event::Type::MiddlePress, x, y);
        Util::TrimQueue(_buffer, QUEUE_SIZE);
    }

    void Mouse::OnMiddleMouseReleased(int x, int y)
    {
        _x = x;
        _y = y;
        _middlePressed = false;

        _buffer.emplace(Event::Type::MiddleRelease, x, y);
        Util::TrimQueue(_buffer, QUEUE_SIZE);
    }

    void Mouse::OnRightMousePressed(int x, int y)
    {
        _x = x;
        _y = y;
        _rightPressed = true;

        _buffer.emplace(Event::Type::RightPress, x, y);
        Util::TrimQueue(_buffer, QUEUE_SIZE);
    }

    void Mouse::OnRightMouseReleased(int x, int y)
    {
        _x = x;
        _y = y;
        _rightPressed = true;

        _buffer.emplace(Event::Type::RightRelease, x, y);
        Util::TrimQueue(_buffer, QUEUE_SIZE);
    }

    void Mouse::OnWheelUp(int x, int y)
    {
        _x = x;
        _y = y;

        _buffer.emplace(Event::Type::WheelUp, x, y);
        Util::TrimQueue(_buffer, QUEUE_SIZE);
    }

    void Mouse::OnWheelDown(int x, int y)
    {
        _x = x;
        _y = y;

        _buffer.emplace(Event::Type::WheelDown, x, y);
        Util::TrimQueue(_buffer, QUEUE_SIZE);
    }

    void Mouse::OnWheelDelta(int x, int y, int wheelDelta)
    {
        _wheelDelta += wheelDelta;

        // 根据文档，应该每120产生一个事件
        while (_wheelDelta >= WHEEL_DELTA)
        {
            _wheelDelta -= WHEEL_DELTA;
            OnWheelUp(x, y);
        }
        while (_wheelDelta <= -WHEEL_DELTA)
        {
            _wheelDelta += WHEEL_DELTA;
            OnWheelDown(x, y);
        }
    }

    void Mouse::OnMouseEnter()
    {
        _isMouseInWindow = true;
    }

    void Mouse::OnMouseLeave()
    {
        _isMouseInWindow = false;
    }

}