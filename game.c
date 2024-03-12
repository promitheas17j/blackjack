#include <limits.h>
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
	for (int i = 0; i < DECK_SIZE; i++)
	{
		// initialise shuffled deck to be all 0s
		shuffled_deck[i] = 0;
		player_hand[i] = 0;
		dealer_hand[i] = 0;

		if (i < 4)
			deck[i] = 1;
		else if (i >= 4 && i < 8)
			deck[i] = 2;
		else if (i >= 8 && i < 12)
			deck[i] = 3;
		else if (i >= 12 && i < 16)
			deck[i] = 4;
		else if (i >= 16 && i < 20)
			deck[i] = 5;
		else if (i >= 20 && i < 24)
			deck[i] = 6;
		else if (i >= 24 && i < 28)
			deck[i] = 7;
		else if (i >= 28 && i < 32)
			deck[i] = 8;
		else if (i >= 32 && i < 36)
			deck[i] = 9;
		else if (i >= 36 && i < 40)
			deck[i] = 10;
		else if (i >= 40 && i < 44)
			deck[i] = 11;
		else if (i >= 44 && i < 48)
			deck[i] = 12;
		else if (i >= 48 && i < 52)
			deck[i] = 13;
		else
			deck[i] = -1;
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
		int random = (rand() % (size));
		// If deck[random] is less than 0, then it has already been selected. Select another card by essentially decrementing i
		// else select random element from deck and place it in shuffled_deck[i]. set deck[random] to -1
		/* printf("deck[random]: %d\n", deck[random]); */
		if (deck[random] != -1)
		{
			shuffled_deck[i] = deck[random];
			/* printf("\tshuffled_deck[i]: %d\n", shuffled_deck[i]); */
			deck[random] = -1;
		}
		else
		{
			i--;
		}
	}

	return DECK_SHUFFLE_SUCCESS;
}

int deal_card(int* hand, int* shuffled_deck, int index_of_top_card)
{
	// loop through the hand (player or dealer) until you find "the top", or the first index without a card in it
	for (int i = 0; i < DECK_SIZE / 2; i++)
	{
		if (hand[i] == 0)
		{
			hand[i] = shuffled_deck[index_of_top_card];
			index_of_top_card--; // remove a card from the top of the deck
			
			// Index 51 is the 52nd card bottom-up, i.e. the first card to be dealt.
			// Since only 2 players are in this version, the second card (index 50) will be the one dealt to the dealer and must be face down
			if (index_of_top_card != 50)
			{
				if (hand[i] >= 2 && hand[i] <= 10)
				{
					printf("%d was dealt...\n", hand[i]);
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

int* game_round(int* shuffled_deck, int* player_hand, int* dealer_hand, int* player_score)
{
	int deck_top_card = DECK_SIZE;

	deck_top_card = deal_card(player_hand, shuffled_deck, deck_top_card);

	int choice = prompt_choice("Stay or Hit", "Would you like to stay or hit?", "Stay", "Hit", NULL);

	return player_score;
}

int main() {
	srand(time(0));

	int deck[DECK_SIZE];
	int shuffled_deck[DECK_SIZE];
	int player_hand[DECK_SIZE / 2];
	int dealer_hand[DECK_SIZE / 2];

	int player_score = 0;

	int initialise_status = initialise_decks(deck, shuffled_deck, DECK_SIZE);
	if (initialise_status != INITIALISE_DECKS_SUCCESS)
	{
		return INITILIASE_DECKS_FAIL;
	}
	
	int shuffle_status = shuffle_deck(deck, shuffled_deck, DECK_SIZE);
	if (shuffle_status != DECK_SHUFFLE_SUCCESS)
	{
		return shuffle_status;
	}

	for (int i = 0; i < DECK_SIZE; i++)
	{
		printf("%d ", shuffled_deck[i]);
	}

	int choice = prompt_choice("Test Menu", "This is a message", "First option", "Second option", "Third option");
	printf("Option picked: %d\n", choice);

	return PROCESS_EXIT_NORMAL;
}
