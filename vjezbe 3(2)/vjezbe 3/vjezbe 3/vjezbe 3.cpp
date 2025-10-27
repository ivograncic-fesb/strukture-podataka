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

// nova osoba
Person* CreatePerson(const char* firstName, const char* lastName, int birthYear) {
    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (!newPerson) {
        printf("greska u alociranju memorije\n");
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
        printf("\nlista je prazna.\n");
        return;
    }

    printf("\nlista ljudi:\n");
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
        printf("osoba s prezimenom '%s' nije pronadena\n", lastName);
        return;
    }

    if (prev == NULL)
        *head = temp->next;
    else
        prev->next = temp->next;

    free(temp);
    printf("osoba '%s' je obrisana\n", lastName);
}

// 3. zadatak

   // dodaj nakon zadane osobe
void AddAfter(Person* head, const char* targetLastName, const char* firstName, const char* lastName, int birthYear) {
    Person* target = FindByLastName(head, targetLastName);
    if (!target) {
        printf("osoba s prezimenom '%s' niej pronadena\n", targetLastName);
        return;
    }

    Person* newPerson = CreatePerson(firstName, lastName, birthYear);
    newPerson->next = target->next;
    target->next = newPerson;
}

// dodaj prije zadane osobe
void AddBefore(Person** head, const char* targetLastName, const char* firstName, const char* lastName, int birthYear) {
    Person* newPerson = CreatePerson(firstName, lastName, birthYear);
    if (!newPerson) return;

	// ako je lista prazna
    if (*head == NULL) {
        *head = newPerson;
        return;
    }

	// ako je ciljna osoba na pocetku
    if (strcmp((*head)->lastName, targetLastName) == 0) {
        newPerson->next = *head;
        *head = newPerson;
        return;
    }

    Person* prev = *head;
    Person* curr = (*head)->next;

    while (curr != NULL && strcmp(curr->lastName, targetLastName) != 0) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL) {
        printf("Person with last name '%s' not found.\n", targetLastName);
        free(newPerson);
        return;
    }

    newPerson->next = curr;
    prev->next = newPerson;
}

