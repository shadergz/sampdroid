#pragma once

#define SAVE_REGS_07()\
    asm volatile(\
        "stp x0, x1, [sp, #-16]!\n"\
        "stp x2, x3, [sp, #-16]!\n"\
        "stp x4, x5, [sp, #-16]!\n"\
        "stp x6, x7, [sp, #-16]!\n"\
    )

#define RESTORE_REGS_70()\
    asm volatile(\
        "ldp x6, x7, [sp], #16\n"\
        "ldp x4, x5, [sp], #16\n"\
        "ldp x2, x3, [sp], #16\n"\
        "ldp x0, x1, [sp], #16\n"\
    )
