#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS
#define MAX 50

typedef struct Person* Position;

struct Person {
    char name[MAX];
    Position next;
};

// helper: returns 1 if fullname's surname equals surnameToMatch
static int SurnameEquals(const char* fullname, const char* surnameToMatch) {
    const char* lastSpace = strrchr(fullname, ' ');
    const char* actualSurname = lastSpace ? lastSpace + 1 : fullname;
    return strcmp(actualSurname, surnameToMatch) == 0;
}

// izdvoji prezime iz punog imena 
static const char* GetSurname(const char* fullname) {
    const char* lastSpace = strrchr(fullname, ' ');
    return lastSpace ? lastSpace + 1 : fullname;
}

// dodavanje na početak liste
int AddAtBeginning(Position head, const char* name) {
    Position newPerson = (Position)malloc(sizeof(struct Person));
    if (!newPerson) {
        printf("Failed to allocate memory");
        return -1;
    }

    strcpy_s(newPerson->name, MAX, name);
    newPerson->next = head->next;
    head->next = newPerson;

    return 0;
}

// dodavanje na kraj liste
int AddAtEnd(Position head, const char* name) {
    Position newPerson = (Position)malloc(sizeof(struct Person));
    if (!newPerson) {
        printf("Failed to allocate memory");
        return -1;
    }
    strcpy_s(newPerson->name, MAX, name);
    newPerson->next = NULL;

    Position pointer = head;
    while (pointer->next != NULL) {
        pointer = pointer->next;
    }
    pointer->next = newPerson;
    return 0;
}

// ispis liste
void PrintList(Position head) {
    Position pointer;
    for (pointer = head->next; pointer != NULL; pointer = pointer->next) {
        printf("Name: %s\n", pointer->name);
    }
}

// pronalazak po prezimenu
Position FindByName(Position head, const char* surname) {
    Position pointer = head->next;

    while (pointer != NULL) {
        if (SurnameEquals(pointer->name, surname)) {
            return pointer;
        }
        pointer = pointer->next;
    }
    return NULL;
}

// brisanje po prezimenu
int DeleteByName(Position head, const char* surname) {
    Position previous = head;
    Position current = head->next;

    while (current != NULL && !SurnameEquals(current->name, surname)) {
        previous = current;
        current = current->next;
    }
    if (current == NULL) {
        printf("Person with surname %s not found.\n", surname);
        return -1;
    }
    previous->next = current->next;
    free(current);
    return 0;
}

// dodavanje iza određenog elementa 

int AddAfter(Position head, const char* surname, const char* newName) {
    Position target = FindByName(head, surname);
    if (!target) {
        printf("Person with surname %s not found.\n", surname);
        return -1;
    }

    Position newPerson = (Position)malloc(sizeof(struct Person));
    if (!newPerson) {
        printf("Failed to allocate memory.\n");
        return -1;
    }

    strcpy_s(newPerson->name, MAX, newName);
    newPerson->next = target->next;
    target->next = newPerson;

    return 0;
}

//dodavanje ispred odredenog elementa 

int AddBefore(Position head, const char* surname, const char* newName) {
    Position previous = head;
    Position current = head->next;

    while (current != NULL && !SurnameEquals(current->name, surname)) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Person with surname %s not found.\n", surname);
        return -1;
    }

    Position newPerson = (Position)malloc(sizeof(struct Person));
    if (!newPerson) {
        printf("Failed to allocate memory.\n");
        return -1;
    }

    strcpy_s(newPerson->name, MAX, newName);
    newPerson->next = current;
    previous->next = newPerson;

    return 0;
}

//sortiranje liste po prezimenima 

void SortBySurname(Position head) {
    if (!head->next || !head->next->next)
        return; // nema potrebe za sortiranjem

    Position i, j, prev_j, temp;
    int swapped;

    do {
        swapped = 0;
        prev_j = head;
        j = head->next;

        while (j->next != NULL) {
            const char* s1 = GetSurname(j->name);
            const char* s2 = GetSurname(j->next->name);

            if (strcmp(s1, s2) > 0) {
                // zamjena cvorova
                temp = j->next;
                j->next = temp->next;
                temp->next = j;
                prev_j->next = temp;
                swapped = 1;
            }

            prev_j = j;
            j = j->next;
        }
    } while (swapped);
}

//upis liste u datoteku

int WriteToFile(Position head, const char* filename) {
    FILE* fp = NULL;
    if (fopen_s(&fp, filename, "w") != 0 || !fp) {
        printf("Failed to open file for writing.\n");
        return -1;
    }

    Position p = head->next;
    while (p != NULL) {
        fprintf(fp, "%s\n", p->name);
        p = p->next;
    }

    fclose(fp);
    return 0;
}

// citanje liste iz datoteke

int ReadFromFile(Position head, const char* filename) {
    FILE* fp = NULL;
    if (fopen_s(&fp, filename, "r") != 0 || !fp) {
        printf("Failed to open file for reading.\n");
        return -1;
    }

    char buffer[MAX];
    while (fgets(buffer, MAX, fp)) {
        buffer[strcspn(buffer, "\n")] = '\0'; // ukloni \n
        AddAtEnd(head, buffer);
    }

    fclose(fp);
    return 0;
}

//glavni program

int main() {
    struct Person head;
    head.next = NULL;

    AddAtBeginning(&head, "Marko Maric");
    AddAtEnd(&head, "Iva Ivic");
    AddAtBeginning(&head, "Anita Anic");

    printf("=== Pocetna lista ===\n");
    PrintList(&head);

    printf("\nA) Dodavanje iza 'Anic':\n");
    AddAfter(&head, "Anic", "Luka Lukic");
    PrintList(&head);

    printf("\nB) Dodavanje ispred 'Ivic':\n");
    AddBefore(&head, "Ivic", "Petar Peric");
    PrintList(&head);

    printf("\nC) Sortiranje po prezimenima:\n");
    SortBySurname(&head);
    PrintList(&head);

    printf("\nD) Upis u datoteku 'osobe.txt'\n");
    WriteToFile(&head, "osobe.txt");

    printf("\nE) Citanje nove liste iz datoteke:\n");
    struct Person head2;
    head2.next = NULL;
    ReadFromFile(&head2, "osobe.txt");
    PrintList(&head2);

    return 0;
}
