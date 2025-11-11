#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct _postfix* Position;

typedef struct _postfix {
    double number;
    Position next;
} Postfix;

// Prototipi funkcija
Position createNode(double number);
void push(Position head, double number);
double pop(Position head);
double calculatePostfix(Position head, const char* filename);

int main(void) {
    Postfix head;
    head.number = 0;
    head.next = NULL;

    // Naziv datoteke
    const char* filename = "postfix.txt";

    double rezultat = calculatePostfix(&head, filename);
    printf("Rezultat izraza: %.2lf\n", rezultat);

    return 0;
}


Position createNode(double number) {
    Position newNode = (Position)malloc(sizeof(Postfix));
    if (!newNode) {
        printf("Greska: memorijska alokacija nije uspjela!\n");
        exit(EXIT_FAILURE);
    }
    newNode->number = number;
    newNode->next = NULL;
    return newNode;
}

// PUSH — dodaj broj na vrh stoga
void push(Position head, double number) {
    Position newNode = createNode(number);
    newNode->next = head->next;
    head->next = newNode;
}

// POP — skini broj sa stoga i vrati ga
double pop(Position head) {
    if (head->next == NULL) {
        printf("Greska: stog je prazan!\n");
        exit(EXIT_FAILURE);
    }
    Position temp = head->next;
    double value = temp->number;
    head->next = temp->next;
    free(temp);
    return value;
}

// Funkcija za računanje izraza iz datoteke
double calculatePostfix(Position head, const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Greska: ne mogu otvoriti datoteku '%s'!\n", filename);
        exit(EXIT_FAILURE);
    }

    char token[50];

    while (fscanf(f, " %49s", token) == 1) {
        // a) Ako je broj → push
        if (isdigit(token[0]) ||
            (token[0] == '-' && isdigit(token[1]))) {
            double value = atof(token);
            push(head, value);
        }
        // b) Ako je operator → pop 2 broja, računaj, push rezultat
        else if (strlen(token) == 1) {
            double b = pop(head);
            double a = pop(head);
            double result = 0;

            switch (token[0]) {
            case '+': result = a + b; break;
            case '-': result = a - b; break;
            case '*': result = a * b; break;
            case '/':
                if (b == 0) {
                    printf("Greska: dijeljenje s nulom!\n");
                    fclose(f);
                    exit(EXIT_FAILURE);
                }
                result = a / b;
                break;
            default:
                printf("Nepoznat operator: %s\n", token);
                fclose(f);
                exit(EXIT_FAILURE);
            }
            push(head, result);
        }
        // c) Trash — ako nije ni broj ni operator
        else {
            printf("Upozorenje: nepoznat unos '%s' (preskacem)\n", token);
        }
    }

    fclose(f);

    // Na stogu bi trebao ostati samo jedan element → rezultat
    if (head->next == NULL || head->next->next != NULL) {
        printf("Greska: neispravan postfiks izraz!\n");
        exit(EXIT_FAILURE);
    }

    return pop(head);
}

