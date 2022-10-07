#pragma once

#include <Windows.h>
#include <iostream>

class VMTHook
{
    std::unique_ptr<unsigned int[]> VT_New = nullptr;
    unsigned int* VT_Backup = nullptr;
    unsigned int VT_Functions = 0;

public:
    VMTHook(void* Instance)
    {
        //   Store the old Virtual Table   //
        VT_Backup = *reinterpret_cast<unsigned int**>(Instance);

        //   Get the Virtual Table Function count   //
        while (reinterpret_cast<unsigned int*>(*reinterpret_cast<unsigned int*>(Instance))[VT_Functions])
        {
            VT_Functions++;
        }

        //   Store the size of the Virtual Table   //
        const unsigned int VT_Size = ((VT_Functions * 0x4) + 0x4);

        //   Add 1 because we need extra space for the RTTI   //
        VT_New = std::make_unique<unsigned int[]>(VT_Functions + 1);

        //   Copy the old Virtual Table with the RTTI   //
        memcpy(VT_New.get(), &VT_Backup[-1], VT_Size);

        //   Replace old Virtual Table with the new one skipping first 4 bytes because RTTI is now there   //
        *reinterpret_cast<unsigned int**>(Instance) = &VT_New.get()[1];
    };

    void* GetDefaultFunc(const unsigned int Index)
    {
        return reinterpret_cast<void*>(VT_Backup[Index]);
    }

    bool HookFunc(void* New, const unsigned int Index) const
    {
        VT_New[Index + 1] = reinterpret_cast<unsigned int>(New);
        return true;
    }

    bool UnhookFunc(const unsigned int Index) const
    {
        VT_New[Index + 1] = VT_Backup[Index];
        return true;
    }

};