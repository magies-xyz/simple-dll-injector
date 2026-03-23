Simple DLL Injector written in C
A minimal DLL injector written in C using the Windows API.  
Implements injection through **CreateRemoteThread + LoadLibraryA**.
Features
- Injects a DLL into a target process using its PID
- Uses pure WinAPI (no external libraries)
- Debug output with success/failure info
- Lightweight (take it or leave it 70 lines of code, very shitty tho)
- still a cli however, because im lazy deal with it
How It Works
1. Opens the target process
2. Allocates memory inside it
3. Writes the DLL path into that memory
4. Gets address of `LoadLibraryA`
5. Creates a remote thread to execute it
6. DLL gets loaded inside target process

### Build injector:
gcc dll-injector.c -o dll.exe
### to Build the dll
gcc -shared -o example.dll exampledll.c -luser32
usage:
- use it as dll.exe <use the full path here, even if its on the same folder> <pid>
- all credits to leet cipher: https://www.youtube.com/watch?v=RasdnQmM3IY
- most of  the code i took from his video, only translating it to C


