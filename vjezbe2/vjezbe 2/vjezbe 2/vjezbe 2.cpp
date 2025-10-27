#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct person {
    char firstName[50];
    char lastName[50];
    int birthYear;
    struct person* next;
} Person;

// definiranje strukture
Person* CreatePerson(const char* firstName, const char* lastName, int birthYear) {
    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (!newPerson) {
        printf("Memory allocation error!\n");
        return NULL;
    }
    strcpy(newPerson->firstName, firstName);
    strcpy(newPerson->lastName, lastName);
    newPerson->birthYear = birthYear;
    newPerson->next = NULL;
    return newPerson;
}

// dodaj na pocetak
void AddToBeginning(Person** head, const char* firstName, const char* lastName, int birthYear) {
    Person* newPerson = CreatePerson(firstName, lastName, birthYear);
    if (!newPerson) return;
    newPerson->next = *head;
    *head = newPerson;
}

// dodaj na kraj
void AddToEnd(Person** head, const char* firstName, const char* lastName, int birthYear) {
    Person* newPerson = CreatePerson(firstName, lastName, birthYear);
    if (!newPerson) return;

    if (*head == NULL) {
        *head = newPerson;
        return;
    }

    Person* temp = *head;
    while (temp->next != NULL)
        temp = temp->next;
    temp->next = newPerson;
}

// ispis liste
void PrintList(Person* head) {
    if (head == NULL) {
        printf("\nThe list is empty.\n");
        return;
    }

    printf("\nList of persons:\n");
    while (head != NULL) {
        printf("%s %s (%d)\n", head->firstName, head->lastName, head->birthYear);
        head = head->next;
    }
}

// pronadi po prezimenu
Person* FindByLastName(Person* head, const char* lastName) {
    while (head != NULL) {
        if (strcmp(head->lastName, lastName) == 0)
            return head;
        head = head->next;
    }
    return NULL;
}

// obrisi po prezimenu
void DeleteByLastName(Person** head, const char* lastName) {
    Person* temp = *head;
    Person* prev = NULL;

    while (temp != NULL && strcmp(temp->lastName, lastName) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Person with last name '%s' not found.\n", lastName);
        return;
    }

    if (prev == NULL)
        *head = temp->next;
    else
        prev->next = temp->next;

    free(temp);
    printf("Person '%s' has been deleted.\n", lastName);
}

int main() {
    Person* head = NULL;
    int choice;
    char firstName[50], lastName[50];
    int birthYear;

    do {
        printf("1. dodaj osobu na pocetak\n");
        printf("2. dodaj osobu na kraj\n");
        printf("3. ispisi listu\n");
        printf("4. pronadi po prezimenu\n");
        printf("5. obrisi po prezimenu\n");
        printf("0. izlaz\n");
        printf("unesi svoj izbor: ");
        scanf("%d", &choice);
        getchar(); 

        switch (choice) {
        case 1:
            printf("unesi ime: ");
            fgets(firstName, sizeof(firstName), stdin);
            firstName[strcspn(firstName, "\n")] = 0;

            printf("unei prezime: ");
            fgets(lastName, sizeof(lastName), stdin);
            lastName[strcspn(lastName, "\n")] = 0;

            printf("uneis godinu rodenja: ");
            scanf("%d", &birthYear);

            AddToBeginning(&head, firstName, lastName, birthYear);
            break;

        case 2:
            printf("unesi ime: ");
            fgets(firstName, sizeof(firstName), stdin);
            firstName[strcspn(firstName, "\n")] = 0;

            printf("unesi prezime: ");
            fgets(lastName, sizeof(lastName), stdin);
            lastName[strcspn(lastName, "\n")] = 0;

            printf("unesi godinu rodenja: ");
            scanf("%d", &birthYear);

            AddToEnd(&head, firstName, lastName, birthYear);
            break;

        case 3:
            PrintList(head);
            break;

        case 4:
            printf("unesi prezime koje trazis: ");
            fgets(lastName, sizeof(lastName), stdin);
            lastName[strcspn(lastName, "\n")] = 0;

            {
                Person* found = FindByLastName(head, lastName);
                if (found)
                    printf("pronaden/a: %s %s (%d)\n", found->firstName, found->lastName, found->birthYear);
                else
                    printf("osoba nije pronadena.\n");
            }
            break;

        case 5:
            printf("uneis prezime koje treba obrisati: ");
            fgets(lastName, sizeof(lastName), stdin);
            lastName[strcspn(lastName, "\n")] = 0;

            DeleteByLastName(&head, lastName);
            break;

        case 0:
            printf("kraj programa...\n");
            break;

        default:
            printf("krivi odabir\n");
        }
    } while (choice != 0);

    // Free memory before exiting
    Person* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}
