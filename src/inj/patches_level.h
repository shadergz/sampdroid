#pragma once

#include <array>
#include <linux/mman.h>
#include <random>
#include <unistd.h>
#include <sys/mman.h>

#include <core/log_client.h>

class AArch64Patcher {
public:
    static constexpr uint16_t PATCHER_PAGE_SIZE{4096 * 3};
    static constexpr uint8_t PATCHER_MAX_INST{28};

    static constexpr uint8_t PATCHER_HOOK_SIZE{
        sizeof(uint) + sizeof(uintptr_t) +
        sizeof(unsigned char) + sizeof(uint32_t) * PATCHER_MAX_INST +
        3 * sizeof(char)
    };
    static constexpr uint8_t PATCHER_HOOK_COUNT{PATCHER_PAGE_SIZE / PATCHER_HOOK_SIZE};

    AArch64Patcher() = default;

    void placeHookAt(const uintptr_t method, const uintptr_t replace, uintptr_t* saveIn);
    
    void emplaceMethod(const uintptr_t method, const uintptr_t super, uint8_t instCount, bool runAfter);
    
    static void turnTextSegmentMutable(uintptr_t unfuckAddr, uint64_t regionSize);
    auto getNewTrampoline() noexcept 
    {
        SALOG_ASSERT(m_trBank.m_tIndex < PATCHER_HOOK_COUNT - 1,
            "Our trampoline bank's data buffer has been exhausted!");
        return reinterpret_cast<uint32_t*>(m_trBank.m_tRWXData + PATCHER_HOOK_SIZE * m_trBank.m_tIndex++);
    }

private:
    std::random_device m_device;
    std::mt19937_64 m_nemesis{m_device()};
    std::uniform_int_distribution<uint> m_randomDist{0, UINT16_MAX};

    struct MicroRaw_Trampoline {
        static constexpr uint8_t ARCH_MAX_INST{10};
        static constexpr uint8_t ARCH_INST_SIZE{8};

        MicroRaw_Trampoline() {
            // Allocating memory at the process level is necessary because we can share this memory segment with other threads within the same process
            m_tRWXData = reinterpret_cast<uint8_t*>(mmap(nullptr, PATCHER_PAGE_SIZE, 
                PROT_EXEC | PROT_READ | PROT_WRITE, 
                MAP_SHARED | MAP_ANONYMOUS, -1, 0));

            SALOG_ASSERT(m_tRWXData != MAP_FAILED, "m_tRWXData is pointing to an invalid address space");
            salog::printFormat(salog::Info, "Hook: patch data allocated at: %#p", m_tRWXData);

#ifndef NDEBUG
            for (uintptr_t iIdex{}; iIdex != PATCHER_PAGE_SIZE; iIdex += 4)
                *reinterpret_cast<uint32_t*>(m_tRWXData + iIdex) = __builtin_bswap32(0x000020d4);
#endif
        }

        ~MicroRaw_Trampoline() {
            if (m_tRWXData) {
                std::memset(m_tRWXData, 0xff, PATCHER_PAGE_SIZE);
                munmap(m_tRWXData, PATCHER_PAGE_SIZE);
            }
            
        }
        uint8_t* m_tRWXData{};
        uint8_t m_tIndex{};
    };

    MicroRaw_Trampoline m_trBank{};
};

namespace sapatch {
    void applyOnGame();
}

