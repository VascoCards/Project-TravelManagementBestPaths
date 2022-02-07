//
// Created by Vasco on 08/01/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <errno.h>
#include <float.h>
#include "gestao.h"
#define MAX_TRAJETOS        20
#define NGEN                50
#define MUTATION_PROB       0.1
#define CRUZ_PROB       0.6


int traj_size  = 0;
int min = 0;
int max = 0;
int dataset_len = 0;
int rand_seed = 0;
int** dataset;
int k;


int main_AG(){

    struct populacao *p = (struct populacao*) calloc(1, sizeof(struct populacao));
    struct populacao *newp = (struct populacao*) calloc(1, sizeof(struct populacao));
    k = 20;

    read_file_dataAG("testes/data.txt");

    gen_population(p, k);
    gen_population(newp, k);

    avaliar_aptidao(p);
    sel_best_trajeto(p);
    report_state(p);

    while (p->gen < NGEN){
        p->gen++;
        selecao(p, newp);
        cruzamento(p);
        mutacao(p);
        report_state(p);
        avaliar_aptidao(p);
        elitismo(p);
    }

    printf("\nBest trajeto:\n");
    for(int i=0;i<p->trajetos[k].size;++i){
        printf(" %d ", p->trajetos[k].cidades[i].id);
    }
    printf("\n");
    printf("\n Best aptidao: %lf\n\n", p->trajetos[k].aptidao);

    free(p);
        free(newp);
    free(dataset);


    return 0;
}


void gen_population(struct populacao *p, int psize){
    p->size = psize;
    p->gen = 0;
    p->best_id = 0;
    p->trajetos = (struct trajeto*)calloc(p->size+1, sizeof(struct trajeto));

    for(int i=0; i<p->size+1; i++){
        ini_cidades(&p->trajetos[i], traj_size);
    }
}

void ini_cidades(struct trajeto *t, int s){
    int *cidade_ids = (int*)calloc(s, sizeof(int));
    struct cidade * cidades_data = (struct cidade*)calloc(s, sizeof(struct cidade));
    for(int i=0;i<s;++i){
        cidade_ids[i] = dataset[i][0];
    }
    t->size = s;

    t->cidades = (struct cidade*) calloc(t->size, sizeof(struct cidade));
    for(int i=0;i<t->size;++i){
        set_points(&t->cidades[i], dataset[i][0], dataset[i][1], dataset[i][2]);
    }

    for(int i=0;i<s;i++){
        cidades_data[i].id = t->cidades[i].id;
        cidades_data[i].x = t->cidades[i].x;
        cidades_data[i].y = t->cidades[i].y;
    }

    int* shuffled_trajetos = shuffle_trajetos(cidade_ids, s);

    for (int j = 0; j < t->size; ++j) {
        t->cidades[j].id = shuffled_trajetos[j];
        t->cidades[j].x = cidades_data[shuffled_trajetos[j]].x;
        t->cidades[j].y = cidades_data[shuffled_trajetos[j]].y;
    }

    t->aptidao = 0.0f;
    t->rapt = 0.0f;
    t->aapt = 0.0f;
    free(cidade_ids);
    free(shuffled_trajetos);
    free(cidades_data);

}

void set_points(struct cidade *c, int i, int x, int y){
    c->id = i;
    c->x = x;
    c->y = y;
}

int* shuffle_trajetos(int *data, int n)
{
    int* ret = (int*)calloc(n, sizeof(int));
    if (n > 1) {
        for (int i = 0; i < n - 1; i++) {
            int j = i + 1 + (rand() + rand_seed) % (n - i - 1);
            int t = data[j];
            data[j] = data[i];
            data[i] = t;
        }
    }
    for (int i = 0; i < n; i++) {
        ret[i] = data[i];
    }

    rand_seed+=k;

    return ret;
}


void avaliar_aptidao(struct populacao *p){
    for(int i=0; i<p->size; i++){
        p->trajetos[i].aptidao = calc_aptidao(&p->trajetos[i]);
    }
}


