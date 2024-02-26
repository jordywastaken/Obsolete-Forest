
#include "Memory.hpp"

__declspec(naked, noinline) unsigned long long Syscall(int index, ...)
{
    // Assembly go brrr
    asm(
        // Save the link register
        "mflr %r0;"
        "std %r0, 0x10(%r1);"
        "stdu %r1, -0x70(%r1);"

        // Cache index in r0, then move all the args forward
        "mr %r0, %r3;"
        "mr %r3, %r4;"
        "mr %r4, %r5;"
        "mr %r5, %r6;"
        "mr %r6, %r7;"
        "mr %r7, %r8;"
        "mr %r8, %r9;"
        "mr %r9, %r10;"
        "mr %r10, %r11;"

        // Execute syscall
        "mr %r11, %r0;"
        "sc;"

        // Restore link register, free stack and return
        "addi %r1, %r1, 0x70;"
        "ld %r0, 0x10(%r1);"
        "mtlr %r0;"
        "blr;"
    );

    return __reg(3);
}

unsigned int sceKernelGetCurrentProcessId()
{
    return Syscall(1);
}

int sceKernelGetTargetType()
{
    unsigned long long targetType = 0;
    if (Syscall(985, &targetType) == 0)
        return targetType;

    return 0;
}

int sceKernelIsConsoleRetail()
{
    return sceKernelGetTargetType() == 1;
}

int sceKernelReadProcessMemory(unsigned int pid, unsigned int address, void* data, int size)
{
    if (!sceKernelIsConsoleRetail())
        return Syscall(904, pid, address, size, data);

    return Syscall(8, 0x7777, 0x31, pid, address, data, size);
}

int sceKernelWriteProcessMemory(unsigned int pid, unsigned int address, const void* data, int size)
{
    if (!sceKernelIsConsoleRetail())
        return Syscall(905, pid, address, size, data);

    return Syscall(8, 0x7777, 0x32, pid, address, data, size);
}

void PatchInJump(unsigned int functionStartAddress, unsigned int newFunction)
{
    unsigned int jumpInstructions[]
    {
        0x3D600000 | ((newFunction >> 16) & 0xFFFF), // lis %r11, newFunction@h
        0x616B0000 | (newFunction & 0xFFFF),         // ori %r11, %r11, newFunction@l        
        0x7D6903A6,                                  // mtctr %r11
        0x4E800420                                   // bctr
    };

    sceKernelWriteProcessMemory(sceKernelGetCurrentProcessId(), functionStartAddress, jumpInstructions, 0x10);
}

void HookFunctionStart(unsigned int functionStartAddress, unsigned int newFunction, unsigned int functionStub)
{
    unsigned int originalInstructions[4];
    sceKernelReadProcessMemory(sceKernelGetCurrentProcessId(), functionStartAddress, originalInstructions, 0x10);

    // The stub will execute the first 4 instruction and then jump back to where it should
    sceKernelWriteProcessMemory(sceKernelGetCurrentProcessId(), functionStub, originalInstructions, 0x10);
    PatchInJump(functionStub + 0x10, functionStartAddress + 0x10);

    // Finally, make the function we want to hook jump to ours
    PatchInJump(functionStartAddress, newFunction);
}

void UnHookFunctionStart(unsigned int functionStartAddress, unsigned int functionStub)
{
    unsigned int originalInstructions[4];
    sceKernelReadProcessMemory(sceKernelGetCurrentProcessId(), functionStub, originalInstructions, 0x10);

    // Restore the original instructions saved in the stub
    sceKernelWriteProcessMemory(sceKernelGetCurrentProcessId(), functionStartAddress, originalInstructions, 0x10);
}