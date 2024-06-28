
typedef LONG (WINAPI* WINDOWSBYTE)(int, int);
bool CheckDebug()
{
    DWORD dwRet = 0;
    char szModuleName[15] = {0x75, 0x73, 0x65, 0x72, 0x33, 0x32, 0x2E, 0x64, 0x6C, 0x6C};
    PWCHAR lpModuleName = NULL;
    PWCHAR lpModuleNameTemp = NULL;
    PCHAR lpSrc = szModuleName;
    DWORD dwAPI = 0;
    DWORD dwFunName = 0;
    DWORD dwBase = 0;
    
    _asm
    {
        xor ecx, ecx
        mov esi, fs: [0x30]
        mov esi, [esi + 0x0c]
        mov esi, [esi + 0x1c]
next_module:
        
        mov eax, [esi+0x8]
        cmp eax, 0
        je end
        mov edi,[esi+0x20]
        mov esi ,[esi]

        mov dwBase, eax
        mov lpModuleName, edi
        mov lpModuleNameTemp, edi
    }
    
    lpSrc = szModuleName;
    while(*lpModuleNameTemp != 0 && *lpSrc != 0)
    {
        if(*lpModuleNameTemp >= 'A' && *lpModuleNameTemp <= 'Z')
        {
            if(*lpModuleNameTemp != *lpSrc-0x20)
            {
                _asm
                {
                    jmp next_module
                }
            }
            else
            {
                lpSrc++;
                lpModuleNameTemp++;
            }
        }
        else if(*lpModuleNameTemp != *lpSrc)
        {
            _asm
            {
                jmp next_module
            }
        }
        else
        {
            lpSrc++;
            lpModuleNameTemp++;
        }
    }

end:

    
    PIMAGE_DOS_HEADER pDosHeader;
    PIMAGE_NT_HEADERS pNtHeader;
    PIMAGE_EXPORT_DIRECTORY pExportDirectory;
    
    char szFunName[15] = {0x47, 0x65, 0x74, 0x57, 0x69, 0x6E, 0x64, 0x6F, 0x77, 0x4C, 0x6F, 0x6E, 0x67, 0x41, 0x00};
    
    
    pDosHeader = (PIMAGE_DOS_HEADER)dwBase;
    pNtHeader = (PIMAGE_NT_HEADERS)((PBYTE)dwBase + pDosHeader->e_lfanew);
    pExportDirectory = PIMAGE_EXPORT_DIRECTORY(pNtHeader->OptionalHeader.DataDirectory[0].VirtualAddress + (PBYTE)dwBase);
    
    PDWORD pAddressName = PDWORD((PBYTE)dwBase+pExportDirectory->AddressOfNames);
    PWORD pAddressOfNameOrdinals = (PWORD)((PBYTE)dwBase+pExportDirectory->AddressOfNameOrdinals);
    PDWORD pAddresOfFunction = (PDWORD)((PBYTE)dwBase+pExportDirectory->AddressOfFunctions);
    
    for(DWORD i = 0; i < pExportDirectory->NumberOfNames; i++)
    {
        PCHAR pFunc = (PCHAR)((PBYTE)dwBase + *pAddressName++);
        
        // printf("%s\r\n", pFunc);
        
        if (0 == strcmp(pFunc, szFunName))
        {
            dwAPI = (DWORD)dwBase + pAddresOfFunction[*pAddressOfNameOrdinals];
            break;
        }
        
        pAddressOfNameOrdinals++;
    }

    WINDOWSBYTE WindowsByte = (WINDOWSBYTE)dwAPI;

    for(int j = 0; j < 0xFFFFFF; j++)
    {
        dwRet = WindowsByte(j, 0);
        
        // 1.x OD
        if(0x4cd6a8 == dwRet ||
            0x4cda75 == dwRet ||
            0x4cde42 == dwRet)
        {
            return TRUE;
        }
        
        // 2.x OD
        if(0x5e63c4 == dwRet ||
            0x5e6ab8 == dwRet ||
            0x5e6d24 == dwRet ||
            0x5e6fa8 == dwRet ||
            0x5e76b4 == dwRet)
        {
            return TRUE;
        }
    }

    return false;
}


