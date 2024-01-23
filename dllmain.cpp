#include "pch.h"
#include <stdlib.h>
#include <conio.h>
#include <iostream>

#define KEYPAD_2 98

#include <Windows.h>
void setCursorCenter() {
    // Get the handle of the primary monitor
    HMONITOR primaryMonitor = MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY);

    // Get the monitor info
    MONITORINFO monitorInfo;
    monitorInfo.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(primaryMonitor, &monitorInfo);

    // Calculate the center coordinates
    int centerX = monitorInfo.rcMonitor.left + (monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left) / 2;
    int centerY = monitorInfo.rcMonitor.top + (monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top) / 2;

    // Set the cursor position to the center
    SetCursorPos(centerX, centerY);
        // Send a left mouse button down and up event
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}


// Keyboard hook procedure

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    // Check if it is a keypress event and not a key release event
    if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
        // Cast the lParam to KBDLLHOOKSTRUCT pointer
        KBDLLHOOKSTRUCT* pKeyboardHookStruct = (KBDLLHOOKSTRUCT*)lParam;

        // Check if the key press event is from a USB keyboard
        if ((pKeyboardHookStruct->flags & LLKHF_INJECTED) == 0 && (pKeyboardHookStruct->flags & LLKHF_LOWER_IL_INJECTED) == 0) {
            // Print the virtual key code of the USB key that was pressed
            std::cout << "USB Key Pressed: " << pKeyboardHookStruct->vkCode << std::endl;
        }

        if (pKeyboardHookStruct->vkCode == KEYPAD_2) {
            setCursorCenter();
            
        }

    }



    // Pass the event to the next hook procedure in the hook chain
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int setHook() {

    std::cout << "Keyboard Hook Set.";

    // Install the hook procedure
    HHOOK hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);

    if (hHook == NULL) {
        std::cout << "Failed to install the keyboard hook." << std::endl;
        return 1;
    }

    // Message loop to keep the program running
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Uninstall the hook procedure
    UnhookWindowsHookEx(hHook);

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        setHook();
        break;

    case DLL_PROCESS_DETACH:
        MessageBox(NULL, TEXT("Goodbye Cruel World"), TEXT("In a DLL"), MB_OK);
        break;
    }
    return TRUE;
}
