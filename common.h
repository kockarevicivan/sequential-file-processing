#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#define F_BLOCK 5
#define MAX_FILE_NAME 30
#define NULL_FILE_ERR -1
#define INV_OPT_ERR -2
#define MAX_PROJ_NAME 40
#define MAX_HALL_LABEL 5

typedef enum {CREATE = 1, UPDATE, DELETE} ChangeType;

struct syllable {
    int record_number;
    char projection_name[MAX_PROJ_NAME];
    char hall_label[MAX_HALL_LABEL];
    int projection_duration;
} typedef SYLLABLE;

struct syllable_change {
    int record_number;
    char projection_name[MAX_PROJ_NAME];
    char hall_label[MAX_HALL_LABEL];
    int projection_duration;
    ChangeType change_type;
} typedef SYLLABLE_CHANGE;

struct block {
    SYLLABLE syllables[F_BLOCK];
} typedef BLOCK;

struct block_change {
    SYLLABLE_CHANGE syllables[F_BLOCK];
} typedef BLOCK_CHANGE;

#endif // COMMON_H_INCLUDED
