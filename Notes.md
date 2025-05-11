# Brainfuck Interpreter Notes

Attempt to implement the language at the level of an abstract syntax tree.

Using data types to describe possible behaviors, then generate those behaviors.

Brainfuck has 8 possible commands:

| Character | Behavior |
|-----------|----------|
| ```>``` | Increment the data pointer by one (to point to the next cell to the right) |
| ```<``` | Decrement the data pointer by one (to point to the next cell to the left) |
| ```+``` | Increment (increase by one) the byte at the data pointer |
| ```-``` | Decrement (decrease by one) the byte at the data pointer |
| ```.``` | Output the byte at the data pointer |
| ```,``` | Accept one byte of input, storing its value in the byte at the data pointer |
| ```[``` | If the byte at the data pointer is zero, then instead of moving the instruction pointer forward to the next command, jump it forward to the command after the matching ] command |
| ```]``` | If the byte at the data pointer is nonzero, then instead of moving the instruction pointer forward to the next command, jump it back to the command after the matching [ command |

This means I must have a data type that allows me to represent each of the different possible commands, then a function that executes the behavior of each command as it processes the abstract syntax tree.

## Language and Implementation Constraints

- A Brainfuck program is a sequence of commands; any character that is not a command is considered a comment and is ignored.
- The source code of a program is a single ASCII text file with the file type ".b".
- A program uses a pointer that moves around the cells, which must be at least 30,000. Each cell contains a one-byte integer (0-255). The pointer starts at cell 0. Each cell is initialized to 0.
- The control flow, is as follows.
  - Control begins at the first command in the program.
  - When a command has control, it is executed, and control passes to the next command (to the right, unless the command is a loop).
  - When no command has control, the program terminates.
- The only possible syntax error is the imbalance of ```[``` and ```]```.
- When the output value given by the ```.``` command does not fit in a byte, it is reduced modulo 256.
- The bytes that are input or output by the program are ASCII characters and are displayed in a monospaced font.
- Output and Input are interleaved in chronological order.

## Design desitions

cells = 30000
