# Surds Calculator

A basic calculator I built to simplify surds and to factor out existing squares.

Limitations:
- Doesn't yet support brackets outside of the set on `sqrt()`
- Cannot have a space after the `sqrt` and before the `()`, it will error.
- Can only work with decimals inside of the `sqrt()` if they are equivalent to 1 over something.

Notes:
- `sqrt(10 + 10), sqrt(3 + 3)` is the syntax for splitting up expressions.
- Simplifies the insides of the square roots, whether they're on their own or they're in a multi-surd expression.
- The available operations are: `/` for division, `*` for multiplication, `-` for subtraction, and `+` for addition.
- Extracts the greatest common root from two roots when adding or subtracting them and shows it in a coefficient-surd format.
- `3sqrt(27)` means the 3rd root of 27 (if there is a space between the 3 and the `sqrt` then it will only do the square root)

To compile yourself:
- If on Linux, simply download the code and run `cmake . && make && ./surds`, as well as `x86_64-w64-mingw32-g++ main.cpp --static -o surds.exe`
- On Windows, change the line for the executable in the CMakeLists.txt to say `add_executable(surds.exe main.cpp)` and compile the project with CMake.
