// Author:          Alexander M. Terp
// Date:            January 4, 2016
// Purpose:         Facilitates the playing of a custom variant of Blackjack.
//                  Allows the user to participate and/or play against multiple
//                  bots.


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Check whether system OS is Windows or Unix and imports appropriate library.
#ifdef _WIN32
    #define OS_IS_WINDOWS 1
    #include <windows.h>
#else
    #define OS_IS_WINDOWS 0
    #include <unistd.h>
#endif

// Define macro values.
#define WIN_VALUE 21
#define NUM_CARDS 52
#define NUM_SUITS 4
#define LOWEST_CARD_NUM 2

// Theoretical max before losing. 4 twos, 4 threes, 1 ace (value 1), plus one
// additional card to go over 21.
#define MAX_CARDS_IN_HAND 10

#define JACK 11
#define QUEEN 12
#define KING 13
#define ACE 14

#define SPADES 'S'
#define HEARTS 'H'
#define DIAMONDS 'D'
#define CLUBS 'C'

#define LINE "-----------------------------------------------"

// Define global variables.
int USER_PLAYING;
int NUM_PLAYERS = 0;

typedef struct {
    int id;
    int rank;
    char suit;
    int in_deck;
} Card;

typedef struct {
    Card cards[NUM_CARDS];
    int cards_left;
} Deck;

typedef struct {
    int id;
    Card hand[MAX_CARDS_IN_HAND];
    int num_cards_in_hand;
    int deck_value;
    int out;
    int wins;
    int losses;
    int is_ai;
} Player;

typedef struct {
    Player* players;
    Deck deck;
} Game;

void cross_sleep(int ms);
void init_deck(Deck* deck);
void repopulate_deck(Deck* deck);
void print_cards(int num_cards, Card cards[num_cards]);
Card create_card(int id);
void play_game(int game_num, Game *game_data);
int init_players(Game *game_data);
int check_game_fin(Game *game_data);
int get_next_player(int prev_player, Game *game_data);
void deal_card(Player *player, Game *game_data);
void refresh_deck_value(Player *player);
int ai_hit(Player *player, Game *game_data);

int main(void) {
    // Initialize game_data and seed the random number generator.
    Game game_data;
    srand(time(NULL));

    // Initialize and populate deck.
    init_deck(&(game_data.deck));

    printf("Would you like to play? (1/0)\n> ");
    scanf("%d", &USER_PLAYING);

    printf("How many bots?\n> ");
    scanf("%d", &NUM_PLAYERS);
    NUM_PLAYERS += USER_PLAYING;

    // Initialize players.
    printf("Init players: %d\n", init_players(&game_data));

    int play_again = 1;

    while (play_again) {
        play_game(1, &game_data);
        play_again = 0;
    }

    return 0;
}

void cross_sleep(int ms) {
    if (OS_IS_WINDOWS) {
        Sleep(ms);
    } else {
        usleep(ms);
    }
}

void init_deck(Deck* deck) {
    /* Given a deck, creates each card and puts it into the array contained in
    deck. */

    int i;
    for (i = 0; i < NUM_CARDS; i++) {
        deck->cards[i] = create_card(i);
    }

    deck->cards_left = NUM_CARDS;
}

void repopulate_deck(Deck* deck) {
    /* "Refills" the deck by switching the in_deck status of every card in the 
    deck to 1. */

    int i;
    for (i = 0; i < NUM_CARDS; i++) {
        deck->cards[i].in_deck = 1;
    }

    deck->cards_left = NUM_CARDS;
}

void print_cards(int num_cards, Card cards[num_cards]) {

    int i;
    for (i = 0; i < num_cards; i++) {
        printf("%d%c, ", cards[i].rank, cards[i].suit);
    }
}

Card create_card(int id) {
    /* Given an ID between 0 and 51, returns a card struct with accompanying
    rank and suit. */

    Card card;
    card.in_deck = 1;
    card.id = id;
    card.rank = (id / NUM_SUITS) + LOWEST_CARD_NUM;
    
    if (id % NUM_SUITS == 0)
        card.suit = SPADES;
    else if (id % NUM_SUITS == 1)
        card.suit = HEARTS;
    else if (id % NUM_SUITS == 2)
        card.suit = DIAMONDS;
    else if (id % NUM_SUITS == 3)
        card.suit = CLUBS;

    return card;
}

