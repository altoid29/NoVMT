#pragma once

#include <Windows.h>

class VirtualMethodTable
{
public:
    void* _originalAddress;

    // ----------------------------------------------------------------------------------------------------------------------

    // Acquire the address of the virtual function in a class.
    // Example usage: reinterpret_cast<void*>(GetVirtualFunction(pClass, nIndex));
    __forceinline unsigned int GetVirtualFunction(void* virtualClass, unsigned int virtualIndex)
    {
        return static_cast<unsigned int>((*static_cast<int**>(virtualClass))[virtualIndex]);
    }

    // ----------------------------------------------------------------------------------------------------------------------

    bool AddHook(void* vTable, void* detouredFunction, int virtualIndex, void* originalAddress)
    {
        // Information related to the virtual table.
        uintptr_t* swapTable = reinterpret_cast<uintptr_t*>(vTable);
        swapTable            = reinterpret_cast<uintptr_t*>(swapTable[NULL]);

        // Set the original address.
        originalAddress = reinterpret_cast<void*>(GetVirtualFunction(vTable, virtualIndex));

        // Set original address for restoring.
        this->_originalAddress = originalAddress;

        // Sanity checks.
        if (virtualIndex < 0 || vTable == nullptr || detouredFunction == nullptr)
            return false;

        // Store the original page protection rights.
        DWORD dwOldRights = 0x0;

        // Detour.
        VirtualProtect(swapTable, 0x1000, PAGE_READWRITE, &dwOldRights);
        swapTable[virtualIndex] = reinterpret_cast<DWORD_PTR>(detouredFunction);
        VirtualProtect(swapTable, 0x1000, dwOldRights, &dwOldRights);

        return true;
    }

    // ----------------------------------------------------------------------------------------------------------------------

    bool RestoreHook(void* vTable, int virtualIndex)
    {
        // Sanity checks.
        if (virtualIndex < 0 || vTable == nullptr || this->_originalAddress == nullptr)
            return false;

        // Information related to the virtual table.
        uintptr_t* swapTable = reinterpret_cast<uintptr_t*>(vTable);
        swapTable = reinterpret_cast<uintptr_t*>(swapTable[NULL]);

        // Store the original page protection rights.
        DWORD dwOldRights = 0x0;

        // Detour.
        VirtualProtect(swapTable, 0x1000, PAGE_READWRITE, &dwOldRights);
        swapTable[virtualIndex] = reinterpret_cast<DWORD_PTR>(this->_originalAddress);
        VirtualProtect(swapTable, 0x1000, dwOldRights, &dwOldRights);

        return true;
    }

    // ----------------------------------------------------------------------------------------------------------------------
};