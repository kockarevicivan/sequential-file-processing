#include <stdio.h>
#include "functions.h"

int create_file() {
    FILE* new_file;
    char file_name[MAX_FILE_NAME];

    system("cls");
    printf("Unesite naziv nove datoteke: \n");

    while(1) {
        scanf("%s", file_name);

        if(strlen(file_name) != 0) {
            break;
        }

        system("cls");
        printf("Nevalidan naziv datoteke. Pokusajte ponovo: \n");
    }

    strcat(file_name, ".bin");

    new_file = fopen(file_name, "wb");

    if(new_file == NULL) {
        printf("Datoteka nije kreirana.");
        return NULL_FILE_ERR;
    }

    fclose(new_file);

    printf("Datoteka '%s' je uspesno kreirana.", file_name);

    return 0;
}

int choose_active_file(FILE** current_active, char* active_name[]) {

    char new_active_name[MAX_FILE_NAME];

    system("cls");
    printf("Unesite ime fajla, koji zelite da postavite za aktivni:\n");

    while(1) {
        scanf("%s", new_active_name);

        if(strlen(new_active_name) != 0) {
            break;
        }

        system("cls");
        printf("Nevalidan naziv datoteke. Pokusajte ponovo: \n");
    }

    strcat(new_active_name, ".bin");
    FILE* new_active_file = fopen(new_active_name, "rb+");

    if (new_active_file == NULL) {
        printf("Greska, datoteka nije uspesno otvorena.");
        return NULL_FILE_ERR;
    }

    if (*current_active != NULL)
        fclose(*current_active);

    *current_active = new_active_file;

    printf("Datoteka '%s' je postavljena na aktivnu.", new_active_name);

    strcpy(active_name, new_active_name);

    return 0;
}
