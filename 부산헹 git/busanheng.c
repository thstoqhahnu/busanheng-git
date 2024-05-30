#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

// Test
#define LEN_MIN 15 // 기차 길이
#define LEN_MAX 50
#define STM_MIN 0 // 마동석 체력
#define STM_MAX 5
#define PROB_MIN 10 // 확률
#define PROB_MAX 90
#define AGGRO_MIN 0 // 어그로 범위
#define AGGRO_MAX 5

// 마동석 이동 방향
#define MOVE_LEFT 1
#define MOVE_STAY 0

// 좀비의 공격 대상
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2

// 마동석 행동
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

// 전역 변수 선언
int train_length; // 기차 길이
int p; // 확률
int stamina; // 체력
int citizen_position, zombie_position, madongseok; // 시민, 좀비, 마동석 위치
int zombie_move_count = 0; // 좀비의 이동 턴 카운터
int aggro = 1; // 기본값 : 1

void print_intro() {
    printf(" ============= 부산헹 시작 =============\n");
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
    citizen_position = train_length - 6; // 시민의 초기 위치
    zombie_position = train_length - 3; // 좀비의 초기 위치
    madongseok = train_length - 2; // 마동석의 초기 위치
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
    if (rand() % 100 < p && citizen_position > 1) { // p% 확률로 왼쪽으로 이동
        citizen_position--;
    }
}

void move_zombie() {
    if (zombie_move_count % 2 == 0 && zombie_position > 1) { // 2번째 턴마다 좀비 이동
        if (rand() % 100 < p) { // p% 확률로 왼쪽으로 이동
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
    printf("\n프로그램을 성공적으로 종료합니다.\n");
}

int main(void) {

    print_intro();
    Sleep(3000); // 3초 대기
    system("cls"); // 화면 지우기
    srand(time(NULL)); // 난수 생성기 초기화
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
            break; // 시민이 탈출하면 무한 루프 종료
        }

        int move_direction = get_madongseok_move();
        move_madongseok(move_direction);

        print_train_state();
        print_madongseok_status(move_direction);

        // Sleep(4000); // 4초 대기
    }

    print_outro();
    Sleep(3000); // 3초 대기

    return 0;
}
