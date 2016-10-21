// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Game.h"
#include "DracView.h"

#define TOTAL_AMBIGUOUS_MOVES 7

static int isHideInTrail(DracView gameState);
static int isDoubleBackInTrail(DracView gameState);

void decideDraculaMove(DracView gameState)
{
        Round currRound = giveMeTheRound(gameState);
        LocationID dracLocation = whereIs(gameState, PLAYER_DRACULA);
        srand(time(NULL)); // set a seed
        char *nextMove;

        if (currRound == 0) {
                // when starting out, we place Dracula in MUNICH
	        registerBestPlay("MU", "Mwuhahahaha");
        } else {
                int numLocations = 0; 
                int canHide = FALSE; int canDoubleBack = FALSE; int canTeleport = FALSE;
                // we set sea and road to be TRUE because
                // we want to consider all possible Moves 
                // on LAND and WATER for dracula
                LocationID *legalMoves = whereCanIgo(gameState, &numLocations, TRUE, TRUE);
                // by now numLocations should be changed to an appropriate value
                // which contains the number of legal possible moves 
                // NOTE: numLocations only account for the number of possible moves
                //       via locationID and not moves with names such as
                //       HIDE/DOUBLE_BACK_#/TELEPORT 
                // Map.c is responsible for setting this value
                int numOfLegalMoves = numLocations;
                // check if Drac's trail contains a HIDE move
                if (isHideInTrail(gameState) == FALSE && idToType(dracLocation) != SEA) {
                        canHide = TRUE; numOfLegalMoves++;
                }
                // check if Drac's trail contains a Double Back move  
                if (isDoubleBackInTrail(gameState) == FALSE) {
                        canDoubleBack = TRUE; numOfLegalMoves++;
                        if (currRound >= 2) {  numOfLegalMoves++; }
                        if (currRound >= 3) {  numOfLegalMoves++; }
                        if (currRound >= 4) {  numOfLegalMoves++; }
                        if (currRound >= 5) {  numOfLegalMoves++; }
                }    
                // check if there are no legal moves available, 
                // if so Dracula can teleport back to castle
                if (numOfLegalMoves == 0) {
                        canTeleport = TRUE; numOfLegalMoves++;
                }
                  
                int numOfAmbiguousMoves = 0;  
                // ambiguousMoves is an array that contains all possible ambiguous moves
                // made by dracula. e.g. HIDE/DOUBLE_BACK_#/TELEPORT
                LocationID *ambiguousMoves;
                // e.g. if all ambiguous moves are available
                // then total numOfAmbiguousMoves would be 7 
                if (numOfLegalMoves > numLocations) {
                        numOfAmbiguousMoves = numOfLegalMoves - numLocations;
                        ambiguousMoves = malloc(sizeof(LocationID) * numOfAmbiguousMoves);
                        int index = 0;
                        if (canHide == TRUE) { ambiguousMoves[index] = HIDE; index++; }
                        if (canDoubleBack == TRUE) {
                                ambiguousMoves[index] = DOUBLE_BACK_1; index++;
                                if (currRound >= 2) { ambiguousMoves[index] = DOUBLE_BACK_2; index++; }
                                if (currRound >= 3) { ambiguousMoves[index] = DOUBLE_BACK_3; index++; }
                                if (currRound >= 4) { ambiguousMoves[index] = DOUBLE_BACK_4; index++; }
                                if (currRound >= 5) { ambiguousMoves[index] = DOUBLE_BACK_5; index++; }
                        }
                        if (canTeleport == TRUE) { ambiguousMoves[index] = TELEPORT; }
                }

                // NOTE: the legalMoves array should contain all possible moves made by Drac
                // hence we choose a random value by randomly choosing any index
                // randomly choose a value between 0 and (numOfLegalMoves-1)
                int chosenIndex = rand() % numOfLegalMoves; // 0 <= x <= (numOfLegalMoves-1)
                int chosenMove; //  
                // numLocations minus 1 because rand gives us a value between 0 and max-1
                if (chosenIndex > numLocations-1) {
                        // doing the operation below gives us the true index
                        // which will be used to obtain an element from the ambiguousMoves array
                        chosenIndex = chosenIndex - numLocations;
                        chosenMove = ambiguousMoves[chosenIndex];
                } else {
                        chosenMove = legalMoves[chosenIndex];
                }

                // checks if the chosen location is an actual locationID that represent cities 
                // on the map and not an ambiguous move ID such as HIDE/DOUBLE_BACK_#/TELEPORT
                if (validPlace(chosenMove)) {
                        nextMove = idToAbbrev(chosenMove);
                } else {
                        switch (chosenMove) {
                                case HIDE: nextMove = "HI"; break;
                                case DOUBLE_BACK_1: nextMove = "D1"; break;
                                case DOUBLE_BACK_2: nextMove = "D2"; break;
                                case DOUBLE_BACK_3: nextMove = "D3"; break;
                                case DOUBLE_BACK_4: nextMove = "D4"; break;
                                case DOUBLE_BACK_5: nextMove = "D5"; break;
                                case TELEPORT: nextMove = "TP"; break;
                        }
                }

                // NOTE: the registerBestPlay() function is already is written for us
	        registerBestPlay(nextMove, "Mwuhahahaha");

                // IF THIS WORKS, Dracula should be able to randomly make a legal move
        }
}

//--------------------------- Auxillary functions ---------------------------
static int isHideInTrail(DracView gameState)
{
        LocationID *reviewTrail = malloc(sizeof(LocationID) * TRAIL_SIZE); 
        giveMeTheMoves(gameState, PLAYER_DRACULA, reviewTrail);
        int i;
        for (i = 0; i < TRAIL_SIZE; i++) {
                if (reviewTrail[i] == HIDE) {
                        return TRUE;  
                } 
        }
        free(reviewTrail);
        return FALSE;
}

static int isDoubleBackInTrail(DracView gameState)
{
        LocationID *reviewTrail = malloc(sizeof(LocationID) * TRAIL_SIZE); 
        giveMeTheMoves(gameState, PLAYER_DRACULA, reviewTrail);
        int i;
        for (i = 0; i < TRAIL_SIZE; i++) {
                switch (reviewTrail[i]) {
                        case DOUBLE_BACK_1: 
                        case DOUBLE_BACK_2:
                        case DOUBLE_BACK_3:
                        case DOUBLE_BACK_4:
                        case DOUBLE_BACK_5:
                        return TRUE;
                } 
        }
        free(reviewTrail);
        return FALSE;
}
