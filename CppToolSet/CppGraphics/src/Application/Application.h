#pragma once

#include <Windows.h>
#include "Util/NonCopyable.h"

class Application : public NonCopyable
{
public:
    void InitWindow(const wchar_t* name);
    int GetWindowHeight() const;
    int GetWindowWidth() const;

private:
    Application(int initHeight, int initWidth, const wchar_t* name);

private:
    static LRESULT CALLBACK HandleMsg(HWND, UINT, WPARAM, LPARAM);

private:
    int _height;
    int _width;
    HINSTANCE _hInst;
    HWND _hWnd;

private:
    static Application* _instance;
    static constexpr LPCWSTR WND_CLASS_NAME = L"Graphic Renderer";

    static void CreateInstance(int initHeight, int initWidth, const wchar_t* name);
    static const Application* GetInstance();
};