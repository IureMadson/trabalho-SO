#include "system.h"
 #include <stdio.h>
 #include <stdlib.h>
 PTR_DESC c01, c02, prin;
 /* co-rotinas (devem aceitar um void *arg) */
 void corotina1(void *arg)
 {
 while(1)
 {
 printf("tic- ");
 transfer(c01, c02); /* Congela c01, pula para c02 */
 }
 }

 void corotina2(void *arg)
 {
 while(1)
 {
 printf("tac\n");
 transfer(c02, c01); /* Congela c02, pula de volta para c01 */
 }
 }
/* Programa Principal */
 int main(void)
{
 system("cls"); /* Alternativa moderna ao clrscr() */

 /* cria descritores de co-rotinas */
 c01 = cria_desc();
 c02 = cria_desc();
 prin = cria_desc();

 /* associa descritores com as co-rotinas */
 /* Na API do projeto, o argumento do meio é um void* para parâmetros. Passamos NULL.
*/
 newprocess(corotina1, NULL, c01);
 newprocess(corotina2, NULL, c02);

 /* OBRIGATÓRIO: Converte o fluxo principal do Windows em uma Fiber antes de fazer a
transferência */
 system_init_main(prin);

 /* transfere o controle da thread/fiber principal para a co-rotina1 */
 transfer(prin, c01);
return 0;
}
