#pragma once

#include <Windows.h>
#include <intrin.h>
#include <winternl.h>
#include <format>
#include <iostream>

namespace MemoryApiTest
{

    void Test()
    {
        // stack info
        NT_TIB* teb = reinterpret_cast<NT_TIB*>(NtCurrentTeb());
        DWORD stackTop = reinterpret_cast<DWORD>(teb->StackBase);
        DWORD stackBase = reinterpret_cast<DWORD>(teb->StackLimit);
        DWORD distance = stackTop - stackBase;

        std::cout << std::format("stack top: {:#x}", stackTop) << std::endl;
        std::cout << std::format("stack base: {:#x}", stackBase) << std::endl;
        std::cout << std::format("distance: {}", distance) << std::endl;

        void* currentRbp;
        void* currentRsp;

        // Needs clang. MSVC x64 does not support inline assembly
        _asm 
        {
            mov currentRbp, rbp
            mov currentRsp, rsp
        }

        std::cout << std::format("rbp: {:#x}", reinterpret_cast<DWORD>(currentRbp)) << std::endl;
        std::cout << std::format("rsp: {:#x}", reinterpret_cast<DWORD>(currentRsp)) << std::endl;


        
    }



}