

#include "gestao.h"


int main_gestao(){

    GESTOR listaClientes;
    listaClientes.lista = NULL;
    listaClientes.tam = 0;

    CLIENTE *remove = NULL;


    insere_cliente_ordenado_NIF(&listaClientes, "vasco Cardoso", 12345678, "porto", 935873444, 0);
    insere_cliente_ordenado_NIF(&listaClientes, "Joao Ferreira",23456789, "Porto", 923456785, 0);
    insere_cliente_ordenado_NIF(&listaClientes,"Joao Pedro", 224455632, "Porto", 932145678, 0 );
    remove = remover_cliente(&listaClientes, 23456789);
    free(remove);


    lista_clientes(listaClientes);

    CLIENTE * teste = find_cli_NIF(listaClientes, 224455632);
    printf("\nEncontrou: %s -> %ld", teste->nome, teste->NIF);

    cria_Arr_Viagens_cli(&listaClientes, 224455632, 2);
    cria_Arr_Viagens_cli(&listaClientes, 12345678, 5);
    insere_viagem_cli(&listaClientes, 224455632, "Portugal", 1);
    insere_viagem_cli(&listaClientes, 224455632, "Espanha", 2);
    insere_viagem_cli(&listaClientes, 224455632, "Italia", 3);
    insere_viagem_cli(&listaClientes, 224455632, "Fraça", 4);

    //remove_viagem(&listaClientes, 224455632, 2);




    cria_Arr_Cidades(&listaClientes, 224455632, 1, 5);
    insere_cidades_viagem(&listaClientes, 224455632, 1, "Porto", 1, 15, 0);
    insere_cidades_viagem(&listaClientes, 224455632, 1, "VNGaia", 2, 15, 10);
    insere_cidades_viagem(&listaClientes, 224455632, 1, "Ovar", 3, 10, 35);

    cria_Arr_pontosInt(&listaClientes, 224455632, 1, 2, 3);
    insere_POI_cidade(&listaClientes, 224455632, 1, 2, 1, "Jardim do Morro", 2, 20);
    insere_POI_cidade(&listaClientes, 224455632, 1, 2, 2, "Qualquer", 20, 20);
    insere_POI_cidade(&listaClientes, 224455632, 1, 2, 3, "Jardim", 2, 20);


    historico_cliente(&listaClientes, 224455632);

    return 0;
}


void insere_cliente_ordenado_NIF(GESTOR *lista, char nome[20], long NIF, char morada[20], int contacto, long faturacao){
    CLIENTE *novoCliente = malloc(sizeof(CLIENTE));
    CLIENTE *aux;
    if (novoCliente){
        strcpy(novoCliente->nome, nome);
        novoCliente->NIF = NIF;
        novoCliente->id = 0;
        strcpy(novoCliente->morada, morada);
        novoCliente->faturacao = faturacao;
        novoCliente->contacto = contacto;

        if(lista->lista == NULL){
            novoCliente->prox = NULL;
            novoCliente->id = 1;
            lista->lista=novoCliente;

        }else if(novoCliente->NIF < lista->lista->NIF){
            novoCliente->prox = lista->lista;
            lista->lista = novoCliente;

        } else{
            aux = lista->lista;
            while(aux->prox && novoCliente->NIF > aux->prox->NIF)
                aux = aux->prox;

            novoCliente->prox = aux->prox;
            aux->prox = novoCliente;
        }
        lista->tam++;
    }else{
        printf("ERRO a criar o cliente");
    }
}

void insere_ini_cliente(GESTOR *lista, char nome[20], long NIF, char morada[20], int contacto, long faturacao){
    CLIENTE *novoCliente = malloc(sizeof(CLIENTE));

    if(novoCliente){
        strcpy(novoCliente->nome, nome);
        novoCliente->NIF = NIF;
        strcpy(novoCliente->morada, morada);
        novoCliente->faturacao = faturacao;
        novoCliente->contacto = contacto;
        novoCliente->prox = lista->lista;
        lista->lista = novoCliente;
        lista->tam++;
    }else{
        printf("erro!\n");
    }
}


