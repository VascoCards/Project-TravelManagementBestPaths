//
// Created by Vasco on 23/11/2021.
//

#ifndef PROJLP_AED_GESTAO_H
#define PROJLP_AED_GESTAO_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>




struct poi{

    char nome[15];
    int id;
    int x;
    int y;

}POI;


struct cidade{

    char nome[10];
    int id;
    char info[50];
    struct poi *pontosInteresse;
    int maxPoi;
    int nPoi;
    int x;
    int y;

};


struct viagem{

    int id;
    char nome[10];
    struct cidade *cidades;
    int nCidades;
    int maxCidades;

};

typedef struct cliente{

    char nome[20];
    int id;
    long NIF;
    char morada[20];
    int contacto;
    long faturacao;
    struct viagem *viagens;
    int nViagens;
    int maxViagens;
    struct cliente * prox;

}CLIENTE;

typedef struct {
    CLIENTE  *lista;
    int tam;

}GESTOR;


struct populacao{

    int size;
    struct trajeto *trajetos;
    int gen;
    int best_id;


} *pPop;

struct trajeto{

    int size;
    struct cidade *cidades;
    double aptidao;
    double aapt;
    double rapt;

};


int main_gestao();
void historico_cliente(GESTOR *lista, int NIF);
void insere_ini_cliente(GESTOR *lista, char nome[20], long NIF, char morada[20], int contacto, long faturacao);
void insere_fim_cliente(GESTOR *lista, char nome[20], long NIF, char morada[20], int contacto, long faturacao);
void insere_cliente_ordenado_NIF(GESTOR *lista, char nome[20], long NIF, char morada[20], int contacto, long faturacao);
CLIENTE * find_cli_NIF(GESTOR lista, long NIF);
void lista_clientes(GESTOR lista);
CLIENTE* remover_cliente(GESTOR *lista, int NIF);
void cria_Arr_Viagens_cli(GESTOR *lista, long NIF, int size);
void insere_viagem_cli(GESTOR *lista, long NIF, char *nomeViagem, int idViagem);
int viagens_realloc(struct cliente *c);
void remove_viagem(GESTOR *lista, int NIF, int idViagem);
void insere_cidades_viagem(GESTOR *lista, int NIF, int idViagem, char *nomeCidade, int idCidade, int x, int y);
void cria_Arr_Cidades(GESTOR *lista, int NIF, int idViagem, int size);
void cria_Arr_pontosInt(GESTOR *lista, int NIF, int idViagem, int idCidade,int size);
void insere_POI_cidade(GESTOR *lista, int NIF, int idViagem, int idCidade, int idPOI, char *nomePOI, int x, int y);




int main_AG();
void read_file_dataAG();
void gen_population(struct populacao *p, int psize);
void ini_cidades(struct trajeto *t, int s);
void set_points(struct cidade *c, int i, int x, int y);
int* shuffle_trajetos(int *data, int n);
void avaliar_aptidao(struct populacao *p);
double calc_aptidao(struct trajeto *t);
void sel_best_trajeto(struct populacao *p);
void selecao(struct populacao *p, struct populacao *newp);
void cruzamento(struct populacao *p);
void mutacao(struct populacao *p);
void troca_cidades(struct trajeto *t, int id1, int id2);
int rand_pos(int l, int h);
void report_state(struct populacao *p);
void elitismo(struct populacao *p);


#endif //PROJLP_AED_GESTAO_H
