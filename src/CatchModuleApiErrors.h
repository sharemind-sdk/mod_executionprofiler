/*
 * Copyright (C) 2015 Cybernetica
 *
 * Research/Commercial License Usage
 * Licensees holding a valid Research License or Commercial License
 * for the Software may use this file according to the written
 * agreement between you and Cybernetica.
 *
 * GNU General Public License Usage
 * Alternatively, this file may be used under the terms of the GNU
 * General Public License version 3.0 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU General Public License version 3.0 requirements will be
 * met: http://www.gnu.org/copyleft/gpl-3.0.html.
 *
 * For further information, please contact us at sharemind@cyber.ee.
 */

#include <exception>
#include <new>
#include <sharemind/module-apis/api_0x1.h>


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
