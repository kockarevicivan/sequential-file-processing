#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#define MAX_FILE_NAME 30
#define NULL_FILE_ERR -1

int create_file();
int choose_active_file(FILE** current_active, char** active_name);

#endif // FUNCTIONS_H_INCLUDED