void insere_fim_cliente(GESTOR *lista, char nome[20], long NIF, char morada[20], int contacto, long faturacao){

    CLIENTE *aux, *novoCliente = malloc(sizeof(CLIENTE));

    if(novoCliente){
        strcpy(novoCliente->nome, nome);
        novoCliente->NIF = NIF;
        strcpy(novoCliente->morada, morada);
        novoCliente->faturacao = faturacao;
        novoCliente->contacto = contacto;
        novoCliente->prox = NULL;

        if (lista->lista == NULL)
            lista->lista = novoCliente;
        else{
            aux = lista->lista;
            while (aux->prox)
                aux = aux->prox;
            aux->prox = novoCliente;
        }
        lista->tam++;
    }else{
        printf("erro!\n");
    }
}


CLIENTE* remover_cliente(GESTOR *lista, int NIF){
    CLIENTE *aux = NULL, *remover = NULL;

    if(lista->lista){
        if(lista->lista->NIF == NIF){
            remover = lista->lista;
            lista->lista = remover->prox;
            lista->tam--;
        }else{
            aux= lista->lista;
            while (aux->prox && aux->prox->NIF !=NIF){
                aux = aux->prox;
            }
            if (aux->prox){
                remover = aux->prox;
                aux->prox = remover->prox;
                lista->tam--;
            }
        }
    }

    return aux;
}


CLIENTE * find_cli_NIF(GESTOR lista, long NIF){
    CLIENTE *aux = lista.lista;

    if(aux){
        if(aux->NIF == NIF){
            return aux;
        }else{
            while (aux && aux->NIF !=NIF){
                aux = aux->prox;
            }
            return aux;
        }
    }
}



void lista_clientes(GESTOR lista){
    CLIENTE * aux = lista.lista;
    printf("LISTA DE CLIENTES\n\n");
    while (aux){
        printf("%s,  %s,  %d,  %ld,  \n", aux->nome, aux->morada, aux->contacto, aux->faturacao);
        aux = aux->prox;
    }
    printf("\n");
}

/*
     *    VIAGENS
     */


void cria_Arr_Viagens_cli(GESTOR *lista, long NIF, int size){

    CLIENTE *aux = lista->lista;

    if(aux){
        if(lista->lista->NIF == NIF){
            aux->maxViagens = size;
            aux->viagens = (struct viagem*)calloc(aux->maxViagens, sizeof(struct viagem));
            aux->nViagens = 0;
            return;
        }else{
            while (aux && aux->NIF != NIF){
                aux = aux->prox;
            }
            if(aux){
                aux->maxViagens = size;
                aux->viagens = (struct viagem*)calloc(aux->maxViagens, sizeof(struct viagem));
                aux->nViagens = 0;
                return;
            }
        }
    }
}


void insere_viagem_cli(GESTOR *lista, long NIF, char *nomeViagem, int idViagem){

    CLIENTE *aux = lista->lista;


    if(aux){
        if(aux->NIF == NIF){
            if(aux->nViagens == aux->maxViagens){
                if(viagens_realloc(aux) == 0){
                    printf("erro a reaocar");
                }
            }
            aux->viagens[idViagem-1].id = idViagem;
            aux->nViagens++;
            strcpy(aux->viagens[idViagem-1].nome, nomeViagem);
            printf("\n inseriu %d, %s\n", aux->viagens[idViagem].id, aux->viagens[idViagem].nome);

        } else{
            while (aux && aux->NIF != NIF){
                aux = aux->prox;
            }
            if(aux->nViagens == aux->maxViagens){
                if(viagens_realloc(aux) == 0){
                    printf("erro a reaocar");
                }
            }
            aux->viagens[idViagem-1].id = idViagem;
            aux->nViagens++;
            strcpy(aux->viagens[idViagem-1].nome, nomeViagem);
            printf("\n inseriu %d, %s\n", aux->viagens[idViagem-1].id, aux->viagens[idViagem-1].nome);
        }
    }
}

int viagens_realloc(struct cliente *c){

    if(!c) return 0;
    c->maxViagens *= 2;
    c->viagens = (struct viagem*) realloc(c->viagens, c->maxViagens * sizeof(struct viagem));
    printf("\n realloc viagens: tamanho= %d", c->maxViagens);
    if(!c->viagens) return 0;
    return 1;
}



void remove_viagem(GESTOR *lista, int NIF, int idViagem){
    CLIENTE *aux = lista->lista;

    if(aux){
        if(aux->NIF == NIF){
            for (int i = 0; i <= aux->nViagens; ++i) {
                if (aux->viagens[i].id == idViagem-1){
                    for (int j = i; j <= aux->maxViagens; ++j) {
                        aux->viagens[i] = aux->viagens[i+1];
                    }
                }
            }
        }
    }
}


