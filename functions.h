#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#define MAX_FILE_NAME 30
#define NULL_FILE_ERR -1

#include "common.h"

int create_file();
int choose_active_file(FILE** current_active, char** active_name);
int form_serial_file();
SYLLABLE_CHANGE fill_syllable(int record_number);
void print_file(char* file_name);
void serial_to_sequential();
SYLLABLE_CHANGE* sort_syllables(SYLLABLE_CHANGE* raw_data, int n);
void swap_syllables(SYLLABLE_CHANGE* a, SYLLABLE_CHANGE* b);

#endif // FUNCTIONS_H_INCLUDED
