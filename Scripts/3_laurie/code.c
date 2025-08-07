#include "out.h"

int main(int argc,char **argv)

{
  undefined4 uVar1;
  int in_stack_00000004;
  undefined4 *in_stack_00000008;
  
  if (in_stack_00000004 == 1) {
    infile = stdin;
  }
  else {
    if (in_stack_00000004 != 2) {
      printf("Usage: %s [<input_file>]\n",*in_stack_00000008);
                    // WARNING: Subroutine does not return
      exit(8);
    }
    infile = (_IO_FILE *)fopen((char *)in_stack_00000008[1],"r");
    if ((FILE *)infile == (FILE *)0x0) {
      printf("%s: Error: Couldn\'t open %s\n",*in_stack_00000008,in_stack_00000008[1]);
                    // WARNING: Subroutine does not return
      exit(8);
    }
  }
  initialize_bomb(argv);
  printf("Welcome this is my little bomb !!!! You have 6 stages with\n");
  printf("only one life good luck !! Have a nice day!\n");
  uVar1 = read_line();
  phase_1(uVar1);
  phase_defused();
  printf("Phase 1 defused. How about the next one?\n");
  uVar1 = read_line();
  phase_2(uVar1);
  phase_defused();
  printf("That\'s number 2.  Keep going!\n");
  uVar1 = read_line();
  phase_3(uVar1);
  phase_defused();
  printf("Halfway there!\n");
  uVar1 = read_line();
  phase_4(uVar1);
  phase_defused();
  printf("So you got that one.  Try this one.\n");
  uVar1 = read_line();
  phase_5(uVar1);
  phase_defused();
  printf("Good work!  On to the next...\n");
  uVar1 = read_line();
  phase_6(uVar1);
  phase_defused();
  return 0;
}


void phase_1(undefined4 param_1)

{
  int iVar1;
  
  iVar1 = strings_not_equal(param_1,"Public speaking is very easy.");
  if (iVar1 != 0) {
    explode_bomb();
  }
  return;
}



void phase_2(undefined4 param_1)

{
  int iVar1;
  int aiStack_20 [7];
  
  read_six_numbers(param_1,aiStack_20 + 1);
  if (aiStack_20[1] != 1) {
    explode_bomb();
  }
  iVar1 = 1;
  do {
    if (aiStack_20[iVar1 + 1] != (iVar1 + 1) * aiStack_20[iVar1]) {
      explode_bomb();
    }
    iVar1 = iVar1 + 1;
  } while (iVar1 < 6);
  return;
}


1

(1 + 1) * 1 = 2

(2 + 1) * 2 = 6

(3 + 1) * 6 = 24

(4 + 1) * 24 = 120

(5 + 1) * 120 = 720


void phase_3(char *param_1)

{
  int iVar1;
  char cVar2;
  undefined4 local_10;
  char local_9;
  int local_8;
  
  iVar1 = sscanf(param_1,"%d %c %d",&local_10,&local_9,&local_8);
  if (iVar1 < 3) {
    explode_bomb();
  }
  switch(local_10) {
  case 0:
    cVar2 = 'q';
    if (local_8 != 0x309) {
      explode_bomb();
    }
    break;
  case 1:
    cVar2 = 'b';
    if (local_8 != 0xd6) {
      explode_bomb();
    }
    break;
  case 2:
    cVar2 = 'b';
    if (local_8 != 0x2f3) {
      explode_bomb();
    }
    break;
  case 3:
    cVar2 = 'k';
    if (local_8 != 0xfb) {
      explode_bomb();
    }
    break;
  case 4:
    cVar2 = 'o';
    if (local_8 != 0xa0) {
      explode_bomb();
    }
    break;
  case 5:
    cVar2 = 't';
    if (local_8 != 0x1ca) {
      explode_bomb();
    }
    break;
  case 6:
    cVar2 = 'v';
    if (local_8 != 0x30c) {
      explode_bomb();
    }
    break;
  case 7:
    cVar2 = 'b';
    if (local_8 != 0x20c) {
      explode_bomb();
    }
    break;
  default:
    cVar2 = 'x';
    explode_bomb();
  }
  if (cVar2 != local_9) {
    explode_bomb();
  }
  return;
}



