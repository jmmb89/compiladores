#include <stdio.h>
#include <stdlib.h>

typedef enum Token {
  adicao,
  subtracao,
  multiplicacao,
  divisao,
  pow,
  int_number,
  float_number,
  ignorar,
  outros,
} Token;

struct Tokens {
  enum Token token;
  char char_val;
  int int_num;
  float float_num;
};


int main(int argc, char** argv) {
  int increase = 1024;
  int allocated = increase;
  int src_size = 0;
  int index = 0;
  int t_ind = 0;
  int line_num = 1;
  char ch;
  char *source = malloc(allocated);

  //validacoes de input
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

  //le do arquivo e adiciona os caractes a string.
  while ((ch = getc(input_file)) != EOF) {
    source[src_size] = ch;
    src_size++;

    //aumenta dinamicamente o tamanho da array segundo a necessidade.
    if (src_size >= allocated) {
      allocated += increase;
      source = realloc(source, allocated);
    }
  }

  //adiciona um espaco para o \0
  src_size++;

  //reduz ao espaco realmete utilizado.
  source = realloc(source, src_size);

  //adiciona o final simbolo de fim da string
  source[src_size -1] = '\0'; 

  fclose(input_file);

  struct Tokens tokens[src_size+1];

  while (index < src_size) {
    ch = source[index];
    if (ch == '\n') {
      line_num++; 
      tokens[t_ind].token = outros;
      tokens[t_ind].char_val = ch;
    }
    else if (ch == '+') {
      tokens[t_ind].token = adicao; 
      tokens[t_ind].char_val = ch;
    }
    else if (ch == '-') {
      tokens[t_ind].token = subtracao; 
      tokens[t_ind].char_val = ch;
    }
    else if (ch == '/') {
      tokens[t_ind].token = divisao; 
      tokens[t_ind].char_val = ch;
    }
    else if (ch == '*') {
      if (index+1 < src_size) {
        if (source[index+1] == '*') {
          tokens[t_ind].token = pow;
          tokens[t_ind].char_val = '^';
          index++;
        }
        else {
          tokens[t_ind].token = multiplicacao;
          tokens[t_ind].char_val = '*';
        }
      }
      else {
        tokens[t_ind].token = multiplicacao;
        tokens[t_ind].char_val = '*';
      }
    }
    else {
      tokens[t_ind].token = ignorar;
      tokens[t_ind].char_val = ch;
    }

    t_ind++;
    index++;
  }

  for (int i=0; i<t_ind; i++) {
    printf("\nToken: %c", tokens[i].char_val);
  }
  printf("\n");

  free(source);

  return 0;
}
