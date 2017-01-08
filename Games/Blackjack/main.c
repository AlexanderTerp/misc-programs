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

// Define DEBUG level. Each subsequent level includes previous levels' features.
// LEVEL 0: No debugging features. Plays as intended for end-users.
// LEVEL 1: Don't hide other players' cards (who are not out).
// LEVEL 2: Don't hide players' cards even if they're out.
#define DEBUG 0

#define MIN_AI_DIFFICULTY 1
#define MAX_AI_DIFFICULTY 3
#define EASY_AI_PASS_VALUE 16

// Determine the amount of time AI wait between moves.
// Pause Time (ms) = AI_DELAY * MS_PER_DELAY.
#define AI_DELAY 2
#define MS_PER_DELAY 750

#define WIN_VALUE 21
#define NUM_CARDS 52
#define NUM_SUITS 4
#define LOWEST_CARD_NUM 2

// Theoretical max before losing. 4 twos, 4 threes, 1 ace (value 1), plus one
// additional card to go over 21.
#define MAX_CARDS_IN_HAND 10
#define INIT_MOVES 8

#define JACK 11
#define QUEEN 12
#define KING 13
#define ACE 14
#define MAX_ACE_VALUE 11
#define MIN_ACE_VALUE 1

#define MAX_NON_ACE_VALUE 10

#define SPADES 'S'
#define HEARTS 'H'
#define DIAMONDS 'D'
#define CLUBS 'C'

#define JACK_CHAR 'J'
#define QUEEN_CHAR 'Q'
#define KING_CHAR 'K'
#define ACE_CHAR 'A'


#define HIT 'H'
#define PASS 'P'
#define LINE "-----------------------------------------------"

// Define global variables.
int USERS_PLAYING;
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
    int num_cards;
    int hand_value;
    int out;
    int wins;
    int is_ai;
    int difficulty;
    int num_moves;
    int moves_array_size;
    char* moves;
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
int init_players(Game *game_data);
void play_game(int game_num, Game *game_data);
void print_heading(Game *game_data, int next_player);
void print_moves(Player *player);
int check_game_fin(Game *game_data);
int get_next_player(int prev_player, Game *game_data);
void deal_card(Player *player, Game *game_data);
void refresh_hand_value(Player *player);
int ai_hit(Player *player, Game *game_data);
int is_face_card(Card *card);
void add_move(Player *player, char move);
void declare_victor(Game *game_data);
void reset_game(Game *game_data);

