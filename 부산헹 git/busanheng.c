#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

// Test
#define LEN_MIN 15 // ���� ����
#define LEN_MAX 50
#define STM_MIN 0 // ������ ü��
#define STM_MAX 5
#define PROB_MIN 10 // Ȯ��
#define PROB_MAX 90
#define AGGRO_MIN 0 // ��׷� ����
#define AGGRO_MAX 5

// ������ �̵� ����
#define MOVE_LEFT 1
#define MOVE_STAY 0

// ������ ���� ���
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2

// ������ �ൿ
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

// ���� ���� ����
int train_length; // ���� ����
int p; // Ȯ��
int stamina; // ü��
int citizen_position, zombie_position, madongseok; // �ù�, ����, ������ ��ġ
int zombie_move_count = 0; // ������ �̵� �� ī����
int aggro = 1; // �⺻�� : 1

void print_intro() {
    printf(" ============= �λ��� ���� =============\n");
    printf(" _________________________________________\n");
    printf("|  _     ___      ________       _     _  |\n");
    printf("| | |   |   |    |________|     | |   | | |\n");
    printf("| |_|   |___|    |________|     |_|___|_| |\n");
    printf("|_________________________________________|\n");
    printf("| | | | | | | | | | | | | | | | | | | | | | \n");
    printf("|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|\n");
    printf("   O      0      0       0       0      O \n");
}

void get_user_input() {
    while (1) {
        printf("train length(15 ~ 50) >> ");
        if (scanf_s("%d", &train_length) == 1 && train_length >= LEN_MIN && train_length <= LEN_MAX) {
            break;
        }
        else {
            while (getchar() != '\n'); 
        }
    }

    while (1) {
        printf("madongseok stamina(0 ~ 5) >> ");
        if (scanf_s("%d", &stamina) == 1 && stamina >= STM_MIN && stamina <= STM_MAX) {
            break;
        }
        else {
            while (getchar() != '\n'); 
        }
    }

    while (1) {
        printf("percentile probability 'p'(10 ~ 90) >> ");
        if (scanf_s("%d", &p) == 1 && p >= PROB_MIN && p <= PROB_MAX) {
            break;
        }
        else {
            while (getchar() != '\n'); 
        }
    }
    printf("\n");
}

void initialize_positions() {
    citizen_position = train_length - 6; // �ù��� �ʱ� ��ġ
    zombie_position = train_length - 3; // ������ �ʱ� ��ġ
    madongseok = train_length - 2; // �������� �ʱ� ��ġ
}

void print_train_state() {
    printf("###############\n");
    printf("#");
    for (int i = 1; i <= train_length; i++) {
        if (i == madongseok) {
            printf("M#");
        }
        else if (i == citizen_position) {
            printf("C");
        }
        else if (i == zombie_position) {
            printf("Z");
        }
        else {
            printf(" ");
        }
    }
    for (int i = train_length; i < 14; i++) {
        printf(" ");
    }
    printf("\n");
    printf("###############\n");
    printf("\n\n");
}

void move_citizen() {
    if (rand() % 100 < p && citizen_position > 1) { // p% Ȯ���� �������� �̵�
        citizen_position--;
    }
}

void move_zombie() {
    if (zombie_move_count % 2 == 0 && zombie_position > 1) { // 2��° �ϸ��� ���� �̵�
        if (rand() % 100 < p) { // p% Ȯ���� �������� �̵�
            zombie_position--;
        }
    }
    zombie_move_count++;
    if (zombie_move_count == 4) {
        zombie_move_count = 0;
    }
}

int get_madongseok_move() {
    int move_direction;
    while (1) {
        printf("madongseok move (0 : stay, 1 : left) >> ");
        if (scanf_s("%d", &move_direction) == 1 && (move_direction == MOVE_STAY || move_direction == MOVE_LEFT)) {
            break;
        }
        else {
            while (getchar() != '\n');
        }
    }
    return move_direction;
}

void move_madongseok(int move_direction) {
    if (move_direction == MOVE_LEFT && madongseok > 1) {
        madongseok--;
    }
    printf("\n");
}

void print_status(int prev_citizen_position, int current_zombie_position) {
    if (citizen_position <= 1) {
        printf("Citizen : %d -> %d (aggro : %d) \n", prev_citizen_position, citizen_position, aggro);
        printf("Zombie : stay %d (cannot move) \n", zombie_position);
        printf("SUCCESS! citizen(s) escaped to the next train\n");
    }
    else {
        printf("Citizen : %d -> %d (aggro : %d)\n", prev_citizen_position, citizen_position, aggro);
        if (zombie_move_count % 2 == 0) {
            printf("Zombie : stay %d (cannot move) \n", zombie_position);
        }
        else {
            printf("Zombie : %d -> %d\n", current_zombie_position, zombie_position);
        }
        printf("\n");
    }
}

void print_madongseok_status(int move_direction) {
    char move_direction_str[10];
    if (move_direction == MOVE_LEFT) {
        strcpy_s(move_direction_str, sizeof(move_direction_str), "left");
    } else {
        strcpy_s(move_direction_str, sizeof(move_direction_str), "stay");
    }
    printf("madongseok : %s %d (aggro : %d, stamina : %d)\n", move_direction_str, madongseok, aggro, stamina);
}

void print_outro() {
    printf(" _____  _                              _ \n");
    printf("|_   _|| |                            | |\n");
    printf("  | |  | |__    ___    ___  _ __    __| |\n");
    printf("  | |  | '_ \\  / _ \\  / _ \\| '_ \\  / _` |\n");
    printf("  | |  | | | ||  __/ |  __/| | | || (_| |\n");
    printf("  \\_/  |_| |_| \\___|  \\___||_| |_| \\__,_|\n");
    printf("                                         \n");
    printf("\n���α׷��� ���������� �����մϴ�.\n");
}

int main(void) {

    print_intro();
    Sleep(3000); // 3�� ���
    system("cls"); // ȭ�� �����
    srand(time(NULL)); // ���� ������ �ʱ�ȭ
    get_user_input();
    initialize_positions();

    print_train_state();

    while (1) {
        int prev_citizen_position = citizen_position;
        int current_zombie_position = zombie_position;

        move_citizen();
        move_zombie();

        print_train_state();
        print_status(prev_citizen_position, current_zombie_position);

        if (citizen_position <= 1) {
            break; // �ù��� Ż���ϸ� ���� ���� ����
        }

        int move_direction = get_madongseok_move();
        move_madongseok(move_direction);

        print_train_state();
        print_madongseok_status(move_direction);

        // Sleep(4000); // 4�� ���
    }

    print_outro();
    Sleep(3000); // 3�� ���

    return 0;
}
