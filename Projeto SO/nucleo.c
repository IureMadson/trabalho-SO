#include "nucleo.h"
#include <stdlib.h>
#include <stdio.h>


    /* Fila circular (Variáveis reais) */
    PTR_DESC_PROC prim = NULL;
    PTR_DESC_PROC atual = NULL;
    /* Contexto do main (como fiber - substitui o conceito do antigo "d_esc") */
    static descritor main_desc;
    static PTR_DESC main_ctx = &main_desc;
    static int main_ready = 0;
    /* ... outras funções auxiliares como processo_trampolim ... */
    /* Função Básica de Preparação */

void inicia_fila_prontos(void){
    prim = NULL;
    atual = NULL;
}
void cria_processo(void (*end_proc)(void), const char *nome_p){
    DESCRITOR_PROC proc = (DESCRITOR_PROC)malloc(sizeof(DESCRITOR_PROC));
    strcpy(proc->nome, nome_p);
    proc->estado = ATIVO;
    fila_sem(NULL);
}
static PTR_DESC_PROC proximo_ativo_depois(PTR_DESC_PROC a_partir){
    //verifica se a fila circular (prim) existe e não está vazia; 
    if(/*   */==NULL){
        printf("Fila não existe ou não está alocada");
        return;
    }
    //define o ponto inicial da busca (usa prim se a_partir for nulo); 
    if(a_partir==NULL){
        
    }
}
static void processo_trampolim(void* arg){
    PTR_DESC_PROC qlqr = (PTR_DESC_PROC)*arg;
}
void dispara_sistema(void){
    
}
void yield(void){
    
}
void termina_processo(void){
    
}
