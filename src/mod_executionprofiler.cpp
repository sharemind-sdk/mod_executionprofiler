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

#include <cassert>
#include <limits>
#include <sharemind/ExecutionProfiler.h>
#include <sharemind/module-apis/api_0x1.h>
#include "CatchModuleApiErrors.h"

extern "C" {
    struct __attribute__ ((visibility("internal"))) ModuleData {
        ModuleData(sharemind::ExecutionProfiler & ep) : executionProfiler(ep) {}
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
        || num_args || returnValue
        || (static_cast<void>(assert(crefs[0u].pData)), crefs[1u].pData)
        || (static_cast<void>(assert(refs[0u].pData)), refs[1u].pData)
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
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }

    return SHAREMIND_MODULE_API_0x1_OK;
}

/**
 * SysCall: ProcessProfiler_startSection
 * Args:
 * 0) p[0]   input uint32, the profiling section type identifier
 * 1) p[1]   input uint64, the profiling section complexity parameter
 * 2) ref[0] output uint32, the identifier of the new started profiling section
 *
 * \pre There exists a profiling section type identifier usable for creating a
 *      new profiling section
 * \post If successful, a new profiling section is started and its identifier is
 *       output
 */
SHAREMIND_MODULE_API_0x1_SYSCALL(ProcessProfiler_startSection,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    if (num_args != 2 || ! refs // Mandatory checks
        // Optional checks:
        || crefs || returnValue
        || (static_cast<void>(assert(refs[0u].pData)), refs[1u].pData)
        || refs[0u].size != sizeof(uint32_t))
    {
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;
    }

    assert(c);
    assert(c->moduleHandle);
    sharemind::ExecutionProfiler & profiler =
            static_cast<const ModuleData *>( c->moduleHandle)->executionProfiler;
    try {
        const uint32_t sectionType = args[0u].uint32[0u];
        const uint64_t sectionComplexity = args[1u].uint64[0u];
        uint32_t& sectionId = *static_cast<uint32_t*>(refs[0u].pData);

        sectionId = profiler.startSection<uint32_t>(sectionType,
                                                    sectionComplexity);

        // Push this section as parent for next ones:
        profiler.pushParentSection(sectionId);

    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }

    return SHAREMIND_MODULE_API_0x1_OK;
}

/**
 * SysCall: ProcessProfiler_endSection
 * Args:
 *      0) p[0]           input uint32, the identifier of a profiling section
 *
 * \pre There exists a started profiling section with identifier SID
 * \post The profiling section with identifier SID is closed
 */
SHAREMIND_MODULE_API_0x1_SYSCALL(ProcessProfiler_endSection,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    const sharemind::UsTime endTime = sharemind::getUsTime();

    if (num_args != 1u // Mandatory checks
        || crefs || refs || returnValue) // Optional checks
    {
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;
    }

    assert(c);
    assert(c->moduleHandle);
    sharemind::ExecutionProfiler & profiler =
            static_cast<const ModuleData *>( c->moduleHandle)->executionProfiler;

    try {
        const uint32_t sectionId = args[0u].uint32[0u];
        profiler.popParentSection();
        profiler.endSection(sectionId, endTime);
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
    }

    return SHAREMIND_MODULE_API_0x1_OK;
}

/**
 * SysCall: ProcessProfiler_flushLog
 */
SHAREMIND_MODULE_API_0x1_SYSCALL(ProcessProfiler_flushLog,
                                 args, num_args, refs, crefs,
                                 returnValue, c)
{
    (void) args;

    if (num_args != 0u || crefs || refs || returnValue) {
        return SHAREMIND_MODULE_API_0x1_INVALID_CALL;
    }

    assert(c);
    assert(c->moduleHandle);
    sharemind::ExecutionProfiler & profiler =
            static_cast<const ModuleData *>( c->moduleHandle)->executionProfiler;

    try {
        profiler.processLog();
    } catch (const std::bad_alloc &) {
        return SHAREMIND_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return SHAREMIND_MODULE_API_0x1_MODULE_ERROR;
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
    SAMENAME(ProcessProfiler_newSectionType),
    SAMENAME(ProcessProfiler_startSection),
    SAMENAME(ProcessProfiler_endSection),
    SAMENAME(ProcessProfiler_flushLog)

);

} // extern "C" {
