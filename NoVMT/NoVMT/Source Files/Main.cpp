#include "../Header Files/ExampleVMT.hpp"
#include "../Header Files/ExampleVMT2.hpp"
#include "../Header Files/ExampleVMT3.hpp"
#include "../Header Files/ShadowVMT.hpp"

class VirtualClassExample
{
public:
    virtual void DummyFunction(const char* parameter)
    {
        std::cout << parameter << std::endl;
    }
};

typedef void(__stdcall* original)(const char*);
inline original originalAddr = nullptr;

// ----------------------------------------------------------------------------------------------------------------------

void __stdcall Replacement(const char* parameter)
{
    return originalAddr("|_ Intercepted");
}

// ----------------------------------------------------------------------------------------------------------------------

int main()
{
    // A new instance towards are class that contains the virtual function.
    VirtualClassExample* dummyClass       = new VirtualClassExample();

    // New hook instances.
    VirtualMethodTable* virtualMethodHook = new VirtualMethodTable();
    VMTShadowing* vmtShadowing            = new VMTShadowing(dummyClass);
    c_vmt* virtualMethodHook2             = new c_vmt(dummyClass);
    VMTHook* virtualMethodHook3           = new VMTHook(dummyClass);

    // Set our swap table 
    uintptr_t* swapTable = reinterpret_cast<uintptr_t*>(dummyClass);
    swapTable            = reinterpret_cast<uintptr_t*>(swapTable[NULL]);

    // Our old swap table.
    // Should be at the beginning of your program or near initialization / anti-cheat relative.
    static const auto oldSwapTable = swapTable[0];

    // The address of our virtual function.
    void* virtualAddr = reinterpret_cast<void*>(virtualMethodHook->GetVirtualFunction(dummyClass, 0));

    // Set our original address.
    originalAddr = (original)virtualAddr;

    // Place hook.
    
    //vmtShadowing->Apply(0, (uintptr_t*)Replacement);
    //virtualMethodHook2->hook(0, Replacement);
    //virtualMethodHook3->HookFunc(Replacement, 0);
    //virtualMethodHook4->Hook(Replacement, 0);
    if (virtualMethodHook->AddHook(dummyClass, Replacement, 0, originalAddr))
        std::cout << "[+] Hooked" << std::endl;

    dummyClass->DummyFunction("Test");

    // Our new swap table.
    // Should always be after where a hook is placed.
    // Or in a thread / loop constantly comparing the oldSwapTable with this.
    static const auto newSwapTable = swapTable[0];

    // Print out our addresses and compare them.
    std::cout << "[+] Original: 0x" << std::hex << oldSwapTable << std::endl;
    std::cout << "[+] New: 0x" << std::hex << newSwapTable << std::endl;

    // Old swap pointer isn't the same as the current.
    bool vTablePtrSwapDetected = (oldSwapTable != newSwapTable || oldSwapTable + 0x244 == newSwapTable || oldSwapTable + 0x2B7 == newSwapTable);

    // Found.
    if (vTablePtrSwapDetected)
        std::cout << "[!] Virtual table swap detected" << std::endl;

    // Prevention of immediate closing.
    std::cin.get();
    std::cin.get();

    return 0;
}

// ----------------------------------------------------------------------------------------------------------------------
