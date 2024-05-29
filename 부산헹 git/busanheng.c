#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#define LEN_MIN		15	// 기차 길이
#define LEN_MAX		50	
#define PROB_MIN	10	// 확률
#define PROB_MAX	90

//Test

int main(void) {

	// 인트로

	printf(" ============= 부산헹 시작 =============\n");
	printf(" _________________________________________\n");
	printf("|  _     ___      ________       _     _  |\n");
	printf("| | |   |   |    |________|     | |   | | |\n");
	printf("| |_|   |___|    |________|     |_|___|_| |\n");
	printf("|_________________________________________|\n");
	printf("| | | | | | | | | | | | | | | | | | | | | | \n");
	printf("|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|\n");
	printf("   O      0      0       0       0      O \n");

	Sleep(3000);	// 3초 대기

	system("cls"); // 화면 지우기

	srand(time(NULL)); // 난수 생성기 초기화

	int train_length;	// 기차 길이
	int p;	// 확률
	int C, Z, M;	// 시민, 좀비, 마동석

	printf("train length(15 ~ 50) >> ");

	if (scanf_s("%d", &train_length) != 1 || train_length < 15 || train_length > 50) {
		printf("잘못된 입력입니다. 프로그램을 종료합니다.\n");
		exit(1);
	}

	printf("percentitle probability 'p'(10 ~ 90) >> ");

	if (scanf_s("%d", &p) != 1 || p < 10 || p > 90) {
		printf("잘못된 입력입니다. 프로그램을 종료합니다.\n");
		exit(1);
	}

	printf("\n");

	int citizen_position = train_length - 6;	// 시민의 초기 위치	
	int zombie_position = train_length - 3;	// 좀비의 초기 위치 	
	int madongseok = train_length - 2;	// 마동석의 초기 위치

	// 열차 초기 상태 출력
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

	// 빈 공간 출력
	for (int i = train_length; i < 14; i++) {
		printf(" ");
	}

	printf("\n");
	printf("###############\n");

	printf("\n");

	int prev_zombie_position = zombie_position; // 좀비의 이전 위치 초기화
	int current_zombie_position; // 좀비의 현재 위치
	int zombie_move_count = 0; // 좀비의 이동 턴 카운터

	// 4초마다 시민, 좀비 이동 / 열차 출력 / 시민, 좀비 상태 출력
	while (1) {

		// 시민 이동
		int prev_citizen_position = citizen_position; // 이전 위치 저장
		int citizen_moved = 0; // 시민이 이동했는지 여부를 저장하는 변수

		if (rand() % 100 < p) { // (100 - p)% 확률로 왼쪽으로 이동하거나, 제자리에 대기
			if (citizen_position > 1) {
				citizen_position--;    // 왼쪽으로 이동
				citizen_moved = 1; // 시민이 이동함을 표시
			}
		}
		else {
			// 아무것도 하지 않음
		}

		// 좀비 이동
		if (zombie_move_count % 2 == 0) { // 2번째 턴일 때

			if (zombie_move_count == 0 || zombie_move_count == 2) { // 좀비가 움직이는 턴일 때

				// 좀비의 현재 위치 저장
				current_zombie_position = zombie_position;

				if (rand() % 100 < p) { // p% 확률로 왼쪽으로 이동하거나, (100 - p)% 확률로 제자리에 대기

					if (zombie_position > 1)
						zombie_position--; // 왼쪽으로 이동
				}
				// 아무것도 하지 않음
			}
			else { // 좀비가 움직이지 않는 턴일 때도 좀비의 현재 위치 업데이트
				current_zombie_position = prev_zombie_position;
			}
		}

		// 좀비 이동 턴 카운터 증가
		zombie_move_count++;
		if (zombie_move_count == 4) {	// 4번째 턴이면 다시 1번째 턴으로 초기화
			zombie_move_count = 0;
		}

		// 열차 상태 출력
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

		// 빈 공간 출력
		for (int i = train_length; i < 14; i++) {
			printf(" ");
		}

		printf("\n");
		printf("###############\n");

		printf("\n");

		// 시민, 좀비 상태 출력
		if (citizen_position <= 1) {
			printf("Citizen : %d -> %d\n", prev_citizen_position, citizen_position);

			if (zombie_move_count % 2 == 0) {
				printf("Zombie : stay %d (cannot move) \n", zombie_position);
			}

			else {
				printf("Zombie : %d -> %d\n", current_zombie_position, zombie_position);
			}

			printf("SUCCESS! citizen(s) escaped to the next train\n");

			break; // 무한 루프 종료
		}

		if (citizen_moved) {
			printf("Citizen : %d -> %d\n", prev_citizen_position, citizen_position);
		}
		else {
			printf("Citizen : stay %d\n", prev_citizen_position);
		}

		if (zombie_move_count % 2 == 0) {
			printf("Zombie : stay %d (cannot move) \n", zombie_position);
		}
		else {
			printf("Zombie : %d -> %d\n", current_zombie_position, zombie_position);
		}

		printf("\n");

		Sleep(4000);
	}

	// 아웃트로

	printf(" _____  _                              _ \n");
	printf("|_   _|| |                            | |\n");
	printf("  | |  | |__    ___    ___  _ __    __| |\n");
	printf("  | |  | '_ \\  / _ \\  / _ \\| '_ \\  / _` |\n");
	printf("  | |  | | | ||  __/ |  __/| | | || (_| |\n");
	printf("  \\_/  |_| |_| \\___|  \\___||_| |_| \\__,_|\n");
	printf("                                         \n");

	Sleep(3000); // 3초 대기

	printf("\n프로그램을 성공적으로 종료합니다.\n");

	return 0;
}