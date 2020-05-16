#include <stdio.h>
#include <stdlib.h>

const size_t length = 200;
size_t n_odd = 0;
//size_t n_even = 0;

__attribute__ ((noinline))
void oddeven( int* src0, size_t size )
{
   size_t i = 0;
  
  for (i; i < size; i++ ){
    // __asm__ volatile ( "nop 1(%rax)" );
    if (!(src0[i]%2)){
      n_odd++;
    }
  // __asm__ volatile ( "nop 2(%rax)" );
  }
   
}  

int main(void)
{
  
  int src0[length];
  //int src1[length];
 // int  ref[length];
  //int dest[length];
  size_t i;
  for (i = 0; i < length; i++){
    src0[i] = i;
    //src1[i] = 1;
    //ref[i]  = (i%2);
  }
  __asm__ volatile ( "nop 1(%rax)" );
  oddeven( src0, length );
  __asm__ volatile ( "nop 2(%rax)" );
 //printf("Even = %d\n", n_even);
 printf("Odd  = %d", n_odd);
  //size_t i = 0;
  

  printf( "\n [ passed ] \n\n" );
  return 0;
}