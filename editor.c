/// Copyright 2017 Gheorghe Bogdan

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Node {
    struct _Node *next;
    struct _Node *previous;
    char character;
} Node;

void determineNumberOfCharacters(int *numberOfCharacters,
                                 FILE *pointerToFile);

void scanDoubleLinkedList(Node **headOfList,
                          FILE *pointerToFile,
                          int numberOfElements);

void moveCursor(FILE *pointerToFile, Node **headOfList);

void insertString(FILE *pointerToFile, Node **headOfList);

void deleteNextElements(FILE *pointerToFile, Node **headOfList);

void deleteFromHead(Node **headOfList);

void deleteElement(Node **elementFromList);

void copyElements(FILE *pointerToFile,
                  char *addressOfElementsToCopy,
                  Node *headOfList);

void pasteElementsBehindCurrent(Node **headOfList, char *elementsToCopy);

void backspace(Node **headOfList);

void saveListInArray(Node *elementFromList, char *arrayOfElements);

void saveArrayInList(Node **headOfList, char *arrayElements);

void findNumberOfElementsFromList(Node *elementFromList,
                                  int *numberOfElements);

void determinePositionOfElementInList(int *actualPosition,
                                      Node *elementFromList);

void positionTheCoursorAtHeadOfList(Node **elementFromList);

void goToPosition(Node **headOfList, int position);

void makeArrayForReplace(Node *headOfList,
                         char *arrayForUndo,
                         char **arrayForReplace);

void printListFromHeadToTextFile(Node *headOfList, FILE *pointerToFile);

void printListFromHead(Node *headOfList);

void printListFromBack(Node *headOfList);

void cleanList(Node *headOfList);

int main(int argc, char *argv[]) {
    Node *headOfList = NULL;
    FILE *pointerToFileDateIn;
    FILE *pointerToFileForInput;
    FILE *pointerToFileOperatiiIn;
    int position;
    int position1;
    int numberOfCharacters;
    int numberOfOperations;
    int indexOperations;
    int lengthOfTest;
    char nameOfOperation[9];
    char elementsToCopy[500];
    char arrayForUndo[500];
    char arrayForRedo[500];
    char *arrayForReplace = (char *)malloc(sizeof(char));
    if (!(pointerToFileDateIn = fopen(argv[1], "rt"))) {
        fprintf(stdout, "File date.in can't be opened.");
        exit(1);
    }
    if (!(pointerToFileOperatiiIn = fopen(argv[2], "rt"))) {
        fprintf(stdout, "File operatii.in can't be opened.");
        exit(1);
    }
    if (!(pointerToFileForInput = fopen(argv[3], "wt"))) {
        fprintf(stdout, "The file rezultate.txt can't be opened");
        exit(1);
    }
    determineNumberOfCharacters(&numberOfCharacters, pointerToFileDateIn);
    scanDoubleLinkedList(&headOfList, pointerToFileDateIn, numberOfCharacters);
    fscanf(pointerToFileOperatiiIn, "%d", &numberOfOperations);
    for (indexOperations = 0;
         indexOperations < numberOfOperations;
         indexOperations++) {
        fscanf(pointerToFileOperatiiIn, "%s", *(&nameOfOperation));
        if (!strcmp(nameOfOperation, "move")) {
            determinePositionOfElementInList(&position, headOfList);
            saveListInArray(headOfList, arrayForUndo);
            moveCursor(pointerToFileOperatiiIn, &headOfList);
        } else if (!strcmp(nameOfOperation, "insert")) {
            determinePositionOfElementInList(&position, headOfList);
            saveListInArray(headOfList, arrayForUndo);
            insertString(pointerToFileOperatiiIn, &headOfList);
        } else if (!strcmp(nameOfOperation, "del")) {
            determinePositionOfElementInList(&position, headOfList);
            saveListInArray(headOfList, arrayForUndo);
            deleteNextElements(pointerToFileOperatiiIn, &headOfList);
        } else if (!strcmp(nameOfOperation, "copy")) {
            determinePositionOfElementInList(&position, headOfList);
            saveListInArray(headOfList, arrayForUndo);
            copyElements(pointerToFileOperatiiIn, elementsToCopy, headOfList);
        } else if (!strcmp(nameOfOperation, "paste")) {
            determinePositionOfElementInList(&position, headOfList);
            saveListInArray(headOfList, arrayForUndo);
            pasteElementsBehindCurrent(&headOfList, elementsToCopy);
        } else if (!strcmp(nameOfOperation, "backspace")) {
            determinePositionOfElementInList(&position, headOfList);
            saveListInArray(headOfList, arrayForUndo);
            backspace(&headOfList);
        } else if (!strcmp(nameOfOperation, "undo")) {
            determinePositionOfElementInList(&position1, headOfList);
            saveListInArray(headOfList, arrayForRedo);
            cleanList(headOfList);
            headOfList = NULL;
            saveArrayInList(&headOfList, arrayForUndo);
            positionTheCoursorAtHeadOfList(&headOfList);
            goToPosition(&headOfList, position);
        } else if (!strcmp(nameOfOperation, "redo")) {
            cleanList(headOfList);
            headOfList = NULL;
            saveArrayInList(&headOfList, arrayForRedo);
            positionTheCoursorAtHeadOfList(&headOfList);
            goToPosition(&headOfList, position1);
        }/** else if (!strcmp(nameOfOperation, "replace")) {//When i have time./
            saveListInArray(headOfList, &arrayForUndo);
            makeArrayForReplace(headOfList,
                                arrayForUndo,
                                &arrayForReplace);
            saveArrayInList(&headOfList, arrayForReplace);
            positionTheCoursorAtHeadOfList(&headOfList);
            goToPosition(&headOfList, position);
        } **/
    }
    ///printListFromHead(headOfList);
    ///printListFromBack(headOfList);
    printListFromHeadToTextFile(headOfList, pointerToFileForInput);
    free(arrayForReplace);
    cleanList(headOfList);
    fclose(pointerToFileDateIn);
    fclose(pointerToFileForInput);
    fclose(pointerToFileOperatiiIn);
    return 0;
}

