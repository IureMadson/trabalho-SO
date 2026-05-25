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
