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
int citizen_aggro = 1;  // �ù� ��׷� �⺻�� : 1
int madongseok_aggro = 1;   // ������ ��׷� �⺻�� : 1
int madongseok_pulled_zombie = 0; // �������� ���� �ٵ������ ���θ� �����ϴ� ����

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
    citizen_aggro = 1; // �ù� ��׷� �ʱⰪ ����
    madongseok_aggro = 1; // ������ ��׷� �ʱⰪ ����
}

void print_train_state() {
    // ���� õ��
    for (int i = 0; i < train_length + 2; i++) {
        printf("#");
    }
    printf("\n");
    // ���� ����
    printf("#");
    for (int i = 0; i < train_length; i++) {
        if (i == madongseok) {
            printf("M");
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
    printf("#\n"); // ������ ��
    // ���� �ٴ�
    for (int i = 0; i < train_length + 2; i++) {
        printf("#");
    }
    printf("\n\n");
}

void move_citizen() {
    if (rand() % 100 >= p) { // p% Ȯ���� �̵�
        if (citizen_position > 1) {
            citizen_position--;
        }
        if (citizen_aggro < AGGRO_MAX) {
            citizen_aggro++; // �̵��ϸ� citizen_aggro ����
        }
    }
    else { // (100-p)% Ȯ���� �̵����� ����
        if (citizen_aggro > AGGRO_MIN) {
            citizen_aggro--; // �̵����� ������ citizen_aggro ����
        }
    }
}

void move_zombie() {
    static int madongseok_held = 0; // ���� �Ͽ��� �������� ���� �ٵ������ ���θ� �����ϴ� ����

    // ���� �Ͽ��� �������� ���� �ٵ���ٸ� �̵� �Ұ���
    if (madongseok_pulled_zombie) {
        madongseok_pulled_zombie = 0; // �̵� �Ұ��� ���� ����
        madongseok_held = 1; // ���� �Ͽ� �̵� �Ұ� ǥ��
        printf("madongseok pulled zombie... Next turn, it can't move\n");
        return;
    }

    if (madongseok_held) {
        madongseok_held = 0; // �̵� �Ұ��� ���� ����
        return;
    }

    if (zombie_move_count % 2 == 0 && zombie_position > 1) { // 2��° �ϸ��� ���� �̵�
        // ��׷ΰ� ���� ������ �̵�
        if (citizen_aggro > 1 && zombie_position - madongseok > 1) { // ��׷ΰ� ����, �������� �������� ����
            if (madongseok < zombie_position) {
                zombie_position--;
            }
        }
        else if (citizen_position < zombie_position && zombie_position - citizen_position > 1) { // ��׷ΰ� ���ų� ������ �ù������� �̵�
            if (rand() % 100 < p) { // p% Ȯ���� �������� �̵�
                zombie_position--;
            }
        }
    }
    zombie_move_count++; // ���� �̵��� ������ ī��Ʈ

    // 4�ϸ��� ī��Ʈ �ʱ�ȭ
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

    if (move_direction == MOVE_LEFT && madongseok > 1 && madongseok - zombie_position > 1) {
        madongseok--;
        if (citizen_aggro < AGGRO_MAX) {
            citizen_aggro++;
        }
    }
    else if (move_direction == MOVE_STAY) {
        if (madongseok > 1) { // ���ڸ��� ���� ���� ��׷� ����
            if (citizen_aggro > AGGRO_MIN) {
                citizen_aggro--;
            }
        }
    }
    printf("\n");
}

void print_status(int prev_citizen_position, int current_zombie_position) {

    if (prev_citizen_position == citizen_position) { // ó�� ��µ� ���� aggro ���� 1�� ���
        printf("Citizen : %d -> %d (aggro : 1)\n", prev_citizen_position, citizen_position);
    }
    else {
        printf("Citizen : %d -> %d (aggro : %d -> ", prev_citizen_position, citizen_position, citizen_aggro);
        if (citizen_aggro > 1) {
            printf("%d)\n", citizen_aggro - 1); // ��׷ΰ� ������ ��� ���� ��׷� �� ���
        }
        else {
            printf("%d)\n", citizen_aggro); // ��׷ΰ� ������ ��� ���� ��׷� �� ���
        }
    }

    if (citizen_position <= 1) {
        printf("Zombie : stay %d (cannot move) \n", zombie_position);
        printf("SUCCESS! citizen(s) escaped to the next train\n");
    }
    else {
        if (zombie_move_count % 2 == 0) {
            printf("Zombie : stay %d (cannot move) \n", zombie_position);
        }
        else {
            printf("Zombie : %d -> %d\n", current_zombie_position, zombie_position);
        }
        printf("\n");
    }
}

void zombie_action_rule() {
    int attack_target = ATK_NONE; // ���� ��� (0: �ƹ��� ���� ����, 1: �ù� ����, 2: ������ ����)

    // ����� �ù��� ������ ���
    if (citizen_position == zombie_position + 1) {
        attack_target = ATK_CITIZEN;
    }
    // ����� �������� ������ ���
    if (madongseok == zombie_position + 1) {
        if (attack_target == ATK_NONE) {
            attack_target = ATK_DONGSEOK;
        }
        else {
            // �� �� ������ ���, ��׷� ���� ���� ���� ��� ����
            if (citizen_aggro >= 3) { // ��׷ΰ� 3 �̻��̸� �������� ����
                attack_target = ATK_DONGSEOK;
            }
            else { // ��׷ΰ� 3 �̸��̸� �ù��� ����
                attack_target = ATK_CITIZEN;
            }
        }
    }
    // ���� ��� ���� �ൿ
    if (attack_target == ATK_CITIZEN) {
        printf("Zombie attacks citizen!\n");
        printf("citizen does nothing.\n");
        printf("GAME OVER! citizen dead...\n");
        exit(0); // ���� ����
    }
    else if (attack_target == ATK_DONGSEOK) {
        printf("Zombie attacks madongseok!\n");
        stamina--; // �������� stamina ����
        if (stamina < STM_MIN) {
            stamina = STM_MIN;
        }
        if (stamina == STM_MIN) {
            printf("madongseok does nothing.\n");
            printf("GAME OVER! madongseok dead...\n");
            exit(0); // ���� ����
        }
    }
}

void print_madongseok_status(int move_direction) {
    char move_direction_str[10];
    if (move_direction == MOVE_LEFT) {
        strcpy_s(move_direction_str, sizeof(move_direction_str), "left");
    }
    else {
        strcpy_s(move_direction_str, sizeof(move_direction_str), "stay");
    }
    printf("madongseok : %s %d (aggro : %d, stamina : %d)\n", move_direction_str, madongseok, madongseok_aggro, stamina);
    printf("\n");
}

void print_actions(int citizen_action, int zombie_action) {
    if (citizen_action == 0) {
        printf("Citizen does nothing.\n");
    }

    if (zombie_action == 0) {
        printf("Zombie attacked nobody.\n");
    }
    printf("\n");
}

void madongseok_action_rule(int action) {
    // �޽�
    if (action == ACTION_REST) {
        printf("madongseok rests...\n");
        madongseok_aggro--; // ��׷� ����
        if (madongseok_aggro < AGGRO_MIN) {
            madongseok_aggro = AGGRO_MIN; // ��׷ΰ� �ּҰ����� ������ �ּҰ����� ����
        }
        stamina++; // ü�� ����
        if (stamina > STM_MAX) {
            stamina = STM_MAX; // ü���� �ִ밪���� ũ�� �ִ밪���� ����
        }
        printf("madongseok : %d (aggro : %d -> %d, stamina : %d -> %d)\n", madongseok, madongseok_aggro + 1, madongseok_aggro, stamina - 1, stamina);
    }
    // ����
    else if (action == ACTION_PROVOKE) {
        printf("madongseok provoked zombie...\n");
        madongseok_aggro = AGGRO_MAX; // ��׷� �ִ밪���� ����
        printf("madongseok : %d (aggro : %d -> %d, stamina : %d)\n", madongseok, madongseok_aggro - 1, madongseok_aggro, stamina);
    }
    // �ٵ��
    else if (action == ACTION_PULL) {
        madongseok_aggro += 2; // ��׷� 2 ����
        if (madongseok_aggro > AGGRO_MAX) {
            madongseok_aggro = AGGRO_MAX; // ��׷ΰ� �ִ밪���� ũ�� �ִ밪���� ����
        }
        stamina--; // ü�� 1 ����
        if (stamina < STM_MIN) {
            stamina = STM_MIN; // ü���� �ּҰ����� ������ �ּҰ����� ����
        }
        if (rand() % 100 >= p) { // (100-p)% Ȯ���� ����
            madongseok_pulled_zombie = 1; // ���� �ٵ������ ǥ��
            printf("madongseok pulled zombie... Next turn, it can't move\n");
        }
        else { // �ٵ�� ����
            printf("madongseok failed to pull the zombie\n");
        }
        printf("madongseok : %d (aggro : %d, stamina : %d)\n", madongseok, madongseok_aggro, stamina);
    }
    printf("\n");
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

    printf("\n");

    while (1) {

        int prev_citizen_position = citizen_position;
        int current_zombie_position = zombie_position;

        // �̵� ������
        move_citizen();
        move_zombie();

        print_train_state();

        print_status(prev_citizen_position, current_zombie_position);

        int move_direction = get_madongseok_move();
        move_madongseok(move_direction);

        print_train_state();
        print_madongseok_status(move_direction);

        if (citizen_position <= 1) {
            break; // �ù��� Ż���ϸ� ���� ���� ����
        }

        // �ൿ ������
        int citizen_action = 0;
        int zombie_action = 0;

        // �ù��� �ൿ ��Ģ
        if (citizen_position == 1) {
            printf("YOU WIN!\n");
            break; // ���� ����
        }

        print_actions(citizen_action, zombie_action);

        // ������ �ൿ ��Ģ �Լ� ȣ��
        zombie_action_rule();

        int madongseok_action;
        while (1) {
            printf("madongseok action (0.rest, 1.provoke, 2.pull) >> ");
            if (scanf_s("%d", &madongseok_action) == 1 && (madongseok_action == ACTION_REST || madongseok_action == ACTION_PROVOKE || madongseok_action == ACTION_PULL)) {
                break;
            }
            else {
                while (getchar() != '\n');
            }
            printf("\n");
        }
        printf("\n");

        // �������� �ൿ ��Ģ �Լ� ȣ��
        madongseok_action_rule(madongseok_action);
    }

    print_outro();
    Sleep(3000); // 3�� ���

    return 0;
}
