// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Game.h"
#include "DracView.h"
#include "Map.h"

#define TOTAL_AMBIGUOUS_MOVES 7

static void checkAmbiguousMovesAvailability(int *ambiguousMoves, Round currRound);
static void validateDoubleBackandHide(DracView gameState, int *ambiguousMoves, LocationID currPos);
static char *ambiguousIdToAbbrev(LocationID move);
static void incrementNumOfLocations(int *ambiguousMoves, int *numAmbiguousMoves);
static LocationID *createPossibleMovesArray(LocationID *legalMoves, int numLocations, int numAmbiguousMoves,
                                            int *totalNumMoves, int *ambiguousMoves);

void decideDraculaMove(DracView gameState)
{
    Round currRound = giveMeTheRound(gameState);
    LocationID currPos = whereIs(gameState, PLAYER_DRACULA);
    int numLocations = 0; int numAmbiguousMoves = 0; int totalNumMoves = 0;
    srand(time(NULL)); // set a seed
    char *nextMove;

    printf("\n");
    printf("currRound: %d\n", currRound);
    printf("dracPos: %d\n", currPos);

    if (currRound == 0) {
        // when starting out, we place Dracula in MUNICH
        registerBestPlay("MU", "Mwuhahahaha");
    } else {
        // we want to consider all possible Moves 
        // on LAND and WATER for dracula
        LocationID dracTrail[TRAIL_SIZE]; giveMeTheTrail(gameState, PLAYER_DRACULA, dracTrail);
        LocationID dracTrailMoves[TRAIL_SIZE]; giveMeTheMoves(gameState, PLAYER_DRACULA, dracTrailMoves);
        LocationID *legalMoves = whereCanIgo(gameState, &numLocations, TRUE, TRUE);
        
        Map g = newMap(); int numLocs = 0;  
        LocationID *adjacentLocations = reachableLocations(g, &numLocs, currPos, TRUE, FALSE, TRUE, TRUE);
    
        // used for debugging purposes
        //----------------------------------------
        printf("adjacentLocations: ");
        int j;
        for (j = 0; j < numLocs; j++) {
            printf("[%d]", adjacentLocations[j]); 
        }
        printf("\n");
        //----------------------------------------
        printf("dracTrail: ");
        for (j = 0; j < TRAIL_SIZE; j++) {
            printf("[%d]", dracTrail[j]); 
        }
        printf("\n");
        //----------------------------------------
        printf("dracTrailMoves: ");
        for (j = 0; j < TRAIL_SIZE; j++) {
            printf("[%d]", dracTrailMoves[j]); 
        }
        printf("\n");
        //----------------------------------------
        printf("legalMoves: ");
        for (j = 0; j < numLocations; j++) {
            printf("[%d][%s] ", legalMoves[j], idToAbbrev(legalMoves[j])); 
        }
        printf("\n");
        //----------------------------------------

        // | [0] = db1 | [1] = db2 | [2] = db3 | [3] = db4 | [4] = db5 | [5] = h1 | [6] = tp |
        int ambiguousMoves[TOTAL_AMBIGUOUS_MOVES];
        int i = 0; for (i = 0; i < TOTAL_AMBIGUOUS_MOVES; i++) { ambiguousMoves[i] = FALSE; }
        checkAmbiguousMovesAvailability(ambiguousMoves, currRound);
        validateDoubleBackandHide(gameState, ambiguousMoves, currPos);
        incrementNumOfLocations(ambiguousMoves, &numAmbiguousMoves);
        LocationID *possibleMoves = createPossibleMovesArray(legalMoves, numLocations, numAmbiguousMoves, &totalNumMoves, ambiguousMoves);
        
        // used for debugging
        printf("ambiguousMoves: ");
        for (i = 0; i < TOTAL_AMBIGUOUS_MOVES; i++) {
            printf("[%d] ", ambiguousMoves[i]);
        }
        printf("\n");

        printf("numLocations: %d\n", numLocations);
        printf("numAmbiguousMoves: %d\n", numAmbiguousMoves);
        printf("totalNumMoves: %d\n", totalNumMoves);

        printf("possibleMoves: ");
        for (i = 0; i < totalNumMoves; i++) {
            printf("[%d] ", possibleMoves[i]);
        }
        printf("\n");

        LocationID chosenMove; int chosenIndex = 0;
        chosenIndex = rand() % totalNumMoves;
        chosenMove = possibleMoves[chosenIndex];
        if (!validPlace(chosenMove)) {
            nextMove = ambiguousIdToAbbrev(chosenMove);
        } else { 
            nextMove = idToAbbrev(chosenMove);
        }
        // freeing the malloc'd pointers
        free(legalMoves); legalMoves = NULL;
        free(possibleMoves); possibleMoves = NULL;
        free(legalMoves); legalMoves = NULL;
        free(adjacentLocations); adjacentLocations = NULL;
        disposeMap(g); 

        printf("nextMove: [%s]", nextMove);
        // IF THIS WORKS, Dracula should be able to randomly choose and make any possible legal move
        registerBestPlay(nextMove, "Mwuhahahaha");
    }
}

