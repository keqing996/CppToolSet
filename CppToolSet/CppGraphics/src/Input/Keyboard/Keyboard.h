#pragma once

#include <queue>
#include <bitset>

class Application;
class ApplicationWinImp;

namespace Input
{
    class Keyboard
    {
    public:
        friend class ::Application;
        friend class ::ApplicationWinImp;

    public:
        class Event
        {
        public:
            enum class Type { Invalid, Press, Release };

        private:
            Type _type;
            unsigned char _code;

        public:
            Event();
            Event(Type type, unsigned char code);
            bool IsPressed() const;
            bool IsReleased() const;
            bool IsValid() const;
        };

    public:
        Keyboard() = default;
        ~Keyboard() = default;
        Keyboard(const Keyboard&) = delete;
        Keyboard& operator= (const Keyboard&) = delete;

    public:
        bool IsKeyPressed(unsigned char keycode) const;
        Event ReadKey();
        bool IsKeyEmpty() const;
        void ClearKey();

        wchar_t ReadCharW();
        bool IsCharEmpty() const;
        void ClearChar();

        void Clear();

        void EnableAutoRepeat();
        void DisableAutoRepeat();
        bool IsAutoRepeatEnabled() const;

    private:
        void OnKeyPressed(unsigned char keycode);
        void OnKeyReleased(unsigned char keycode);
        void OnCharW(wchar_t c);
        void ClearState();

    private:
        static constexpr unsigned int NUM_OF_KEYS = 256;
        static constexpr unsigned int QUEUE_SIZE = 16;

    private:
        bool _autoRepeat = false;
        std::bitset<NUM_OF_KEYS> _keyStateSet;
        std::queue<Event> _keyBuffer;
        std::queue<wchar_t> _charBuffer;
    };
}