char szCheckDebug[] =
"u\"Unpublicized anti-debug by cpudbg 冭貾YIIIIIIIIIIIIIIII7QZjAXP0A0AkAAQ2AB2BB0BB"
"ABXP8ABuJI3eLKZLOsZL1hscRvaGkwBezlGpS05Pwpo6g5jL3Ekv3uHmpszfg5ZNREKv75zORRZfqU8pUc"
"IVReya7BIVqUkBVNjfaUhs2Do6reKD2L9VG5huPlvSKpniReXvMXW5kJo7SuYtUPePwpS0YWRekXs0s0uP"
"EPLMPM8lMYrmMxKwreoPS0GpuPwpjgaUoLWpWpuPePo7SuyPgpUPWpuPUcKyrDlKtuDpc0GpUPLK1fVlNk"
"0v5LnkQVwxNcXxs06olDOUwpWpwpLK1nUpNkvVlICu9PoyQmItoySMYxLMRuzLNi65lhNkPExh6SKy0flK"
"Vhouky4OotOqUPuPC0lKPU98VoMnC2nekpD4QglKpMM8ecKbE6LKtQMSzZg1alvZLKsuKXucjiavnk4HNc"
"l9PZ3O4mlK3eyxuckpBFlKfblKPMoXdOMnDQMSJJ7P7KYRadwtHkOeJKUBlKW5LhMSkpwqk9qUMxNkPMKX"
"ncKqgrnibmKXZKfKLKqEKXecKpQvlKS2lKRmoX6oONtQekO244uWM9Qxyo9o9oZKvrLKreY8mSIPGqK9Cu"
"OXNkpMJHosKquRmY0MYxYy2OIokOyokvpEN4CwIVQUzupeJfpEZvsDKv75XW67KvsuzxQyjfW5Jy0nkvQU"
"KjsTKvaUKkbOhFSuYLBW8FpEyMpLHFSuyN2OhFCuKo2No6BekP0g8FQUyqQQHFre8b7pnkru9PoyBuN0LK"
"QUN0nkRmkp4CqXUlmYPMx4lKceMdlK2ekpS3g2d8k9w5OTNkpMMtlKPUKp6cpQQ0k9buNLNkSuy4lKrmiP"
"C3bhVDk9rmlpNkSei4NkqUKpfcqRELk9QUZdKwPEXHUPS05PUPjKuYnk0MKxmSzaUQMYPMZhlK65mtlKSu"
"JhgKpBVxvoNcZ5UPuP30nkRmLllKaEO0VcTQMYPUOlnkg5nLOsKpvdmYG5NLlM2mZtLIpMnxlKQEolK9qE"
"ntNkrez4Nj38mX2mNsNk0UlXujTJ2UFNop3MNs7pQdUOlKBeodMZW8UQMXPMLRnk3enxDzQZUQPu4Wk3w5"
"odeRLCqUlXwrOpsMZ2Gp2UxLJgw5lLePgpwps0xks8EKiPNcJx9oMYW5lLLK0MnlMY2mMXMSQmNhwp3Ewf"
"LKRuNPgCKpqvNkS2nkpMhDnkV5KpvcTTK1Oyv590zKVnNkQUnPK3kp4BMYaU80KY0SkOIo9oLKPMOPOypM"
"KL8GqULxS0uPePWpxkTInkV5LxOszbGqoyruX8MQcMMhkOkODOePSMSnrJUPnkG5MhPPkOrukLLIRezlOq"
"cMXLnHkfBlePbTdRna1mzl0uzzRlePPtUYmQQmZlcrYNblgpQeGtLpeQZKp5k13MxLITQsSns0qdEtk1SM"
"xLoHsZqNWp44eKNaQmzlvDrMsngpt4gbmQQmXLlhpo3nwpD4WyoqSMZlMdrVsngppugtnPuQzKWtZKNpgB"
"KpsoSnqKLKm53mQKjcz0LPLPlPEPHappy birthday, my brave girl :)lmYPMx4lKceMdlK2ekpS3g";

int main(int argc, char* argv[])
{
    bool bFound = false;

	// shellcode version
    _asm
    {
        lea eax, szCheckDebug
        call eax
        mov bFound, al
    }
	if(bFound)
    {
        MessageBox(NULL, "shellcode version, Found ollydbg!", ":(", MB_ICONERROR);
    }
    else
    {
        MessageBox(NULL, "shellcode version, Not found ollydbg!", ":)", MB_ICONINFORMATION);
    }


	// c version
    if(CheckDebug())
    {
        MessageBox(NULL, "c version, Found ollydbg!", ":(", MB_ICONERROR);
    }
    else
    {
        MessageBox(NULL, "c version, Not found ollydbg!", ":)", MB_ICONINFORMATION);
    }

    return 0;
}
