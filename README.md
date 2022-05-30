# IN104 - Creating your own language

## Usage

The makefile has two target : *build* and *run*. \
There is one command line argument : the input file's name.
To run with the Makefile (no filename will default to *showcase.txt*):
```
make run file="filename.txt"
```
or
```
make run
```

## File Structure
- **eval.c** contains the evaluator and the main function
- **parse.c** contains the parser functions
- **lex.c** contains the lexer functions 
- **.txt** are for testing purposes