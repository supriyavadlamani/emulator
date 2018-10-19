/*
Disassemble an assembly code as per the 8080 instruction set
*/
#include <stdio.h>
#include <stdlib.h>

char *opcode(int instruction) {
  switch (instruction) {
    case 0:
      return "NOP";
      break;
    case 1:
      return "LXI B,D16";
      break;
    case 2:
      return "STAX B";
      break;
    case 3:
      return "INX B";
      break;
    case 4:
      return "INR B";
      break;
  }

  return NULL;
}

int instruction_size(int instruction) {
  int size = 0;

  switch (instruction) {
    case 1:
      size = 3;
      break;
    default:
      size = 1;
      break;
  }
  return size;
}

int disassemble8080(unsigned char *code_buffer, long code_size)
{
  int inst_size = 1;
  int pc = 0;
  unsigned char arg1 = '\0';
  unsigned char arg2 = '\0';

  while(pc < code_size) {
    inst_size = instruction_size(code_buffer[pc]);
    switch(inst_size) {
      case 3:
        arg1 = code_buffer[pc+2];
        arg2 = code_buffer[pc+1];
        break;
      case 2:
        arg1 = code_buffer[pc+1];
        arg2 = '\0';
        break;
      case 1:
        arg1 = '\0';
        arg2 = '\0';
        break;
    }

    printf("%04x %02x %02x %02x %s\n", pc, code_buffer[pc], arg1, arg2, opcode(code_buffer[pc]));
    pc = pc + inst_size;
  }
  return 0;
}

int NUM_INPUTS = 1;
int main(int argc, char **argv) {
  FILE *fp;
  long size = 0;
  long bytes_read = 0;
  unsigned char *buffer = NULL;

  if (argc < NUM_INPUTS + 1) {
    printf("Pass in path to the assembly code\n");
    exit(1);
  }

  printf("Opening file %s\n", argv[argc-1]);
  fp = fopen(argv[argc-1], "rb");
  if (fp == NULL) {
    printf("Couldn't open the file %s\n", argv[argc-1]);
    exit(2);
  }

  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  printf("code_size %ld\n", size);
  rewind(fp);

  buffer = (unsigned char*) malloc(size);
  if (buffer == NULL) {
    printf("Memory allocation error\n");
    exit(3);
  }

  bytes_read = fread(buffer, size, 1, fp);
  fclose(fp);

  disassemble8080(buffer, size);
  return 0;
}

