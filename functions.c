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
    free(&temp);

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

    SYLLABLE_CHANGE* ret_array = (SYLLABLE_CHANGE*)malloc(sizeof(SYLLABLE_CHANGE));
    SYLLABLE_CHANGE temp;
    int i = 0;

    while(fread(&temp, sizeof(SYLLABLE_CHANGE), 1, bin_file_r)) {
        ret_array = realloc(ret_array, (i+1) * sizeof(SYLLABLE_CHANGE));
        ret_array[i] = temp;
        printf("%d %s %s %d %d\n", ret_array[i].record_number, ret_array[i].projection_name, ret_array[i].hall_label, ret_array[i].projection_duration, (int)ret_array[i].change_type);
        i++;
    }

    fclose(bin_file_r);

    ret_array = sort_syllables(ret_array, i);

    FILE* sequential_change = fopen("sequential_change.bin", "wb");

    int k;
    for(k = 0; k < i; k++) {

        fwrite(&(ret_array[k]), sizeof(SYLLABLE_CHANGE), 1, sequential_change);
    }

    free(ret_array);

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

int form_sequential_final(char* old_active_name) {
    system("cls");

    // Open necessary files
    FILE* sequential_internal = fopen("sequential_change.bin", "rb");
    FILE* old_active = fopen(old_active_name, "rb");
    FILE* error_file = fopen("error_file.bin", "wb");
    FILE* final_sequential = fopen("final_sequential.bin", "wb");

    BLOCK block_old, block_temp;
    BLOCK_CHANGE block_sequential, err_block;
    SYLLABLE syllable_old, syllable_temp;
    SYLLABLE_CHANGE syllable_sequential, syllable_err;

    int old_read_status = fread(&block_old, sizeof(BLOCK), 1, old_active);
    int seq_read_status = fread(&block_sequential, sizeof(BLOCK_CHANGE), 1, sequential_internal);

    int old_count = 0;
    int seq_count = 0;
    int write_count = 0;
    int err_count = 0;
    while(old_read_status || seq_read_status) {
        syllable_old = block_old.syllables[old_count];
        syllable_sequential = block_sequential.syllables[seq_count];

        // end?
        if(syllable_old.record_number == -5) {
            old_read_status = 0;
        }
        if(syllable_sequential.record_number == -5) {
            seq_read_status = 0;
        }

        // Code branches based on status variables
        if (old_read_status && seq_read_status) {
            // Syllable doesn't exist(only adding is a valid operation)
            if (syllable_sequential.record_number < syllable_old.record_number) {
                if(syllable_sequential.change_type == CREATE) {
                    syllable_temp.record_number = syllable_sequential.record_number;
                    strcpy(syllable_temp.projection_name, syllable_sequential.projection_name);
                    strcpy(syllable_temp.hall_label, syllable_sequential.hall_label);
                    syllable_temp.projection_duration = syllable_sequential.projection_duration;

                    block_temp.syllables[write_count++] = syllable_temp;

                    if (write_count == F_BLOCK) {
                        fwrite(&block_temp, sizeof(BLOCK), 1, final_sequential);
                        write_count = 0;
                    }
                }
                // Anything else is not a valid operation - write to error file
                else {
                    // Write to error file
                    err_block.syllables[err_count++] = syllable_sequential;

                    if (err_count == F_BLOCK) {
                        fwrite(&err_block, sizeof(BLOCK_CHANGE), 1, error_file);
                        err_count = 0;
                    }
                }

                seq_count++;
                if (seq_count == F_BLOCK) {
                    fread(&block_sequential, sizeof(BLOCK_CHANGE), 1, sequential_internal);
                    seq_count = 0;
                }
            }
            // Syllable exists(only changing and deleting are valid operations)
            else if (syllable_sequential.record_number == syllable_old.record_number) {
                if (syllable_sequential.change_type == UPDATE) {

                    syllable_temp.record_number = syllable_sequential.record_number;
                    strcpy(syllable_temp.projection_name, syllable_sequential.projection_name);
                    strcpy(syllable_temp.hall_label, syllable_sequential.hall_label);
                    syllable_temp.projection_duration = syllable_sequential.projection_duration;

                    block_temp.syllables[write_count++] = syllable_temp; //upisemo ga u novi slog
                    if (write_count == F_BLOCK) {
                        fwrite(&block_temp, sizeof(BLOCK), 1, final_sequential);
                        write_count = 0;
                    }
                }
                else if(syllable_sequential.change_type == CREATE) {
                    // Write to error file
                    err_block.syllables[err_count++] = syllable_sequential;

                    if (err_count == F_BLOCK) {
                        fwrite(&err_block, sizeof(BLOCK_CHANGE), 1, error_file);
                        err_count = 0;
                    }

                    syllable_temp.record_number = syllable_old.record_number;
                    strcpy(syllable_temp.projection_name, syllable_old.projection_name);
                    strcpy(syllable_temp.hall_label, syllable_old.hall_label);
                    syllable_temp.projection_duration = syllable_old.projection_duration;

                    block_temp.syllables[write_count++] = syllable_temp;
                    if(write_count == F_BLOCK) {
                        fwrite(&block_temp, sizeof(BLOCK), 1, final_sequential);
                        write_count = 0;
                    }
                }

                // If delete, just ignore

                seq_count++;
                old_count++;
                if(seq_count == F_BLOCK) {
                    fread(&block_sequential, sizeof(BLOCK_CHANGE), 1, sequential_internal);
                    seq_count = 0;
                }
                if(old_count == F_BLOCK) {
                    fread(&block_old, sizeof(BLOCK), 1, old_active);
                    old_count = 0;
                }
            }
            else if (syllable_sequential.record_number > syllable_old.record_number) {
                syllable_temp.record_number = syllable_old.record_number;
                strcpy(syllable_temp.projection_name, syllable_old.projection_name);
                strcpy(syllable_temp.hall_label, syllable_old.hall_label);
                syllable_temp.projection_duration = syllable_old.projection_duration;

                block_temp.syllables[write_count++] = syllable_temp;

                if (write_count == F_BLOCK) {
                    fwrite(&block_temp, sizeof(BLOCK), 1, final_sequential);
                    write_count = 0;
                }

                old_count++;
                if (old_count == F_BLOCK) {
                    fread(&block_old, sizeof(BLOCK), 1, old_active);
                    old_count = 0;
                }
            }
        }
        else if(seq_read_status) {
            syllable_temp.record_number = syllable_old.record_number;
            strcpy(syllable_temp.projection_name, syllable_old.projection_name);
            strcpy(syllable_temp.hall_label, syllable_old.hall_label);
            syllable_temp.projection_duration = syllable_old.projection_duration;

            block_temp.syllables[write_count++] = syllable_temp;

            if (write_count == F_BLOCK) {
                fwrite(&block_temp, sizeof(BLOCK), 1, final_sequential);
                write_count = 0;
            }

            old_count++;
            if (old_count == F_BLOCK) {
                fread(&block_old, sizeof(BLOCK), 1, old_active);
                old_count = 0;
            }
        }
        else if(old_read_status) {
            if(syllable_sequential.change_type == CREATE) {
                syllable_temp.record_number = syllable_sequential.record_number;
                strcpy(syllable_temp.projection_name, syllable_sequential.projection_name);
                strcpy(syllable_temp.hall_label, syllable_sequential.hall_label);
                syllable_temp.projection_duration = syllable_sequential.projection_duration;

                block_temp.syllables[write_count++] = syllable_temp;

                if (write_count == F_BLOCK) {
                    fwrite(&block_temp, sizeof(BLOCK), 1, final_sequential);
                    write_count = 0;
                }

                seq_count++;
                if(seq_count == F_BLOCK) {
                    fread(&block_old, sizeof(BLOCK_CHANGE), 1, sequential_internal);
                    seq_count = 0;
                }
            }
            // Trying to edit or remove a non-existing element
            else {
                err_block.syllables[err_count++] = syllable_sequential;

                if (err_count == F_BLOCK) {
                    fwrite(&err_block, sizeof(BLOCK_CHANGE), 1, error_file);
                    err_count = 0;
                }

                seq_count++;
                if (seq_count == F_BLOCK) {
                    fread(&block_sequential, sizeof(BLOCK_CHANGE), 1, sequential_internal);
                    seq_count = 0;
                }
            }

        }
    }

    // Write an end syllable
    block_temp.syllables[write_count++].record_number = -5;
    err_block.syllables[err_count++].record_number = -5;
    fwrite(&err_block, sizeof(BLOCK_CHANGE),1, error_file);
    fwrite(&block_temp, sizeof(BLOCK), 1, final_sequential);

    // Close all opened files
    fclose(final_sequential);
    fclose(error_file);
    fclose(old_active);
    fclose(sequential_internal);

    //upis iz nove datoteke u aktivnu
    final_sequential = fopen("final_sequential.bin", "rb");
    old_active = fopen(old_active_name, "wb");
    while(fread(&blokNovi, sizeof(Blok), 1, datotekaNova)) {
        fwrite(&blokNovi, sizeof(Blok), 1, datotekaStara);
    }

    printf("Nova izlazna sekvencijalna datoteka je formirana.\n");

    return 0;
}
