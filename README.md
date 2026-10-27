# Lab package for Corso di Reti di Calcolatori

**Outline**
- [Development environment setup](#install-eclipse)
- [Socket API reference (cheat sheet)](socket_API_reference.pdf)
- [Client/server C source template](template.c)
- [Lab exercises](exercises/)
<!-- - [Lab exams](lab-exams/) -->

## Install Eclipse
Download _Eclipse IDE for C/C++ Developers_ from [http://www.eclipse.org/downloads/packages](http://www.eclipse.org/downloads/packages). Make sure to select the right package for your architecture and OS.

## Install GCC
The installation steps for the _GNU C Compiler (GCC)_ depend on your OS.

### Mac OS X
1. Install [Homebrew](https://brew.sh)
2. Install _Xcode Command Line tools_ (without _Xcode_). If you have _Xcode_ already installed, this step is likely unnecessary.
From the terminal, run the following and then select `Install`:
```bash
$ xcode-select --install
```
3. Double-check that everything works by running:
```bash
$ gcc --version
```
4. To configure the `lldb` debugger (_Xcode_ own debugger), first launch _Eclipse_; then:
    1. Go to _Help > Install new Software_
    2. Select the _CDT update site_ (9.5 or greater) from the dropdown list
    3. Under _CDT Optional Features_, select **_C/C++ LLDB Debugger Integration_**
    4. From now on, for each project, right-click to show the context menu and select _Debug as > Debug Configurations..._; then, change or update the debug configuration from _GDB_ to _LLDB_, as shown in the pic below.
    
    ![alt text](https://wiki.eclipse.org/images/b/ba/Lldb-set-delagate.png)
    
    5. For more information, see the [Official wiki support page](https://wiki.eclipse.org/CDT/User/FAQ#How_do_I_get_the_LLDB_debugger.3F).
 

### Linux (Debian derivative distros)
1. From the terminal, run:
```bash
$ apt install build-essential -y
```
2. Double-check that everything works by running:
```bash
$ gcc --version
```

### Windows
Use command-line package managers, such as [chocolatey](https://chocolatey.org/install) or [WinGet](https://learn.microsoft.com/en-us/windows/package-manager/winget/)

#### Chocolatey
From the command line, run:
```bash
> choco install mingw -y
```

#### WinGet
From the command line, run:
```bash
> winget install MSYS2.MSYS2
```
Then, run `MSYS2` to open the MSYS2 terminal and execute this command:
```bash
> pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
```

After installing _MinGW_, double-check that everything works by running in the Windows terminal:
```bash
> refreshenv
> gcc --version
```

Those who might have issues in setting up _MinGW_ may instead install
the [Windows Subsystem for Linux (WSL)](https://docs.microsoft.com/en-us/windows/wsl/install-win10) and use `gcc` from the Bash shell, after also following the instructions above.

#### Link the Winsock library
On Windows, socket programs must be linked with the Winsock 2 library `ws2_32`:
- in _Eclipse CDT_: _Project > Properties > C/C++ Build > Settings > MinGW C Linker > Libraries_, then add `ws2_32` (without the `lib` prefix and the `.a` extension);
- from the command line: `gcc client.c -o client -lws2_32`.

The sources in this repository select the Windows code with `#if defined _WIN32` and include `winsock2.h`, plus `ws2tcpip.h` for `getaddrinfo()` and `getnameinfo()`.