//--------------------------- Auxillary functions ---------------------------
static void checkAmbiguousMovesAvailability(int *ambiguousMoves, Round currRound)
{
    if (currRound >= 1) { ambiguousMoves[0] = TRUE; ambiguousMoves[5] = TRUE; } 
    if (currRound >= 2) { ambiguousMoves[1] = TRUE; } 
    if (currRound >= 3) { ambiguousMoves[2] = TRUE; } 
    if (currRound >= 4) { ambiguousMoves[3] = TRUE; } 
    if (currRound >= 5) { ambiguousMoves[4] = TRUE; } 
}

static void validateDoubleBackandHide(DracView gameState, int *ambiguousMoves, LocationID currPos)
{
    Map g = newMap(); int numLocs = 0;  
    LocationID *adjacentLocations = reachableLocations(g, &numLocs, currPos, TRUE, FALSE, TRUE, TRUE);
    LocationID dracTrail[TRAIL_SIZE]; giveMeTheTrail(gameState, PLAYER_DRACULA, dracTrail);
    int canDB = TRUE;
    LocationID dracMovesTrail[TRAIL_SIZE]; giveMeTheMoves(gameState, PLAYER_DRACULA, dracMovesTrail);
    int i; int move;
    // apparently the last value in the trail is not counted as a move...
    for (i = 0; i < TRAIL_SIZE-1; i++) {
        move = dracMovesTrail[i]; 
        switch (move) {
            case DOUBLE_BACK_1: ambiguousMoves[0] = FALSE; canDB = FALSE; break;
            case DOUBLE_BACK_2: ambiguousMoves[1] = FALSE; canDB = FALSE; break;
            case DOUBLE_BACK_3: ambiguousMoves[2] = FALSE; canDB = FALSE; break;
            case DOUBLE_BACK_4: ambiguousMoves[3] = FALSE; canDB = FALSE; break;
            case DOUBLE_BACK_5: ambiguousMoves[4] = FALSE; canDB = FALSE; break;
            case HIDE: ambiguousMoves[5] = FALSE; break;
        } 
    }

    // we can't double back to a location that is NOT adjacent to where we are 
    int ambiguousCounter; int adjCounter; LocationID adjLoc;
    for (ambiguousCounter = 1; ambiguousCounter < 5; ambiguousCounter++) {
        if (ambiguousMoves[ambiguousCounter] == TRUE) {
            for (adjCounter = 0; adjCounter < numLocs; adjCounter++) {
                adjLoc = adjacentLocations[adjCounter];
                if (dracTrail[ambiguousCounter] != adjLoc) { ambiguousMoves[ambiguousCounter] = FALSE; }
                else if (dracTrail[ambiguousCounter] == adjLoc) { ambiguousMoves[ambiguousCounter] = TRUE; break; }
            }
        }
    }

    // if one double back move is in the trail, 
    // then all double backs move 1 to 5 are canceled
    if (canDB == FALSE) { 
        for (i = 0; i < 5; i++) {
            ambiguousMoves[i] = FALSE;
        }
    }
    // Drac cannot hide at Sea
    if (isSea(dracTrail[0])) { ambiguousMoves[5] = FALSE; }

    disposeMap(g); free(adjacentLocations); adjacentLocations = NULL;
}

