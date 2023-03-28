#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 30000

typedef enum Token {
  adicao,
  subtracao,
  multiplicacao,
  divisao,
  outros,
} Token;

struct Tokens {
  enum Token token;
  char val;
};


int main(int argc, char** argv) {
  struct Tokens a_tokens[MAXSIZE];
  char ch;
  int a_size = 0;
  int i = 0;
  

  if (argc < 2) {
    printf("\nUSAGE: ./main <input_file>\n");
    return 1;
  }
  
  FILE *input_file;
  input_file = fopen(argv[1],"r");

  if (!input_file) {
    printf("\nError loading input file!\n");
    return 1;
  }

  while ((ch = getc(input_file)) != EOF) {
    if (ch == '+') {
      a_tokens[i].token = adicao;
      a_tokens[i].val = ch;
    }
    else if (ch == '-') {
      a_tokens[i].token = subtracao; 
      a_tokens[i].val = ch;
    }
    else if (ch == '*') { 
      a_tokens[i].token = multiplicacao; 
      a_tokens[i].val = ch;
    }
    else if (ch == '/') {
      a_tokens[i].token = divisao; 
      a_tokens[i].val = ch;
    }
    else {
      a_tokens[i].token = outros;
      a_tokens[i].val = ch;
    }

    i++;
  }

  a_size = i;

  for (i=0; i < a_size; i++) {
    if (a_tokens[i].token == adicao) printf("\nAdicao: %c", a_tokens[i].val);
    else if (a_tokens[i].token == subtracao) printf("\nSubtracao: %c", a_tokens[i].val);
    else if (a_tokens[i].token == multiplicacao) printf("\nMultiplicacao: %c", a_tokens[i].val);
    else if (a_tokens[i].token == divisao) printf("\nDivisao: %c", a_tokens[i].val);
    else printf("\nOutros: %c", a_tokens[i].val);
  }
  printf("\n");

  return 0;
}
