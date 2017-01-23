#include <stdio.h>
#include <stdlib.h>

#include "functions.h"
#include "common.h"

FILE* active = NULL;
char active_name[MAX_FILE_NAME];

int main()
{
    int i, position;
    /*FILE* bin_file = fopen("test.bin", "wb");

    for(i = 0; i < 1000; i++) {
        SYLLABLE tst;
        tst.record_number = 1234;
        tst.projection_duration = 1222;
        fwrite(&tst, sizeof(SYLLABLE), 1, bin_file);
    }
    fclose(bin_file);

    FILE* bin_file_r = fopen("test.bin", "rb");

    for(i = 0; i < 1000; i++) {
        SYLLABLE tst3;
        fread(&tst3, sizeof(SYLLABLE), 1, bin_file_r);
        position = ftell(bin_file_r);
        printf("\n\nPOZICIJA: %p\n\n", position);
    }

    fclose(bin_file_r);*/

    while(1) {

        int option = menu();

        switch(option) {
            case 1:
                {
                    int is_created = create_file();
                    break;
                }
            case 2:
                {
                    int is_active_set = choose_active_file(&active, &active_name);
                    break;
                }
            case 3:
                {
                    system("cls");
                    printf("%s", active_name);
                    break;
                }
            case 4:
                {
                    form_serial_file();
                    break;
                }
            case 5:
                {
                    serial_to_sequential();

                    break;
                }
            case 6:
                {

                    break;
                }
            case 7:
                {
                    print_file(active_name);
                    break;
                }
            default:
                exit(INV_OPT_ERR);
        }
    }

    return 0;
}

int menu() {

    int option = -1;

    printf("Odaberite opciju:\n\n");

    while(1) {

        printf("1. Formiraj praznu datoteku.\n"
            "2. Izaberi aktivnu datoteku zadavanjem naziva.\n"
            "3. Prikazi naziv aktivne datoteke.\n"
            "4. Formiraj vodecu serijsku datoteku promena.\n"
            "5. Formiraj sekvencijalnu datoteku promena.\n"
            "6. Formiraj izlaznu sekvencijalnu datoteku.\n"
            "7. Prikazi sve slogove aktivne datoteke.\n"
            "8. Izlazak.\n\n");

        fflush(stdin);
        scanf("%d", &option);

        system("cls");

        if(option >=1 && option <= 8) {
            return option;
        }
        else {
            printf("Nevalidna opcija, pokusajte ponovo:\n\n");
        }
    }
}

