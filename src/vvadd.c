#include <stdio.h>
#include <stdlib.h>

const size_t length = 1024;

__attribute__ ((noinline))
void vvadd( int* dest, int* src0, int* src1, size_t size )
{
   size_t i = 0;
  
  for (i; i < size; i++ )
    dest[i] = src0[i] + src1[i];

}

int main(void)
{
  
  int src0[length];
  int src1[length];
  int  ref[length];
  int dest[length];
  size_t i;
  for (i = 0; i < length; i++){
    src0[i] = i;
    src1[i] = 1;
    ref[i]  = (i+1);
  }
 
  vvadd( dest, src0, src1, length );
  
  //size_t i = 0;
   __asm__ volatile ( "nop 1(%rax)" );
  for ( i=0 ; i < length; i++ ) {
    if ( dest[i] != ref[i] ) {
      printf( "\n [ FAILED ] dest[%d] != ref[%d] (%d != %d) \n\n",
              i, i, dest[i], ref[i] );
      return 1;
    }
  }
    __asm__ volatile ( "nop 2(%rax)" );

  printf( "\n [ passed ] \n\n" );
  return 0;
}