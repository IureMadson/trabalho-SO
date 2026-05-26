#include "system.h"
#include "semaforo.h"
#include "pausa.h"
#include "nucleo.h"
#include <windows.h>

int main(){
 int in = 0;
 int out = 0;
 inicia_fila_prontos();
 
 inicia_semaforo(cheio, 0);
 inicia_semaforo(vazio, TAM);
 inicia_semaforo(mutex, 1);
 
 cria_processo(Produtor, "Produtor");
 cria_processo(Consumidor, "Consumidor");
 printf("Sistema pronto. Iniciando escalonador cooperativo...\n");
 
 dispara_sistema();
 printf("\nSistema finalizado com sucesso.\n");
 
 return 0;
}
