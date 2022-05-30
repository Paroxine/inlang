.DEFAULT_GOAL := run

build:
	gcc eval.c lex.c

run: build
	ifdef file
		./a.out $(file)
	else
		./a.out
	endif
	