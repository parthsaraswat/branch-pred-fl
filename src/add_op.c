__attribute__ ((noinline))
void main() {
   __asm__ volatile ( "nop 1(%rax)" );
   int i = 0;
   for (i; i < 10; i++)
   {
   }
  __asm__ volatile ( "nop 2(%rax)" );
}