#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _WIN32
void set_console_utf8() {
    SetConsoleOutputCP(CP_UTF8);
}
#endif

const char* suits[] = { "S", "H", "D", "C" };
const char* ranks[] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
const int num_suits = 4;
const int num_ranks = 13;
const int num_cards = 52;

typedef struct {
    const char* suit;
    const char* rank;
} Card;

void shuffle_deck(Card* deck) {
    srand((unsigned int)time(NULL));
    for (int i = num_cards - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

void deal_cards(Card* deck, int num_players, Card** players_hands) {
    for (int i = 0; i < num_players; i++) {
        players_hands[i] = (Card*)malloc((num_cards / num_players) * sizeof(Card));
        if (players_hands[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for player %d\n", i);
            exit(1);
        }
        for (int j = 0; j < num_cards / num_players; j++) {
            players_hands[i][j] = deck[i * (num_cards / num_players) + j];
        }
    }
}

int choose_starting_player(int num_players) {
    srand((unsigned int)time(NULL));
    return rand() % num_players;
}

void print_hand(Card* hand, int num_cards) {
    for (int i = 0; i < num_cards; i++) {
        printf("%s%s ", hand[i].rank, hand[i].suit);
    }
    printf("\n");
}

int main() {
#ifdef _WIN32
    set_console_utf8();
#endif

    Card* deck = (Card*)malloc(num_cards * sizeof(Card));
    if (deck == NULL) {
        fprintf(stderr, "Memory allocation failed for deck\n");
        return 1;
    }
    for (int i = 0; i < num_suits; i++) {
        for (int j = 0; j < num_ranks; j++) {
            deck[i * num_ranks + j].suit = suits[i];
            deck[i * num_ranks + j].rank = ranks[j];
        }
    }

    shuffle_deck(deck);

    int num_players = 2;
    Card** players_hands = (Card**)malloc(num_players * sizeof(Card*));
    if (players_hands == NULL) {
        fprintf(stderr, "Memory allocation failed for players_hands\n");
        free(deck);
        return 1;
    }

    deal_cards(deck, num_players, players_hands);

    int starting_player = choose_starting_player(num_players);

    printf("Players' Hands:\n");
    for (int i = 0; i < num_players; i++) {
        printf("Player %d: ", i + 1);
        print_hand(players_hands[i], num_cards / num_players);
    }
    printf("Starting Player: %d\n", starting_player + 1);

    int current_player = starting_player;
    int turn = 1;
    while (1) {
        printf("Turn %d: Player %d's turn\n", turn, current_player + 1);
        printf("Player %d's hand: ", current_player + 1);
        print_hand(players_hands[current_player], num_cards / num_players);

        char input[3]; // 假設玩家輸入的格式為 2 個字符，如 "AS" 或 "10H"
        printf("Please enter the card you want to play (e.g., 'AS' for Ace of Spades): ");

        // 處理玩家的輸入
        int itemsRead = scanf_s("%2s", input);

        // 添加零結尾
        input[2] = '\0';

        //


        // 檢查玩家輸入是否有效
        if (itemsRead != 1) {
            printf("Invalid input! Please enter a valid card.\n");
            while (getchar() != '\n'); // 清除輸入緩衝區
            continue; // 繼續下一個迴圈
        }

        // 檢查玩家輸入的牌是否在手牌中
        int found = 0;
        for (int i = 0; i < num_cards / num_players; i++) {
            if (strcmp(input, players_hands[current_player][i].rank) == 0 &&
                strcmp(input + 1, players_hands[current_player][i].suit) == 0) {
                printf("Player %d plays: %s%s\n", current_player + 1, players_hands[current_player][i].rank, players_hands[current_player][i].suit);
                // 從手牌中移除該牌
                for (int j = i; j < num_cards / num_players - 1; j++) {
                    players_hands[current_player][j] = players_hands[current_player][j + 1];
                }
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("Invalid card! Please enter a valid card.\n");
        }

        current_player = (current_player + 1) % num_players;
        turn++;

        // 檢查遊戲是否結束的條件
        if (turn > 10) {
            break;
        }
    }

    // Print remaining cards for each player
    printf("Remaining cards for each player:\n");
    for (int i = 0; i < num_players; i++) {
        printf("Player %d: ", i + 1);
        print_hand(players_hands[i], num_cards / num_players);
    }

    free(deck);
    for (int i = 0; i < num_players; i++) {
        free(players_hands[i]);
    }
    free(players_hands);

    return 0;
}
