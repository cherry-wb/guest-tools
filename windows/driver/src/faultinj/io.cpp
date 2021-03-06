///
/// Copyright (C) 2018, Cyberhaven
/// All rights reserved.
///
/// Licensed under the Cyberhaven Research License Agreement.
///

#include <fltKernel.h>

extern "C" {
#include <s2e/s2e.h>
#include <s2e/GuestCodeHooking.h>

#include "../log.h"
#include "faultinj.h"
#include "apis.h"
}

#include "faultinj.hpp"

PIRP S2EHook_IoAllocateIrp(
    CCHAR StackSize,
    BOOLEAN ChargeQuota
)
{
    const UINT_PTR CallSite = reinterpret_cast<UINT_PTR>(_ReturnAddress());
    return FaultInjTemplate1<PIRP>(
        CallSite, "IoAllocateIrp", FALSE, nullptr, &IoAllocateIrp,
        StackSize, ChargeQuota
    );
}

PMDL S2EHook_IoAllocateMdl(
    PVOID VirtualAddress,
    ULONG Length,
    BOOLEAN SecondaryBuffer,
    BOOLEAN ChargeQuota,
    PIRP Irp
)
{
    const UINT_PTR CallSite = reinterpret_cast<UINT_PTR>(_ReturnAddress());
    return FaultInjTemplate1<PMDL>(
        CallSite, "IoAllocateMdl", FALSE, nullptr, &IoAllocateMdl,
        VirtualAddress, Length, SecondaryBuffer, ChargeQuota, Irp
    );
}

PIO_WORKITEM S2EHook_IoAllocateWorkItem(
    PDEVICE_OBJECT DeviceObject
)
{
    const UINT_PTR CallSite = reinterpret_cast<UINT_PTR>(_ReturnAddress());
    return FaultInjTemplate1<PIO_WORKITEM>(
        CallSite, "IoAllocateWorkItem", FALSE, nullptr, &IoAllocateWorkItem,
        DeviceObject
    );
}

PIRP S2EHook_IoBuildDeviceIoControlRequest(
    ULONG IoControlCode,
    PDEVICE_OBJECT DeviceObject,
    PVOID InputBuffer,
    ULONG InputBufferLength,
    PVOID OutputBuffer,
    ULONG OutputBufferLength,
    BOOLEAN InternalDeviceIoControl,
    PKEVENT Event,
    PIO_STATUS_BLOCK IoStatusBlock
)
{
    const UINT_PTR CallSite = reinterpret_cast<UINT_PTR>(_ReturnAddress());
    return FaultInjTemplate1<PIRP>(
        CallSite, "IoBuildDeviceIoControlRequest", FALSE, nullptr, &IoBuildDeviceIoControlRequest,
        IoControlCode, DeviceObject, InputBuffer, InputBufferLength, OutputBuffer, OutputBufferLength,
        InternalDeviceIoControl, Event, IoStatusBlock
    );
}

PIRP S2EHook_IoBuildSynchronousFsdRequest(
    ULONG MajorFunction,
    PDEVICE_OBJECT DeviceObject,
    PVOID Buffer,
    ULONG Length,
    PLARGE_INTEGER StartingOffset,
    PKEVENT Event,
    PIO_STATUS_BLOCK IoStatusBlock
)
{
    const UINT_PTR CallSite = reinterpret_cast<UINT_PTR>(_ReturnAddress());
    return FaultInjTemplate1<PIRP>(
        CallSite, "IoBuildSynchronousFsdRequest", FALSE, nullptr, &IoBuildSynchronousFsdRequest,
        MajorFunction, DeviceObject, Buffer, Length, StartingOffset, Event, IoStatusBlock
    );
}

NTSTATUS S2EHook_IoCreateDevice(
    PDRIVER_OBJECT DriverObject,
    ULONG DeviceExtensionSize,
    PUNICODE_STRING DeviceName,
    DEVICE_TYPE DeviceType,
    ULONG DeviceCharacteristics,
    BOOLEAN Exclusive,
    PDEVICE_OBJECT *DeviceObject
)
{
    const UINT_PTR CallSite = reinterpret_cast<UINT_PTR>(_ReturnAddress());
    return FaultInjTemplate1<NTSTATUS>(
        CallSite, "IoCreateDevice", FALSE, STATUS_INSUFFICIENT_RESOURCES, &IoCreateDevice,
        DriverObject, DeviceExtensionSize, DeviceName, DeviceType, DeviceCharacteristics, Exclusive, DeviceObject
    );
}

// This raises an exception
PFILE_OBJECT S2EHook_IoCreateStreamFileObjectLite(
    PFILE_OBJECT FileObject,
    PDEVICE_OBJECT DeviceObject
)
{
    const UINT_PTR CallSite = reinterpret_cast<UINT_PTR>(_ReturnAddress());
    return FaultInjTemplate1<PFILE_OBJECT>(
        CallSite, "IoCreateStreamFileObjectLite", FALSE, nullptr, &IoCreateStreamFileObjectLite,
        FileObject, DeviceObject
    );
}

NTSTATUS S2EHook_IoVerifyVolume(
    PDEVICE_OBJECT DeviceObject,
    BOOLEAN AllowRawMount
)
{
    const UINT_PTR CallSite = reinterpret_cast<UINT_PTR>(_ReturnAddress());
    return FaultInjTemplate1<NTSTATUS>(
        CallSite, "IoVerifyVolume", FALSE, STATUS_INSUFFICIENT_RESOURCES, &IoVerifyVolume,
        DeviceObject, AllowRawMount
    );
}

const S2E_GUEST_HOOK_LIBRARY_FCN g_kernelIoHooks[] = {
    S2E_KERNEL_FCN_HOOK("ntoskrnl.exe", "IoAllocateIrp", S2EHook_IoAllocateIrp),
    S2E_KERNEL_FCN_HOOK("ntoskrnl.exe", "IoAllocateMdl", S2EHook_IoAllocateMdl),
    S2E_KERNEL_FCN_HOOK("ntoskrnl.exe", "IoAllocateWorkItem", S2EHook_IoAllocateWorkItem),
    S2E_KERNEL_FCN_HOOK("ntoskrnl.exe", "IoBuildDeviceIoControlRequest", S2EHook_IoBuildDeviceIoControlRequest),
    S2E_KERNEL_FCN_HOOK("ntoskrnl.exe", "IoBuildSynchronousFsdRequest", S2EHook_IoBuildSynchronousFsdRequest),
    S2E_KERNEL_FCN_HOOK("ntoskrnl.exe", "IoCreateDevice", S2EHook_IoCreateDevice),
    S2E_KERNEL_FCN_HOOK("ntoskrnl.exe", "IoCreateStreamFileObjectLite", S2EHook_IoCreateStreamFileObjectLite),
    S2E_KERNEL_FCN_HOOK("ntoskrnl.exe", "IoVerifyVolume", S2EHook_IoVerifyVolume),
    S2E_KERNEL_FCN_HOOK_END()
};
