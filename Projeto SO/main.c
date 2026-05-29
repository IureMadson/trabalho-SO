#include "system.h"
#include "semaforo.h"
#include "pausa.h"
#include "nucleo.h"
#include <windows.h>
#include <stdio.h>

void produtor(void);
void consumidor(void);

int main(){
 int in = 0;
 int out = 0;

 inicia_fila_prontos();

 cria_processo(produtor, "Produtor");
 cria_processo(consumidor, "Consumidor");
 printf("Sistema pronto. Iniciando escalonador cooperativo...\n");
 
 dispara_sistema();
 printf("\nSistema finalizado com sucesso.\n");
 
 return 0;
}

void produtor(void) {
	int cont = 0;
	while (cont < 1000) {
		printf("Produzindo item... %d\n", cont);
		cont++;
		yield();
	}
}
void consumidor(void) {
	int cont = 0;
	while (cont<1000) {
		printf("Consumindo item... %d\n", cont);
		cont++;
		yield();
	}
}