void cria_Arr_Cidades(GESTOR *lista, int NIF, int idViagem, int size){
    CLIENTE *aux = lista->lista;

    if(aux){
        if(aux->NIF == NIF){
            for (int i = 0; i <= aux->nViagens; ++i) {
                if (aux->viagens[i].id == idViagem){
                    aux->viagens[i].maxCidades = size;
                    aux->viagens[i].cidades = (struct cidade*) calloc(aux->viagens[i].maxCidades, sizeof(struct cidade));
                    aux->viagens[i].nCidades = 0;
                    return;
                }
            }
        }else{
            while (aux && aux->NIF != NIF){
                aux = aux->prox;
            }
            for (int i = 0; i <= aux->nViagens; ++i) {
                if (aux->viagens[i].id == idViagem){
                    aux->viagens[i].maxCidades = size;
                    aux->viagens[i].cidades = (struct cidade*) calloc(aux->viagens[i].maxCidades, sizeof(struct cidade));
                    aux->viagens[i].nCidades = 0;
                    return;
                }
            }
        }
    }
}

void insere_cidades_viagem(GESTOR *lista, int NIF, int idViagem, char *nomeCidade, int idCidade, int x, int y){

    CLIENTE *aux = lista->lista;

    if(aux){
        if(aux->NIF == NIF){
            for (int i = 0; i <= aux->nViagens; ++i) {
                if(aux->viagens[i].nCidades == aux->viagens[i].maxCidades){
                    //cidades_realloc(aux->viagens[i])
                }
                if (aux->viagens[i].id == idViagem){
                    //insere
                    aux->viagens[i].cidades[idCidade-1].id = idCidade;
                    strcpy(aux->viagens[i].cidades[idCidade-1].nome, nomeCidade);
                    aux->viagens[i].cidades[idCidade-1].x = x;
                    aux->viagens[i].cidades[idCidade-1].y = y;
                    aux->viagens[i].nCidades++;
                    printf("\n inseriu cidade %d, %s", aux->viagens[i].cidades[idCidade-1].id, aux->viagens[i].cidades[idCidade-1].nome);
                }
            }
        } else{
            while (aux && aux->NIF != NIF){
                aux = aux->prox;
            }
            for (int i = 0; i <= aux->nViagens; ++i) {
                if(aux->viagens[i].nCidades == aux->viagens[i].maxCidades){
                    //cidades_realloc(aux->viagens[i])
                }
                if (aux->viagens[i].id == idViagem){
                    //insere
                    aux->viagens[i].cidades[idCidade-1].id = idCidade;
                    strcpy(aux->viagens[i].cidades[idCidade-1].nome, nomeCidade);
                    aux->viagens[i].cidades[idCidade-1].x = x;
                    aux->viagens[i].cidades[idCidade-1].y = y;
                    aux->viagens[i].nCidades++;
                    printf("\n inseriu cidade %d, %s", aux->viagens[i].cidades[idCidade-1].id, aux->viagens[i].cidades[idCidade-1].nome);
                }
            }
        }
    }
}



void cria_Arr_pontosInt(GESTOR *lista, int NIF, int idViagem, int idCidade,int size){

    CLIENTE *aux = lista->lista;

    if(aux){
        if(aux->NIF == NIF){
            for (int i = 0; i <= aux->nViagens; ++i) {
                if (aux->viagens[i].id == idViagem){
                    for (int j = 0; j <= aux->viagens[i].nCidades; ++j) {
                        if (aux->viagens[i].cidades[j].id == idCidade){
                            aux->viagens[i].cidades[j].maxPoi = size;
                            aux->viagens[i].cidades[j].pontosInteresse = (struct poi*) calloc(aux->viagens[i].cidades[j].maxPoi, sizeof(struct poi));
                            aux->viagens[i].cidades[j].nPoi = 0;
                        }
                    }
                }
            }
        } else{
            while (aux && aux->NIF != NIF){
                aux = aux->prox;
            }
            for (int i = 0; i <= aux->nViagens; ++i) {
                if (aux->viagens[i].id == idViagem){
                    for (int j = 0; j <= aux->viagens[i].nCidades; ++j) {
                        if (aux->viagens[i].cidades[j].id == idCidade){
                            aux->viagens[i].cidades[j].maxPoi = size;
                            aux->viagens[i].cidades[j].pontosInteresse = (struct poi*) calloc(aux->viagens[i].cidades[j].maxPoi, sizeof(struct poi));
                            aux->viagens[i].cidades[j].nPoi = 0;
                        }
                    }
                }
            }
        }
    }
}


