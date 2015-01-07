/*
 * This file is a part of the Sharemind framework.
 * Copyright (C) Cybernetica AS
 *
 * All rights are reserved. Reproduction in whole or part is prohibited
 * without the written consent of the copyright owner. The usage of this
 * code is subject to the appropriate license agreement.
 */

#ifndef SHAREMIND_MOD_ALGORITHMS_DECLARESYSCALL_H
#define SHAREMIND_MOD_ALGORITHMS_DECLARESYSCALL_H

#include <sharemind/extern_c.h>
#include <sharemind/libmodapi/api_0x1.h>


#define SHAREMIND_MOD_ALGORITHMS_DECLARE_SYSCALL(name,...) \
    SHAREMIND_EXTERN_C_BEGIN \
    SHAREMIND_MODULE_API_0x1_SYSCALL(name, args, num_args, refs, crefs, \
                                     returnValue, c) \
        __attribute__((visibility("hidden") __VA_ARGS__ )); \
    SHAREMIND_EXTERN_C_END

#endif /* SHAREMIND_MOD_ALGORITHMS_DECLARESYSCALL_H */
