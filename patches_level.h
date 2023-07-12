#pragma once

#include <cstdint>
#include <string_view>
#include <array>

#include <unistd.h>
#include <sys/mman.h>
#include <log_client.h>

class AArch64Patcher {
public:
    static constexpr uint8_t PATCHER_HOOK_COUNT{64};
    static constexpr uint8_t PATCHER_MAX_INST{7};

    static constexpr uint8_t PATCHER_SYMBOL_NAME{24};
    static constexpr uint8_t PATCHER_HOOK_SIZE{
        sizeof(uint16_t) + sizeof(uintptr_t) +
        sizeof(uint8_t) + sizeof(uint8_t) + sizeof(char[PATCHER_SYMBOL_NAME]) +
        sizeof(uint32_t) * PATCHER_MAX_INST
    };
    AArch64Patcher() {}

    void placeHookAt(const std::string_view sbName, const uintptr_t method, const uintptr_t replace, uintptr_t* saveIn);
    static void unfuckPageRWX(uintptr_t unfuckAddr, uint64_t regionSize);
    auto getNewTrampoline() noexcept 
    {
        SALOG_ASSERT(m_trBank.m_tIndex < PATCHER_HOOK_COUNT - 1,
            "Our trampoline bank data buffer has exhausted!");
        return reinterpret_cast<uint32_t*>(&m_trBank.m_tRWXData[PATCHER_HOOK_SIZE * m_trBank.m_tIndex++]);
    }

private:
    struct MicroRaw_Trampoline {
        static constexpr uint8_t ARCH_MAX_INST = 10;
        static constexpr uint8_t ARCH_INST_SIZE = 8;

        MicroRaw_Trampoline() {
            static_assert(sizeof m_tRWXData == PAGE_SIZE, "Trampoline data size is wrong! fix now!");
            static_assert(sizeof m_tRWXData / PATCHER_HOOK_SIZE == PATCHER_HOOK_COUNT,
                "PAGE_SIZE isn't the desired value!");
            unfuckPageRWX((uintptr_t)(m_tRWXData.data()), m_tRWXData.size());

        }

        __attribute__((__aligned__(PAGE_SIZE))) std::array<uint8_t, PAGE_SIZE> m_tRWXData;
        uint8_t m_tIndex{};
    };

    MicroRaw_Trampoline m_trBank;
};

namespace sapatch {
    void applyOnGame();

}

