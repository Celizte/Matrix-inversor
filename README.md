# Matrix-inversor
Program that inverses any matrix, i guess?

## Features
Invert any matrix size with the Gauss–Jordan method, step by step! from 1x1 up to 32x32 (you don't want to type any more, however, if you do then you can change the `MAX_BUFFER_SIZE` flag in main.c, not recommended)

## How to use
### Linux
- Open your preferred terminal.
- Head to the folder where Matrix_Inversor_(VERSION) is at.

``` bash
$ chmod +x Matrix_Inversor*
$ ./Matrix-Inversor
```

### Windows
- I don't knowww
- Download git bash, it might also work with cmd or powershell
- Open it, idk how

## Building
- Download git, git bash if you're on windows

``` bash
$ git clone https://github.com/Celizte/Matrix-inversor.git
$ cd Matrix-inversor
$ mkdir build
$ gcc -I./include src/*.c -o build/a.out
```
