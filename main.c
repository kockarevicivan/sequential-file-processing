#include <stdio.h>
#include <stdlib.h>

#include "functions.h"
#include "common.h"

FILE* active = NULL;
char active_name[MAX_FILE_NAME];

int main()
{
    int i, position;

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

