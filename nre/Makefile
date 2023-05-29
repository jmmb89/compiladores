# Makefile

.PHONY: help clean all build 
.SUFFIXES: .c .o 

CC=gcc
CFLAGS=-Wall -Wextra -pedantic-errors -std=c99
LDFLAGS= 
LDLIBS=

BIN=nre
SRC=main.c
OBJ=$(SRC:.c=.o)

help: 
	@echo 
	@echo "Uso: make <opção>"
	@echo  
	@echo "    help      : mostra esta mensagem"
	@echo "    clean     : elimina arquivos gerados pelo compilador"
	@echo "    all       : cria binário executável e limpa diretorio" 
	@echo "    build     : cria binário executável" 
	@echo "    run       : roda o execuavel sobre o input (input.txt)" 
	@echo

run: 
	@./main out.nre

clean:
	@echo "\nLimpando diretorio..\n" 
	@rm -f *.o

build: $(BIN)

all: $(BIN)
	@rm *.o

.c.o:
	@echo "> Compilando [$<]"
	$(CC) $< $(CFLAGS) -c -o $@ 
	@echo

$(BIN): $(OBJ)
	@echo "> Gerando binário executável [$@]"
	$(CC) $^ $(LDFLAGS) $(LDLIBS) -o $@
	@echo

$(OBJ): $(SRC) 
