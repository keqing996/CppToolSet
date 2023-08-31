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
        std::cout << "This stack frame" << std::endl;

        void* currentRbp;
        void* currentRsp;

        // Needs clang. MSVC x64 does not support inline assembly
        _asm
        {
            mov currentRbp, rbp
            mov currentRsp, rsp
        }

        DWORD currentRbpValue = reinterpret_cast<DWORD>(currentRbp);
        DWORD currentRspValue = reinterpret_cast<DWORD>(currentRsp);
        DWORD distanceRbpRsp = currentRbpValue - currentRspValue;

        std::cout << std::format("rbp: {:#x}", currentRbpValue) << std::endl;
        std::cout << std::format("rsp: {:#x}", currentRspValue) << std::endl;
        std::cout << std::format("distance: {} B", distanceRbpRsp) << std::endl;

        std::cout << std::endl;
        std::cout << "This thread stack" << std::endl;
        
        NT_TIB* teb = reinterpret_cast<NT_TIB*>(::NtCurrentTeb());
        DWORD stackBase = reinterpret_cast<DWORD>(teb->StackBase);
        DWORD stackLimit = reinterpret_cast<DWORD>(teb->StackLimit);
        DWORD distance = (stackLimit - stackBase) / 1024 / 1024;

        std::cout << std::format("stack base: {:#x}", stackBase) << std::endl;
        std::cout << std::format("stack limit: {:#x}", stackLimit) << std::endl;
        std::cout << std::format("distance: {} MB", distance) << std::endl;
        
        std::cout << std::endl;
        std::cout << "Process heaps" << std::endl;

        DWORD numberOfHeaps = ::GetProcessHeaps(0, nullptr);
        std::cout << std::format("current process heaps number = {}", numberOfHeaps) << std::endl;

        HANDLE* heaps = new HANDLE[numberOfHeaps];
        ::GetProcessHeaps(numberOfHeaps, heaps);

        HANDLE defaultHeap = ::GetProcessHeap();
        
        for (int i = 0; i < numberOfHeaps; i++)
        {
            if (defaultHeap == heaps[i])
                std::cout << std::format("[Default] heap {} addr: {:#x}", i, reinterpret_cast<DWORD>(heaps[i]));
            else
                std::cout << std::format("heap {} addr: {:#x}", i, reinterpret_cast<DWORD>(heaps[i]));
            
            std::cout << std::endl;
        }

        
        
    }



}