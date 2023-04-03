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
  int count = 0;
  int line_num = 1;
  int nch;
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

  source[src_size -1] = '\0'; 

  fclose(input_file);

  struct Tokens tokens[src_size+1];

  while (index < src_size) {
    ch = source[index];
    nch = ch;

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
    // valida operador de multiplicacao e potenciacao.
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
        //o ultumo caractere nao deveria ser *
        printf("\nError on line %d\nInvalid token: '*' ", line_num);
      }
    }
    // verifica numeros inteiros e floats.
    else if (nch >= 48 && nch <= 57) {
      char *ch_number = malloc(1024);
      int n_size = 0;
      int dot = 0;
      int error = 0;
      ch_number[n_size] = ch;
      n_size++;
      count = index+1;
      while (count < src_size) {
        nch = source[count];
        ch = source[count];
        if (ch == ' ' || ch == '\t' || ch == '\n') {
          ch_number[n_size] = '\0';
          break;
        }
        else if (nch >= 48 && nch <= 57) {
          ch_number[n_size] = ch;
          n_size++;
        }
        else if (ch == '.' && dot) {
          printf("Error on line %d\nInvalid token: '.'", line_num);
          error = 1;
          break;
        }
        else if (ch == '.' && ! dot) {
          dot = 1;
          ch_number[n_size] = ch;
          n_size++;
        }
        else {
          break;
        }

        count++;
      }

      index = count - 1;

      if (error) return 1;

      if (dot) {
        tokens[t_ind].token = float_number;
        tokens[t_ind].float_num = atof(ch_number);
      }
      else {
        tokens[t_ind].token = int_number;
        tokens[t_ind].int_num = atoi(ch_number);
      }

      free(ch_number);
    }
    else {
      tokens[t_ind].token = ignorar;
      tokens[t_ind].char_val = ch;
    }

    t_ind++;
    index++;
  }

  for (int i=0; i<t_ind; i++) {
    if (tokens[i].token == int_number) printf("\nToken: %d", tokens[i].int_num);
    else if (tokens[i].token == float_number) printf("\nToken: %f", tokens[i].float_num);
    else printf("\nToken: %c", tokens[i].char_val);
  }
  printf("\n");

  free(source);

  return 0;
}