void determineNumberOfCharacters(int *numberOfCharacters, FILE *pointerToFile) {
    int length;
    fseek(pointerToFile, 0L, SEEK_END);
    length = ftell(pointerToFile);
    (*numberOfCharacters) = length / sizeof(char);
    fseek(pointerToFile, 0L, SEEK_SET);
}

void scanDoubleLinkedList(Node **headOfList,
                          FILE *pointerToFile,
                          int numberOfElements){
    Node *previousNode;
    int indexElements;
    for (indexElements = 0; indexElements < numberOfElements; indexElements++) {
        Node *newNode;
        newNode = (Node *)malloc(sizeof(Node));
        fscanf(pointerToFile, "%c", &newNode->character);
        newNode->next     = NULL;
        newNode->previous = NULL;
        if (*headOfList) {
            previousNode->next = newNode;
            newNode->previous = previousNode;
        } else {
            (*headOfList) = newNode;
        }
        previousNode  = newNode;
    }
}

void moveCursor(FILE *pointerToFile, Node **headOfList) {
    int numberOfPositions;
    fscanf(pointerToFile, "%d", &numberOfPositions);
    if (numberOfPositions > 0) {
        while ((*headOfList)->next && numberOfPositions) {
            (*headOfList) = (*headOfList)->next;
            numberOfPositions--;
        }
    } else if (numberOfPositions < 0) {
        while ((*headOfList)->previous && numberOfPositions) {
            (*headOfList) = (*headOfList)->previous;
            numberOfPositions++;
        }
    }
}

void insertString(FILE *pointerToFile, Node **headOfList) {
    char *stringToInsert = (char *)malloc(500 * sizeof(char));
    Node *previousNode, *newHead;
    int indexElements;
    int numberOfElements;
    fscanf(pointerToFile, "%s", &(*stringToInsert));
    numberOfElements = strlen(stringToInsert);
    for (indexElements = 0; indexElements < numberOfElements; indexElements++) {
        Node *newNode;
        newNode = (Node *)malloc(sizeof(Node));
        newNode->character = stringToInsert[indexElements];
        newNode->next      = NULL;
        newNode->previous  = NULL;
        if (indexElements) {
            previousNode->next = newNode;
            newNode->previous = previousNode;
        } else {
            newHead = newNode;
        }
        previousNode = newNode;
    }
    if ((*headOfList)->previous) {
        (*headOfList)->previous->next = newHead;
        newHead->previous = (*headOfList)->previous;
    } else {
        newHead->previous = NULL;
    }
    previousNode->next = (*headOfList);
    (*headOfList)->previous = previousNode;
    free(stringToInsert);
}

