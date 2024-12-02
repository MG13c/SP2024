/* 5. Napisati program koji iz datoteke cita postfiks izraz i zatim korištenjem stoga racuna
rezultat. Stog je potrebno realizirati preko vezane liste.*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 50
#define MAX_LENGTH 1024

struct _stackElement;
typedef struct _stackElement* Position;
typedef struct _stackElement {
    double number;
    Position next;
} StackElement;


int calculatePostfixFromFile(Position head, char* fileName, double* result);
int readFile(char* fileName, char* buffer);
int parseStringIntoPostfix(Position head, char* buffer, double* result);
int checkStackAndExtractResult(Position head, double* result);
Position createStackElement(double number);
int push(Position head, Position newStackElement);
int printStack(Position first);
int pop(Position head, double* result);
int popAndPerformOperation(Position head, char operation, double* result);

int main() {
    StackElement head = { .number = 0, .next = NULL };
    double result = 0;

    if (calculatePostfixFromFile(&head, "postfix.txt", &result) == EXIT_SUCCESS) {
        printf("Result is: %0.1lf\n", result);
    }

    return EXIT_SUCCESS;
}

int calculatePostfixFromFile(Position head, char* fileName, double* result) {
    char buffer[MAX_LENGTH] = { 0 };
    int status = 0;

    if (readFile(fileName, buffer) != EXIT_SUCCESS || !buffer) {
        return EXIT_FAILURE;
    }

    status = parseStringIntoPostfix(head, buffer, result);
    if (status != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int readFile(char* fileName, char* buffer) {  //procitamo sve iz datoteke i spremimo u buffer
    FILE* filePointer = NULL;
    filePointer = fopen(fileName, "r");
    if (!filePointer) {
        perror("Can't open file!\n");
        return NULL;
    }

    fgets(buffer, MAX_LENGTH, filePointer);
    printf("|%s|\n", buffer);

    fclose(filePointer);

    return EXIT_SUCCESS;
}

int parseStringIntoPostfix(Position head, char* buffer, double* result)
{
    char* currentBuffer = buffer;
    int status = 0;
    int numBytes = 0;
    char operation = 0;
    double number = 0.0;
    Position newStackElement = NULL;

    while (strlen(currentBuffer) > 0) {
        status = sscanf(currentBuffer, " %lf %n", &number, &numBytes);//iz currentBuffera procitamo element i razmak
        if (status != 1) { //kad ne procitamo status nam je nula i ulazimo u if tj kad imamo neku operaciju
            sscanf(currentBuffer, " %c %n", &operation, &numBytes);
            status = popAndPerformOperation(head, operation, result);

            if (status != EXIT_SUCCESS) {
                return EXIT_FAILURE;
            }

            number = *result;  //tu zapisujemo rzultat kojeg smo dobili iz vrsenja operacije na neka dva broja
        }

        newStackElement = createStackElement(number); //number stavimo u novi element 
        if (!newStackElement) {
            return EXIT_FAILURE;
        }

        currentBuffer += numBytes; //currentBuffer nam se povecava za svaki procitani razmak
        printf("|%s| <-->", currentBuffer); //ispisujemo sve sta jos imamo u bufferu i onda <-->
        push(head, newStackElement); //izbacuje zadnji element tj dodaje ga u listu i ispisuje u stog 
    }

    return checkStackAndExtractResult(head, result);
}

int checkStackAndExtractResult(Position head, double* result) {
    int status = EXIT_SUCCESS;

    status = pop(head, result);

    if (status != EXIT_SUCCESS) {
        return status;
    }

    if (head->next) {
        system("cls"); //clear screen
        printf("Invalid postfix, please check the file!\r\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

Position createStackElement(double number)
{
    Position newStackElement = NULL;

    newStackElement = (Position)malloc(sizeof(StackElement));
    if (!newStackElement) {
        perror("Can't allocate memory!\n");
        return NULL;
    }

    newStackElement->number = number;
    newStackElement->next = NULL;

    return newStackElement;
}

int push(Position head, Position newStackElement) { //svaki novo procitani element stavljamo livo od zadnjeg procitanog, tako da nam zadnji bude 1.procitani
    newStackElement->next = head->next;
    head->next = newStackElement;

    printStack(head->next);

    return EXIT_SUCCESS;
}

int printStack(Position first) {
    Position current = first;

    while (current) {
        printf(" %0.1lf", current->number);
        current = current->next;
    }
    printf("\n");

    return EXIT_SUCCESS;
}

int pop(Position head, double* result) { //radimo pop na zadnjem dodanom broju
    Position toDelete = NULL;

    toDelete = head->next;
    if (!toDelete) {
        printf("Stack is empty! Nothing to pop!\n");
        return -1;
    }

    head->next = toDelete->next;
    *result = toDelete->number;//upisujemo zadnji broj u rezultat, a to je ustvari operand 1 ili 2, a brisemo ga iz liste
    free(toDelete);

    return EXIT_SUCCESS;
}

int popAndPerformOperation(Position head, char operation, double* result) {
    double operand1 = 0;
    double operand2 = 0;
    int status1 = 0;
    int status2 = 0;

    status1 = pop(head, &operand1); //zadnji broj dodan u stog upisujemo u operand 1
    if (status1 != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    status2 = pop(head, &operand2); //broj prije tog u operand 2
    if (status2 != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    switch (operation) {
    case '+':
        *result = operand2 + operand1;
        break;
    case '-':
        *result = operand2 - operand1;
        break;
    case '*':
        *result = operand2 * operand1;  //spremimo u resultat i vrtimo se nazad u if gdje smo pozvali popAndPerformOperation
        break;
    case '/':
        *result = operand2 / operand1;
        break;
    default:
        printf("Operation %c not supported yet!\r\n", operation);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}