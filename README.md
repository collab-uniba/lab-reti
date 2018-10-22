# Lab package for Corso di Reti di Calcolatori

**Outline**
- [Development environment setup](#install-eclipse)
- [Socket API reference (cheat sheet)](socket_API_reference.pdf)
- [Client/server C source template](template.c)
- [Exercises](exercises/)

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
4. To configure the `lldb` debugger (_Xcode_ own debugger), first launch Eclipse; then:
    1. Go to _Help > Install new Software_
    2. Select the _CDT update site_ (9.1 or greater) from the dropdown list
    3. Under _CDT Optional Features_, select **_C/C++ LLDB Debugger Integration_**

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
1. Install [chocolatey](https://chocolatey.org/install)
2. From the command line, run:
```bash
> choco install mingw -y
```
3. Double-check that everything works by running:
```bash
> refreshenv
> gcc --version
```

Those who might have issues in setting up _MinGW_, may instead install
the [Windows Subsystem for Linux (WSL)](https://docs.microsoft.com/en-us/windows/wsl/install-win10) and use `gcc` from the Bash shell, after also following the instructions above.
