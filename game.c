#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int DECK_SIZE = 52;
const int DEALER_ID = 0;
const int PLAYER_ID = 1;
const int TRUE = 0;
const int FALSE = 1;

// FUNCTION EXIT CODE CONSTANTS
const int PROCESS_EXIT_NORMAL = 0;
const int DECK_SHUFFLE_SUCCESS = 100;
const int SUPPLIED_DECK_NULL = 200;
const int INITIALISE_DECKS_SUCCESS = 300;
const int INITILIASE_DECKS_FAIL = 400;
const int DEAL_CARD_FAIL = 500;
const int DECK_FINISHED = 600;
const int PLAYER_WIN = 700;
const int DEALER_WIN = 800;
const int ROUND_PUSH = 900;
const int PLAYER_CARD_DEALT_UNKNOWN = 1000;
const int DEALER_CARD_DEALT_UNKNOWN = 1100;
const int ROUND_WINNER_UNDETERMINED = 1200;

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

void show_hand(int player_id, int* hand, int round_is_done)
{
	if (player_id == PLAYER_ID)
	{
		printf("Player's hand: ");
	}
	else if (player_id == DEALER_ID)
	{
		printf("Dealer's hand: ");
	}

	for (int i = 0; i < DECK_SIZE / 2; i++)
	{
		/*
			If all of the following conditions hold true, then we must keep the first dealer card hidden:
			- It is the first card of the deck
			- The card is not an empty card
			- We want to show the dealer's hand
			- The round is not over (if it is over we want to show the entire dealer's hand)
		*/
		if (i == 0 && hand[i] != 0 && player_id == DEALER_ID && round_is_done == FALSE)
		{
			printf("? ");
			continue;
		}

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
	int player_bust = FALSE;
	int dealer_bust = FALSE;

	// Opening deals of the round (i.e. two to palyer two to dealer, first face down)
	card_dealt = deal_card(PLAYER_ID, player_hand, shuffled_deck, &index_of_top_card);
	player_score = player_score + output_player_dealt_card(player_hand, card_dealt);
	card_dealt = deal_card(PLAYER_ID, player_hand, shuffled_deck, &index_of_top_card);
	player_score = player_score + output_player_dealt_card(player_hand, card_dealt);

	card_dealt = deal_card(DEALER_ID, dealer_hand, shuffled_deck, &index_of_top_card);
	dealer_score = dealer_score + output_dealer_dealt_card(dealer_hand, dealer_score, card_dealt);
	card_dealt = deal_card(DEALER_ID, dealer_hand, shuffled_deck, &index_of_top_card);
	dealer_score = dealer_score + output_dealer_dealt_card(dealer_hand, dealer_score, card_dealt);
	
		
	int keep_betting = TRUE;
	while (keep_betting == TRUE)
	{
		printf("\n");
		show_hand(PLAYER_ID, player_hand, FALSE);
		show_hand(DEALER_ID, dealer_hand, FALSE);

		int choice = prompt_choice("Hit or Stay", "Would you like to HIT or STAY?", "Hit", "Stay", NULL);
		if (choice == 2)
		{
			keep_betting = FALSE;
		}
		else
		{
			card_dealt = deal_card(PLAYER_ID, player_hand, shuffled_deck, &index_of_top_card);
			player_score = player_score + output_player_dealt_card(player_hand, card_dealt);

			if (player_score > 21)
			{
				keep_betting = FALSE;
				player_bust = TRUE;
			}
		}
	}

	printf("\n-+++- Dealer Draws -+++-\n");
	while (keep_betting == TRUE)
	{
		if (dealer_score <= 16)
		{
			card_dealt = deal_card(DEALER_ID, dealer_hand, shuffled_deck, &index_of_top_card);
			dealer_score = dealer_score + output_dealer_dealt_card(dealer_hand, dealer_score, card_dealt);

			if (dealer_score > 21)
			{
				keep_betting = FALSE;
				dealer_bust = TRUE;
			}
		}
		else
		{
			keep_betting = FALSE;
		}
	}

	show_hand(PLAYER_ID, player_hand, TRUE);
	show_hand(DEALER_ID, dealer_hand, TRUE);


	printf("Player score: %d\nDealer score: %d\n", player_score, dealer_score);

	/*
		Round end cases:
			player and dealer have same score --> draw
			player has higher score than dealer but not bust --> player wins
			player has higher score than dealer but bust --> dealer wins
			dealer has higher score than player but not bust --> dealer wins
			dealer has higher score than player but bust --> player wins
	*/
	if (player_score == dealer_score)
	{
		return ROUND_PUSH;
	}
	else if (player_score > dealer_score)
	{
		if (player_bust == FALSE)
		{
			return PLAYER_WIN;
		}
		else
		{
			return DEALER_WIN;
		}
	}
	else if (player_score < dealer_score)
	{
		if (dealer_bust == FALSE)
		{
			return DEALER_WIN;
		}
		else
		{
			return PLAYER_WIN;
		}
	}

	return ROUND_WINNER_UNDETERMINED;
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

	int round_result = game_round(shuffled_deck, player_hand, dealer_hand);

	if (round_result == ROUND_WINNER_UNDETERMINED)
	{
		printf("\n\nError: Round winner could be determined. This should not happen normally.\n");
		return ROUND_WINNER_UNDETERMINED;
	}
	else if (round_result == PLAYER_WIN)
	{
		printf("\n\nYou won!!! Congratulations.\n");
	}
	else if (round_result == DEALER_WIN)
	{
		printf("\n\nDealer won. Better luck next time.\n");
	}
	else // round_result == ROUND_PUSH
	{
		printf("\n\nRound is a draw. Nobody wins.\n");
	}

	return PROCESS_EXIT_NORMAL;
}