void deleteNextElements(FILE *pointerToFile, Node **headOfList) {
    Node *newHead, *auxiliarNode;
    int numberOfElementsToDelete;
    int indexElements;
    fscanf(pointerToFile, "%d", &numberOfElementsToDelete);
    if ((*headOfList)->previous) {
        newHead = (*headOfList)->previous;
        for (indexElements = 0;
             indexElements < numberOfElementsToDelete && (*headOfList);
             indexElements++) {
            auxiliarNode = (*headOfList);
            (*headOfList) = (*headOfList)->next;
            deleteElement(&auxiliarNode);
        }
        newHead->next = (*headOfList);
        (*headOfList)->previous = newHead;
    } else {
        auxiliarNode = (*headOfList);
        for (indexElements = 0;
             indexElements < numberOfElementsToDelete;
             indexElements++) {
            (*headOfList) = (*headOfList)->next;
            deleteFromHead(&auxiliarNode);
        }
    }
}

void deleteElement(Node **elementFromList) {
    Node *auxiliar;
    auxiliar = (*elementFromList);
    (*elementFromList) = (*elementFromList)->next;
    free(auxiliar);
}

void deleteFromHead(Node **headOfList) {
    Node *auxiliar;
    auxiliar = (*headOfList);
    (*headOfList) = (*headOfList)->next;
    (*headOfList)->previous = NULL;
    free(auxiliar);
}

void copyElements(FILE *pointerToFile,
                  char *addressOfElementsToCopy,
                  Node *headOfList) {
    int numberOfElementsToCopy;
    int indexElements;
    fscanf(pointerToFile, "%d", &numberOfElementsToCopy);
    for (indexElements = 0;
         indexElements < numberOfElementsToCopy && headOfList;
         indexElements++) {
        addressOfElementsToCopy[indexElements] = headOfList->character;
        headOfList = headOfList->next;
    }
}

void pasteElementsBehindCurrent(Node **headOfList, char *elementsToCopy) {
    int numberOfElementsToPaste;
    int indexElements;
    Node *newHead, *previousNode;
    numberOfElementsToPaste = strlen(elementsToCopy);
    for (indexElements = 0;
         indexElements < numberOfElementsToPaste;
         indexElements++) {
        Node *newNode;
        newNode = (Node *)malloc(sizeof(Node));
        newNode->character = elementsToCopy[indexElements];
        newNode->next      = NULL;
        newNode->previous  = NULL;
        if (indexElements) {
            previousNode->next = newNode;
            newNode->previous = previousNode;
        } else {
            newHead = newNode;
        }
        previousNode = newNode;
    }
    if ((*headOfList)->previous && (*headOfList)->next) {
        (*headOfList)->previous->next = newHead;
        newHead->previous = (*headOfList)->previous;
        previousNode->next = (*headOfList);
        (*headOfList)->previous = previousNode;
    } else if ((!(*headOfList)->next) && (*headOfList)->previous) {
        newHead->previous = (*headOfList);
        (*headOfList)->next = newHead;
        (*headOfList) = previousNode;
    } else if ((!(*headOfList)->previous) && (*headOfList)->next){
        newHead->previous = NULL;
        previousNode->next = (*headOfList);
        (*headOfList)->previous = previousNode;
    }


}

void backspace(Node **headOfList) {
    Node *auxiliar;
    if ((*headOfList)->previous && (*headOfList)->next) {
        auxiliar = (*headOfList)->previous;
        (*headOfList)->previous = auxiliar->previous;
        auxiliar->previous->next = (*headOfList);
        free(auxiliar);
    } else if (!(*headOfList)->next) {
        (*headOfList) = (*headOfList)->previous;
        free((*headOfList)->next);
        (*headOfList)->next = NULL;
    }
}

