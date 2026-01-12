#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>              
#include <stdlib.h>

// struktura čvora binarnog stabla pretraživanja
typedef struct Cvor {
    int vrijednost;             
    struct Cvor* lijevo;        
    struct Cvor* desno;         
} Cvor;

// funkcija za stvaranje novog čvora
Cvor* stvoriCvor(int x) {
    Cvor* novi = (Cvor*)malloc(sizeof(Cvor));
    if (novi == NULL)
        return NULL;
    novi->vrijednost = x;
    novi->lijevo = NULL;
    novi->desno = NULL;
    return novi;
}

// umetanje elementa u stablo
Cvor* umetni(Cvor* korijen, int x) {
    if (korijen == NULL)
        return stvoriCvor(x);

    if (x < korijen->vrijednost)
        korijen->lijevo = umetni(korijen->lijevo, x);
    else if (x > korijen->vrijednost)
        korijen->desno = umetni(korijen->desno, x);

    return korijen;
}

// inorder ispis
int inorder(Cvor* korijen) {
    int brojac = 0;
    if (korijen != NULL) {
        brojac += inorder(korijen->lijevo);
        printf("%d ", korijen->vrijednost);
        brojac++;
        brojac += inorder(korijen->desno);
    }
    return brojac;
}

// preorder ispis
int preorder(Cvor* korijen) {
    int brojac = 0;
    if (korijen != NULL) {
        printf("%d ", korijen->vrijednost);
        brojac++;
        brojac += preorder(korijen->lijevo);
        brojac += preorder(korijen->desno);
    }
    return brojac;
}

// postorder ispis
int postorder(Cvor* korijen) {
    int brojac = 0;
    if (korijen != NULL) {
        brojac += postorder(korijen->lijevo);
        brojac += postorder(korijen->desno);
        printf("%d ", korijen->vrijednost);
        brojac++;
    }
    return brojac;
}

// pronalaženje minimalnog čvora
Cvor* minCvor(Cvor* korijen) {
    Cvor* trenutni = korijen;
    while (trenutni != NULL && trenutni->lijevo != NULL)
        trenutni = trenutni->lijevo;
    return trenutni;
}

// brisanje elementa
Cvor* obrisi(Cvor* korijen, int x) {
    if (korijen == NULL)
        return NULL;

    if (x < korijen->vrijednost)
        korijen->lijevo = obrisi(korijen->lijevo, x);
    else if (x > korijen->vrijednost)
        korijen->desno = obrisi(korijen->desno, x);
    else {
        if (korijen->lijevo == NULL) {
            Cvor* temp = korijen->desno;
            free(korijen);
            return temp;
        }
        if (korijen->desno == NULL) {
            Cvor* temp = korijen->lijevo;
            free(korijen);
            return temp;
        }
        Cvor* temp = minCvor(korijen->desno);
        korijen->vrijednost = temp->vrijednost;
        korijen->desno = obrisi(korijen->desno, temp->vrijednost);
    }
    return korijen;
}

// pretraživanje
int pretrazi(Cvor* korijen, int x) {
    if (korijen == NULL)
        return 0;
    if (korijen->vrijednost == x)
        return 1;
    if (x < korijen->vrijednost)
        return pretrazi(korijen->lijevo, x);
    else
        return pretrazi(korijen->desno, x);
}

// funkcija za oslobađanje cijelog stabla
void oslobodiStablo(Cvor* korijen) {
    if (korijen == NULL)
        return;

    oslobodiStablo(korijen->lijevo);
    oslobodiStablo(korijen->desno);
    free(korijen);
}

// izbornik
int izbornik() {
    int izbor;
    printf("\n1 - Umetni element\n");
    printf("2 - Inorder ispis\n");
    printf("3 - Preorder ispis\n");
    printf("4 - Postorder ispis\n");
    printf("5 - Obrisi element\n");
    printf("6 - Pretrazi element\n");
    printf("0 - Izlaz\n");
    printf("Izbor: ");
    scanf("%d", &izbor);
    return izbor;
}

int main() {
    Cvor* korijen = NULL;
    int izbor, x;

    do {
        izbor = izbornik();

        if (izbor == 1) {
            scanf("%d", &x);
            korijen = umetni(korijen, x);
        }
        else if (izbor == 2) {
            inorder(korijen);
        }
        else if (izbor == 3) {
            preorder(korijen);
        }
        else if (izbor == 4) {
            postorder(korijen);
        }
        else if (izbor == 5) {
            scanf("%d", &x);
            korijen = obrisi(korijen, x);
        }
        else if (izbor == 6) {
            scanf("%d", &x);
            if (pretrazi(korijen, x))
                printf("Postoji\n");
            else
                printf("Ne postoji\n");
        }

    } while (izbor != 0);

  
    oslobodiStablo(korijen);

    return 0;
}
