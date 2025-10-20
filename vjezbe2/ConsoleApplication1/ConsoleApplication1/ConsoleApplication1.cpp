#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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

//Dodavanje na pocetak liste
int AddAtBeginning(Position head, const char* name) {								//dinamicko stvaranje novog cvora
	Position newPerson = (Position)malloc(sizeof(struct Person));
	if (!newPerson) {
		printf("Failed to allocate memory");
		return -1;
	}

	strcpy_s(newPerson->name, MAX, name);
	newPerson->next = head->next;													//next pokazivac novog cvora pokazuje na prvi element liste
	head->next = newPerson;															//head pokazivac pokazuje na novi cvor

	return 0;
}
//Ispis liste
void PrintList(Position head) {
	Position pointer;
	for (pointer = head->next; pointer != NULL; pointer = pointer->next) {			//prolazak kroz listu
		printf("Name: %s\n", pointer->name);
	}
}
//Dodavanje na kraj liste
int AddAtEnd(Position head, const char* name) {										
	Position newPerson = (Position)malloc(sizeof(struct Person));
	if (!newPerson) {
		printf("Failed to allocate memory");
		return -1;
	}
	strcpy_s(newPerson->name, MAX, name);
	newPerson->next = NULL;															//novi cvor ce biti na kraju liste pa next pokazuje na NULL

	Position pointer = head;
	while (pointer->next != NULL) {
		pointer = pointer->next;

	}
	pointer->next = newPerson;														
	return 0;
}
//Pronalazak po prezimenu
Person* FindByName(Position head, const char* name) {								//trazenje cvora sa zadatim prezimenom
	Position pointer = head->next;

	while (pointer != NULL) {
		if (SurnameEquals(pointer->name, name)) {										//pronaden cvor sa zadatim prezimenom
			return pointer;
		}
		pointer = pointer->next;												
	}
	return NULL;
}
//Brisanje iz liste
int DeleteByName(Position head, const char* name) {									//trazenje i brisanje cvora sa zadatim prezimenom
	Position previous = head;														
	Position current = head->next;	

	while (current != NULL && !SurnameEquals(current->name, name)) {					
		previous = current;
		current = current->next;
	}
	if (current == NULL) {
		printf("Person with surname %s not found.\n", name);
		return -1;
	}
	previous->next = current->next;													//izbacivanje cvora iz liste
	free(current);
	return 0;
}
//Glavni program
int main() {
	struct Person head;																
	head.next = NULL;

	AddAtBeginning(&head, "Marko Maric");											
	AddAtEnd(&head, "Iva Ivic");
	AddAtBeginning(&head, "Anita Anic");

	printf("List of persons:\n");
	PrintList(&head);

	const char* surnameToFind = "Ivic";
	Position foundPerson = FindByName(&head, surnameToFind);						
	if (foundPerson) 
		printf("Found person: %s\n", foundPerson->name);
	else 
		printf("Person with surname %s not found.\n", surnameToFind);
	
	const char* surnameToDelete = "Maric";
	if (DeleteByName(&head, surnameToDelete) == 0) {
		printf("Deleted person with surname %s\n", surnameToDelete);
	}

	printf("List after deletion:\n");
	PrintList(&head);

	return 0;
}