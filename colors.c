//
// Created by anthony on 06/03/2022.
//

#include "colors.h"
#include "string.h"
#include <stdio.h>

char* getColor(char* code) {
    if (strcmp(code, "BLK") == 0) return BLK;
    if (!strcmp(code, "RED")) return RED;
    if (!strcmp(code, "GRN")) return GRN;
    if (!strcmp(code, "YEL")) return YEL;
    if (!strcmp(code, "BLU")) return BLU;
    if (!strcmp(code, "MAG")) return MAG;
    if (!strcmp(code, "CYN")) return CYN;
    return WHT;
}

void setColor(char* code) {
    printf("%s", getColor(code));
}

void resetColor() {
    printf("%s", RESET);
}