double calc_aptidao(struct trajeto *t){
    double distanceQuadraticSum = 0.0f;
    for(int i=0;i<t->size-1;++i){
        distanceQuadraticSum += (pow(t->cidades[i].x - t->cidades[i+1].x, 2) + pow(t->cidades[i].y - t->cidades[i+1].y, 2));
    }
    double dist_val = 1.0/ sqrt(distanceQuadraticSum);
    return dist_val;
}



void sel_best_trajeto(struct populacao *p){
    double maxDist = 0;
    p->best_id = 0;

    for(int i=0; i<p->size; ++i){
        //ultimo trajeto é melhor
        if (maxDist < p->trajetos[i].aptidao){
            maxDist = p->trajetos[i].aptidao;
            p->best_id = i;
        }
    }
    p->trajetos[k].aptidao = maxDist;

    /* encontra a melhor aptidao e copia as cidades */
    for(int i=0;i<p->trajetos[p->best_id].size;++i){
        p->trajetos[k].cidades[i].id = p->trajetos[p->best_id].cidades[i].id;
        p->trajetos[k].cidades[i].x = p->trajetos[p->best_id].cidades[i].x;
        p->trajetos[k].cidades[i].y = p->trajetos[p->best_id].cidades[i].y;
    }
}


void selecao(struct populacao *p, struct populacao *newp){
    double sum_fit = 0.0f;
    double prob = 0.0f;
    /* total de aptidao da populacao */
    for(int i=0; i< p->size; ++i){
        sum_fit+=p->trajetos[i].aptidao;
    }
    /* frequencia relativa aptidao */
    for(int i=0; i<p->size; ++i){
        p->trajetos[i].rapt = p->trajetos[i].aptidao/sum_fit;
    }
    p->trajetos[0].aapt = p->trajetos[0].rapt;

    /* frequencia absoluta aptidao */
    for(int i=1; i< p->size; ++i){
        p->trajetos[i].aapt = p->trajetos[i-1].aapt + p->trajetos[i].rapt;
    }
    /* escolhe herdeiros com a fa */
    for(int i=0;i<p->size;++i){
        prob = 0.01;
        for (int j = 0; j<p->size; ++j) {
            if (prob <= p->trajetos[j].aapt) {
                newp->trajetos[i] = p->trajetos[j];
                break;
            }
        }
    }
    /* atualizar nova população */
    for(int i=0 ;i<p->size; ++i)
        p->trajetos[i] = newp->trajetos[i];
}



void cruzamento(struct populacao *p){
    int nt = 0;
    double prob = 0.0f;
    struct trajeto *taux = NULL;

    for (int i = 0; i < p->size; ++i) {
        prob = rand()%1000/1000.0;
        if (prob < CRUZ_PROB){
            nt++;
            if(nt%2 == 0){
                for (int j = 0; j < p->trajetos[i].size; ++j) {
                    int id, x, y;
                    id = taux->cidades[j].id;
                    x = taux->cidades[j].x;
                    y = taux->cidades[j].y;

                    taux->cidades[i].id = p->trajetos[i].cidades[j].id;
                    taux->cidades[i].x = p->trajetos[i].cidades[j].x;
                    taux->cidades[i].y = p->trajetos[i].cidades[j].y;

                    p->trajetos[i].cidades[j].id = id;
                    p->trajetos[i].cidades[j].x = x;
                    p->trajetos[i].cidades[j].y = y;
                }
            } else{
                taux = &p->trajetos[i];
            }
        }
    }
}



void mutacao(struct populacao *p){
    double prob = 0.0f;
    for(int i=0;i<p->size;++i){
        for(int j=0;j<p->trajetos[i].size;++j){
            prob = rand()%1000/1000.0;
            if(prob < MUTATION_PROB){
                int rand_id = rand_pos(min, max);
                int id = p->trajetos[i].cidades[j].id;
                if(rand_id != id){
                    troca_cidades(&p->trajetos[i], id, rand_id);
                }
            }
        }
    }
}