void saveListInArray(Node *elementFromList, char *addressOfArray) {
    int numberOfElements;
    int indexElements;
    findNumberOfElementsFromList(elementFromList, &numberOfElements);
    positionTheCoursorAtHeadOfList(&elementFromList);
    for (indexElements = 0;
         indexElements < numberOfElements && elementFromList;
         indexElements++) {
        addressOfArray[indexElements] = elementFromList->character;
        elementFromList = elementFromList->next;
    }
    addressOfArray[indexElements] = '\0';
}

void saveArrayInList(Node **headOfList, char *arrayOfElements) {
    int numberOfElements;
    int indexElements;
    Node *previousNode;
    numberOfElements = strlen(arrayOfElements);
    for (indexElements = 0; indexElements < numberOfElements; indexElements++) {
        Node *newNode;
        newNode = (Node *)malloc(sizeof(char));
        newNode->next      = NULL;
        newNode->previous  = NULL;
        newNode->character = arrayOfElements[indexElements];
        if (*headOfList) {
            newNode->previous = previousNode;
            previousNode->next = newNode;
        } else {
            (*headOfList) = newNode;
        }
        previousNode = newNode;
    }
}

void positionTheCoursorAtHeadOfList(Node **elementFromList) {
    while ((*elementFromList)->previous) {
        (*elementFromList) = (*elementFromList)->previous;
    }
}

void determinePositionOfElementInList(int *actualPosition,
                                      Node *elementFromList) {
    (*actualPosition) = 1;
    while (elementFromList->previous) {
        elementFromList = elementFromList->previous;
        (*actualPosition)++;
    }
}

void findNumberOfElementsFromList(Node *elementFromList,
                                  int *numberOfElements) {
    while (elementFromList->next) {
        (*numberOfElements)++;
        elementFromList = elementFromList->next;
    }
}

void goToPosition(Node **headOfList, int position) {
    position--;
	while ((*headOfList)->next && position) {
        (*headOfList) = (*headOfList)->next;
        position--;
    }
}

void makeArrayForReplace(Node *headOfList,
                         char *arrayForUndo,
                         char **arrayForReplace) {
    int numberOfCharacters;
    int indexElements;
    char *wordToReplace = (char *)malloc(500 * sizeof(char));
    char *wordReplaced = (char *)malloc(500 * sizeof(char));
    int lengthWordToReplace;
    char *auxiliarForReplacing = (char *)malloc(sizeof(char));
    int auxiliarForActualPosition;
    positionTheCoursorAtHeadOfList(&headOfList);
    fscanf(stdin, "%s", &(*wordReplaced));
    fscanf(stdin, "%s", &(*wordToReplace));

    for (indexElements = 0;
         headOfList->next;
         indexElements++, headOfList = headOfList->next) {
        if (headOfList->character != ' ') {
            auxiliarForReplacing = (char *)realloc(auxiliarForReplacing,
                                                   (indexElements + 1)*
                                                   sizeof(char));
            auxiliarForReplacing[indexElements] = headOfList->character;
            if (!strcmp(auxiliarForReplacing, wordToReplace)) {
                break;
            }
        } else {
            indexElements = 0;
        }
    }
    lengthWordToReplace = strlen(wordToReplace);
    while (headOfList->previous && lengthWordToReplace) {
        headOfList = headOfList->previous;
        lengthWordToReplace--;
    }
}

void printListFromHeadToTextFile(Node *headOfList, FILE *pointerToFile) {
    positionTheCoursorAtHeadOfList(&headOfList);
    while (headOfList) {
        fprintf(pointerToFile, "%c", headOfList->character);
        headOfList = headOfList->next;
    }
}

void printListFromHead(Node *headOfList) {
  positionTheCoursorAtHeadOfList(&headOfList);
  while (headOfList) {
      fprintf(stdout, "%c", headOfList->character);
      headOfList = headOfList->next;
  }
}

void printListFromBack(Node *headOfList) {
    while (headOfList->next) {
      headOfList = headOfList->next;
    }
    while (headOfList->previous) {
        fprintf(stdout, "%c", headOfList->character);
        headOfList = headOfList->previous;
    }
}

void cleanList(Node *headOfList) {
    Node *auxiliar;
    positionTheCoursorAtHeadOfList(&headOfList);
    while(headOfList) {
        auxiliar = headOfList;
        headOfList = headOfList->next;
        free(auxiliar);
    }
}

