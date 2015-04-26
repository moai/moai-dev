
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liboil/liboil.h>
#include <liboil/liboilfault.h>
#include <stdio.h>

#ifdef HAVE_I386
void unknown_insn(void *far)
{
  asm ("ud2");
}
#define HAVE_ILLEGAL_INSN
#endif

#ifdef HAVE_POWERPC
void unknown_insn(void *far)
{
  asm (".long 0");
}
#define HAVE_ILLEGAL_INSN
#endif

int main(void)
{
  oil_init();

#ifdef HAVE_ILLEGAL_INSN
  oil_fault_check_enable();

  if (oil_fault_check_try(unknown_insn, NULL)) {
    printf("didn't catch failure\n");
    return 1;
  }

  if (oil_fault_check_try(unknown_insn, NULL)) {
    printf("didn't catch failure\n");
    return 1;
  }

  oil_fault_check_disable();

  printf("OK\n");
#else
  printf("No illegal instruction for this architecture, test inconclusive\n");
#endif

  return 0;
}
