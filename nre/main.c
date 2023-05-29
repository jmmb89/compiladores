#include <stdio.h>
#include <string.h>

/* NRE */

/* HEADER
42 | Acc | Pc | Payload (min 1 byte, max 256 bytes)


CMD | DEC | HEX  | COMENTARIO
_______________________________________

NOP | 00  | 0x00 | [Nenhuma operacao]
STA | 16  | 0x10 | [MEM(end)<- AC] 
LDA | 32  | 0x20 | [AC<-MEM(end)]
ADD | 48  | 0x30 | [AC<-MEM(end) + AC] 
OR  | 64  | 0x40 | [AC<-MEM(end) OR AC] 
AND | 80  | 0x50 | [AC<-MEM(end) AND AC] 
NOT | 96  | 0x60 | [AC<- NOT AC] 
JMP | 128 | 0x80 | [AC<-end]
JN  | 144 | 0x90 | [IF N=1 THEN PC<-end]
JZ  | 160 | 0xA0 | [IF Z=1 THEN PC<-end]
HLT | 240 | 0xF0 | [HALT]
________________________________________

EX ADD: 42, 0, 0, 32, END1, 48, END2, 16, END3, 240, END1, END2, END3

USAGE: ./nre <file.nar>

*/

#define MAX_VAL 256

int main(int argc, char** argv) {
  unsigned char buffer[MAX_VAL];
  int acc = 0;
  int pc = 0;
  int n_flag = 0; // acc have negative number
  int z_flag = 0; // acc is zero 
  int run = 1;

  if (argc < 2) {
    printf("\nUSAGE: ./nre <file.nar>\n");
    return 1;
  }

  FILE *input_file;
  input_file = fopen(argv[1], "rb");

  if (! input_file) {
    printf("\nError loading input file!\n");
    return 1;
  }

  //initialize array with 0s
  memset(buffer, 0, sizeof(buffer));

  // Load from file
  fread(buffer, sizeof(unsigned char), MAX_VAL, input_file);
  fclose(input_file);


  if (buffer[0] != 0x2A) {
    printf("\nMagic number 0x2A not found!\nAborting..\n\n");
    return 1;
  }

  // Load ACC and PC
  acc = buffer[1];
  pc = buffer[2];

  // raise acc flags
  z_flag = (acc == 0) ? 1 : 0;
  n_flag = (acc < 0) ? 1 : 0;

  while (pc < MAX_VAL && run) {
    switch (buffer[pc]) {
      // HLT (halt the program)
      case 0xF0:
        run = 0;
        break;
      // STA (store acc) 
      case 0x10:
        pc++;
        buffer[buffer[pc]] = acc;
        break;
      // LDA (load acc) [n,z]
      case 0x20:
        pc++;
        acc = buffer[buffer[pc]];
        z_flag = (acc == 0) ? 1 : 0;
        n_flag = (acc < 0) ? 1 : 0;
        break;
      // ADD [n,z]
      case 0x30:
        pc++;
        acc += buffer[buffer[pc]]; 
        z_flag = (acc == 0) ? 1 : 0;
        n_flag = (acc < 0) ? 1 : 0;
        break;
      // OR
      case 0x40:
        pc++;
        acc |= buffer[buffer[pc]];
        z_flag = (acc == 0) ? 1 : 0;
        n_flag = (acc <= 0) ? 1 : 0;
        break;
      // AND
      case 0x50:
        pc++;
        acc &= buffer[buffer[pc]];
        z_flag = (acc == 0) ? 1 : 0;
        n_flag = (acc <= 0) ? 1 : 0;
        break;
      // NOT
      case 0x60:
        acc = acc * -1;
        break;
      // JMP
      case 0x80:
        pc++;
        pc = buffer[pc];
        pc--; // to match the correct adress next itertion 
        break;
      // JN (jump on negative)
      case 0x90:
        if (n_flag) {
          pc++;
          pc = buffer[pc];
          pc--;
        }
        break;
      // JZ (jump zero)
      case 0xA0:
        if (z_flag) {
          pc++;
          pc = buffer[pc];
          pc--;
        }
        break;
      // NOP/BAD INSTRUCTIONS
      default:
        break;
    }
    pc++;
  }

  /* PROGRAM DETAILS */
  printf("\nACC: %d", acc);
  printf("\nPC: %d", pc);
  printf("\nN_FLAG: %d\n", n_flag);
  printf("\nZ_FLAG: %d", z_flag);

  printf("\nMEMORY MAP:\n\n");

  for (int i=0; i < 256; i++) {
    if (i != 0 && i % 16 == 0) printf("\n");
    printf("0x%02x ", buffer[i]);
  }
  printf("\n\n");

  return 0;
}
