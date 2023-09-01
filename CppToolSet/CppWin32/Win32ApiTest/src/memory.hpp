#pragma once

#include <Windows.h>
#include <intrin.h>
#include <winternl.h>
#include <format>
#include <iostream>

namespace MemoryApiTest
{
    
    void ShowThreadTIB()
    {
        std::cout << std::endl;
        std::cout << "Thread TIB:" << std::endl;
        
        NT_TIB* teb = reinterpret_cast<NT_TIB*>(::NtCurrentTeb());
        DWORD stackBase = reinterpret_cast<DWORD>(teb->StackBase);
        DWORD stackLimit = reinterpret_cast<DWORD>(teb->StackLimit);
        DWORD distance = (stackLimit - stackBase) / 1024 / 1024;

        std::cout << std::format("stack base: {:#x}", stackBase) << std::endl;
        std::cout << std::format("stack limit: {:#x}", stackLimit) << std::endl;
        std::cout << std::format("distance: {} MB", distance) << std::endl;

        std::cout << std::endl;
    }
    
    SIZE_T GetHeapSize(HANDLE hHeap)
    {
        if (hHeap == nullptr)
            return 0;
        
        ::HeapLock(hHeap);
        
        PROCESS_HEAP_ENTRY entry;
        entry.lpData = nullptr;

        // trans all memory block
        SIZE_T totalSize = 0;
        while (::HeapWalk(hHeap, &entry))
        {
            if (entry.wFlags & PROCESS_HEAP_ENTRY_BUSY)
            {
                totalSize += entry.cbData;
            }
        }
        
        ::HeapUnlock(hHeap);

        return totalSize;
    }

    HEAP_SUMMARY GetHeapSummary(HANDLE hHeap)
    {
        HEAP_SUMMARY heapSummary;
        heapSummary.cb = sizeof(HEAP_SUMMARY);
        ::HeapSummary(hHeap, 0, &heapSummary);

        return heapSummary;
    }

    void ShowProcessHeapsInfo()
    {
        std::cout << std::endl;
        std::cout << "Process heaps" << std::endl;

        DWORD numberOfHeaps = ::GetProcessHeaps(0, nullptr);
        std::cout << std::format("current process heaps number = {}", numberOfHeaps) << std::endl;

        HANDLE* heaps = new HANDLE[numberOfHeaps];
        ::GetProcessHeaps(numberOfHeaps, heaps);

        HANDLE defaultHeap = ::GetProcessHeap();

        size_t totalSize = 0;
        for (int i = 0; i < numberOfHeaps; i++)
        {
            if (defaultHeap == heaps[i])
                std::cout << "[Default] ";
            else
                std::cout << "          ";
            
            auto thisHeapSize = GetHeapSize(heaps[i]);
            totalSize += thisHeapSize;
            
            auto thisHeapSizeKB = thisHeapSize / 1024.0;
            std::cout << std::format("heap {:3} addr: {:#x}, size: {:9.5} KB, ", i, reinterpret_cast<DWORD>(heaps[i]), thisHeapSizeKB);

            HEAP_SUMMARY thisHeapSummary = GetHeapSummary(heaps[i]);
            std::cout << std::format("allocated: {:9.5} KB, ", thisHeapSummary.cbAllocated / 1024.0);
            std::cout << std::format("committed: {:9.5} KB, ", thisHeapSummary.cbCommitted / 1024.0);
            std::cout << std::format("reserved: {:9.5} KB, ", thisHeapSummary.cbReserved / 1024.0);
            std::cout << std::format("max reserved: {:9.5} KB, ", thisHeapSummary.cbMaxReserve / 1024.0);
            
            std::cout << std::endl;
        }

        std::cout << std::format("Total Heap Size: {} KB", totalSize / 1024.0);
        std::cout << std::endl;
    }
    
    void Test()
    {
        std::cout << "This stack frame:" << std::endl;

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

        // TIB
        ShowThreadTIB();

        // Heap
        HANDLE hPrivateHeap = HeapCreate(0, 0, 0);
        void* buffer = HeapAlloc(hPrivateHeap, 0, 100); // allocate 100B
        
        ShowProcessHeapsInfo();
        HeapFree(hPrivateHeap, 0, buffer);
        HeapDestroy(hPrivateHeap);

        
    }



}