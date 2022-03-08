//
// Created by anthony on 06/03/2022.
//

#ifndef PROJET_COLORS_H
#define PROJET_COLORS_H

#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"


//Reset
#define RESET "\e[0m"
#define CRESET "\e[0m"
#define COLOR_RESET "\e[0m"

void setColor(char* code);
void resetColor();

#endif //PROJET_COLORS_H
