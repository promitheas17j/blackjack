#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DECK_SIZE 52
#define DEALER_ID 0
#define PLAYER_ID 1

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
#define PLAYER_CARD_DEALT_UNKNOWN 900
#define DEALER_CARD_DEALT_UNKNOWN 1000

/* This is a function that allows us to easily create text based menus. */
/* The minimum number of options supplied is 2, and the maximum is 3. */
/* To make a menu with ONLY 2 options, you must pass NULL in place of option3. */
int prompt_choice(char menu_name[], char msg[], char option1[], char option2[], char option3[])
{
	int num_options = 0;
	printf("\n=================%s==================\n", menu_name);
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

int deal_card(int player_id, int* hand, int* shuffled_deck, int *index_of_top_card)
{
	printf("\n");
	// loop through the hand (player or dealer) until you find "the top", or the first index without a card in it
	for (int i = 0; i < DECK_SIZE / 2; i++)
	{
		/* printf("O: Index of top card: %d\n", *index_of_top_card); */
		if (hand[i] == 0)
		{
			hand[i] = shuffled_deck[*index_of_top_card];
			(*index_of_top_card)--; // remove a card from the top of the deck
			/* printf("I: Index of top card: %d\n", *index_of_top_card); */
			
			return hand[i];
		}
	}

	return DEAL_CARD_FAIL;
}

void show_hand(char* player, int* hand)
{
	printf("%s hand: ", player);
	for (int i = 0; i < DECK_SIZE / 2; i++)
	{
		if (hand[i] == 0)
		{
			break;
		}

		if (hand[i] == 1)
		{
			printf("A ");
		}
		else if (hand[i] >= 2 && hand[i] <= 10)
		{
			printf("%d ", hand[i]);
		}
		else if (hand[i] == 11)
		{
			printf("J ");
		}
		else if (hand[i] == 12)
		{
			printf("Q ");
		}
		else if (hand[i] == 13)
		{
			printf("K ");
		}
	}

	printf("\n");
}

int output_player_dealt_card(int* hand, int card_dealt)
{
	if (card_dealt >= 2 && card_dealt <= 10)
	{
		printf("%d was dealt\n", card_dealt);
		return card_dealt;
	}
	else if (card_dealt == 11)
	{
		printf("Jack was dealt\n");
		return 10;
	}
	else if (card_dealt == 12)
	{
		printf("Queen was dealt\n");
		return 10;
	}
	else if (card_dealt == 13)
	{
		printf("King was dealt\n");
		return 10;
	}
	else if (card_dealt == 1)
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
	
	return PLAYER_CARD_DEALT_UNKNOWN;
}

int output_dealer_dealt_card(int* hand, int dealer_score, int card_dealt)
{
	if (hand[0] != 0 && hand[1] == 0) // dealer only has one card i.e. the first card which must be hidden
	{
		printf("Dealer gets dealt a card face down.\n");

		if (card_dealt == 1)
		{
			if ((dealer_score + 11) > 21)
			{
				return 1;
			}
			else
			{
				return 11;
			}
		}
		return card_dealt; // if its not a 1, simply return its value as is
	}
	else // this is not the dealers first card, so it must be shown to the player
	{
		if (card_dealt >= 2 && card_dealt <= 10)
		{
			printf("%d was dealt\n", card_dealt);
			return card_dealt;
		}
		else if (card_dealt == 11)
		{
			printf("Jack was dealt\n");
			return 10;
		}
		else if (card_dealt == 12)
		{
			printf("Queen was dealt\n");
			return 10;
		}
		else if (card_dealt == 13)
		{
			printf("King was dealt\n");
			return 10;
		}
		else if (card_dealt == 1)
		{
			printf("Ace was dealt\n");

			if ((dealer_score + 11) > 21)
			{
				return 1;
			}
			else
			{
				return 11;
			}
		}
		return DEALER_CARD_DEALT_UNKNOWN;
	}
}

int game_round(int* shuffled_deck, int* player_hand, int* dealer_hand)
{
	int index_of_top_card = DECK_SIZE - 1;
	int player_score = 0;
	int dealer_score = 0;
	int card_dealt = 0;

	// Opening deals of the round (i.e. two to palyer two to dealer, first face down)
	card_dealt = deal_card(PLAYER_ID, player_hand, shuffled_deck, &index_of_top_card);
	player_score = player_score + output_player_dealt_card(player_hand, card_dealt);
	card_dealt = deal_card(PLAYER_ID, player_hand, shuffled_deck, &index_of_top_card);
	player_score = player_score + output_player_dealt_card(player_hand, card_dealt);

	card_dealt = deal_card(DEALER_ID, dealer_hand, shuffled_deck, &index_of_top_card);
	dealer_score = dealer_score + output_dealer_dealt_card(dealer_hand, dealer_score, card_dealt);
	card_dealt = deal_card(DEALER_ID, dealer_hand, shuffled_deck, &index_of_top_card);
	dealer_score = dealer_score + output_dealer_dealt_card(dealer_hand, dealer_score, card_dealt);
	
	card_dealt = deal_card(PLAYER_ID, player_hand, shuffled_deck, &index_of_top_card);
	player_score = player_score + output_player_dealt_card(player_hand, card_dealt);
	card_dealt = deal_card(DEALER_ID, dealer_hand, shuffled_deck, &index_of_top_card);
	dealer_score = dealer_score + output_dealer_dealt_card(dealer_hand, dealer_score, card_dealt);


	show_hand("Player", player_hand);
	show_hand("Dealer", dealer_hand);

	// while (condition)

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
