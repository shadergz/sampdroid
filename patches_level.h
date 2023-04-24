#pragma once

#include <cstdint>

#include <unistd.h>

#include <android/log.h>

extern const char* g_mtmTag;

class AArch64_Patcher {
public:
    static constexpr uint8_t PATCHER_HOOK_COUNT = 64;
    static constexpr uint8_t PATCHER_MAX_INST = 7;

    static constexpr uint8_t PATCHER_SYMBOL_NAME = 24;
    static constexpr uint8_t PATCHER_HOOK_SIZE = 
        sizeof(uint16_t) + sizeof(uintptr_t) +
        sizeof(uint8_t)  + sizeof(uint8_t)   +
        sizeof(char[PATCHER_SYMBOL_NAME])    +
        sizeof(uint32_t) * PATCHER_MAX_INST; 

    AArch64_Patcher() {
    }

    void replaceMethod(const char* sb_name, const uintptr_t method, const uintptr_t replace, uintptr_t* saveIn); 
    static void unfuckPageRWX(uintptr_t unfuckAddr, uint64_t regionSize);
    uint32_t* getNewTrampoline() noexcept 
    {
        if (m_tr_bank.m_tramindex == PATCHER_HOOK_COUNT - 1)
            __android_log_assert("m_tr_bank.m_tramindex == PATCHER_HOOK_COUNT - 1",
                g_mtmTag, "our trampoline bank data buffer has exhausted!");
        return reinterpret_cast<uint32_t*>(&m_tr_bank.m_t_RWX_data[PATCHER_HOOK_SIZE * m_tr_bank.m_tramindex++]);
    }

private:
    struct MicroRaw_Trampoline {
        static constexpr uint8_t ARCH_MAX_INST = 10;
        static constexpr uint8_t ARCH_INST_SIZE = 8;

        MicroRaw_Trampoline() {
            static_assert(sizeof m_t_RWX_data == PAGE_SIZE, "trampoline data size is wrong! fix now!");
            static_assert(sizeof m_t_RWX_data / PATCHER_HOOK_SIZE == 
                PATCHER_HOOK_COUNT, "PAGE_SIZE isn't the desired value!");

            unfuckPageRWX((uintptr_t)(m_t_RWX_data), sizeof m_t_RWX_data);

        }

        __attribute__((__aligned__(PAGE_SIZE))) uint8_t m_t_RWX_data[PAGE_SIZE];
        uint8_t m_tramindex{};
    };

    MicroRaw_Trampoline m_tr_bank;
};

void applyGlobalPatches();