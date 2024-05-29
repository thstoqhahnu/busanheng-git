#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#define LEN_MIN		15	// ���� ����
#define LEN_MAX		50	
#define PROB_MIN	10	// Ȯ��
#define PROB_MAX	90

//Test

int main(void) {

	// ��Ʈ��

	printf(" ============= �λ��� ���� =============\n");
	printf(" _________________________________________\n");
	printf("|  _     ___      ________       _     _  |\n");
	printf("| | |   |   |    |________|     | |   | | |\n");
	printf("| |_|   |___|    |________|     |_|___|_| |\n");
	printf("|_________________________________________|\n");
	printf("| | | | | | | | | | | | | | | | | | | | | | \n");
	printf("|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|\n");
	printf("   O      0      0       0       0      O \n");

	Sleep(3000);	// 3�� ���

	system("cls"); // ȭ�� �����

	srand(time(NULL)); // ���� ������ �ʱ�ȭ

	int train_length;	// ���� ����
	int p;	// Ȯ��
	int C, Z, M;	// �ù�, ����, ������

	printf("train length(15 ~ 50) >> ");

	if (scanf_s("%d", &train_length) != 1 || train_length < 15 || train_length > 50) {
		printf("�߸��� �Է��Դϴ�. ���α׷��� �����մϴ�.\n");
		exit(1);
	}

	printf("percentitle probability 'p'(10 ~ 90) >> ");

	if (scanf_s("%d", &p) != 1 || p < 10 || p > 90) {
		printf("�߸��� �Է��Դϴ�. ���α׷��� �����մϴ�.\n");
		exit(1);
	}

	printf("\n");

	int citizen_position = train_length - 6;	// �ù��� �ʱ� ��ġ	
	int zombie_position = train_length - 3;	// ������ �ʱ� ��ġ 	
	int madongseok = train_length - 2;	// �������� �ʱ� ��ġ

	// ���� �ʱ� ���� ���
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

	// �� ���� ���
	for (int i = train_length; i < 14; i++) {
		printf(" ");
	}

	printf("\n");
	printf("###############\n");

	printf("\n");

	int prev_zombie_position = zombie_position; // ������ ���� ��ġ �ʱ�ȭ
	int current_zombie_position; // ������ ���� ��ġ
	int zombie_move_count = 0; // ������ �̵� �� ī����

	// 4�ʸ��� �ù�, ���� �̵� / ���� ��� / �ù�, ���� ���� ���
	while (1) {

		// �ù� �̵�
		int prev_citizen_position = citizen_position; // ���� ��ġ ����
		int citizen_moved = 0; // �ù��� �̵��ߴ��� ���θ� �����ϴ� ����

		if (rand() % 100 < p) { // (100 - p)% Ȯ���� �������� �̵��ϰų�, ���ڸ��� ���
			if (citizen_position > 1) {
				citizen_position--;    // �������� �̵�
				citizen_moved = 1; // �ù��� �̵����� ǥ��
			}
		}
		else {
			// �ƹ��͵� ���� ����
		}

		// ���� �̵�
		if (zombie_move_count % 2 == 0) { // 2��° ���� ��

			if (zombie_move_count == 0 || zombie_move_count == 2) { // ���� �����̴� ���� ��

				// ������ ���� ��ġ ����
				current_zombie_position = zombie_position;

				if (rand() % 100 < p) { // p% Ȯ���� �������� �̵��ϰų�, (100 - p)% Ȯ���� ���ڸ��� ���

					if (zombie_position > 1)
						zombie_position--; // �������� �̵�
				}
				// �ƹ��͵� ���� ����
			}
			else { // ���� �������� �ʴ� ���� ���� ������ ���� ��ġ ������Ʈ
				current_zombie_position = prev_zombie_position;
			}
		}

		// ���� �̵� �� ī���� ����
		zombie_move_count++;
		if (zombie_move_count == 4) {	// 4��° ���̸� �ٽ� 1��° ������ �ʱ�ȭ
			zombie_move_count = 0;
		}

		// ���� ���� ���
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

		// �� ���� ���
		for (int i = train_length; i < 14; i++) {
			printf(" ");
		}

		printf("\n");
		printf("###############\n");

		printf("\n");

		// �ù�, ���� ���� ���
		if (citizen_position <= 1) {
			printf("Citizen : %d -> %d\n", prev_citizen_position, citizen_position);

			if (zombie_move_count % 2 == 0) {
				printf("Zombie : stay %d (cannot move) \n", zombie_position);
			}

			else {
				printf("Zombie : %d -> %d\n", current_zombie_position, zombie_position);
			}

			printf("SUCCESS! citizen(s) escaped to the next train\n");

			break; // ���� ���� ����
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

	// �ƿ�Ʈ��

	printf(" _____  _                              _ \n");
	printf("|_   _|| |                            | |\n");
	printf("  | |  | |__    ___    ___  _ __    __| |\n");
	printf("  | |  | '_ \\  / _ \\  / _ \\| '_ \\  / _` |\n");
	printf("  | |  | | | ||  __/ |  __/| | | || (_| |\n");
	printf("  \\_/  |_| |_| \\___|  \\___||_| |_| \\__,_|\n");
	printf("                                         \n");

	Sleep(3000); // 3�� ���

	printf("\n���α׷��� ���������� �����մϴ�.\n");

	return 0;
}