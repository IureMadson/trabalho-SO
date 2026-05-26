#include "system.h"
#include "semaforo.h"
#include "pausa.h"
#include "nucleo.h"
#include <windows.h>

#DEFINE TAM 10 // TAMANHO DO BUFFER
#DEFINE 20 // PRODUZIR/CONSUMIR
LPVOID fiberprincipal;
STR_DESC_PROC atual;

int buffer[TAM];
int *in, *out;

int main(){
 fiberprincipal = ConvertThreadToFiber(NULL);
 
 return 0;
}