int func4(int param_1)

{
  int iVar1;
  int iVar2;
  
  if (param_1 < 2) {
    iVar2 = 1;
  }
  else {
    iVar1 = func4(param_1 + -1);
    iVar2 = func4(param_1 + -2);
    iVar2 = iVar2 + iVar1;
  }
  return iVar2;
}



void phase_4(char *param_1)

{
  int iVar1;
  int local_8;
  
  iVar1 = sscanf(param_1,"%d",&local_8);
  if ((iVar1 != 1) || (local_8 < 1)) {
    explode_bomb();
  }
  iVar1 = func4(local_8);
  if (iVar1 != 0x37) {
    explode_bomb();
  }
  return;
}



void phase_5(int param_1)

{
  int iVar1;
  undefined1 local_c [6];
  undefined1 local_6;
  
  iVar1 = string_length(param_1);
  if (iVar1 != 6) {
    explode_bomb();
  }
  iVar1 = 0;
  do {
    local_c[iVar1] = (&array_123)[(char)(*(byte *)(iVar1 + param_1) & 0xf)];
    iVar1 = iVar1 + 1;
  } while (iVar1 < 6);
  local_6 = 0;
  iVar1 = strings_not_equal(local_c,"giants");
  if (iVar1 != 0) {
    explode_bomb();
  }
  return;
}



void phase_6(undefined4 param_1)

{
  int *piVar1;
  int iVar2;
  undefined1 *puVar3;
  int *piVar4;
  int iVar5;
  undefined1 *local_38;
  int *local_34;
  int local_30 [5];
  int local_1c [6];
  
  local_38 = node1;
  read_six_numbers(param_1,local_1c);
  iVar5 = 0;
  do {
    iVar2 = iVar5;
    if (5 < local_1c[iVar5] - 1U) {
      explode_bomb();
    }
    while (iVar2 = iVar2 + 1, iVar2 < 6) {
      if (local_1c[iVar5] == local_1c[iVar2]) {
        explode_bomb();
      }
    }
    iVar5 = iVar5 + 1;
  } while (iVar5 < 6);
  iVar5 = 0;
  do {
    iVar2 = 1;
    puVar3 = local_38;
    if (1 < local_1c[iVar5]) {
      do {
        puVar3 = *(undefined1 **)(puVar3 + 8);
        iVar2 = iVar2 + 1;
      } while (iVar2 < local_1c[iVar5]);
    }
    local_30[iVar5 + -1] = (int)puVar3;
    iVar5 = iVar5 + 1;
  } while (iVar5 < 6);
  iVar5 = 1;
  piVar4 = local_34;
  do {
    piVar1 = (int *)local_30[iVar5 + -1];
    piVar4[2] = (int)piVar1;
    iVar5 = iVar5 + 1;
    piVar4 = piVar1;
  } while (iVar5 < 6);
  piVar1[2] = 0;
  iVar5 = 0;
  do {
    if (*local_34 < *(int *)local_34[2]) {
      explode_bomb();
    }
    local_34 = (int *)local_34[2];
    iVar5 = iVar5 + 1;
  } while (iVar5 < 5);
  return;
}



int fun7(int *param_1,int param_2)

{
  int iVar1;
  
  if (param_1 == (int *)0x0) {
    iVar1 = -1;
  }
  else if (param_2 < *param_1) {
    iVar1 = fun7(param_1[1],param_2);
    iVar1 = iVar1 * 2;
  }
  else if (param_2 == *param_1) {
    iVar1 = 0;
  }
  else {
    iVar1 = fun7(param_1[2],param_2);
    iVar1 = iVar1 * 2 + 1;
  }
  return iVar1;
}



void secret_phase(void)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = read_line();
  iVar2 = __strtol_internal(uVar1,0,10,0);
  if (1000 < iVar2 - 1U) {
    explode_bomb();
  }
  iVar2 = fun7(n1,iVar2);
  if (iVar2 != 7) {
    explode_bomb();
  }
  printf("Wow! You\'ve defused the secret stage!\n");
  phase_defused();
  return;
}



void read_six_numbers(char *param_1,int param_2)

