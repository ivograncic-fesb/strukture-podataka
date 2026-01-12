#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Grad {
    char ime[50];
    int stanovnici;
    struct Grad* left;
    struct Grad* right;
    struct Grad* next;
} Grad;

typedef struct Drzava {
    char ime[50];
    Grad* gradovi;          
    struct Drzava* next;
} Drzava;

typedef struct DrzavaBST {
    char ime[50];
    Grad* gradovi;          
    struct DrzavaBST* left;
    struct DrzavaBST* right;
} DrzavaBST;

/* -------------------- USPOREDBA -------------------- */
int usporediGradove(Grad* g1, Grad* g2) {
    if (g1->stanovnici != g2->stanovnici)
        return g1->stanovnici - g2->stanovnici;
    return strcmp(g1->ime, g2->ime);
}

/* -------------------- INSERTI -------------------- */
Grad* insertGradBST(Grad* root, Grad* novi) {
    if (!root) return novi;
    if (usporediGradove(novi, root) < 0)
        root->left = insertGradBST(root->left, novi);
    else
        root->right = insertGradBST(root->right, novi);
    return root;
}

Drzava* insertDrzavaLista(Drzava* head, Drzava* nova) {
    if (!head || strcmp(nova->ime, head->ime) < 0) {
        nova->next = head;
        return nova;
    }
    Drzava* t = head;
    while (t->next && strcmp(nova->ime, t->next->ime) > 0)
        t = t->next;
    nova->next = t->next;
    t->next = nova;
    return head;
}

Grad* insertGradLista(Grad* head, Grad* novi) {
    if (!head || usporediGradove(novi, head) < 0) {
        novi->next = head;
        return novi;
    }
    Grad* t = head;
    while (t->next && usporediGradove(novi, t->next) > 0)
        t = t->next;
    novi->next = t->next;
    t->next = novi;
    return head;
}

DrzavaBST* insertDrzavaBST(DrzavaBST* root, DrzavaBST* nova) {
    if (!root) return nova;
    if (strcmp(nova->ime, root->ime) < 0)
        root->left = insertDrzavaBST(root->left, nova);
    else
        root->right = insertDrzavaBST(root->right, nova);
    return root;
}

/* -------------------- UCITAVANJE -------------------- */
Grad* ucitajGradoveBST(char* imeDat) {
    FILE* f = fopen(imeDat, "r");
    if (!f) return NULL;

    Grad* root = NULL;
    char ime[50]; int br;

    while (fscanf(f, " %[^,],%d", ime, &br) == 2) {
        Grad* g = malloc(sizeof(Grad));
        strcpy(g->ime, ime);
        g->stanovnici = br;
        g->left = g->right = g->next = NULL;
        root = insertGradBST(root, g);
    }
    fclose(f);
    return root;
}

Grad* ucitajGradoveLista(char* imeDat) {
    FILE* f = fopen(imeDat, "r");
    if (!f) return NULL;

    Grad* head = NULL;
    char ime[50]; int br;

    while (fscanf(f, " %[^,],%d", ime, &br) == 2) {
        Grad* g = malloc(sizeof(Grad));
        strcpy(g->ime, ime);
        g->stanovnici = br;
        g->left = g->right = g->next = NULL;
        head = insertGradLista(head, g);
    }
    fclose(f);
    return head;
}

/* -------------------- ISPISI -------------------- */
void pretragaGradovaBST(Grad* root, int min) {
    if (!root) return;
    pretragaGradovaBST(root->left, min);
    if (root->stanovnici > min)
        printf("   %s (%d)\n", root->ime, root->stanovnici);
    pretragaGradovaBST(root->right, min);
}

void ispisListaDrzava(Drzava* head, int min) {
    while (head) {
        printf("\nDrzava: %s\n", head->ime);
        pretragaGradovaBST(head->gradovi, min);
        head = head->next;
    }
}

/* -------------------- FREE FUNKCIJE -------------------- */

// brise stablo gradova
void oslobodiGradBST(Grad* root) {
    if (!root) return;
    oslobodiGradBST(root->left);
    oslobodiGradBST(root->right);
    free(root);
}

// brise listu gradova
void oslobodiGradLista(Grad* head) {
    while (head) {
        Grad* temp = head;
        head = head->next;
        free(temp);
    }
}

// brise listu drzava (A dio)
void oslobodiDrzavaLista(Drzava* head) {
    while (head) {
        oslobodiGradBST(head->gradovi);
        Drzava* temp = head;
        head = head->next;
        free(temp);
    }
}

// brise stablo drzava (B dio)
void oslobodiDrzavaBST(DrzavaBST* root) {
    if (!root) return;
    oslobodiDrzavaBST(root->left);
    oslobodiDrzavaBST(root->right);
    oslobodiGradLista(root->gradovi);
    free(root);
}

/* -------------------- MAIN -------------------- */
int main() {
    FILE* f = fopen("drzave.txt", "r");
    if (!f) {
        printf("Greska pri otvaranju datoteke.\n");
        return 1;
    }

    Drzava* listaDrzava = NULL;
    DrzavaBST* stabloDrzava = NULL;

    char imeDrzave[50], datGradova[50];

    while (fscanf(f, "%s %s", imeDrzave, datGradova) == 2) {
        Drzava* d = malloc(sizeof(Drzava));
        strcpy(d->ime, imeDrzave);
        d->gradovi = ucitajGradoveBST(datGradova);
        d->next = NULL;
        listaDrzava = insertDrzavaLista(listaDrzava, d);

        DrzavaBST* db = malloc(sizeof(DrzavaBST));
        strcpy(db->ime, imeDrzave);
        db->gradovi = ucitajGradoveLista(datGradova);
        db->left = db->right = NULL;
        stabloDrzava = insertDrzavaBST(stabloDrzava, db);
    }
    fclose(f);

    int min;
    printf("Unesi minimalan broj stanovnika: ");
    scanf("%d", &min);

    ispisListaDrzava(listaDrzava, min);

    // 🔥 OSLABAĐANJE MEMORIJE
    oslobodiDrzavaLista(listaDrzava);
    oslobodiDrzavaBST(stabloDrzava);

    return 0;
}
