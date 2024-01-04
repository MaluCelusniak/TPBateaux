/*===========================================================================
 * ENTÊTE DU PROGRAMME
 *===========================================================================
 *
 *                    THÈME : Location de bateaux
 *
 *===========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "MachineBateaux.h"


/*----------------------LEXIQUE PRINCIPAL---------------------------------*/
/* Voir surtout MachineBateaux.h */

RubanBateaux Rd, Rr; //{le ruban des départs et le ruban des retours}
RubanLocations Rl; // {rubans des locations}

/*------------------------------------------------------------------------*/
void saisie(RubanBateaux* rb) {
    // {ei: La machine bateaux a été déclarée. rb est soit le ruban des départs soit celui des retours
    //      rb->nomFichier est le nom du fichier où vont être enregistrés les bateaux}
    Bateau b;

    amorcer(rb);
    puts("Terminez la saisie avec 0 comme numéro de bateau...");
    do {
        puts("Entrer un bateau (numéro , (heures, minutes))");
        do {
            printf("Numéro : ");
            b.num = lireEntier();
        } while (b.num < 0 || b.num > NBATMAX);

        if (b.num != 0) {
            do {
                printf("Heure : ");
                b.h.heures = lireEntier();
            } while (b.h.heures < H_OUVR || b.h.heures > H_FERM);

            if (b.h.heures == H_FERM) {
                b.h.minutes = 0;
                printf("Minutes : 00");
            } else {
                do {
                    printf("Minutes : ");
                    b.h.minutes = lireEntier();
                } while (b.h.minutes < 0 || b.h.minutes > 59);
            }
            enregistrer(rb, b);
        }
    } while (b.num != 0);
    marquer(rb);
    // *** {ef :  on a composé un ruban de bateaux}
}

/*-----------------------------------------------------------------------*/
void saisieRubans() {
    int choix;

    puts("-----------------");
    puts("Saisie des rubans");
    puts("_________________");
    do {
        //  *** {MENU SAISIE} ***
        puts("\n\n     -- Menu Saisie --\n");
        puts("\t1 : fichier des départs");
        puts("\t2 : fichier des retours");
        puts("\t3 : sortie de Menu Saisie");
        //  *** {test de validité du choix} ***
        do {
            printf("\t\tVotre choix : ");
            choix = lireEntier();
        } while (choix < 1 || choix > 3);

        switch (choix) {
            case 1:
                saisie(&Rd);
                break;
            case 2:
                saisie(&Rr);
                break;
            case 3:
                break;
        }
    } while (choix != 3);
}

/*------------------------------------------------------------------------*/
void afficheUnRubanBateaux(RubanBateaux* rb) {
    Bateau b;
    demarrer(rb);
    while (bc(rb).num != MARQUE.num) {
        b = bc(rb);
        printf("<%d, <%d, %02d>>\n", b.num, b.h.heures, b.h.minutes);
        avancer(rb);
    }
    fermer(rb);
}

/*------------------------------------------------------------------------*/
void afficheBateaux() {
    // {ei : les rubans de bateaux sont disponibles }

    printf("------------------------------------\n");
    printf("Affichage des Départs et des Retours\n");
    printf("____________________________________\n");
    printf("les Départs\n");
    afficheUnRubanBateaux(&Rd);

    printf("les Retours\n");
    afficheUnRubanBateaux(&Rr);
    // { ef : on a affiché la séquence des départs et celle des retours }
}

/*------------------------------------------------------------------------
 *--------           DESCRIPTION DES PROCÉDURES ASSOCIÉES
 *--------           AUX AUTRES FONCTIONNALITÉS DEMANDÉES
 *------------------------------------------------------------------------*/



Duree Plus(Duree d, Heure h) {
        int SomM = d.nbMinutes + h.minutes;
        if (SomM <= 59) {
            return (Duree){d.nbHeures + h.heures,SomM};
        } else {
            return (Duree){1 + d.nbHeures + h.heures,SomM - 60};
        }
    }

Duree Moins(Duree d1,Duree d2) {
        int DiffM = d1.nbMinutes - d2.nbMinutes;
        if (DiffM >= 0) {
            return (Duree){d1.nbHeures - d2.nbHeures, DiffM};
        } else {
            return (Duree){d1.nbHeures - d2.nbHeures - 1, 60+DiffM};
        }
    }
    
void tempsLocation() {
    
    // Programme de calcul du temps total d'occupation des bateaux pendant la journée

    // {Etat Initial : les rubans de Bateaux sont disponibles}

    Duree SomHd = {0, 0};
    Duree SomHr = {0, 0};

    demarrer(&Rd);
    demarrer(&Rr);
    while (bc(&Rd).num != MARQUE.num)
    {
        SomHd = Plus(SomHd, bc(&Rd).h);
        SomHr = Plus(SomHr, bc(&Rr).h);

        avancer(&Rd);
        avancer(&Rr);
    }

    Duree somMoins = Moins(SomHr, SomHd);

    printf("le temps total d'occupation est: %d:%02d\n", somMoins.nbHeures, somMoins.nbMinutes);

    //{Etat Final : on a affiche le temps total de location des bateaux durant la journee ecoulee}
}