void insere_POI_cidade(GESTOR *lista, int NIF, int idViagem, int idCidade, int idPOI, char *nomePOI, int x, int y){

    CLIENTE *aux = lista->lista;

    if(aux){
        if(aux->NIF == NIF){
            for (int i = 0; i <= aux->nViagens; ++i) {
                if (aux->viagens[i].id == idViagem){
                    for (int j = 0; j <= aux->viagens[i].nCidades; ++j) {
                        if (aux->viagens[i].cidades[j].id == idCidade){
                            aux->viagens[i].cidades[j].nPoi++;
                            aux->viagens[i].cidades[j].pontosInteresse[idPOI-1].id = idPOI;
                            strcpy(aux->viagens[i].cidades[j].pontosInteresse[idPOI-1].nome, nomePOI);
                            aux->viagens[i].cidades[j].pontosInteresse[idPOI-1].x = x;
                            aux->viagens[i].cidades[j].pontosInteresse[idPOI-1].y = y;
                            printf("\n inseriu ponto de interesse %d, %s", aux->viagens[i].cidades[j].pontosInteresse[idPOI-1].id, aux->viagens[i].cidades[j].pontosInteresse[idPOI-1].nome);
                        }
                    }
                }
            }
        } else{
            while (aux && aux->NIF != NIF){
                aux = aux->prox;
            }
            for (int i = 0; i <= aux->nViagens; ++i) {
                if (aux->viagens[i].id == idViagem){
                    for (int j = 0; j <= aux->viagens[i].nCidades; ++j) {
                        if (aux->viagens[i].cidades[j].id == idCidade){
                            aux->viagens[i].cidades[j].nPoi++;
                            aux->viagens[i].cidades[j].pontosInteresse[idPOI-1].id = idPOI;
                            strcpy(aux->viagens[i].cidades[j].pontosInteresse[idPOI-1].nome, nomePOI);
                            aux->viagens[i].cidades[j].pontosInteresse[idPOI-1].x = x;
                            aux->viagens[i].cidades[j].pontosInteresse[idPOI-1].y = y;
                            printf("\n inseriu ponto de interesse %d, %s", aux->viagens[i].cidades[j].pontosInteresse[idPOI-1].id, aux->viagens[i].cidades[j].pontosInteresse[idPOI-1].nome);

                        }
                    }
                }
            }
        }
    }
}



void historico_cliente(GESTOR *lista, int NIF){
    CLIENTE *aux = lista->lista;

    if(aux){
        if(aux->NIF == NIF){
            printf("\n Cliente: %s", aux->nome);
            printf("\n Viagens:   ");
            for (int i = 0; i <= aux->nViagens; ++i){
                printf("%d, %s,    ", aux->viagens[i].id, aux->viagens[i].nome);
                printf("\n Cidades:   ");
                for (int j = 0; j <= aux->viagens[i].nCidades; ++j){
                    printf(" %d, %s,   ", aux->viagens[i].cidades[j].id, aux->viagens[i].cidades[j].nome);
                    printf("\n Pontos de Interesse:   ");
                    for (int k = 0; k <= aux->viagens[i].cidades[j].nPoi; ++k) {
                        printf(" %d, %s,    ", aux->viagens[i].cidades[j].pontosInteresse[k].id, aux->viagens[i].cidades[j].pontosInteresse[k].nome);
                    }
                }
            }
        }else{
            while (aux && aux->NIF != NIF){
                aux = aux->prox;
            }
            printf("\n Cliente: %s", aux->nome);
            printf("\n Viagens:   ");
            for (int i = 0; i <= aux->nViagens; ++i){
                printf("%d, %s,    ", aux->viagens[i].id, aux->viagens[i].nome);
                printf("\n Cidades:   ");
                for (int j = 0; j <= aux->viagens[i].nCidades; ++j){
                    printf(" %d, %s,   ", aux->viagens[i].cidades[j].id, aux->viagens[i].cidades[j].nome);
                    printf("\n Pontos de Interesse:   ");
                    for (int k = 0; k <= aux->viagens[i].cidades[j].nPoi; ++k) {
                        printf(" %d, %s,    ", aux->viagens[i].cidades[j].pontosInteresse[k].id, aux->viagens[i].cidades[j].pontosInteresse[k].nome);
                    }
                }
            }
        }
    }
}