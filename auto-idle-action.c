/*
 * Windows Auto-Idle Action Project (WAIAP)
 *
 * Written by Mig
 */
//#define TEST_ONLY
//#define DEBUG

#include <windows.h>
#include <stdio.h>

#define IDLE_TIME_SEC "3600"

int addToStartup() {
    HKEY hKey;
    const char* runKeyPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    const char* valueName = "AutoShutdown";

    char exePath[MAX_PATH];
    GetModuleFileName(NULL, exePath, MAX_PATH);

    if (RegOpenKeyEx(HKEY_CURRENT_USER, runKeyPath, 0, KEY_SET_VALUE, &hKey) != ERROR_SUCCESS) {
        return 1;
    }

    if (RegSetValueEx(hKey, valueName, 0, REG_SZ, (const BYTE*)exePath, strlen(exePath) + 1) != ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return 1;
    }

    RegCloseKey(hKey);
    return 0;
}

DWORD GetIdleTime() {
    LASTINPUTINFO lii;
    lii.cbSize = sizeof(LASTINPUTINFO);

    if (!GetLastInputInfo(&lii)) {
        return 0;
    }

    DWORD tickCount = GetTickCount();
    return (tickCount - lii.dwTime) / 1000;
}

int terminateWindows(char *termType)
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    if (!OpenProcessToken(GetCurrentProcess(),
                          TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
                          &hToken)) {
        return 1;
    }

    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);

    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

    if (GetLastError() != ERROR_SUCCESS) {
        return 1;
    }

    UINT flags = EWX_POWEROFF;
    if ((strcmp(termType, "logout") == 0) || (strcmp(termType, "logoff") == 0)) {
        flags = EWX_LOGOFF;
    }
    else
    if (strcmp(termType, "shutdown") == 0) {
        flags = EWX_SHUTDOWN;
    }
    else
    if (strcmp(termType, "poweroff") == 0) {
        flags = EWX_POWEROFF;
    }
    else
    if (strcmp(termType, "reboot") == 0) {
        flags = EWX_REBOOT;
    }

    if (!ExitWindowsEx(flags | EWX_FORCE, SHTDN_REASON_MAJOR_OTHER)) {
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    char path[MAX_PATH];
    DWORD result = GetModuleFileName(NULL, path, MAX_PATH);
    if (result == 0) {
	fprintf(stderr, "Error: Cannot retrieve the current path\n");
	fflush(stderr);
	return 1;
    }
    if (path == NULL) {
	fprintf(stderr, "Error: Cannot retrieve the current path\n");
	fflush(stderr);
	return 1;
    }
    path[ strlen(path) - 1] = 'i';
    path[ strlen(path) - 2] = 'n';
    path[ strlen(path) - 3] = 'i';
    
    addToStartup();

    char idle_time[256], action[256];
    GetPrivateProfileString("Settings", "IdleTime", IDLE_TIME_SEC, idle_time, sizeof(idle_time), path);
    GetPrivateProfileString("Settings", "Action", "poweroff", action, sizeof(action), path);

    int num = atoi(idle_time);

    printf(">>> Idle Time: %s (%d)\n", idle_time, num);

    while ( 1 ) {
	    if (GetIdleTime() > num) {
            if (terminateWindows(action) != 0) {
		        MessageBox(0, "Cannot shutdown Windows", "Error", MB_ICONERROR | MB_OK);
		    }
	    }
	    Sleep(1000);
    }
    return 0;
}