{
  int iVar1;
  
  iVar1 = sscanf(param_1,"%d %d %d %d %d %d",param_2,param_2 + 4,param_2 + 8,param_2 + 0xc,
                 param_2 + 0x10,param_2 + 0x14);
  if (iVar1 < 6) {
    explode_bomb();
  }
  return;
}



int string_length(char *param_1)

{
  char cVar1;
  int iVar2;
  
  iVar2 = 0;
  cVar1 = *param_1;
  while (cVar1 != '\0') {
    param_1 = param_1 + 1;
    iVar2 = iVar2 + 1;
    cVar1 = *param_1;
  }
  return iVar2;
}



undefined4 strings_not_equal(char *param_1,char *param_2)

{
  char cVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  
  iVar2 = string_length(param_1);
  iVar3 = string_length(param_2);
  if (iVar2 == iVar3) {
    cVar1 = *param_1;
    while (cVar1 != '\0') {
      if (*param_1 != *param_2) goto LAB_08049057;
      param_1 = param_1 + 1;
      param_2 = param_2 + 1;
      cVar1 = *param_1;
    }
    uVar4 = 0;
  }
  else {
LAB_08049057:
    uVar4 = 1;
  }
  return uVar4;
}


undefined4 blank_line(char *param_1)

{
  char cVar1;
  
  cVar1 = *param_1;
  while( true ) {
    if (cVar1 == '\0') {
      return 1;
    }
    cVar1 = *param_1;
    param_1 = param_1 + 1;
    if ((*(byte *)(__ctype_b + 1 + cVar1 * 2) & 0x20) == 0) break;
    cVar1 = *param_1;
  }
  return 0;
}



char * skip(void)

{
  char *pcVar1;
  int iVar2;
  
  do {
    pcVar1 = fgets(input_strings + num_input_strings * 0x50,0x50,(FILE *)infile);
    if (pcVar1 == (char *)0x0) {
      return (char *)0x0;
    }
    iVar2 = blank_line(pcVar1);
  } while (iVar2 != 0);
  return pcVar1;
}



undefined1 * read_line(void)

{
  char cVar1;
  int iVar2;
  char *pcVar3;
  uint uVar4;
  
  iVar2 = skip();
  if (iVar2 == 0) {
    if (infile != stdin) {
      pcVar3 = getenv("GRADE_BOMB");
      if (pcVar3 != (char *)0x0) {
                    // WARNING: Subroutine does not return
        exit(0);
      }
      infile = stdin;
      iVar2 = skip();
      if (iVar2 != 0) goto LAB_0804925f;
    }
    printf("Error: Premature EOF on stdin\n");
    explode_bomb();
  }
LAB_0804925f:
  uVar4 = 0xffffffff;
  pcVar3 = input_strings + num_input_strings * 0x50;
  do {
    if (uVar4 == 0) break;
    uVar4 = uVar4 - 1;
    cVar1 = *pcVar3;
    pcVar3 = pcVar3 + 1;
  } while (cVar1 != '\0');
  if (~uVar4 == 0x50) {
    printf("Error: Input line too long\n");
    explode_bomb();
  }
  iVar2 = num_input_strings * 0x50;
  *(undefined1 *)(~uVar4 + 0x804b67e + iVar2) = 0;
  num_input_strings = num_input_strings + 1;
  return input_strings + iVar2;
}

void explode_bomb(void)

{
  printf("\nBOOM!!!\n");
  printf("The bomb has blown up.\n");
                    // WARNING: Subroutine does not return
  exit(8);
}



void phase_defused(void)

{
  int iVar1;
  undefined1 local_58 [4];
  undefined1 local_54 [80];
  
  if (num_input_strings == 6) {
    iVar1 = sscanf(input_strings + 0xf0,"%d %s",local_58,local_54);
    if (iVar1 == 2) {
      iVar1 = strings_not_equal(local_54,"austinpowers");
      if (iVar1 == 0) {
        printf("Curses, you\'ve found the secret phase!\n");
        printf("But finding it and solving it are quite different...\n");
        secret_phase();
      }
    }
    printf("Congratulations! You\'ve defused the bomb!\n");
  }
  return;
}
