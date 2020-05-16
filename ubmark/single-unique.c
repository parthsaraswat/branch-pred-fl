//========================================================================
// single-unique
//========================================================================
// First program for demo - one unique branch

#include <stdio.h>
int main()
{
  int a[100];
  int b = 0;
  __asm__ volatile( "nop 1(%rax)" );   // stats on
                                       // single unique branch
  for ( size_t i = 0; i < 100; i++ ) {
    a[i] = b + i;
  }
  
  __asm__ volatile( "nop 2(%rax)" );   // stats off
    return 0;
}