// sortiraj listu po prezimenu
void SortList(Person** head) {
    if (*head == NULL || (*head)->next == NULL)
        return;

    int swapped;
    Person* ptr1, * lptr = NULL;
    do {
        swapped = 0;
        ptr1 = *head;

        while (ptr1->next != lptr) {
            if (strcmp(ptr1->lastName, ptr1->next->lastName) > 0) {
                // swap data
                char tempFirst[50], tempLast[50];
                int tempYear;

                strcpy(tempFirst, ptr1->firstName);
                strcpy(tempLast, ptr1->lastName);
                tempYear = ptr1->birthYear;

                strcpy(ptr1->firstName, ptr1->next->firstName);
                strcpy(ptr1->lastName, ptr1->next->lastName);
                ptr1->birthYear = ptr1->next->birthYear;

                strcpy(ptr1->next->firstName, tempFirst);
                strcpy(ptr1->next->lastName, tempLast);
                ptr1->next->birthYear = tempYear;

                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

// lista u datoteku
void WriteToFile(Person* head, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("greska u otvaranju datoteke\n");
        return;
    }

    Person* temp = head;
    while (temp != NULL) {
        fprintf(file, "%s %s %d\n", temp->firstName, temp->lastName, temp->birthYear);
        temp = temp->next;
    }

    fclose(file);
    printf("lista je uspjesno smjestena u '%s'.\n", filename);
}

// procitaj iz datoteke
void ReadFromFile(Person** head, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("greska u otvaranju datoteke\n");
        return;
    }

	// ocisti trenutnu listu
    Person* temp;
    while (*head != NULL) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }

    char firstName[50], lastName[50];
    int birthYear;
    while (fscanf(file, "%s %s %d", firstName, lastName, &birthYear) == 3) {
        AddToEnd(head, firstName, lastName, birthYear);
    }

    fclose(file);
    printf("lista je uspjesno ucitana iz '%s'.\n", filename);
}

int main() {
    Person* head = NULL;
    int choice;
    char firstName[50], lastName[50], targetLastName[50], filename[100];
    int birthYear;

    do {
        printf("1. dodaj osobu na pocetak\n");
        printf("2. dodaj osobu na kraj\n");
        printf("3. ispisi listui\n");
        printf("4. pronadi po prezimenu\n");
        printf("5. obrisi po prezimenu\n");
        printf("6. dodaj nakon osobe\n");
        printf("7. dodaj prije osobe\n");
        printf("8. sortiraj listu po prezimenu\n");
        printf("9. upisi listu u datoteku\n");
        printf("10. procitaj listu s datoteke\n");
        printf("0. kraj\n");
        printf("upisi svoj izbor: ");
        scanf("%d", &choice);
        getchar(); 

        switch (choice) {
        case 1:
            printf("upisi ime: ");
            fgets(firstName, sizeof(firstName), stdin);
            firstName[strcspn(firstName, "\n")] = 0;

            printf("upisi prezime: ");
            fgets(lastName, sizeof(lastName), stdin);
            lastName[strcspn(lastName, "\n")] = 0;

            printf("upisi godinu rodenja: ");
            scanf("%d", &birthYear);
            getchar();

            AddToBeginning(&head, firstName, lastName, birthYear);
            break;

        case 2:
            printf("upisi ime: ");
            fgets(firstName, sizeof(firstName), stdin);
            firstName[strcspn(firstName, "\n")] = 0;

            printf("upisi prezime: ");
            fgets(lastName, sizeof(lastName), stdin);
            lastName[strcspn(lastName, "\n")] = 0;

            printf("upisi godinu rodenja: ");
            scanf("%d", &birthYear);
            getchar();

            AddToEnd(&head, firstName, lastName, birthYear);
            break;

        case 3:
            PrintList(head);
            break;

        case 4:
            printf("upisi prezime koje trazis: ");
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
            printf("upisi prezime koje brises: ");
            fgets(lastName, sizeof(lastName), stdin);
            lastName[strcspn(lastName, "\n")] = 0;
            DeleteByLastName(&head, lastName);
            break;

        case 6:
            printf("upisi prezime osobe: ");
            fgets(targetLastName, sizeof(targetLastName), stdin);
            targetLastName[strcspn(targetLastName, "\n")] = 0;

            printf("upisi novo ime: ");
            fgets(firstName, sizeof(firstName), stdin);
            firstName[strcspn(firstName, "\n")] = 0;

            printf("upisi novo prezime: ");
            fgets(lastName, sizeof(lastName), stdin);
            lastName[strcspn(lastName, "\n")] = 0;

            printf("upisi godinu rodenja: ");
            scanf("%d", &birthYear);
            getchar();

            AddAfter(head, targetLastName, firstName, lastName, birthYear);
            break;

        case 7:
            printf("upisi prezime osobe: ");
            fgets(targetLastName, sizeof(targetLastName), stdin);
            targetLastName[strcspn(targetLastName, "\n")] = 0;

            printf("upisi novo ime: ");
            fgets(firstName, sizeof(firstName), stdin);
            firstName[strcspn(firstName, "\n")] = 0;

            printf("upisi novo prezime: ");
            fgets(lastName, sizeof(lastName), stdin);
            lastName[strcspn(lastName, "\n")] = 0;

            printf("upisi godinu rodenja: ");
            scanf("%d", &birthYear);
            getchar();

            AddBefore(&head, targetLastName, firstName, lastName, birthYear);
            break;

        case 8:
            SortList(&head);
            printf("lista sortirana po prezimenu\n");
            break;

        case 9:
            printf("upisi ime datoteke za spremanje: ");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0;
            WriteToFile(head, filename);
            break;

        case 10:
            printf("upisi ime datoteke za ucitavanje: ");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0;
            ReadFromFile(&head, filename);
            break;

        case 0:
            printf("napustanje programa...\n");
            break;

        default:
            printf("krivi odabir\n");
        }
    } while (choice != 0);

	// ocisti memoriju
    Person* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}
