// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "minhook/minhook.h"
#include <cstdio>

typedef void(__fastcall* CreateJSOEntry_Number9485D0)(
    __int64 pObject,
    const char* key,
    int value
);
typedef void(__fastcall* CreateJSOEntry_Boolean948510)(
    __int64 pObject,
    const char* key,
    bool value
);
typedef __int64(__fastcall* sub_77D5B0)(__int64 a1, int a2, __int64 a3, __int64 a4, __int64 a5, __int64 a6, __int64 a7);

CreateJSOEntry_Number9485D0 o_CreateJSOEntry_Number9485D0 = nullptr;
CreateJSOEntry_Boolean948510 o_CreateJSOEntry_Boolean948510 = nullptr;
sub_77D5B0 o_sub_77D5B0 = nullptr;
__int64 __fastcall hk_sub_77D5B0(__int64 a1, int a2, __int64 a3, __int64 a4, __int64 a5, __int64 a6, __int64 a7)
{

    if (!strcmp((const char*)a3, "#SFUI_Notice_CTs_Clinched_Match") || !strcmp((const char*)a3, "#SFUI_Notice_Ts_Clinched_Match"))
        return o_sub_77D5B0(a1, a2, (__int64)"#SFUI_match_abort_match_abnormal_behavior", a4, a5, a6, a7);


    return o_sub_77D5B0(a1, a2, a3, a4, a5, a6, a7);
}
void __fastcall hk_CreateJSOEntry_Boolean948510(
    __int64 pObject,
    const char* key,
    bool value
) {

    if (!strcmp(key, "match_cancelled"))
        return o_CreateJSOEntry_Boolean948510(pObject, key, true);

    return o_CreateJSOEntry_Boolean948510(pObject, key, value);
}

void __fastcall hk_CreateJSOEntry_Number9485D0(
__int64 pObject,
const char* key,
int value
) {

    if(!strcmp(key, "cancel_reason_code"))
        return o_CreateJSOEntry_Number9485D0(pObject, key, 3);

    return o_CreateJSOEntry_Number9485D0(pObject, key, value);
}
BOOL WINAPI thread(HMODULE hMod) {

    AllocConsole();
    FILE* fDummy;
    freopen_s(&fDummy, "CONIN$", "r", stdin);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONOUT$", "w", stdout);

    MH_Initialize();
    /*
    xref: match_cancelled
    */
    MH_CreateHook((LPVOID)((uintptr_t)GetModuleHandleA("client.dll") + 0x9485D0), (LPVOID)&hk_CreateJSOEntry_Number9485D0, (LPVOID*)(&o_CreateJSOEntry_Number9485D0));
    /*
    xref: cancel_reason_code
    */
    MH_CreateHook((LPVOID)((uintptr_t)GetModuleHandleA("client.dll") + 0x948510), (LPVOID)&hk_CreateJSOEntry_Boolean948510, (LPVOID*)(&o_CreateJSOEntry_Boolean948510));
    /*
    xref: #SFUI_Notice_CTs_Clinched_Match or #SFUI_Notice_Ts_Clinched_Match.
    There will be function below using the variable which contains one of those strings
    */
    MH_CreateHook((LPVOID)((uintptr_t)GetModuleHandleA("server.dll") + 0x77D5B0), (LPVOID)&hk_sub_77D5B0, (LPVOID*)(&o_sub_77D5B0));
    MH_EnableHook(NULL);

    while (true) {
        Sleep(50);
    }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)thread, 0, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

