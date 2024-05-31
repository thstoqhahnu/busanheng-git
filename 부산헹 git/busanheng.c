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
int citizen_aggro = 1;  // 시민 어그로 기본값 : 1
int madongseok_aggro = 1;   // 마동석 어그로 기본값 : 1
int madongseok_pulled_zombie = 0; // 마동석이 좀비를 붙들었는지 여부를 저장하는 변수

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
    citizen_aggro = 1; // 시민 어그로 초기값 설정
    madongseok_aggro = 1; // 마동석 어그로 초기값 설정
}

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

void move_citizen() {
    if (rand() % 100 >= p) { // p% 확률로 이동
        if (citizen_position > 1) {
            citizen_position--;
        }
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

void move_zombie() {
    static int madongseok_held = 0; // 이전 턴에서 마동석이 좀비를 붙들었는지 여부를 저장하는 변수

    // 이전 턴에서 마동석이 좀비를 붙들었다면 이동 불가능
    if (madongseok_pulled_zombie) {
        madongseok_pulled_zombie = 0; // 이동 불가능 상태 해제
        madongseok_held = 1; // 다음 턴에 이동 불가 표시
        printf("madongseok pulled zombie... Next turn, it can't move\n");
        return;
    }

    if (madongseok_held) {
        madongseok_held = 0; // 이동 불가능 상태 해제
        return;
    }

    if (zombie_move_count % 2 == 0 && zombie_position > 1) { // 2번째 턴마다 좀비 이동
        // 어그로가 높은 쪽으로 이동
        if (citizen_aggro > 1 && zombie_position - madongseok > 1) { // 어그로가 높고, 마동석과 인접하지 않음
            if (madongseok < zombie_position) {
                zombie_position--;
            }
        }
        else if (citizen_position < zombie_position && zombie_position - citizen_position > 1) { // 어그로가 낮거나 같으면 시민쪽으로 이동
            if (rand() % 100 < p) { // p% 확률로 왼쪽으로 이동
                zombie_position--;
            }
        }
    }
    zombie_move_count++; // 좀비가 이동한 턴임을 카운트

    // 4턴마다 카운트 초기화
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
        if (madongseok > 1) { // 제자리에 있을 때만 어그로 감소
            if (citizen_aggro > AGGRO_MIN) {
                citizen_aggro--;
            }
        }
    }
    printf("\n");
}

void print_status(int prev_citizen_position, int current_zombie_position) {

    if (prev_citizen_position == citizen_position) { // 처음 출력될 때만 aggro 값을 1로 출력
        printf("Citizen : %d -> %d (aggro : 1)\n", prev_citizen_position, citizen_position);
    }
    else {
        printf("Citizen : %d -> %d (aggro : %d -> ", prev_citizen_position, citizen_position, citizen_aggro);
        if (citizen_aggro > 1) {
            printf("%d)\n", citizen_aggro - 1); // 어그로가 증가한 경우 이전 어그로 값 출력
        }
        else {
            printf("%d)\n", citizen_aggro); // 어그로가 감소한 경우 현재 어그로 값 출력
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
    int attack_target = ATK_NONE; // 공격 대상 (0: 아무도 공격 안함, 1: 시민 공격, 2: 마동석 공격)

    // 좀비와 시민이 인접한 경우
    if (citizen_position == zombie_position + 1) {
        attack_target = ATK_CITIZEN;
    }
    // 좀비와 마동석이 인접한 경우
    if (madongseok == zombie_position + 1) {
        if (attack_target == ATK_NONE) {
            attack_target = ATK_DONGSEOK;
        }
        else {
            // 둘 다 인접한 경우, 어그로 값에 따라 공격 대상 선택
            if (citizen_aggro >= 3) { // 어그로가 3 이상이면 마동석을 공격
                attack_target = ATK_DONGSEOK;
            }
            else { // 어그로가 3 미만이면 시민을 공격
                attack_target = ATK_CITIZEN;
            }
        }
    }
    // 공격 대상에 따른 행동
    if (attack_target == ATK_CITIZEN) {
        printf("Zombie attacks citizen!\n");
        printf("citizen does nothing.\n");
        printf("GAME OVER! citizen dead...\n");
        exit(0); // 게임 종료
    }
    else if (attack_target == ATK_DONGSEOK) {
        printf("Zombie attacks madongseok!\n");
        stamina--; // 마동석의 stamina 감소
        if (stamina < STM_MIN) {
            stamina = STM_MIN;
        }
        if (stamina == STM_MIN) {
            printf("madongseok does nothing.\n");
            printf("GAME OVER! madongseok dead...\n");
            exit(0); // 게임 종료
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
    // 휴식
    if (action == ACTION_REST) {
        printf("madongseok rests...\n");
        madongseok_aggro--; // 어그로 감소
        if (madongseok_aggro < AGGRO_MIN) {
            madongseok_aggro = AGGRO_MIN; // 어그로가 최소값보다 작으면 최소값으로 설정
        }
        stamina++; // 체력 증가
        if (stamina > STM_MAX) {
            stamina = STM_MAX; // 체력이 최대값보다 크면 최대값으로 설정
        }
        printf("madongseok : %d (aggro : %d -> %d, stamina : %d -> %d)\n", madongseok, madongseok_aggro + 1, madongseok_aggro, stamina - 1, stamina);
    }
    // 도발
    else if (action == ACTION_PROVOKE) {
        printf("madongseok provoked zombie...\n");
        madongseok_aggro = AGGRO_MAX; // 어그로 최대값으로 설정
        printf("madongseok : %d (aggro : %d -> %d, stamina : %d)\n", madongseok, madongseok_aggro - 1, madongseok_aggro, stamina);
    }
    // 붙들기
    else if (action == ACTION_PULL) {
        madongseok_aggro += 2; // 어그로 2 증가
        if (madongseok_aggro > AGGRO_MAX) {
            madongseok_aggro = AGGRO_MAX; // 어그로가 최대값보다 크면 최대값으로 설정
        }
        stamina--; // 체력 1 감소
        if (stamina < STM_MIN) {
            stamina = STM_MIN; // 체력이 최소값보다 작으면 최소값으로 설정
        }
        if (rand() % 100 >= p) { // (100-p)% 확률로 성공
            madongseok_pulled_zombie = 1; // 좀비를 붙들었음을 표시
            printf("madongseok pulled zombie... Next turn, it can't move\n");
        }
        else { // 붙들기 실패
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

    printf("\n");

    while (1) {

        int prev_citizen_position = citizen_position;
        int current_zombie_position = zombie_position;

        // 이동 페이즈
        move_citizen();
        move_zombie();

        print_train_state();

        print_status(prev_citizen_position, current_zombie_position);

        int move_direction = get_madongseok_move();
        move_madongseok(move_direction);

        print_train_state();
        print_madongseok_status(move_direction);

        if (citizen_position <= 1) {
            break; // 시민이 탈출하면 무한 루프 종료
        }

        // 행동 페이즈
        int citizen_action = 0;
        int zombie_action = 0;

        // 시민의 행동 규칙
        if (citizen_position == 1) {
            printf("YOU WIN!\n");
            break; // 게임 종료
        }

        print_actions(citizen_action, zombie_action);

        // 좀비의 행동 규칙 함수 호출
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

        // 마동석의 행동 규칙 함수 호출
        madongseok_action_rule(madongseok_action);
    }

    print_outro();
    Sleep(3000); // 3초 대기

    return 0;
}
