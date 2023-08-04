#pragma comment(lib, "ntdll.lib")
#include <ntddk.h>

void SampleUnload(_In_ PDRIVER_OBJECT Driver_Object) {
	UNREFERENCED_PARAMETER(Driver_Object);

    KdPrint(("Sample driver unload!\n"));

}

extern "C"
NTSTATUS
DriverEntry(_In_ PDRIVER_OBJECT Driver_Object, _In_ PUNICODE_STRING Registry_Path) {
    UNREFERENCED_PARAMETER(Registry_Path);
    NTSTATUS status = STATUS_SUCCESS;
	Driver_Object->DriverUnload = SampleUnload;

    RTL_OSVERSIONINFOW osVers;
    osVers.dwOSVersionInfoSize = sizeof(osVers);
    status = RtlGetVersion(&osVers);

    KdPrint(("Version: %u.%u.%u", osVers.dwMajorVersion, osVers.dwMinorVersion, osVers.dwBuildNumber));
    KdPrint(("Sample driver initialized successfully!\n"));

	return STATUS_SUCCESS;
}

/*
Deploying the driver:
sc create sample type= kernel binpath= C:\Users\ph03n1x\Desktop\windows_kernel_programming_exercises\Chapter_2\Sample\x64\Debug\Sample\Sample.sys

Result in CMD:
[SC] CreateService SUCCESS

Confirming the driver was installed successfully:

reg query HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\sample

HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\sample
	Type    REG_DWORD    0x1
	Start    REG_DWORD    0x3
	ErrorControl    REG_DWORD    0x1
	ImagePath    REG_EXPAND_SZ    \??\C:\Users\ph03n1x\Desktop\windows_kernel_programming_exercises\Chapter_2\Sample\Debug\Sample\Sample.sys

Starting the driver:
sc start sample - fails

Enter test signing mode:
bcdedit /set testsigning on

Note: in the project settings set the platform for x64, disabled Spectre mitigations.

SERVICE_NAME: sample
        TYPE               : 1  KERNEL_DRIVER
        STATE              : 4  RUNNING
                                (STOPPABLE, NOT_PAUSABLE, IGNORES_SHUTDOWN)
        WIN32_EXIT_CODE    : 0  (0x0)
        SERVICE_EXIT_CODE  : 0  (0x0)
        CHECKPOINT         : 0x0
        WAIT_HINT          : 0x0
        PID                : 0
        FLAGS              :

sc stop sample
SERVICE_NAME: sample
        TYPE               : 1  KERNEL_DRIVER
        STATE              : 1  STOPPED
        WIN32_EXIT_CODE    : 0  (0x0)
        SERVICE_EXIT_CODE  : 0  (0x0)
        CHECKPOINT         : 0x0
        WAIT_HINT          : 0x0

To use KdPrint, need to create the following key with value 8:

C:\Users\ph03n1x>reg query "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\Debug Print Filter"

HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\Debug Print Filter
    DEFAULT    REG_DWORD    0x8

To view in DbgView, Capture -> Capture Kernel and you can receive the messages.
*/