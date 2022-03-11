#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "colors.h"


#define bool char
#define true 1
#define false 0

#define GAUGE_SIZE 20


typedef struct s_character {
    // Meta
    char *name;
    // Gauge
    int pv;
    int mana;

    // constants
    int maxPv;
    int maxMana;
    int physicalPower;
    int magicPower;
} character;

typedef struct s_opponent {
    character c;
    char *openingLine;
} opponent;

bool incAny(int *value, int max, int amount) {
    *value += amount;
    if (*value > max) {
        *value = max;
    } else if (*value < 0) {
        *value = 0;
        return false;
    }
    return true;
}

bool characterIncPv(character *c, int amount) {
    return incAny(&c->pv, c->maxPv, amount);

}

bool characterIncMana(character *c, int amount) {
    return incAny(&c->mana, c->maxMana, amount);
}


char *printGauge(int current, int max) {
    float a = ((float) current / (float) max);
    int fillCount = (int) (a * GAUGE_SIZE);

    char *buff = malloc(sizeof(char) * (GAUGE_SIZE + 2));
    buff[0] = '|';

    int i;
    for (i = 1; i < fillCount; i++) {
        buff[i] = '#';
    }

    for (; i < GAUGE_SIZE; i++) {
        buff[i] = ' ';
    }

    buff[i] = '|';

    buff[i + 1] = '\0';


    return buff;
}

char *characterToString(character c) {
    char *buff = malloc(sizeof(char) * 2048);
    sprintf(buff, "[%s]\n\e[0;31mPV   %s\e[0m %.3d / %.3d\t\e[0;34mMANA %s\e[0m %.3d / %.3d\n", c.name, printGauge(c.pv, c.maxPv),
            c.pv, c.maxPv, printGauge(c.mana, c.maxMana), c.mana, c.maxMana);
    return buff;
}

character *newCharacter(char *name) {
    character *c = malloc(sizeof(character));

    c->name = name;
    c->maxPv = 100;
    c->pv = 100;
    c->maxMana = 100;
    c->mana = 100;
    c->magicPower = 8;
    c->physicalPower = 5;
    return c;
}

character *newOpponent(char *name, int pv, int mana, int magicPower, int physicalPower) {
    character *c = malloc(sizeof(character));

    c->name = name;
    c->maxPv = pv;
    c->pv = pv;
    c->maxMana = mana;
    c->mana = mana;
    c->magicPower = magicPower;
    c->physicalPower = physicalPower;
    return c;
}


void throw(char* message) {
    printf("%s[ERROR]%s\n", RED, message);
    exit(1);
}

bool isLongBreak(char c) {
    if (c == '.') return true;
    if (c == '!') return true;
    if (c == '?') return true;
    return false;
}

bool isShortBreak(char c) {
    if (c == ',') return true;
    if (c == ';') return true;
    return false;
}

void fancyText(char *message) {
    int i = 0;
    for (char c = message[0]; c != '\0'; c = message[i]) {
        i++;
        printf("%c", c);
        fflush(stdout);

        if (message[i] != '\0') {
            int pause = 1000 * 40;
            if (isLongBreak(c)) {
                pause = 1000 * 500;
            } else if (isShortBreak(c)) {
                pause = 1000 * 300;
            }
            usleep(pause);
        }
    }
}

void dialogIn(char* definition, char* nextLine) {
    char color[4];
    char name[20];

    bool scanSucceed = sscanf(definition, ">%20"
                                          "s %3s", name, color);
    if (! scanSucceed) {
        throw("Invalid definition");
    }

    setColor(color);
    printf("[%s] ", name);
    fancyText(nextLine);
    resetColor();
}

void hit(character *from, character* to) {
    characterIncPv(to, -from->physicalPower);
}

void magic(character *from, character* to) {
    characterIncPv(to, -from->magicPower);
    characterIncMana(from, -5);
}

int choseAttackRandom(character *attacker) {
    if (attacker->mana < 5) {
        return 0;
    }
    return 1 + rand() % 2;
}

int choseAttack(character *attacker) {
    int choice;
    printf("1. Hit\n2. Magic\n");

    scanf("%1d", &choice);

    while (choice != 1 && choice != 2) {
        printf("Invalid choice, try again :\n");
        scanf("%1d", &choice);

        if (choice == 2 && attacker->mana < 5) {
            printf("Not enough mana.\n");
            choice = 0;
        }

    }
    return choice;
}

int attack(character *from, character *to, int attackType) {
    if (attackType == 2) {
        printf("%s use magic !\n", from->name);
        magic(from, to);
    } else {
        printf("%s use physical attack !\n", from->name);
        hit(from, to);
    }

    return to->pv <= 0;
}


int fight(character *player, character *opponent) {


    do {
        printf("%s", characterToString(*player));
        printf("%s", characterToString(*opponent));

        // Player turn
        int playerAttack = choseAttack(player);
        if (attack(player, opponent, playerAttack)) {
            printf("You won !\n");
            return 1;
        }

        // Opponent turn
        int opponentAttack = choseAttackRandom(opponent);
        if (attack(opponent, player, opponentAttack)) {
            printf("You lost...\n");
            return 0;
        }
    } while (player->pv > 0 && opponent->pv > 0);

}

int initiateFight(character *player, char* line) {
    char name[20];
    int pv;
    int mana;
    int physicalPower;
    int magicPower;
    bool scanSucceed = sscanf(line, "#%20s %d %d %d %d", name, &pv, &mana, &physicalPower, &magicPower);
    if (! scanSucceed) {
        throw("Invalid definition");
    }

    character *opponent = newOpponent(name, pv, mana, magicPower, physicalPower);

    return fight(player, opponent);
}

int lost() {
    printf("Sadly you lost, thanks for playing !");
    exit(1);
}

int win() {
    printf("You won the adventure !");
    exit(0);
}



int main() {
    srand(time(NULL));
    character *c = newCharacter("Toto");

    FILE *file = fopen("script.txt", "r");

    char *line = NULL;
    size_t len = 0;
    int lineCount = 0;

    for (ssize_t read; read != EOF; read = getline(&line, &len, file)) {
        if (len == 0 || line[0] == '\n' || line[0] == ' ') continue;
        if (line[0] == '>') {
            char *nextLine;
            int nextLineLength;
            read = getline(&nextLine, &nextLineLength, file);
            if (read == EOF) {
                throw("Invalid > : Missing line after declaration");
            }
            dialogIn(line, nextLine);
        } else if (line[0] == '#') {
            if (!initiateFight(c, line)) {
                lost();
            }
        }

    }

    win();


    return 0;
}