void troca_cidades(struct trajeto *t, int id1, int id2){
    int id, x, y;
    int enc1 = 0, enc2 = 0;
    for (int i = 0; i < t->size; ++i) {
        if(t->cidades[i].id == id1)
            enc1 = i;
        if(t->cidades[i].id == id2)
            enc2 = i;
    }
    id = t->cidades[enc1].id;
    x = t->cidades[enc1].x;
    y = t->cidades[enc1].y;

    t->cidades[enc1].id = t->cidades[enc2].id;
    t->cidades[enc1].x = t->cidades[enc2].x;
    t->cidades[enc1].y = t->cidades[enc2].y;

    t->cidades[enc2].id = id;
    t->cidades[enc2].x = x;
    t->cidades[enc2].y = y;
}


int rand_pos(int l, int h){

    return rand()%(h - l) + l;
}

/*
 * troca o melhor trajeto da geração anterior pelo pior da atual
 * */
void elitismo(struct populacao *p){
    struct trajeto *melhor = (struct trajeto*) calloc(1, sizeof(struct trajeto));
    struct trajeto *pior = (struct trajeto*) calloc(1, sizeof(struct trajeto));

    int mid = 0, pid = 0;
    ini_cidades(melhor, traj_size);
    ini_cidades(pior, traj_size);
    melhor->aptidao = 0;
    pior->aptidao = 1;

    for (int i = 0; i < p->size; i++){
        if (melhor->aptidao < p->trajetos[i].aptidao){
            melhor->aptidao = p->trajetos[i].aptidao;
            mid = i;
        }
        else if(p->trajetos[i].aptidao < pior->aptidao){
            pior->aptidao = p->trajetos[i].aptidao;
            pid = i;
        }
    }


    if(melhor->aptidao <= p->trajetos[k].aptidao){
        for (int i = 0; i < p->trajetos[mid].size; ++i) {
            p->trajetos[k].cidades[i].id = p->trajetos[mid].cidades[i].id;
            p->trajetos[k].cidades[i].x = p->trajetos[mid].cidades[i].x;
            p->trajetos[k].cidades[i].y = p->trajetos[mid].cidades[i].y;
        }
        p->trajetos[k].aptidao = p->trajetos[mid].aptidao;

    } else{
        for (int i = 0; i < p->trajetos[pid].size; ++i) {
            p->trajetos[pid].cidades[i].id = p->trajetos[k].cidades[i].id;
            p->trajetos[pid].cidades[i].x = p->trajetos[k].cidades[i].x;
            p->trajetos[pid].cidades[i].y = p->trajetos[k].cidades[i].y;
        }
        p->trajetos[pid].aptidao = p->trajetos[k].aptidao;
    }
    free(pior);
    free(melhor);
}


void report_state(struct populacao *p){
    printf("Generaçao: %d | Melhor aptidao: %lf\n", p->gen, p->trajetos[k].aptidao);
}



void read_file_dataAG(){
    FILE *fp = fopen("C:\\Users\\Vasco\\CLionProjects\\ProjLP_AED\\testes\\data.txt", "r");
    if (fp == NULL){
        printf("Fail to open file %s.\n", strerror(errno));
        return;
    }
    printf("Open file OK.\n");
    int id = 0, x = 0, y = 0;
    int in_id = 0;

    dataset = (int **)calloc(MAX_TRAJETOS, sizeof(int*));
    for(int i=0;i<MAX_TRAJETOS;++i){
        dataset[i] = (int *)calloc(3, sizeof(int));
    }

    while (fscanf(fp,"%d,%d,%d", &id, &x, &y) != EOF){
        /* populate the training set vector*/
        dataset[in_id][0] = id;
        dataset[in_id][1] = x;
        dataset[in_id][2] = y;
        /* check if max input reached */
        if(in_id==MAX_TRAJETOS) break;
        in_id++;
    }
    fclose(fp);
    dataset_len = in_id;
    traj_size  = dataset_len;
    min = 1;
    max = traj_size;
}






