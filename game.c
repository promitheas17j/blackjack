#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DECK_SIZE 52

// FUNCTION EXIT CODE DEFINITIONS
#define PROCESS_EXIT_NORMAL 0
#define DECK_SHUFFLE_SUCCESS 100
#define SUPPLIED_DECK_NULL 200
#define INITIALISE_DECKS_SUCCESS 300
#define INITILIASE_DECKS_FAIL 400
#define DEAL_CARD_FAIL 500
#define DECK_FINISHED 600
#define PLAYER_WIN 700
#define DEALER_WIN 800

/* This is a function that allows us to easily create text based menus. */
/* The minimum number of options supplied is 2, and the maximum is 3. */
/* To make a menu with ONLY 2 options, you must pass NULL in place of option3. */
int prompt_choice(char menu_name[], char msg[], char option1[], char option2[], char option3[])
{
	int num_options = 0;
	printf("\n\n=================%s==================\n", menu_name);
	printf("%s\n", msg);
	printf("1. %s\n", option1);
	printf("2. %s\n", option2);
	if (option3 != NULL)
	{
		printf("3. %s\n", option3);
		num_options = 3;
	}
	else
	{
		num_options = 2;
	}

	char input[2];
	char choice = 0;
	while (choice < 1 || choice > num_options)
	{
		printf("Your choice (only the number): ");
		fgets(input, sizeof(input), stdin);

		// Clear input buffer
		while (getchar() != '\n');

		sscanf(input, "%c", &choice);
		choice = choice - 48;

		if (choice < 1 || choice > num_options)
		{
			printf("Invalid input. Please enter only any of the numbers 1, 2, or 3.\n\n");
		}
	}

	return choice;
}

int initialise_decks(int* deck, int* shuffled_deck, int* player_hand, int* dealer_hand, int size)
{
	printf("=================INITIALISING==================\n");
	for (int i = 0; i < size; i++)
	{
		deck[i] = (i % 13) + 1;
	}

	for (int i = 0; i < size / 2; i++)
	{
		player_hand[i] = 0;
		dealer_hand[i] = 0;
	}
	
	return INITIALISE_DECKS_SUCCESS;
}

int shuffle_deck(int* deck, int* shuffled_deck, int size)
{
	printf("=================SHUFFLING==================\n");
	if (deck == NULL || size <= 0)
	{
		return SUPPLIED_DECK_NULL;
	}

	for (int i = 0; i < size; i++)
	{
		shuffled_deck[i] = 0;
	}

	for (int i = 0; i < size; i++)
	{
		int random = (rand() % (size));
		// If deck[random] is less than 0, then it has already been selected. Select another card by essentially decrementing i
		// else select random element from deck and place it in shuffled_deck[i]. set deck[random] to -1
		if (deck[random] != -1)
		{
			shuffled_deck[i] = deck[random];
			deck[random] = -1;
		}
		else
		{
			i--;
		}
	}

	return DECK_SHUFFLE_SUCCESS;
}

int deal_card(int* hand, int* shuffled_deck, int *index_of_top_card)
{
	// loop through the hand (player or dealer) until you find "the top", or the first index without a card in it
	for (int i = 0; i < DECK_SIZE / 2; i++)
	{
		printf("Index of top card: %d\n", *index_of_top_card);
		if (hand[i] == 0)
		{
			hand[i] = shuffled_deck[*index_of_top_card];
			(*index_of_top_card)--; // remove a card from the top of the deck
			
			// Index 51 is the 52nd card bottom-up, i.e. the first card to be dealt.
			// Since only 2 players are in this version, the second card (index 50) will be the one dealt to the dealer and must be face down
			if (*index_of_top_card != 50)
			{
				if (hand[i] >= 2 && hand[i] <= 10)
				{
					printf("%d was dealt\n", hand[i]);
					return hand[i];
				}
				else
				{
					if (hand[i] == 11)
					{
						printf("Jack was dealt\n");
						return 10;
					}
					else if (hand[i] == 12)
					{
						printf("Queen was dealth\n");
						return 10;
					}
					else if (hand[i] == 13)
					{
						printf("King was dealt\n");
						return 10;
					}
					else if (hand[i] == 1)
					{
						int choice = prompt_choice("Ace", "Ace was dealt. Would you like it to count as 1 point or 11 points?", "One point", "Eleven points", NULL);
						if (choice == 1)
						{
							return 1;
						}
						else
						{
							return 11;
						}
					}
				}
			}
			else // index 50 i.e second card to be dealt, i.e. will not show to the player
			{
				printf("Dealer deals himself a card face down\n");
				return hand[i];
			}
		}
	}

	return DEAL_CARD_FAIL;
}

int game_round(int* shuffled_deck, int* player_hand, int* dealer_hand)
{
	int index_of_top_card = DECK_SIZE;
	int player_score = 0;
	int dealer_score = 0;

	// FIX: Figure out why a single card dealing decrements the index_of_top_card variable twice, and why the dealer deals himself a card
	// First card dealt to player
	player_score = player_score + deal_card(player_hand, shuffled_deck, &index_of_top_card);
	// Second card dealt to dealer (should be face down)
	/* dealer_score = dealer_score + deal_card(dealer_hand, shuffled_deck, &index_of_top_card); */

	printf("Player score: %d\nDealer score: %d\n", player_score, dealer_score);

	/* int choice = prompt_choice("Stay or Hit", "Would you like to stay or hit?", "Stay", "Hit", NULL); */	

	if (player_score > 21)
	{
		return DEALER_WIN;
	}
	else if (dealer_score > 21)
	{
		return PLAYER_WIN;
	}

	return player_score;
}

int main() {
	srand(time(0));

	int deck[DECK_SIZE];
	int shuffled_deck[DECK_SIZE];
	int player_hand[DECK_SIZE / 2];
	int dealer_hand[DECK_SIZE / 2];

	int initialise_status = initialise_decks(deck, shuffled_deck, player_hand, dealer_hand, DECK_SIZE);
	if (initialise_status != INITIALISE_DECKS_SUCCESS)
	{
		return INITILIASE_DECKS_FAIL;
	}

	int shuffle_status = shuffle_deck(deck, shuffled_deck, DECK_SIZE);
	if (shuffle_status != DECK_SHUFFLE_SUCCESS)
	{
		return shuffle_status;
	}

	int player_score = game_round(shuffled_deck, player_hand, dealer_hand);
	player_score++; // this is just so the compiler doesn't complain that player_score is unused

	/* int choice = prompt_choice("Test Menu", "This is a message", "First option", "Second option", "Third option"); */
	/* printf("Option picked: %d\n", choice); */

	return PROCESS_EXIT_NORMAL;
}
