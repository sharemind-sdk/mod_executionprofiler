/*
 * This file is a part of the Sharemind framework.
 * Copyright (C) Cybernetica AS
 *
 * All rights are reserved. Reproduction in whole or part is prohibited
 * without the written consent of the copyright owner. The usage of this
 * code is subject to the appropriate license agreement.
 */

#include <exception>
#include <new>
#include <sharemind/libmodapi/api_0x1.h>


inline SharemindModuleApi0x1Error catchModuleApiErrors() noexcept
        __attribute__ ((visibility("internal")));

inline SharemindModuleApi0x1Error catchModuleApiErrors() noexcept {
    try {
        if (const auto eptr = std::current_exception())
            std::rethrow_exception(eptr);
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_GENERAL_ERROR;
    }
}
