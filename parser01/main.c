#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
<expressao> :== <termo> | E
<termo> :== <numero> | <numero> + <numero>
<numero> :== inteiro | real
*/


typedef enum Token {
  add,
  sub,
  mult,
  divi,
  pow,
  int_number,
  float_number,
  string,
  ignore,
  others,
  new_line,
} Token;

struct Tokens {
  enum Token token;
  char char_val;
  int int_num;
  float float_num;
  char str[32];
};

int integer(struct Tokens c_token) {
  if (c_token.token == int_number) return 1;
  return 0;
}

int real(struct Tokens c_token) {
  if (c_token.token == float_number) return 1;
  return 0;
}

int number(struct Tokens c_token) {
  if (integer(c_token) || real(c_token)) return 1;
  return 0;
}

int term(struct Tokens c_token) {
  if (number(c_token)) return 1; 
  return 0;
}

int expression(struct Tokens c_token) {
  if (term(c_token)) return 1;
  return 0;
}

int main(int argc, char** argv) {
  int increase = 1024;
  int allocated = increase;
  int src_size = 0;
  int index = 0;
  int t_ind = 0;
  int count = 0;
  int error = 0;
  int line_num = 1;
  int nch;
  char ch;
  char *source = malloc(allocated);

  //input validation
  if (argc < 2) {
    printf("\nUSAGE: ./main <input_file>\n\n");
    return 1;
  }

  FILE *input_file;
  input_file = fopen(argv[1],"r");

  if (!input_file) {
    printf("\nError loading input file!\n");
    return 1;
  }

  while ((ch = getc(input_file)) != EOF) {
    source[src_size] = ch;
    src_size++;

    //if needed increase memory allocation
    if (src_size >= allocated) {
      allocated += increase;
      source = realloc(source, allocated);
    }
  }

  //add space to \0
  src_size++;

  //trim extra unused space
  source = realloc(source, src_size);

  source[src_size -1] = '\0'; 

  fclose(input_file);

  struct Tokens tokens[src_size+1];

  while (index < src_size) {
    ch = source[index];
    nch = ch;

    if (ch == '\n') {
      line_num++; 
      tokens[t_ind].token = new_line;
      tokens[t_ind].char_val = ch;
    }
    else if (ch == '+') {
      tokens[t_ind].token = add; 
      tokens[t_ind].char_val = ch;
    }
    else if (ch == '-') {
      tokens[t_ind].token = sub; 
      tokens[t_ind].char_val = ch;
    }
    else if (ch == '/') {
      tokens[t_ind].token = divi; 
      tokens[t_ind].char_val = ch;
    }
    //mult and pow operators
    else if (ch == '*') {
      if (index+1 < src_size) {
        if (source[index+1] == '*') {
          tokens[t_ind].token = pow;
          tokens[t_ind].char_val = '^';
          index++;
        }
        else {
          tokens[t_ind].token = mult;
          tokens[t_ind].char_val = '*';
        }
      }
      else {
        //last character shouldnt be *
        printf("\nError on line %d\nInvalid token: '*' ", line_num);
        break;
      }
    }
    //verify is digit 
    else if (nch >= 48 && nch <= 57) {
      char *ch_number = malloc(1024);
      int n_size = 0;
      int dot = 0;
      int is_string = 0;
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
        //in case the string starts with numbers.
        else {
          is_string = 1;
          ch_number[n_size] = ch;
          n_size++;
        }

        count++;
      }

      index = count - 1;

      if (error) return 1;

      if (is_string) {
        tokens[t_ind].token = string;
        strcpy(tokens[t_ind].str, ch_number);
      }
      else {
        if (dot) {
          tokens[t_ind].token = float_number;
          tokens[t_ind].float_num = atof(ch_number);
        }
        else {
          tokens[t_ind].token = int_number;
          tokens[t_ind].int_num = atoi(ch_number);
        }
      }

      free(ch_number);
    }
    else {
      //ignore spaces, tabs and end of string char
      if (nch == 32 || nch == 9 || nch == 0) {
        tokens[t_ind].token = ignore;
        tokens[t_ind].char_val = ch;
      }
      else { 
        tokens[t_ind].token = string;
        int str_size = 0;
        while (nch != ' ' && nch != '\t' && nch != '\n') {
          tokens[t_ind].str[str_size] = ch;
          str_size++;
          index++;
          ch = source[index];
          nch = ch;
        }
        tokens[t_ind].str[str_size] = '\0';
      }
    }

    t_ind++;
    index++;
  }

  for (int i=0; i<t_ind; i++) {
    if (expression(tokens[i])) {
      if (tokens[i].token == int_number) printf("\nExpressao valida: %d", tokens[i].int_num); 
      else if (tokens[i].token == float_number) printf("\nExpressao valida: %f", tokens[i].float_num); 
    }
    else if (tokens[i].token != new_line && tokens[i].token != ignore) printf("\nExpressao invalida: %s", tokens[i].str);
  }
  printf("\n");

  free(source);

  return 0;
}
