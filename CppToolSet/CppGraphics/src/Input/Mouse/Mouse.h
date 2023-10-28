#pragma once

#include <queue>
#include "Util/NonCopyable.h"

class Application;
class ApplicationWinImp;

namespace Input
{
    class Mouse: public NonCopyable
    {
    public:
        friend class ::Application;
        friend class ::ApplicationWinImp;

    public:
        class Event
        {
        public:
            enum class Type
            {
                Invalid,
                Move,
                LeftPress,
                LeftRelease,
                RightPress,
                RightRelease,
                WheelUp,
                WheelDown,
                MiddlePress,
                MiddleRelease,
                Enter,
                Leave
            };

        private:
            Type _type;
            int _x;
            int _y;

        public:
            Event();
            Event(Type t, int x, int y);

        public:
            bool IsValid() const;
            Type GetType() const;
            std::pair<int, int> GetPosition() const;
            int GetPositionX() const;
            int GetPositionY() const;
        };

    public:
        Mouse() = default;
        ~Mouse() = default;
        Mouse(const Mouse& mouse) = delete;
        Mouse& operator= (const Mouse& mouse) = delete;

    public:
        std::pair<int, int> GetPosition() const;
        int GetPositionX() const;
        int GetPositionY() const;
        bool IsLeftPressed() const;
        bool IsMiddlePressed() const;
        bool IsRightPressed() const;
        bool IsInWindow() const;
        Mouse::Event RaiseEvent();
        bool IsEmpty() const;
        void Clear();

    private:
        void OnMouseMove(int x, int y);

        void OnLeftMousePressed(int x, int y);
        void OnLeftMouseReleased(int x, int y);

        void OnMiddleMousePressed(int x, int y);
        void OnMiddleMouseReleased(int x, int y);

        void OnRightMousePressed(int x, int y);
        void OnRightMouseReleased(int x, int y);

        void OnWheelUp(int x, int y);
        void OnWheelDown(int x, int y);
        void OnWheelDelta(int x, int y, int wheelDelta);

        void OnMouseEnter();
        void OnMouseLeave();

    private:
        static constexpr unsigned int QUEUE_SIZE = 16;

    private:
        int _x;
        int _y;
        bool _leftPressed;
        bool _middlePressed;
        bool _rightPressed;
        int _wheelDelta;
        bool _isMouseInWindow;
        std::queue<Event> _buffer;
    };
}