static char *ambiguousIdToAbbrev(LocationID move) 
{
    char *nextMove = NULL;
    switch (move) {
        case DOUBLE_BACK_1: nextMove = "D1"; break;
        case DOUBLE_BACK_2: nextMove = "D2"; break;
        case DOUBLE_BACK_3: nextMove = "D3"; break;
        case DOUBLE_BACK_4: nextMove = "D4"; break;
        case DOUBLE_BACK_5: nextMove = "D5"; break;
        case HIDE: nextMove = "HI"; break;
        case TELEPORT: nextMove = "TP"; break;
    }
    return nextMove;
}

static void incrementNumOfLocations(int *ambiguousMoves, int *numAmbiguousMoves)
{
    // NOTE: the * operator takes precedence over the ++ operator, and the () operators take precedence over everything else. 
    if (ambiguousMoves[0] == TRUE) { (*numAmbiguousMoves)++; }
    if (ambiguousMoves[1] == TRUE) { (*numAmbiguousMoves)++; }
    if (ambiguousMoves[2] == TRUE) { (*numAmbiguousMoves)++; }
    if (ambiguousMoves[3] == TRUE) { (*numAmbiguousMoves)++; }
    if (ambiguousMoves[4] == TRUE) { (*numAmbiguousMoves)++; }
    if (ambiguousMoves[5] == TRUE) { (*numAmbiguousMoves)++; }
    if (ambiguousMoves[6] == TRUE) { (*numAmbiguousMoves)++; }
}

static LocationID *createPossibleMovesArray(LocationID *legalMoves, int numLocations, int numAmbiguousMoves,
                                            int *totalNumMoves, int *ambiguousMoves) 
{
    *totalNumMoves = numLocations + numAmbiguousMoves;
    if (*totalNumMoves == 0) { 
        *totalNumMoves = 1;
        LocationID *possibleMoves = malloc(sizeof(LocationID) * (*totalNumMoves));
        possibleMoves[0] = TELEPORT; 
        return possibleMoves; 
    }
    LocationID *possibleMoves = malloc(sizeof(LocationID) * (*totalNumMoves));
    int i = 0; int ambiguousMovesCounter = 0;
    for (i = 0; i < numLocations; i++) {
        LocationID legalMove = legalMoves[i];
        possibleMoves[i] = legalMove;
    }
    for (i = numLocations; i < *totalNumMoves+1; i++) {
        while (ambiguousMoves[ambiguousMovesCounter] != TRUE && ambiguousMovesCounter < TOTAL_AMBIGUOUS_MOVES) {
            ambiguousMovesCounter++;
        }
        if (ambiguousMoves[ambiguousMovesCounter] == TRUE) {
            switch (ambiguousMovesCounter) {
                case 0: possibleMoves[i] = DOUBLE_BACK_1; break;
                case 1: possibleMoves[i] = DOUBLE_BACK_2; break;
                case 2: possibleMoves[i] = DOUBLE_BACK_3; break;
                case 3: possibleMoves[i] = DOUBLE_BACK_4; break;
                case 4: possibleMoves[i] = DOUBLE_BACK_5; break;
                case 5: possibleMoves[i] = HIDE; break;
            } 
            ambiguousMovesCounter++;
        }
    }
    return possibleMoves;
}
