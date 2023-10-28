
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

    std::pair<int, int> Mouse::GetPosition() const
    {
        return {_x, _y};
    }

    int Mouse::GetPositionX() const
    {
        return _x;
    }

    int Mouse::GetPositionY() const
    {
        return _y;
    }

    bool Mouse::IsLeftPressed() const
    {
        return _leftPressed;
    }

    bool Mouse::IsMiddlePressed() const
    {
        return _middlePressed;
    }

    bool Mouse::IsRightPressed() const
    {
        return _rightPressed;
    }

    bool Mouse::IsInWindow() const
    {
        return _isMouseInWindow;
    }

    Mouse::Event Mouse::RaiseEvent()
    {
        if (!_buffer.empty())
        {
            auto evt = _buffer.front();
            _buffer.pop();
            return evt;
        }
        else
        {
            return {};
        }
    }

    bool Mouse::IsEmpty() const
    {
        return _buffer.empty();
    }

    void Mouse::Clear()
    {
        while (!_buffer.empty())
            _buffer.pop();
    }

#pragma endregion

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