#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Struktura direktorija – child-sibling reprezentacija stabla

typedef struct Directory {
    char name[50];                 // ime direktorija
    struct Directory* child;        // prvo dijete (poddirektorij)
    struct Directory* sibling;      // sljedeći direktorij na istoj razini
    struct Directory* parent;       // roditelj (za cd..)
} Directory;


   // Funkcija za kreiranje novog direktorija

Directory* createDirectory(const char* name, Directory* parent) {
    Directory* dir = (Directory*)malloc(sizeof(Directory));
    if (dir == NULL) {
        printf("Memory allocation error!\n");
        exit(1);
    }

    strcpy(dir->name, name);
    dir->child = NULL;
    dir->sibling = NULL;
    dir->parent = parent;

    return dir;
}


  //md – dodavanje novog direktorija u trenutni direktorij

void makeDirectory(Directory* current) {
    char name[50];
    printf("Enter directory name: ");
    scanf("%s", name);

    Directory* newDir = createDirectory(name, current);

    // Ako trenutni direktorij nema djece, novi postaje prvo dijete
    if (current->child == NULL) {
        current->child = newDir;
    }
    else {
        // Inače se dodaje na kraj sibling liste
        Directory* temp = current->child;
        while (temp->sibling != NULL)
            temp = temp->sibling;
        temp->sibling = newDir;
    }
}

// cd dir – ulazak u poddirektorij

Directory* changeDirectory(Directory* current) {
    char name[50];
    printf("Enter directory name: ");
    scanf("%s", name);

    Directory* temp = current->child;

    // Pretraga među djecom trenutnog direktorija
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0)
            return temp;
        temp = temp->sibling;
    }

    printf("Directory not found!\n");
    return current;
}


 // cd.. – povratak u prethodni direktorij

Directory* goBack(Directory* current) {
    if (current->parent != NULL)
        return current->parent;

    printf("Already in root directory!\n");
    return current;
}


// dir – ispis sadržaja trenutnog direktorija

void listDirectory(Directory* current) {
    Directory* temp = current->child;

    if (temp == NULL) {
        printf("Directory is empty.\n");
        return;
    }

    while (temp != NULL) {
        printf("%s\n", temp->name);
        temp = temp->sibling;
    }
}

//Glavni program – meni koji simulira DOS naredbe

int main() {
    int choice;

    // Kreiranje root direktorija
    Directory* root = createDirectory("root", NULL);
    Directory* current = root;

    while (1) {
        printf("\nCurrent directory: %s\n", current->name);
        printf("1 - md (make directory)\n");
        printf("2 - cd dir\n");
        printf("3 - cd..\n");
        printf("4 - dir\n");
        printf("5 - exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            makeDirectory(current);
            break;
        case 2:
            current = changeDirectory(current);
            break;
        case 3:
            current = goBack(current);
            break;
        case 4:
            listDirectory(current);
            break;
        case 5:
            printf("Exiting program...\n");
            return 0;
        default:
            printf("Invalid choice!\n");
        }
    }
}
