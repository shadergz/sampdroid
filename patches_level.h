#pragma once

#include <cstdint>

#include <unistd.h>
#include <outside.h>

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

    void placeHookAt(const char* sbName, const uintptr_t method, const uintptr_t replace, uintptr_t* saveIn); 
    static void unfuckPageRWX(uintptr_t unfuckAddr, uint64_t regionSize);
    uint32_t* getNewTrampoline() noexcept 
    {
        MT4LOG_RUNTIME_ASSERT(m_trBank.m_tramIndex < PATCHER_HOOK_COUNT - 1,
            "Our trampoline bank data buffer has exhausted!");
        return reinterpret_cast<uint32_t*>(&m_trBank.m_tRWXData[PATCHER_HOOK_SIZE * m_trBank.m_tramIndex++]);
    }

private:
    struct MicroRaw_Trampoline {
        static constexpr uint8_t ARCH_MAX_INST = 10;
        static constexpr uint8_t ARCH_INST_SIZE = 8;

        MicroRaw_Trampoline() {
            static_assert(sizeof m_tRWXData == PAGE_SIZE, "Trampoline data size is wrong! fix now!");
            static_assert(sizeof m_tRWXData / PATCHER_HOOK_SIZE == 
                PATCHER_HOOK_COUNT, "PAGE_SIZE isn't the desired value!");

            unfuckPageRWX((uintptr_t)(m_tRWXData), sizeof m_tRWXData);

        }

        __attribute__((__aligned__(PAGE_SIZE))) uint8_t m_tRWXData[PAGE_SIZE];
        uint8_t m_tramIndex{};
    };

    MicroRaw_Trampoline m_trBank;
};

namespace Mt4Patches {
void applyOnGame();

}

