#pragma once

#include <cstdint>
#include <cstdint>

#include <vector>

class AArch64_Patcher {
public:
    static constexpr uint8_t PATCHER_MIN_TRAMPCOUNT = 8;

    AArch64_Patcher() {
        trs_bank.resize(PATCHER_MIN_TRAMPCOUNT);
    }

    void replaceMethod(const uintptr_t method, const uintptr_t replace, uintptr_t* saveIn); 
    static void unfuckPageRWX(uintptr_t unfuckAddr, uint64_t regionSize);

private:
    struct MicroRaw_Trampoline {
        static constexpr uint8_t ARCH_MAX_INST = 10;
        static constexpr uint8_t ARCH_INST_SIZE = 8;

        MicroRaw_Trampoline() {
            static_assert(sizeof t_RWX_data == PAGE_SIZE, "Trampoline data size is wrong! fix now!");
            unfuckPageRWX((uintptr_t)(t_RWX_data), sizeof t_RWX_data);
        }

        __attribute__((__aligned__(PAGE_SIZE))) uint8_t t_RWX_data[PAGE_SIZE];
    };

    std::vector<MicroRaw_Trampoline> trs_bank;
};

void applyGlobalPatches();