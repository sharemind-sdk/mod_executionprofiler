/*
 * This file is a part of the Sharemind framework.
 * Copyright (C) Cybernetica AS
 *
 * All rights are reserved. Reproduction in whole or part is prohibited
 * without the written consent of the copyright owner. The usage of this
 * code is subject to the appropriate license agreement.
 */

#include <cassert>
#include <limits>
#include <sharemind/ExecutionProfiler.h>
#include <sharemind/libmodapi/api_0x1.h>
#include "CatchModuleApiErrors.h"
 
extern "C" {
    struct __attribute__ ((visibility("internal"))) ModuleData {
        ModuleData(sharemind::ExecutionProfiler & ep) : executionProfiler(ep) {};
        sharemind::ExecutionProfiler &executionProfiler;
    };
}

/**
 * SysCall: ProcessProfiler_newSectionType
 * Args:
 *      0) crefs[0]      input string, the name of the new section type
 *      1) refs[0]       output uint32, the identifier of the new section type
 *
 * \post If successful, a new profiling section type is created and its
 *       identifier is output.
 */
SHAREMIND_MODULE_API_0x1_SYSCALL(ProcessProfiler_newSectionType,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    (void) args;


    if (!crefs || !refs // Mandatory checks
        // Optional checks:
        || num_args || returnValue || (assert(crefs[0u].pData), crefs[1u].pData)
        || (assert(refs[0u].pData), refs[1u].pData)
        || refs[0u].size != sizeof (uint32_t))
    {
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;
    }

    if (crefs[0u].size == std::numeric_limits<size_t>::max())
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;

    assert(c);
    assert(c->moduleHandle);
    sharemind::ExecutionProfiler & executionProfiler =
            static_cast<const ModuleData *>( c->moduleHandle)->executionProfiler;

    try {
        const char * sectionTypeName =
                static_cast<const char*>(crefs[0u].pData);

        uint32_t & sectionType = *static_cast<uint32_t*>(refs[0u].pData);
        sectionType = executionProfiler.newSectionType(sectionTypeName);
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_SHAREMIND_ERROR;
    }

    return SHAREMIND_MODULE_API_0x1_OK;
}


extern "C" {



SHAREMIND_MODULE_API_MODULE_INFO("executionprofiler",
                                 0x00010000,   /* Version 0.1.0.0 */
                                 0x1);         /* Support API version 1. */

SHAREMIND_MODULE_API_0x1_INITIALIZER(c) __attribute__ ((visibility("default")));
SHAREMIND_MODULE_API_0x1_INITIALIZER(c) {
    assert(c);

    const SharemindModuleApi0x1Facility * fexecutionprofiler
            = c->getModuleFacility(c, "Profiler");
    if (!fexecutionprofiler || !fexecutionprofiler->facility)
        return SHAREMIND_MODULE_API_0x1_MISSING_FACILITY;

    sharemind::ExecutionProfiler * executionProfilerFacility =
        static_cast<sharemind::ExecutionProfiler *>(fexecutionprofiler->facility);
    try {

        c->moduleHandle = new ModuleData(*executionProfilerFacility);
        return SHAREMIND_MODULE_API_0x1_OK;
    } catch (...) {
        return catchModuleApiErrors();
    }
}

SHAREMIND_MODULE_API_0x1_DEINITIALIZER(c)
        __attribute__ ((visibility("default")));
SHAREMIND_MODULE_API_0x1_DEINITIALIZER(c) {
    assert(c);
    assert(c->moduleHandle);
    delete static_cast<ModuleData *>(c->moduleHandle);
    #ifndef NDEBUG
    c->moduleHandle = nullptr; // Not needed, but may help debugging.
    #endif
}

#define SAMENAME(f) { #f, &(f) }

SHAREMIND_MODULE_API_0x1_SYSCALL_DEFINITIONS(

    // Misc. syscalls:
    SAMENAME(ProcessProfiler_newSectionType)

);

} // extern "C" {
