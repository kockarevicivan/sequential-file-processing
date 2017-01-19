#include <stdio.h>
#include <stdlib.h>

#include "functions.h"

#define INV_OPT_ERR -1

FILE* active = NULL;
char active_name[MAX_FILE_NAME];

int main()
{
    while(1) {
        //system("cls");

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
                //prikaz naziva
                {
                    system("cls");
                    printf("%s", active_name);
                    break;
                }
            case 4:
                //formiranje serijske
                {
                    break;
                }
            case 5:
                //formiranje sekvencijalne
                {
                    break;
                }
            case 6:
                //formiranje izlazne sekvencijalne
                {
                    break;
                }
            case 7:
                //prikaz svih slogova
                {
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

