
#pragma once

// Override declspec since the ps3 compiler doesn't really support it
#define __declspec(...) __attribute__((__VA_ARGS__))

// Missing ppu asm intrinsic
__declspec(naked) inline void __blr() { asm("blr"); }

unsigned long long Syscall(int index, ...);

unsigned int sceKernelGetCurrentProcessId();
int sceKernelGetTargetType();
int sceKernelIsConsoleRetail();
int sceKernelReadProcessMemory(unsigned int pid, unsigned int address, void* data, int size);
int sceKernelWriteProcessMemory(unsigned int pid, unsigned int address, const void* data, int size);

void PatchInJump(unsigned int functionStartAddress, unsigned int newFunction);
void HookFunctionStart(unsigned int functionStartAddress, unsigned int newFunction, unsigned int functionStub);
void UnHookFunctionStart(unsigned int functionStartAddress, unsigned int functionStub);

// Game call utilities
constexpr unsigned int GameToc = (0xB14ED8);

#define MakeStub(address, ret_type, name, args) \
    unsigned int name##_fptr[]{ address, GameToc }; \
    auto name = reinterpret_cast<ret_type(*)args>(name##_fptr);

#pragma diag_supress 1070
template<typename T>
class Symbol
{
public:
    constexpr Symbol(unsigned int obj)
        : object(obj)
    { }

    operator T* ()
    {
        volatile unsigned int object_fptr[]{ this->object, GameToc };

        T* type = *reinterpret_cast<T*>(this->object);
        return (decltype(type))object_fptr;
    }

    T* get()
    {
        return this->object;
    }

    unsigned int object;
};

template<typename T, typename... Args>
static T GameCall(unsigned int address, Args... args)
{
    volatile unsigned int func_fptr[]{ address, GameToc };
    return reinterpret_cast<T(*)(Args...)>(func_fptr)(args...);
}