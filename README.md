# Surds Calculator

A basic calculator I built to simplify surds and to factor out existing squares.

Limitations:
- Cannot yet perform operations with multiple surds as operands, i.e. `sqrt(x) + sqrt(y)`
- Can only work with decimals inside of the `sqrt()` if they are equivalent to 1 over something.
- Cannot have a space after the `sqrt` and before the `()`, it will error.

Notes:
- `sqrt(10 + 10), sqrt(3 + 3)` works the same as `sqrt(10 + 10) sqrt(3 + 3)` or with any other symbols in between for the time being.
- Simplifies the insides of the square roots.
- `3sqrt(27)` means the 3rd root of 27 (if there is a space between the 3 and the `sqrt` then it will only do the square root)
- Any given expression that isn't a surd won't be evaluated.

To-Do:
- Make this work with multi-surd expressions.

