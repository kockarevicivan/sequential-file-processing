#include <stdio.h>

#include "functions.h"
#include "common.h"

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

int form_serial_file() {

    //mogu samo slogovi
    FILE* serial_change = fopen("serial_change.bin", "wb");
    SYLLABLE_CHANGE temp;

    system("cls");

    while(1) {
        printf("Unesite opciju: \n");
        printf("1.Kreiranje sloga\n");
        printf("2.Izmena sloga\n");
        printf("3.Brisanje sloga\n");
        printf("4.Povratak na glavni meni\n");

        int option;
        fflush(stdin);
        scanf("%d", &option);

        if(option < 1 || option > 4) {
            system("cls");
            printf("Nevalidna opcija. Pokusajte ponovo.\n");
            continue;
        }
        else if(option == 4) {
            break;
        }

        printf("Unesite evidencioni broj sloga: \n");
        int record_number = -1;

        while(1) {
            fflush(stdin);
            scanf("%d", &record_number);
            if(record_number >= 10000000 && record_number <= 99999999) {
                break;
            }
            system("cls");
            printf("Nevalidan evidencioni broj. Pokusajte ponovo.");
        }

        system("cls");
        if(option == 1) {
            printf("Kreiranje sloga:\n");
            temp = fill_syllable(record_number);
            temp.change_type = (ChangeType)option;
        }
        else if(option == 2) {
            printf("Izmena sloga:\n");
            temp = fill_syllable(record_number);
            temp.change_type = (ChangeType)option;
        }
        else if(option == 3) {
            printf("Brisanje sloga:\n");
            temp.record_number = record_number;
            temp.change_type = (ChangeType)option;
        }
        else {
            return INV_OPT_ERR;
        }

        fwrite(&temp, sizeof(SYLLABLE_CHANGE), 1, serial_change);
    }

    fclose(serial_change);

    return 0;
}

SYLLABLE_CHANGE fill_syllable(int record_number) {
    SYLLABLE_CHANGE temp;
    temp.record_number = record_number;
    ///TODO Make protections
    fflush(stdin);
    printf("Unesite ime projekcije: \n");
    scanf("%s", temp.projection_name);

    fflush(stdin);
    printf("Unesite sifru sale: \n");
    scanf("%s", temp.hall_label);

    fflush(stdin);
    printf("Unesite trajanje projekcije: \n");
    scanf("%d", &(temp.projection_duration));

    return temp;
}

void print_file(char* file_name) {
    FILE* active_file = fopen(file_name, "rb");
    BLOCK temp_block;

    system("cls");
    printf("ISPIS AKTIVNE DATOTEKE:\n");

    int i, k = 1;
    while (fread(&temp_block, sizeof(BLOCK), 1, active_file)) {

        printf("\nBlok broj %d sa adresom: %p\n", k, ftell(active_file));

        for(i = 0; i < F_BLOCK; i++) {

            if (temp_block.syllables[i].record_number == -5) {
                return;
            }

            printf("Redni broj sloga unutar bloka: %d\n", i);
            printf("%d\t", temp_block.syllables[i].record_number);
            printf("%s\t", temp_block.syllables[i].projection_name);
            printf("%s\t", temp_block.syllables[i].hall_label);
            printf("%d\t\n", temp_block.syllables[i].projection_duration);
        }
        k++;
    }
}

void serial_to_sequential() {

    FILE* bin_file_r = fopen("serial_change.bin", "rb");

    SYLLABLE_CHANGE* ret_array;
    SYLLABLE_CHANGE temp;
    int i = 0;

    while(fread(&temp, sizeof(SYLLABLE_CHANGE), 1, bin_file_r)) {
        ret_array = realloc(ret_array, (i+1) * sizeof(SYLLABLE_CHANGE));
        ret_array[i] = temp;
        i++;
    }

    fclose(bin_file_r);

    ret_array = sort_syllables(ret_array, i);

    FILE* sequential_change = fopen("sequential_change.bin", "wb");

    int k;
    for(k = 0; k < i; k++) {

        fwrite(&(ret_array[k]), sizeof(SYLLABLE_CHANGE), 1, sequential_change);
    }

    fclose(sequential_change);
}

SYLLABLE_CHANGE* sort_syllables(SYLLABLE_CHANGE* raw_data, int n) {

    int i, j;
    for(i = 0; i < n; i++) {
        for(j = i; j < n; j++) {
            SYLLABLE_CHANGE* a = (raw_data+i);
            SYLLABLE_CHANGE* b = (raw_data+j);

            if(a->record_number > b->record_number) {
                swap_syllables(a, b);
            }
        }
    }

    return raw_data;
}

void swap_syllables(SYLLABLE_CHANGE* a, SYLLABLE_CHANGE* b) {
	SYLLABLE_CHANGE temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

void print_sequential() {
    FILE* sequential_change = fopen("sequential_change.bin", "rb");

    SYLLABLE_CHANGE temp;

    int k;
    for(k = 0; k < 6; k++) {
        fread(&temp, sizeof(SYLLABLE_CHANGE), 1, sequential_change);
        printf("%d %s %s %d %d\n", temp.record_number, temp.projection_name, temp.hall_label,
               temp.projection_duration, (int)temp.change_type);
    }

    fclose(sequential_change);
}
