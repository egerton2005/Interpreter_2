# JSON interpreter for the SM stack machine

The project interprets the stack-machine language described at
<https://dboulytchev.github.io/sm.html>. A program is a JSON array of
instructions.

## Instruction format

```json
[
  "READ",
  { "ST": "x" },
  { "LD": "x" },
  { "CONST": 2 },
  { "BINOP": "+" },
  "WRITE"
]
```

| Instruction | Effect |
| --- | --- |
| `"READ"` | Read an integer and push it onto the stack. |
| `"WRITE"` | Pop and print the top value. |
| `{ "LD": "x" }` | Push the value of variable `x`. |
| `{ "ST": "x" }` | Pop a value and store it in `x`. |
| `{ "CONST": 42 }` | Push a constant. |
| `{ "BINOP": "+" }` | Pop `rhs`, then `lhs`, and push `lhs + rhs`. |
| `{ "LABEL": "name" }` | Declare a jump target. |
| `{ "JMP": "name" }` | Jump unconditionally. |
| `{ "JZ": "name" }` | Pop a value and jump if it is zero. |
| `{ "JNZ": "name" }` | Pop a value and jump if it is non-zero. |

Supported binary operators are `+`, `-`, `*`, `/`, `%`, `<`, `<=`, `>`,
`>=`, `==`, `!=`, `&&`, and `!!`. The spelling `!!` for logical OR matches
the reference language.

## Build and run

```bash
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build --parallel
echo 5 | ./build/bin/interpreter example/factorial.json
```

The factorial example prints `120` for input `5`.

Run the tests with:

```bash
ctest --test-dir build --output-on-failure
```

## Architecture

```text
JSON input
    -> JsonParser
    -> Program (typed instruction objects)
    -> Machine
       -> program counter and label table
       -> value stack
       -> variable environment
       -> input/output
```

Instructions use a visitor, following the structure of the first interpreter.
The machine increments the program counter before executing an instruction;
jump instructions replace it with the target label position.
