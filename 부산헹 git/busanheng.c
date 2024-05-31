#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

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
int count = 0; // 좀비의 이동 턴 카운터
int citizen_aggro = 1;  // 시민 어그로 기본값 : 1
int madongseok_aggro = 4;   // 마동석 어그로 기본값 : 1
int madongseok_pulled_zombie = 0; // 마동석이 좀비를 붙들었는지 여부를 저장하는 변수

/* 인트로 */
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

/* 입력 받기(기차 길이, 마동석 체력, 확률) */
void get_user_input() {
    // 기차 길이 입력
    while (1) {
        printf("train length(15 ~ 50) >> ");
        if (scanf_s("%d", &train_length) == 1 && train_length >= LEN_MIN && train_length <= LEN_MAX) {
            break;
        }
        else {
            while (getchar() != '\n');
        }
    }
    // 마동석 체력 입력
    while (1) {
        printf("madongseok stamina(0 ~ 5) >> ");
        if (scanf_s("%d", &stamina) == 1 && stamina >= STM_MIN && stamina <= STM_MAX) {
            break;
        }
        else {
            while (getchar() != '\n');
        }
    }
    // 확률 입력
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

/* 초깃값 설정 */
void initialize_positions() {
    citizen_position = train_length - 6; // 시민의 초기 위치
    zombie_position = train_length - 3; // 좀비의 초기 위치
    madongseok = train_length - 2; // 마동석의 초기 위치
}

/* 기차 내부 출력 */
void print_train_state() {
    // 기차 천장
    for (int i = 0; i < train_length + 2; i++) {
        printf("#");
    }
    printf("\n");
    // 기차 내부
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
    printf("#\n"); // 오른쪽 벽
    // 기차 바닥
    for (int i = 0; i < train_length + 2; i++) {
        printf("#");
    }
    printf("\n\n");
}

/* 시민 이동 */
void move_citizen() {
    if (rand() % 100 >= p) { // p% 확률로 이동
        citizen_position--;
        if (citizen_aggro < AGGRO_MAX) {
            citizen_aggro++; // 이동하면 citizen_aggro 증가
        }
    }
    else { // (100-p)% 확률로 이동하지 않음
        if (citizen_aggro > AGGRO_MIN) {
            citizen_aggro--; // 이동하지 않으면 citizen_aggro 감소
        }
    }
}

/* 좀비 이동 */
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

/* 마동석 이동 선택지 */
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

/* 마동석 이동 */
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

/* 현재 상태(시민, 좀비) 출력 */
void print_status(int prev_citizen_position, int current_zombie_position, int prev_citizen_aggro) {
    /* 시민 상태 */
    if (prev_citizen_position == citizen_position) { // 처음 출력일 때만 aggro 값을 1로 출력
        printf("Citizen : stay %d (aggro : %d -> %d)\n", prev_citizen_position, prev_citizen_aggro, citizen_aggro);
    }
    else {
        printf("Citizen : %d -> %d (aggro : %d -> %d)\n", prev_citizen_position, citizen_position, prev_citizen_aggro, citizen_aggro);
    }
    /* 좀비 상태 */
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
    int attack_target = ATK_NONE; // 공격 대상 (0 : 아무도 공격 안 함, 1 : 시민 공격, 2 : 마동석 공격)

    // 좀비와 시민이 인접한 경우
    if (zombie_position == citizen_position - 1 && zombie_position == madongseok + 1) {
        if (madongseok_aggro >= citizen_aggro) {
            stamina--; // 마동석의 stamina 감소
            printf("Zombie attacks madongseok! (aggro : %d vs %d, madongseok stamina : %d -> %d\n", citizen_aggro, madongseok_aggro, prev_stm, stamina);
            if (stamina <= STM_MIN) {
                printf("GAME OVER! madongseok dead...\n");
                exit(0); // 게임 종료
            }
        }
        else {
            attack_target = ATK_CITIZEN;
        }
    }
    else if (citizen_position == zombie_position - 1) {
        attack_target = ATK_CITIZEN;
    }
    // 좀비와 마동석이 인접한 경우
    else if (madongseok == zombie_position + 1) {
        attack_target = ATK_DONGSEOK;
    }
    // 공격 대상에 따른 행동
    if (attack_target == ATK_CITIZEN) {
        printf("Zombie attacks citizen!\n");
        printf("GAME OVER! citizen dead...\n");
        exit(0); // 게임 종료
    }
    else if (attack_target == ATK_DONGSEOK) {
        stamina--; // 마동석의 stamina 감소
        if (stamina <= STM_MIN) {
            stamina = STM_MIN;
        }
        printf("Zombie attacks madongseok! madongseok stamina : %d -> %d\n", prev_stm, stamina);
        if (stamina <= STM_MIN) {
            printf("GAME OVER! madongseok dead...\n");
            exit(0); // 게임 종료
        }
    }
}

/* 마동석 상태 출력 */
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
    // 휴식
    if (action == ACTION_REST) {
        printf("madongseok rests...\n");
        madongseok_aggro--; // 어그로 감소
        if (madongseok_aggro < AGGRO_MIN) {
            madongseok_aggro = AGGRO_MIN; // 어그로가 최솟값보다 작으면 최솟값으로 설정
        }
        stamina++; // 체력 증가
        if (stamina > STM_MAX) {
            stamina = STM_MAX; // 체력이 최댓값보다 크면 최댓값으로 설정
        }
        printf("madongseok : %d (aggro : %d -> %d, stamina : %d -> %d)\n", madongseok, prev_madongseok_aggro, madongseok_aggro, prev_stm, stamina);
    }
    // 도발
    else if (action == ACTION_PROVOKE) {
        printf("madongseok provoked zombie...\n");
        madongseok_aggro = AGGRO_MAX; // 어그로 최댓값으로 설정
        printf("madongseok : %d (aggro : %d -> %d, stamina : %d)\n", madongseok, prev_madongseok_aggro, madongseok_aggro, stamina);
    }
    // 붙들기
    else if (action == ACTION_PULL) {
        if (rand() % 100 >= p) { // (100-p)% 확률로 성공
            madongseok_pulled_zombie = 1; // 좀비를 붙들었음을 표시
            madongseok_aggro += 2; // 성공했을 때만 어그로 2 증가
            if (madongseok_aggro > AGGRO_MAX) {
                madongseok_aggro = AGGRO_MAX; // 어그로가 최댓값보다 크면 최댓값으로 설정
            }
            stamina--; // 체력 1 감소
            if (stamina < STM_MIN) {
                stamina = STM_MIN; // 체력이 최솟값보다 작으면 최솟값으로 설정
            }
            printf("madongseok pulled zombie... Next turn, it can't move\n");
        }
        else { // 붙들기 실패
            printf("madongseok failed to pull the zombie\n");
        }
        printf("madongseok : %d (aggro : %d -> %d, stamina : %d -> %d)\n", madongseok, prev_madongseok_aggro, madongseok_aggro, prev_stm, stamina);
    }
    printf("\n");
}