void locationsInf2h() {
       //--{affiche le nombre de bateaux dont la durée de location est inférieure à 2 heures}

    Duree SomHd = {0, 0};
    Duree SomHr = {0, 0};

    demarrer(&Rd);
    demarrer(&Rr);

    int numLocationInfDeux = 0;
    
    while (bc(&Rd).num != MARQUE.num) {
        SomHd = Plus(SomHd, bc(&Rd).h);
        SomHr = Plus(SomHr, bc(&Rr).h);

        Duree somMoins = Moins(SomHr, SomHd);

        if (somMoins.nbHeures < 2) {
            numLocationInfDeux++;
        }

        avancer(&Rd);
        avancer(&Rr);
    };

    printf("Le nombre de locations dont la durée de location est inférieure à 2 heures: %d\n", numLocationInfDeux);
}

void creerLocations() {
   
    //--{Création du Ruban des locations}
    Location l;
    printf("La séquence des locations est\n");
    demarrer(&Rd);
    demarrer(&Rr);
    amorcerL(&Rl);
    while (bc(&Rd).num != MARQUE.num)
    {
        
        printf("barco num: %d", bc(&Rd).num);
        // Afficher le segment de temps de début de location
        printf("< %d , < %d , %d >  ", bc(&Rd).num, bc(&Rd).h.heures, bc(&Rd).h.minutes);
        printf("< %d , %d > >   \n", bc(&Rr).h.heures, bc(&Rr).h.minutes);
        l.num = bc(&Rd).num;
        l.depart.heures = bc(&Rd).h.heures;
        l.depart.minutes = bc(&Rd).h.minutes;
        l.retour.heures = bc(&Rr).h.heures;
        l.retour.minutes = bc(&Rr).h.minutes;
        enregistrerL(&Rl, l);
        avancer(&Rd);
        avancer(&Rr);
    };
        marquerL(&Rl);

    printf("\n");


}

void afficheUnRubanLocations(RubanLocations* rl)
{
    Location l;
    // demarrer(&Rd);
    // demarrer(&Rr);
    demarrerL(rl);
    while (lc(rl).num != LMARQUE.num)
    {
        l = lc(rl);
        printf("< %d , < %d:%02d , %d:%02d > > ", l.num, l.depart.heures, l.depart.minutes, l.retour.heures, l.retour.minutes);
        
        avancerL(rl);
    }
    fermerL(rl);
}



void afficherLocations () {
    //--{Affihage du Ruban des locations}

    printf("-------------------------\n");
    printf("Affichage des Locations\n");
    printf("-------------------------\n");
    afficheUnRubanLocations(&Rl);

}

void tempsMaxAttente () {
    // Function to calculate the wait time between two rentals in minutes
    //era pra ser currentRental e previousRental mas não sei como botar previous
    int calculateWaitTime(lc) { 
        return (lc(&Rl).depart.heures - previousRental->endTime.heures) * 60 +
            (lc(&Rl).depart.heures - previousRental->endTime.minutes);
    }

    // Function to calculate the maximum wait time for a boat
    int calculateMaxWaitTime(lc(&Rl).departs, lc(&Rl).retours, int numBateaux) {
        int maxWaitTime = 0;
        int currentWaitTime = 0;

        for (int i = 1; i < MARQUE.num; ++i) {
            if (lc(&Rl).num == numBateaux) {
                currentWaitTime = calculateWaitTime(&returns->rentals[i - 1], &departures->rentals[i]);
                if (currentWaitTime > maxWaitTime) {
                    maxWaitTime = currentWaitTime;
                }
            }
        }

        return maxWaitTime;
    }

     for (int numBateaux = 1; numBateaux <= numBoats; ++numBateaux) {
        int maxWaitTime = calculateMaxWaitTime(&boatDepartures, &boatReturns, numBateaux);
        printf("bateau numéro %d: attente la plus longue : %d minutes\n", numBateaux, maxWaitTime);
    }

}



//------------------------ la suite à rédiger   -----------------------



/*------------------------- ALGORITHME PRINCIPAL ---------------------*/
int main(int nbArgs, char* arg[])
{
    /*------- Lexique -------*/
    int choix;

    changeNomBateaux(&Rd, "fich_departs");
    changeNomBateaux(&Rr, "fich_retours");
    changeNomLocations(&Rl, "fich_location");
    do {

        // *** {MENU PRINCIPAL} ***
        puts("\n\n                     -----  M E N U -----\n");
        puts("\t1 : Saisie des heures de départs ou d'arrivée des bateaux.");
        puts("\t2 : Affichage du ruban des départs puis de celui des retours.");
        puts("\t3 : Temps total de location des bateaux.");
        puts("\t4 : Nombre de locations inférieures à deux heures.");
        puts("\t5 : Création du ruban des locations.");
        puts("\t6 : Affichage du ruban des locations.");
        puts("\t7 : Temps maximum d'attente.");
        puts("\t8 : Quitter le programme.");

        // {Saisie et test de validité du choix}
        do {
            printf("\t\tVotre choix : ");
            choix = lireEntier();
        } while (choix < 1 || choix > 8);

        switch (choix) {
            case 1:
                saisieRubans();
                break;

            case 2:
                afficheBateaux();
                break;

            case 3:
                tempsLocation();
                break;

            case 4:
                locationsInf2h();
                break;

            case 5:
                creerLocations();
                break;

            case 6:
                afficherLocations();
                break;

            case 7:
                tempsMaxAttente();
                break;

            case 8:
                break;

        }

    } while (choix != 8);

    return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------*/
/*                       FIN DU TP1   BATEAUX                               */
/*--------------------------------------------------------------------------*/
