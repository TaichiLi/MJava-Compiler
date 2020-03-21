# MJava-Compiler

A compiler for Mini-Java

# Build

You can use `CMake` to build this project. Of course, the project is very simple, you can also compile programs directly from the command line.

If you use `CMake`, run

```
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make install
```

I use `MinGW Makefiles` here, but you can use others.

There are two `bat` files in the project directory, `lexer.bat` , `test.bat` . You can run them in `cmd`.

```
lexer.bat <Source File> [Output File]
```

Source file is required, and output file is `tokenOut.txt` by default.

Run `test.bat` , you will get the test result of lexer.