/* 아웃 트로 */
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

    srand(time(NULL)); // 난수 생성기 초기화
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

        /* -----------이동 페이즈------------- */
        move_citizen();
        move_zombie();

        print_train_state();

        print_status(prev_citizen_position, current_zombie_position, prev_citizen_aggro);

        int move_direction = get_madongseok_move();
        move_madongseok(move_direction);

        print_train_state();
        print_madongseok_status(move_direction, prev_madongseok, prev_madongseok_aggro);

        /* -----------행동 페이즈------------- */
        int citizen_action = 0;
        int zombie_action = 0;

        // 시민의 행동 규칙
        if (citizen_position == 1) {
            printf("YOU WIN!\n");
            break; // 게임 종료
        }
        else {
            printf("Citizen does nothing.\n");
        }

        // 좀비의 행동 규칙 함수 호출
        zombie_action_rule(prev_stm);
        // 함수 후에 이전 stm 수정
        prev_stm = stamina;

        int madongseok_action;
        if (madongseok - 1 == zombie_position) { // 좀비와 인접한 경우
            do {
                printf("madongseok action (%d : rest, %d : provoke, %d : pull)>> ", ACTION_REST, ACTION_PROVOKE, ACTION_PULL);
                scanf_s("%d", &madongseok_action);
            } while (madongseok_action != ACTION_REST && madongseok_action != ACTION_PROVOKE && madongseok_action != ACTION_PULL);

        }
        else { // 인접하지 않은 경우
            do {
                printf("madongseok action(%d : rest, %d : provoke)>> ", ACTION_REST, ACTION_PROVOKE);
                scanf_s("%d", &madongseok_action);
            } while (madongseok_action != ACTION_REST && madongseok_action != ACTION_PROVOKE);
        }
        // 마동석의 행동 규칙 함수 호출
        madongseok_action_rule(madongseok_action, prev_madongseok_aggro, prev_stm);
    }
    return 0;
}