void play_game(int game_num, Game *game_data) {

    int i, hit, next_player = 0;
    printf("Round %d\n", game_num);
    while ( !check_game_fin(game_data) ) {
        printf("%s", LINE);
        for (i = 0; i < NUM_PLAYERS; i++) {
            if ( !game_data->players[i].out || game_data->players[i].out ) { // REMOVE OR
                printf("\nPlayer %d's hand (%2d):\t\t", 
                    game_data->players[i].id, game_data->players[i].deck_value);
                print_cards(game_data->players[i].num_cards_in_hand, 
                    game_data->players[i].hand);
            }
        }

        if ( !game_data->players[next_player].is_ai ) {
            printf("\n\nYOUR TURN\n");
            printf("Hit? (1/0)\n> ");
            scanf("%d", &hit);

            if (hit) {
                deal_card( &(game_data->players[next_player]), game_data);
                refresh_deck_value( &(game_data->players[next_player]));
                if (game_data->players[next_player].deck_value > WIN_VALUE) {
                    printf("You're at %d, over %d! You're out!\n", 
                        game_data->players[next_player].deck_value, WIN_VALUE);
                    game_data->players[next_player].out = 1;
                    next_player = get_next_player(next_player, game_data);
                }
            } else {
                next_player = get_next_player(next_player, game_data);
            }

        } else {
            printf("\n\nPlayer %d's turn", next_player+1);
            for (i = 0; i < 3; i++) {
                printf(" .");
                cross_sleep(1000);
            }
            hit = ai_hit( &(game_data->players[next_player]), game_data);

            if (hit) {
                printf(" HIT!\n");
                deal_card( &(game_data->players[next_player]), game_data);
                refresh_deck_value( &(game_data->players[next_player]));
                if (game_data->players[next_player].deck_value > WIN_VALUE) {
                    printf("Player %d is at %d, over %d! Player %d is out!\n", 
                        next_player+1, 
                        game_data->players[next_player].deck_value, WIN_VALUE,
                        next_player+1);
                    game_data->players[next_player].out = 1;
                    next_player = get_next_player(next_player, game_data);
                }
            } else {
                printf(" PASS!\n");
                next_player = get_next_player(next_player, game_data);
            }
        }
    }
}

int init_players(Game *game_data) {

    // Allocate memory to store the players.
    game_data->players = malloc(NUM_PLAYERS * sizeof *(game_data->players) );

    int i;
    for (i = 1; i <= NUM_PLAYERS; i++) {
        Player player = {.id = i, .num_cards_in_hand = 0, .deck_value = 0,
            .out = 0, .wins = 0, .losses = 0, .is_ai = 1};

        game_data->players[i-1] = player;
    }

    if (USER_PLAYING) {
        game_data->players[0].is_ai = 0;
    }

    return 1;
}

int check_game_fin(Game *game_data) {
    /* Returns whether there are 1 or fewer active players still in the game.
    1 if true, 0 if not. */

    int players_left = 0;
    int i;
    for (i = 0; i < NUM_PLAYERS; i++) {
        if (!game_data->players[i].out) {
            players_left++;
        }

        if (players_left >= 2) {
            return 0;
        }
    }

    return 1;
}

int get_next_player(int prev_player, Game *game_data) {
    /* Returns the index in game_data->players for the next player to play.
    Checks which players are out. */

    int i = (prev_player + 1) % NUM_PLAYERS;
    while (game_data->players[i].out) {
        i = (i + 1) % NUM_PLAYERS;
    }
    printf("get_next_player: %d -> %d\n", prev_player, i);
    return i;
}

void deal_card(Player *player, Game *game_data) {
    /* Deals a random card from the deck into a specified player's hand. */

    int random = rand() % NUM_CARDS;
    while ( !game_data->deck.cards[random].in_deck ) {
        random = (random + 1) % NUM_CARDS;
    }

    game_data->deck.cards[random].in_deck = 0;
    player->hand[player->num_cards_in_hand++] = game_data->deck.cards[random];
}

void refresh_deck_value(Player *player) {
    /* Recalculates and updates the .deck_value for a given player. */

    int i, sum = 0;
    for (i = 0; i < player->num_cards_in_hand; i++) {
        sum += player->hand[i].rank;
    }

    player->deck_value = sum;
}

int ai_hit(Player *player, Game *game_data) {
    /* Calculates whether the AI should hit or pass. Returns a 1 for hit and 0
    for pass. */

    // Checks if the AI is behind another player in value. If so, hit no matter
    // what as passing would mean a certain loss.
    int i;
    for (i = 0; i < NUM_PLAYERS; i++) {
        if ( !game_data->players[i].out && 
            player->id != game_data->players[i].id) {
            if (player->deck_value <= game_data->players[i].deck_value) {
                return 1;
            }
        }
    }

    // Must not be behind in value relative to any other player. Calculate the
    // number of cards that would result in losing and the number of cards that
    // would not result in losing.
    int cards_lose = 0, cards_survive = 0, curr_value = player->deck_value,
        card_will_lose;
    for (i = 0; i < NUM_CARDS; i++) {
        card_will_lose = curr_value + game_data->deck.cards[i].rank > WIN_VALUE;
        cards_lose += card_will_lose;
        cards_survive += !card_will_lose;
    }

    // If it's more likely to pick a card that'll keep you in the game, hit.
    // Otherwise, pass.
    return cards_survive >= cards_lose;
}