//========================================================================
// multi-unique
//========================================================================
// First program for demo - multiple unique branches

#include <stdio.h>
int main()
{
  int a[100];
  int b = 0;
  __asm__ volatile( "nop 1(%rax)" );  // stats on
                                      // single unique branch
  for ( size_t i = 0; i < 5; i++ ) {
    a[i] = b + i;
    if ( ( i % 2 ) ) {
      b = a[i];
    }
  }

  for ( size_t j = 0; j < 40; j++ ) {
    a[j] = b + j;
    if ( ( j % 2 ) ) {
      b = a[j];
    }
  }
  for ( size_t j = 0; j < 10; j++ ) {
    a[j] = b + j;
    if ( ( j % 2 ) ) {
      b = a[j];
    }
  }
  for ( size_t j = 0; j < 10; j++ ) {
    a[j] = b + j;
    if ( ( j % 2 ) ) {
      b = a[j];
    }
  }
  for ( size_t j = 0; j < 10; j++ ) {
    a[j] = b + j;
    if ( ( j % 2 ) ) {
      b = a[j];
    }
  }
  for ( size_t j = 0; j < 10; j++ ) {
    a[j] = b + j;
    if ( ( j % 2 ) ) {
      b = a[j];
    }
  }
  for ( size_t j = 0; j < 10; j++ ) {
    a[j] = b + j;
    if ( ( j % 2 ) ) {
      b = a[j];
    }
  }
  for ( size_t j = 0; j < 10; j++ ) {
    a[j] = b + j;
    if ( ( j % 2 ) ) {
      b = a[j];
    }
  }
  for ( size_t j = 0; j < 10; j++ ) {
    a[j] = b + j;
    if ( ( j % 2 ) ) {
      b = a[j];
    }
  }
  for ( size_t j = 0; j < 10; j++ ) {
    a[j] = b + j;
    if ( ( j % 2 ) ) {
      b = a[j];
    }
  }
  for ( size_t j = 0; j < 10; j++ ) {
    a[j] = b + j;
    if ( ( j % 2 ) ) {
      b = a[j];
    }
  }
  for ( size_t j = 0; j < 10; j++ ) {
    a[j] = b + j;
    if ( ( j % 2 ) ) {
      b = a[j];
    }
  }

  __asm__ volatile( "nop 2(%rax)" );  // stats off
  return 0;
}
