# Lab package for Corso di Reti di Calcolatori

**Outline**
- [Development environment setup](#install-eclipse)
- [Socket API reference (cheat sheet)](socket_API_reference.pdf)
- [Client/server C source template](template.c)
- [Exercises](exercises/)

## Install Eclipse
Download _'Eclipse IDE for C/C++ Developers'_ from [http://www.eclipse.org/downloads/packages](http://www.eclipse.org/downloads/packages). Make sure to select the right package for your architecture and OS.

## Install GCC
The installation of the GNU C Compiler depends on your OS.

### Mac OS X
1. Install `homebrew` from  [https://brew.sh](https://brew.sh)
2. Install Xcode Command Line tools (without Xcode). If you have Xcode already installed, this step is likely unnecessary.
From the terminal, run the following and then select `Install`:
```bash
$ xcode-select --install
```
3. Double-check that everything works by running:
```bash
$ gcc --version
```

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
1. Install `chocolatey` from [https://chocolatey.org/install](https://chocolatey.org/install)
2. From the command line, run:
```bash
> choco install mingw -y
```
3. Double-check that everything works by running:
```bash
> refreshenv
> gcc --version
```
