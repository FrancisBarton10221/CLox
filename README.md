# CLox
A Lox interpreter using the C language based on Robert Nystroms book "Crafting Interpreters".
Nystrom uses an OS X machine however my implementation is Windows based.

# How does the interpreter work?
## Scanner 
The scanner can work in one of two ways: A REPL (Read Evauluate Print Loop) or by passing in the desired file through a command line argument.
## Parser 
Nystrom implements a pratt parser and I have chosen to follow in his steps with my own implementation. It works by recursively calling parsePrecedence().
This function evaluates the precedence of it's argument and compares it to the precedence of the infix operator to it's right. If it evauluates true then the infix
operator is called, the operation is performed and parsePrecedence() is called again. Otherwise the loop breaks and if parsePrecedence() is called from a binary operation, 
the operation occurs without calling another operation.
To understand how a Pratt Parser works it is best to think about how it would appear as a list of bytecode instructions and how they would interact with the stack stack.
## Virtual Machine
## Garbage collector.

