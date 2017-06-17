#include "vaisseau.h"


/*Fonction qui initialise les drones*/
//i pour le numero de colis et j pour le numero du slot
Colis_t Init_colis(int i, int j, Colis_t colis){
    colis.ID_client = get_random(NB_CLIENT,0);
    colis.priorite =  get_random(3,1);
    colis.poids = get_random(drone[j].charge,1);
    colis.temps = get_random(AUTONOMIE,1);
    colis.zone = client[colis.ID_client].zone;
    colis.etat = get_random(2,1);
    client[colis.ID_client].NBColis++;
    printf("Colis pour client %d a pour priorite %d, pour un temps maxi %d min, et pese %d a pour destination %d, et l'etat du colis est %d\n", colis.ID_client, colis.priorite, colis.temps, colis.poids, colis.zone, colis.etat);
    return colis;
}

int totalColis(int nb){
    return vaisseau.NBColis + nb;
}

Slot_t initAllColis(int i){
    vaisseau.slot[i].NBColisSlot = 0;
    int j;
        for(j = 0; j<NB_COLIS; ++j){
            vaisseau.slot[i].colis[j] = Init_colis(i, j, vaisseau.slot[i].colis[j]);
            vaisseau.slot[i].NBColisSlot++;
        }
        vaisseau.NBColis = totalColis(vaisseau.slot[i].NBColisSlot);
    return vaisseau.slot[i];
}

/*Fonction qui tri les colis*/
Slot_t triColis(Slot_t slot){
    Colis_t tmp; //variable temporaire
    int i, j;
    int l = 0;
    int k = NB_COLIS;

    /*Tri par priorite*/
    for(i =0 ; i<k; ++i){
        for(j = i+1; j<k; ++j){
            if(slot.colis[l].priorite>slot.colis[j].priorite){
                //printf("VRAI\n");
                tmp = slot.colis[l];
                slot.colis[l] = slot.colis[j];
                slot.colis[j] = tmp;
            }
        }
        l++;
    }

    l = 0;
    /*Tri en fonction par priorite et par le temps de livraison croissant*/
    for(i = 0; i<k; ++i){
        for(j = i+1; j<k; ++j){
            if(slot.colis[l].priorite==slot.colis[j].priorite){
                if(slot.colis[l].temps>slot.colis[j].temps){
                    tmp = slot.colis[l];
                    slot.colis[l] = slot.colis[j];
                    slot.colis[j] = tmp;
                }
            }
        }
        l++;
    }
    //printf("TRI\n");
    for(i = 0; i<k; ++i){
            printf("Colis pour client %d, priorite %d, temps %d\n", slot.colis[i].ID_client, slot.colis[i].priorite, slot.colis[i].temps);
    }
    return slot;
}

void* fonction_vaisseau(void* arg){
    Vaisseau_t *vaisseau = (Vaisseau_t*) arg;
    while(vaisseau->NBColis > 0){
        pthread_mutex_lock(&mVaisseau);
        pthread_cond_wait(&cVaisseau, &mVaisseau);
        printf("REMPLISSAGE COLIS\n");
        vaisseau->NBColis = (NB_SLOT-1) * NB_COLIS;
        pthread_cond_signal(&cDrone);
        pthread_mutex_unlock(&mVaisseau);
    }
    pthread_exit(NULL);
}
