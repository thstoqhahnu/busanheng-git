#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

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
int count = 0; // ������ �̵� �� ī����
int citizen_aggro = 1;  // �ù� ��׷� �⺻�� : 1
int madongseok_aggro = 4;   // ������ ��׷� �⺻�� : 1
int madongseok_pulled_zombie = 0; // �������� ���� �ٵ������ ���θ� �����ϴ� ����

/* ��Ʈ�� */
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

/* �Է� �ޱ�(���� ����, ������ ü��, Ȯ��) */
void get_user_input() {
    // ���� ���� �Է�
    while (1) {
        printf("train length(15 ~ 50) >> ");
        if (scanf_s("%d", &train_length) == 1 && train_length >= LEN_MIN && train_length <= LEN_MAX) {
            break;
        }
        else {
            while (getchar() != '\n');
        }
    }
    // ������ ü�� �Է�
    while (1) {
        printf("madongseok stamina(0 ~ 5) >> ");
        if (scanf_s("%d", &stamina) == 1 && stamina >= STM_MIN && stamina <= STM_MAX) {
            break;
        }
        else {
            while (getchar() != '\n');
        }
    }
    // Ȯ�� �Է�
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

/* �ʱ갪 ���� */
void initialize_positions() {
    citizen_position = train_length - 6; // �ù��� �ʱ� ��ġ
    zombie_position = train_length - 3; // ������ �ʱ� ��ġ
    madongseok = train_length - 2; // �������� �ʱ� ��ġ
}

/* ���� ���� ��� */
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

/* �ù� �̵� */
void move_citizen() {
    if (rand() % 100 >= p) { // p% Ȯ���� �̵�
        citizen_position--;
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

/* ���� �̵� */
void move_zombie() {

    if (count % 2 == 0) {
        return;
    }

    else if (madongseok_pulled_zombie == 1) {
        return;
    }

    else if (rand() % 100 < p) {
        if (madongseok_aggro <= citizen_aggro) {
            if (zombie_position != citizen_position + 1) {
                zombie_position--;
            }

        }
        else if (madongseok_aggro > citizen_aggro) {
            if (zombie_position != madongseok - 1) {
                zombie_position++;
            }
        }
    }
}

/* ������ �̵� ������ */
int get_madongseok_move() {
    int move_direction;
    if (zombie_position + 1 == madongseok) {
        do {
            printf("madongseok move (%d : stay) >> ", MOVE_STAY);
            scanf_s("%d", &move_direction);
        } while (move_direction != MOVE_STAY);
    }
    else {
        do {
            printf("madongseok move (%d : stay, %d : left) >> ", MOVE_STAY, MOVE_LEFT);
            scanf_s("%d", &move_direction);
        } while (move_direction != MOVE_STAY && move_direction != MOVE_LEFT);
    }
    return move_direction;
}

/* ������ �̵� */
void move_madongseok(int move_direction) {
    if (move_direction == MOVE_LEFT) {
        madongseok--;
        if (madongseok_aggro < AGGRO_MAX) {
            madongseok_aggro++;
        }
    }
    else if (move_direction == MOVE_STAY) {
        if (madongseok_aggro > AGGRO_MIN) {
            madongseok_aggro--;
        }
    }
    printf("\n");
}

/* ���� ����(�ù�, ����) ��� */
void print_status(int prev_citizen_position, int current_zombie_position, int prev_citizen_aggro) {
    /* �ù� ���� */
    if (prev_citizen_position == citizen_position) { // ó�� ����� ���� aggro ���� 1�� ���
        printf("Citizen : stay %d (aggro : %d -> %d)\n", prev_citizen_position, prev_citizen_aggro, citizen_aggro);
    }
    else {
        printf("Citizen : %d -> %d (aggro : %d -> %d)\n", prev_citizen_position, citizen_position, prev_citizen_aggro, citizen_aggro);
    }
    /* ���� ���� */
    if (count % 2 == 0) {
        printf("Zombie : stay %d (cannot move) \n", zombie_position);
    }
    else {
        if (current_zombie_position == zombie_position) {
            printf("Zombie : stay %d \n", zombie_position);
        }
        else {
            printf("Zombie : %d -> %d\n", current_zombie_position, zombie_position);
        }
    }
    printf("\n");
}

void zombie_action_rule(int prev_stm) {
    int attack_target = ATK_NONE; // ���� ��� (0 : �ƹ��� ���� �� ��, 1 : �ù� ����, 2 : ������ ����)

    // ����� �ù��� ������ ���
    if (zombie_position == citizen_position - 1 && zombie_position == madongseok + 1) {
        if (madongseok_aggro >= citizen_aggro) {
            stamina--; // �������� stamina ����
            printf("Zombie attacks madongseok! (aggro : %d vs %d, madongseok stamina : %d -> %d\n", citizen_aggro, madongseok_aggro, prev_stm, stamina);
            if (stamina <= STM_MIN) {
                printf("GAME OVER! madongseok dead...\n");
                exit(0); // ���� ����
            }
        }
        else {
            attack_target = ATK_CITIZEN;
        }
    }
    else if (citizen_position == zombie_position - 1) {
        attack_target = ATK_CITIZEN;
    }
    // ����� �������� ������ ���
    else if (madongseok == zombie_position + 1) {
        attack_target = ATK_DONGSEOK;
    }
    // ���� ��� ���� �ൿ
    if (attack_target == ATK_CITIZEN) {
        printf("Zombie attacks citizen!\n");
        printf("GAME OVER! citizen dead...\n");
        exit(0); // ���� ����
    }
    else if (attack_target == ATK_DONGSEOK) {
        stamina--; // �������� stamina ����
        if (stamina <= STM_MIN) {
            stamina = STM_MIN;
        }
        printf("Zombie attacks madongseok! madongseok stamina : %d -> %d\n", prev_stm, stamina);
        if (stamina <= STM_MIN) {
            printf("GAME OVER! madongseok dead...\n");
            exit(0); // ���� ����
        }
    }
}

/* ������ ���� ��� */
void print_madongseok_status(int move_direction, int prev_madongseok, int prev_madongseok_aggro) {
    if (move_direction == MOVE_LEFT) {
        printf("madongseok : left %d -> %d (aggro : %d -> %d, stamina : %d)\n", prev_madongseok, madongseok, prev_madongseok_aggro, madongseok_aggro, stamina);
    }
    else {
        printf("madongseok : stay %d (aggro : %d -> %d, stamina : %d)\n", madongseok, prev_madongseok_aggro, madongseok_aggro, stamina);
    }
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

void madongseok_action_rule(int action, int prev_madongseok_aggro, int prev_stm) {
    // �޽�
    if (action == ACTION_REST) {
        printf("madongseok rests...\n");
        madongseok_aggro--; // ��׷� ����
        if (madongseok_aggro < AGGRO_MIN) {
            madongseok_aggro = AGGRO_MIN; // ��׷ΰ� �ּڰ����� ������ �ּڰ����� ����
        }
        stamina++; // ü�� ����
        if (stamina > STM_MAX) {
            stamina = STM_MAX; // ü���� �ִ񰪺��� ũ�� �ִ����� ����
        }
        printf("madongseok : %d (aggro : %d -> %d, stamina : %d -> %d)\n", madongseok, prev_madongseok_aggro, madongseok_aggro, prev_stm, stamina);
    }
    // ����
    else if (action == ACTION_PROVOKE) {
        printf("madongseok provoked zombie...\n");
        madongseok_aggro = AGGRO_MAX; // ��׷� �ִ����� ����
        printf("madongseok : %d (aggro : %d -> %d, stamina : %d)\n", madongseok, prev_madongseok_aggro, madongseok_aggro, stamina);
    }
    // �ٵ��
    else if (action == ACTION_PULL) {
        if (rand() % 100 >= p) { // (100-p)% Ȯ���� ����
            madongseok_pulled_zombie = 1; // ���� �ٵ������ ǥ��
            madongseok_aggro += 2; // �������� ���� ��׷� 2 ����
            if (madongseok_aggro > AGGRO_MAX) {
                madongseok_aggro = AGGRO_MAX; // ��׷ΰ� �ִ񰪺��� ũ�� �ִ����� ����
            }
            stamina--; // ü�� 1 ����
            if (stamina < STM_MIN) {
                stamina = STM_MIN; // ü���� �ּڰ����� ������ �ּڰ����� ����
            }
            printf("madongseok pulled zombie... Next turn, it can't move\n");
        }
        else { // �ٵ�� ����
            printf("madongseok failed to pull the zombie\n");
        }
        printf("madongseok : %d (aggro : %d -> %d, stamina : %d -> %d)\n", madongseok, prev_madongseok_aggro, madongseok_aggro, prev_stm, stamina);
    }
    printf("\n");
}

/* �ƿ� Ʈ�� */
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

    srand(time(NULL)); // ���� ������ �ʱ�ȭ
    get_user_input();
    initialize_positions();

    print_train_state();

    printf("\n");

    while (1) {

        int prev_citizen_position = citizen_position;
        int current_zombie_position = zombie_position;
        int prev_madongseok = madongseok;
        int prev_citizen_aggro = citizen_aggro;
        int prev_madongseok_aggro = madongseok_aggro;
        int prev_stm = stamina;
        count++;

        /* -----------�̵� ������------------- */
        move_citizen();
        move_zombie();

        print_train_state();

        print_status(prev_citizen_position, current_zombie_position, prev_citizen_aggro);

        int move_direction = get_madongseok_move();
        move_madongseok(move_direction);

        print_train_state();
        print_madongseok_status(move_direction, prev_madongseok, prev_madongseok_aggro);

        /* -----------�ൿ ������------------- */
        int citizen_action = 0;
        int zombie_action = 0;

        // �ù��� �ൿ ��Ģ
        if (citizen_position == 1) {
            printf("YOU WIN!\n");
            break; // ���� ����
        }
        else {
            printf("Citizen does nothing.\n");
        }

        // ������ �ൿ ��Ģ �Լ� ȣ��
        zombie_action_rule(prev_stm);
        // �Լ� �Ŀ� ���� stm ����
        prev_stm = stamina;

        int madongseok_action;
        if (madongseok - 1 == zombie_position) { // ����� ������ ���
            do {
                printf("madongseok action (%d : rest, %d : provoke, %d : pull)>> ", ACTION_REST, ACTION_PROVOKE, ACTION_PULL);
                scanf_s("%d", &madongseok_action);
            } while (madongseok_action != ACTION_REST && madongseok_action != ACTION_PROVOKE && madongseok_action != ACTION_PULL);

        }
        else { // �������� ���� ���
            do {
                printf("madongseok action(%d : rest, %d : provoke)>> ", ACTION_REST, ACTION_PROVOKE);
                scanf_s("%d", &madongseok_action);
            } while (madongseok_action != ACTION_REST && madongseok_action != ACTION_PROVOKE);
        }
        // �������� �ൿ ��Ģ �Լ� ȣ��
        madongseok_action_rule(madongseok_action, prev_madongseok_aggro, prev_stm);
    }
    return 0;
}
