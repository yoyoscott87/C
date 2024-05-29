#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define N 52
#define SWAP(a, b){int t; t=a; a=b; b=t;}

#define BUF_SIZE 3
const char flower[4] = { 'S', 'H', 'D', 'C' }; // 4個花色
const char number[13][BUF_SIZE] = {
    " A", " 2", " 3", " 4", " 5", " 6",
    " 7", " 8", " 9", "10", " J", " Q", " K" };

// 洗牌函數
void shuffle(int* array, int Size)
{
    int i, pos;
    srand((unsigned)time(NULL));

    for (i = Size - 1; i > 0; --i) {
        pos = rand() % (i + 1);
        SWAP(array[i], array[pos]);
    }
}

// 顯示牌組函數
void display_poker(int* poker, int start, int end)
{
    // 先對牌組進行排序
    for (int i = start; i < end - 1; ++i) {
        for (int j = i + 1; j < end; ++j) {
            // 比較兩張牌的大小，先比較點數，再比較花色
            if (poker[j] < poker[i] ||
                (poker[j] == poker[i] && (poker[j] % 4) < (poker[i] % 4))) {
                int temp = poker[i];
                poker[i] = poker[j];
                poker[j] = temp;
            }
        }
    }

    // 顯示排序後的牌組
    for (int i = start; i < end; ++i) {
        // 顯示點數, number_index = poker[i] / 4
        printf("%s", number[poker[i] >> 2]); // faster

        // 顯示花色, flower_index = poker[i] % 4
        putchar(flower[poker[i] % 4]);

        if ((i - start) % 26 == 25) putchar('\n');
        else putchar(' '); 
    }
    putchar('\n'); // 每位玩家的牌顯示後加一個換行符
}

// 解析玩家輸入的函數
void parse_input(const char* input, int* poker, int* played, int num_cards, int* played_count) {
    int len = strlen(input);
    int count = 0;
    for (int i = 0; i < len; i += 2) {
        int rank_index = -1;
        for (int j = 0; j < 13; j++) {
            if (input[i] == number[j][1]) {
                rank_index = j;
                break;
            }
        }
        if (rank_index == -1) continue;

        int suit_index = -1;
        for (int j = 0; j < 4; j++) {
            if (input[i + 1] == flower[j]) {
                suit_index = j;
                break;
            }
        }
        if (suit_index == -1) continue;

        int card_index = rank_index * 4 + suit_index;
        for (int j = 0; j < num_cards; j++) {
            if (poker[j] == card_index && !played[j]) {
                played[j] = 1;
                (*played_count)++;
                break;
            }
        }
    }
}

// 檢查起始玩家的函數
int check_starting_player(int* poker) {
    int starting_player = 0; // 起始玩家
    for (int i = 0; i < N / 2; i++) {
        if (poker[i] == 2) {
            starting_player = 1; // 如果玩家1手上有3C，設置玩家1為起始玩家
            break;
        }
    }
    for (int i = N / 2; i < N; i++) {
        if (poker[i] == 2) {
            starting_player = 2; // 如果玩家2手上有3C，設置玩家2為起始玩家
            break;
        }
    }
    return starting_player;
}

int main() {
    int i, poker[N];
    for (i = 0; i != N; ++i) poker[i] = i;
    shuffle(poker, N);

    int played[N] = { 0 }; // 初始化為0，表示所有牌都没有被打出
    int played_count = 0; // 初始化為0，表示已经打出的牌的数量为0

    // 檢查起始玩家
    int current_player = check_starting_player(poker);

    printf("Player 1's cards:\n");
    display_poker(poker, 0, N / 2); // 顯示玩家1的牌

    printf("Player 2's cards:\n");
    display_poker(poker, N / 2, N); // 顯示玩家2的牌

    while (played_count < N) {
        printf("Player %d's turn:\n", current_player);
        printf("Please enter the cards you want to play (e.g., 'AS 2H 3D'): ");
        char input[100]; 
        fgets(input, sizeof(input), stdin);
        parse_input(input, poker, played, N, &played_count);

        // Print remaining cards for player
        printf("Remaining cards for Player %d:\n", current_player);
        for (int i = 0; i < N; i++) {
            if (!played[i] && ((current_player == 1 && i < N / 2) || (current_player == 2 && i >= N / 2))) {
                printf("%s%c ", number[poker[i] >> 2], flower[poker[i] % 4]);
            }
        }
        putchar('\n');

        int is_empty = 1;
        for (int i = (current_player - 1) * N / 2; i < current_player * N / 2; i++) {
            if (!played[i]) {
                is_empty = 0;
                break;
            }
        }
        if (is_empty) {
            printf("Player %d wins!\n", current_player);
            break; // Exit game loop
        }

        current_player = current_player == 1 ? 2 : 1; // 切换到下一个玩家
    }

    return 0;
}
