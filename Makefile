.DEFAULT_GOAL := run

build:
	gcc eval.c lex.c

run: build
	./a.out