int main(void) {
    // Initialize game_data and seed the random number generator.
    Game game_data;
    srand(time(NULL));

    // Initialize and populate deck.
    init_deck(&(game_data.deck));

    printf("Number of humans playing?\n> ");
    scanf("%d", &USERS_PLAYING);

    printf("\nHow many bots?\n> ");
    scanf("%d", &NUM_PLAYERS);
    NUM_PLAYERS += USERS_PLAYING;

    // Initialize players.
    init_players(&game_data);

    int play_again = 1, game_num = 1;
    while (play_again) {
        play_game(game_num++, &game_data);
        reset_game(&game_data);
        
        printf("\nPlay again? (1/0)\n> ");
        scanf("%d", &play_again);
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
    /* Prints out all he cards in a deck/hand. Prints letters to correspond with
    face cards when encountered. */

    int i;
    for (i = 0; i < num_cards; i++) {
        if (cards[i].rank == JACK) {
            printf("%c%c, ", JACK_CHAR, cards[i].suit);
        } else if (cards[i].rank == QUEEN) {
            printf("%c%c, ", QUEEN_CHAR, cards[i].suit);
        } else if (cards[i].rank == KING) {
            printf("%c%c, ", KING_CHAR, cards[i].suit);
        } else if (cards[i].rank == ACE) {
            printf("%c%c, ", ACE_CHAR, cards[i].suit);
        } else {
            printf("%d%c, ", cards[i].rank, cards[i].suit);
        }
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

int init_players(Game *game_data) {

    // Define a difficulty for bots. 
    // LEVEL 1: Easy. Has no access to card-counting/the deck nor other players'
    //          values.
    // LEVEL 2: Medium. Has access to the deck and so can do better probability 
    //          calculations.
    // LEVEL 3: Hard. Has access to both the deck and hand_values of other players.

    // Allocate memory to store the players.
    game_data->players = malloc(NUM_PLAYERS * sizeof *(game_data->players) );

    int i, difficulty;
    for (i = 1; i <= NUM_PLAYERS; i++) {
        Player player = {.id = i, .num_cards = 0, .hand_value = 0,
            .out = 0, .wins = 0, .num_moves = 0, 
            .moves_array_size = INIT_MOVES};

        player.moves = malloc(INIT_MOVES * sizeof *(player.moves) );

        if (i > USERS_PLAYING) {
            // Must be an AI.
            player.is_ai = 1;

            printf("\nDefine a bot difficulty %d to %d for Player %d:\n> ", 
                MIN_AI_DIFFICULTY, MAX_AI_DIFFICULTY, i);
            scanf("%d", &difficulty);

            player.difficulty = difficulty;
        } else {
            // Must be a human player.
            player.is_ai = 0;
            player.difficulty = -1;
        }

        game_data->players[i-1] = player;
    }

    return 1;
}

void play_game(int game_num, Game *game_data) {

    int i, hit, next_player = 0;

    printf("Round %d\n", game_num);
    do {
        printf("%s", LINE);
        print_heading(game_data, next_player);

        printf("\n\nPlayer %d's turn", next_player+1);
        if ( !game_data->players[next_player].is_ai ) {
            printf("\nHit? (1/0)\n> ");
            scanf("%d", &hit);

            if (hit) {
                add_move( &(game_data->players[next_player]), HIT);
                deal_card( &(game_data->players[next_player]), game_data);
                refresh_hand_value( &(game_data->players[next_player]));
                if (game_data->players[next_player].hand_value > WIN_VALUE) {
                    printf("You're at %d, over %d! You're out!\n", 
                        game_data->players[next_player].hand_value, WIN_VALUE);
                    game_data->players[next_player].out = 1;
                    next_player = get_next_player(next_player, game_data);
                }
            } else {
                add_move( &(game_data->players[next_player]), PASS);
                next_player = get_next_player(next_player, game_data);
            }

        } else {
            for (i = 0; i < AI_DELAY; i++) {
                printf(" .");
                cross_sleep(MS_PER_DELAY);
            }
            hit = ai_hit( &(game_data->players[next_player]), game_data);

            if (hit) {
                printf(" HIT!\n");
                add_move( &(game_data->players[next_player]), HIT);
                deal_card( &(game_data->players[next_player]), game_data);
                refresh_hand_value( &(game_data->players[next_player]));
                if (game_data->players[next_player].hand_value > WIN_VALUE) {
                    printf("Player %d is at %d, over %d! Player %d is out!\n", 
                        next_player+1, 
                        game_data->players[next_player].hand_value, WIN_VALUE,
                        next_player+1);
                    game_data->players[next_player].out = 1;
                    next_player = get_next_player(next_player, game_data);
                }
            } else {
                printf(" PASS!\n");
                add_move( &(game_data->players[next_player]), PASS);
                next_player = get_next_player(next_player, game_data);
            }
        }
    } while ( !check_game_fin(game_data) );

    declare_victor(game_data);
}

void print_heading(Game *game_data, int next_player) {

    // Print player moves/values/cards (heading).
    int i;
    for (i = 0; i < NUM_PLAYERS; i++) {
        // Only print for players still in the game.
        if ( !game_data->players[i].out || DEBUG == 2 ) { // REMOVE OR
            if ( DEBUG == 1 || (!game_data->players[i].is_ai && 
                (USERS_PLAYING == 1 || i == next_player)) ) {
                printf("\nPlayer %d's hand (%2d):\t\t", 
                    game_data->players[i].id, game_data->players[i].hand_value);
                print_cards(game_data->players[i].num_cards, 
                    game_data->players[i].hand);
            } else {
                printf("\n# Player %d's cards:\t\t%d | ", 
                    game_data->players[i].id, game_data->players[i].num_cards);
                print_moves( &(game_data->players[i]) );
            }
        }
    }
}

void print_moves(Player *player) {

    int i;
    for (i = 0; i < player->num_moves; i++) {
        printf("%c, ", player->moves[i]);
    }
}

int check_game_fin(Game *game_data) {
    /* Returns whether or not the game is finished. This is true if there are 
    less than 2 active players left or if, throughout the previous round, nobody
    hit after someone else passed. */

    int i, prev_move_pass = 1, players_left = 0;
    for (i = 0; i < NUM_PLAYERS; i++) {
        if (!game_data->players[i].out) {
            players_left++;

            if (players_left > 1 && prev_move_pass) {
                if (game_data->players[i].num_moves <= 2 || 
                    game_data->players[i].moves[ 
                    game_data->players[i].num_moves-2 ] == HIT) {
                    return 0;
                }
            }
        }

    }

    // Made it through the loop, so we can state logical truths. 
    // If players_left > 1, nobody hit after someone else only passed.
    //      Therefore, return FINISHED.
    // If players <= 1, we know nothing about players hitting after passes. We 
    //      also don't care. Just return FINISHED.
    // Either way, the game is FINISHED. Return 1.
    return 1;
}

int get_next_player(int prev_player, Game *game_data) {
    /* Returns the index in game_data->players for the next player to play.
    Checks which players are out. */

    int i = (prev_player + 1) % NUM_PLAYERS;
    while (game_data->players[i].out) {
        i = (i + 1) % NUM_PLAYERS;
    }

    return i;
}

void deal_card(Player *player, Game *game_data) {
    /* Deals a random card from the deck into a specified player's hand. */

    // Checks if the deck is empty. If so, resets it or "adds a second deck".
    if (game_data->deck.cards_left == 0) {
        printf("\n\nAdding second deck!\n\n");
        repopulate_deck( &(game_data->deck) );
    }

    int random = rand() % NUM_CARDS;
    while ( !game_data->deck.cards[random].in_deck ) {
        random = (random + 1) % NUM_CARDS;
    }

    game_data->deck.cards[random].in_deck = 0;
    game_data->deck.cards_left--;
    player->hand[player->num_cards++] = game_data->deck.cards[random];
}

void refresh_hand_value(Player *player) {
    /* Recalculates and updates the .hand_value for a given player. Includes
    ace handling. */

    int i, sum = 0, aces = 0;
    for (i = 0; i < player->num_cards; i++) {
        if (is_face_card( &(player->hand[i]) )) {
            sum += MAX_NON_ACE_VALUE;
        } else if (player->hand[i].rank == ACE) {
            aces++;
        } else {
            sum += player->hand[i].rank;
        }
    }

    if (aces) {

        // Start off by assuming all aces add their minimum possible value.
        sum += aces;
        if (sum > WIN_VALUE) {
            // Even with all aces valued at MIN_ACE_VALUE, still over WIN_VALUE.
            player->hand_value = sum;
            return;
        }

        for (i = 0; i < aces; i++) {
            // Consider each ace. If we turn one of them into their max value,
            // are we still okay? If so, do that. Consider ace, if present. If 
            // not okay, save the current sum in hand_value and exit.
            if (sum + MAX_ACE_VALUE-MIN_ACE_VALUE <= WIN_VALUE) {
                sum += MAX_ACE_VALUE-MIN_ACE_VALUE;
            } else {
                player->hand_value = sum;
                return;
            }
        }
    }

    player->hand_value = sum;
}

int ai_hit(Player *player, Game *game_data) {
    /* Calculates whether the AI should hit or pass. Returns a 1 for hit and 0
    for pass. */

    int i;

    if (player->difficulty >= 3) {
        // Checks if the AI is behind another player in value. If so, hit no
        // matter what as passing would mean a certain loss.
        for (i = 0; i < NUM_PLAYERS; i++) {
            if ( !game_data->players[i].out && 
                player->id != game_data->players[i].id) {
                if (player->hand_value <= game_data->players[i].hand_value) {
                    return !(player->hand_value == WIN_VALUE);
                }
            }
        }
    }

    if (player->difficulty >= 2) {
        // Calculate the number of cards that would result in losing and the number
        // of cards that would not result in losing.
        int cards_lose = 0, cards_survive = 0, curr_value = player->hand_value,
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
    
    if (player->difficulty >= 1) {
        // Really simple (read: easy) AI, if it can even be called that. If
        // current hand value is EASY_AI_PASS_VALUE or above, pass. 
        // Otherwise, hit.
        return !(player->hand_value >= EASY_AI_PASS_VALUE);
    }

    // Suppress compilation "control reaches end of non-void function".
    return -1;
}

int is_face_card(Card *card) {
    /* Returns 1 if the given card is a jack, queen, or king. Returns 0
    otherwise. */

    return card->rank >= JACK && card->rank < ACE;
}

void add_move(Player *player, char move) {

    player->num_moves++;

    if (player->num_moves > player->moves_array_size) {
        player->moves_array_size *= 2;
        player->moves = realloc(player->moves, 
            player->moves_array_size * sizeof *(player->moves));
    }

    player->moves[player->num_moves-1] = move;
}

void declare_victor(Game *game_data) {
    /* Called at the end of the game. Prints the hand values of each player, 
    declares the winner, and changes number of wins for each player.*/

    Player *winners[NUM_PLAYERS];
    int i, highest_value = 0, num_winners = 0;
    for (i = 0; i < NUM_PLAYERS; i++) {
        if ( !game_data->players[i].out ) {
            if (game_data->players[i].hand_value == highest_value) {
                winners[num_winners] = &(game_data->players[i]);
                num_winners++;
            } else if (game_data->players[i].hand_value > highest_value) {
                highest_value = game_data->players[i].hand_value;
                winners[0] = &(game_data->players[i]);
                num_winners = 1;
            }
        }
    }

    for (i = 0; i < num_winners; i++) {
        winners[i]->wins++;

        if (i == 0) {
            printf("\nCongratulations to Player %d", winners[i]->id);
        } else {
            printf(", Player %d", winners[i]->id);
        }
    }
    printf("!\n");

    for (i = 0; i < NUM_PLAYERS; i++) {
        printf("Player %d: %2d with ", game_data->players[i].id, 
            game_data->players[i].hand_value);
        print_cards(game_data->players[i].num_cards, 
            game_data->players[i].hand);
        printf("\n");
    }

    printf("\nUpdated scores: \n");
    for (i = 0; i < NUM_PLAYERS; i++) {
        printf("Player %d: %2d\n", game_data->players[i].id, 
            game_data->players[i].wins);
    }
}

void reset_game(Game *game_data) {
    /* Resets game_data and items inside it post-game, preparing to start a new
    match. */

    // Reset the game deck.
    repopulate_deck( &(game_data->deck) );

    // Reset each players' hands.
    int i;
    for (i = 0; i < NUM_PLAYERS; i++) {
        game_data->players[i].num_cards = 0;
        game_data->players[i].hand_value = 0;
        game_data->players[i].out = 0;
        game_data->players[i].num_moves = 0;
    }
}