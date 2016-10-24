// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Game.h"
#include "HunterView.h"
#include "Map.h"

#define MAX_ITERATIONS 50

static LocationID getDracRecentLocation(HunterView gameState);
static int isDracAdjacentToMe(LocationID *possibleMoves, int numLocations, LocationID dracPos);
static LocationID getHunterRecentLocation(HunterView gameState, PlayerID hunter);
static void checkSeaAndLandMoveAvailability(LocationID *possibleMoves, int numLocations, int recentHunterMove, int *checkSea, int *landTrue);
static LocationID getDracAccuratePosition(HunterView gameState);
static int isNearDrac(HunterView gameState, LocationID hunterPos);
static int isBestMove(HunterView gameState, PlayerID hunter, LocationID move);

void decideHunterMove(HunterView gameState)
{
    Round currRound = giveMeTheRound(gameState);
    PlayerID currHunter = whoAmI(gameState);
    LocationID currPos = whereIs(gameState, currHunter);
    LocationID hunterLastLocation = getHunterRecentLocation(gameState, currHunter);
    LocationID dracPos = getDracAccuratePosition(gameState);
    LocationID dracLastLocation = getDracRecentLocation(gameState);
    LocationID closestMoveToDrac = NOWHERE;
    int distanceToDest = 0; int numLocations = 0; int rail = FALSE; int sum = currRound + currHunter;
    int isNear = isNearDrac(gameState, currPos);
    srand(time(NULL)); // set a seed
    
    printf("\n");
    printf("hunterPos: %d\n", currPos);
    printf("hunterLastLocation: %d\n", hunterLastLocation);
    printf("dracPos: %d\n", dracPos);
    printf("dracLastLocation: %d\n", dracLastLocation);

    // check if the current player can travel by train this round
    if (sum % 4 > 0) { rail = TRUE; }
    // if we're near drac dont travel too far
    if (isNear == TRUE) { printf("we're near Drac! :D\n"); rail = FALSE; }

    if (currRound == 0) {
        switch (currHunter) {
            case PLAYER_LORD_GODALMING: registerBestPlay("ED", "British Isles"); break;
            case PLAYER_DR_SEWARD: registerBestPlay("CA", "South-Western Spanish border"); break;
            case PLAYER_VAN_HELSING: registerBestPlay("BC", "Eastern Europe"); break;
            case PLAYER_MINA_HARKER: registerBestPlay("BI", "BALI"); break;
        }    
    } else {
        LocationID *possibleMoves = whereCanIgo(gameState, &numLocations, TRUE, rail, TRUE);

        if (!validPlace(hunterLastLocation)) { hunterLastLocation = currPos; } 
        if (validPlace(dracLastLocation)) { 
            printf("finding closest path to target (dracLastLocation)\n");
            closestMoveToDrac = findLocationClosestToTarget(currPos, dracLastLocation, rail, &distanceToDest, possibleMoves, numLocations);
        } 
        if (validPlace(dracPos)) {
            printf("finding closest path to target (dracPos)\n");
            closestMoveToDrac = findLocationClosestToTarget(currPos, dracPos, rail, &distanceToDest, possibleMoves, numLocations);
        }

        int isSeaMoveAvailable = FALSE; int isLandMoveAvailable = FALSE;
        checkSeaAndLandMoveAvailability(possibleMoves, numLocations, hunterLastLocation, &isSeaMoveAvailable, &isLandMoveAvailable);

        printf("possibleMoves: ");
        int i = 0;
        for (i = 0; i < numLocations; i++) {
            printf("{%d}", possibleMoves[i]);
            printf("{%s} ", idToAbbrev(possibleMoves[i]));
        }
        printf("\n");

        LocationID chosenMove;

        if (currHunter == PLAYER_LORD_GODALMING || currHunter == PLAYER_DR_SEWARD ||
            currHunter == PLAYER_MINA_HARKER || currHunter == PLAYER_VAN_HELSING) {
            // check if Dracula is adjacent to where the current hunter is
            if (isDracAdjacentToMe(possibleMoves, numLocations, dracPos) == TRUE && validPlace(dracPos)) {
                printf("TANGO SPOTTED\n");
                chosenMove = dracPos;
            // else check if current hunter is hugging Dracula or if drac is hiding/staying still
            } else if (dracPos == currPos) { 
                printf("Staying still for the kill\n");
                chosenMove = currPos; 
            // move towards drac's last known location
            } else if (validPlace(closestMoveToDrac) && currPos != dracLastLocation) {
                printf("Closing in\n");
                chosenMove = closestMoveToDrac;
            } else {
                printf("random\n");
                chosenMove = hunterLastLocation;
                // the lines below stops hunter from moving back to their last location
                // thus preventing indecisive behaviour
                int chosenIndex = hunterLastLocation;
                while (chosenMove == hunterLastLocation) {
                    chosenIndex = rand() % numLocations;
                    chosenMove = possibleMoves[chosenIndex]; 
                    // if the chosen move is equivalent to one of their last 6 recent locations
                    // re-roll again until we get something different
                    // NOTE: we want hunter to not get stuck cirlcing around 
                    int noPossibleMovesCounter = 0;
                    while (isBestMove(gameState, currHunter, chosenMove) == FALSE && noPossibleMovesCounter < MAX_ITERATIONS) {
                        chosenIndex = rand() % numLocations;
                        chosenMove = possibleMoves[chosenIndex]; 
                        noPossibleMovesCounter++;
                    }
                    // if drac is at sea but we about to make a land move
                    // and check if there's a sea move available because we want to survey the sea
                    // since there's more of a chance that drac might be there
                    if (dracPos == SEA_UNKNOWN && isLand(chosenMove) && isSeaMoveAvailable == TRUE) {
                        chosenIndex = rand() % numLocations;
                        chosenMove = possibleMoves[chosenIndex]; 
                    }
                    // if drac is at land but we about to make a sea move... 
                    // etc similar to b4 but vice-versa
                    if (dracPos == CITY_UNKNOWN && isSea(chosenMove) && isLandMoveAvailable == TRUE) {
                        chosenIndex = rand() % numLocations;
                        chosenMove = possibleMoves[chosenIndex]; 
                    }
                }
            }
        }
        printf("chosenMove = [%d][%s]\n", chosenMove, idToAbbrev(chosenMove));
        printf("numLocations = [%d]", numLocations);
        registerBestPlay(idToAbbrev(chosenMove), "");
        free(possibleMoves);
    }
}

//----------------------------------- Auxillary functions -----------------------------------

static LocationID getDracRecentLocation(HunterView gameState) 
{
    LocationID dracTrail[TRAIL_SIZE]; giveMeTheTrail(gameState, PLAYER_DRACULA, dracTrail);
    int i; 
    for (i = 0; i < TRAIL_SIZE; i++) {
        if (dracTrail[i] != CITY_UNKNOWN && dracTrail[i] != SEA_UNKNOWN && dracTrail[i] != NOWHERE) {
            LocationID recentLoc = dracTrail[i]; 
            if (recentLoc == TELEPORT) { recentLoc = CASTLE_DRACULA; }
            return recentLoc;
        }
    }
    return NOWHERE;
}

static LocationID getDracAccuratePosition(HunterView gameState) 
{
    LocationID dracTrail[TRAIL_SIZE]; giveMeTheTrail(gameState, PLAYER_DRACULA, dracTrail);
    LocationID dPos = whereIs(gameState, PLAYER_DRACULA);
    switch (dPos) {
        case HIDE: dPos = dracTrail[1]; break;
        case DOUBLE_BACK_1: dPos = dracTrail[1]; break;
        case DOUBLE_BACK_2: dPos = dracTrail[2]; break;
        case DOUBLE_BACK_3: dPos = dracTrail[3]; break;
        case DOUBLE_BACK_4: dPos = dracTrail[4]; break;
        case DOUBLE_BACK_5: dPos = dracTrail[5]; break;
        case TELEPORT: dPos = CASTLE_DRACULA; break;
    }
    return dPos;
}

static int isDracAdjacentToMe(LocationID *possibleMoves, int numLocations, LocationID dracPos) 
{
    int i = 0;
    for (i = 0; i < numLocations; i++) {
        if (possibleMoves[i] == dracPos) {
            return TRUE; 
        }    
    }
    return FALSE;  
}

static LocationID getHunterRecentLocation(HunterView gameState, PlayerID hunter) 
{
    LocationID hunterTrail[TRAIL_SIZE]; giveMeTheTrail(gameState, hunter, hunterTrail);
    return hunterTrail[1];
}

static void checkSeaAndLandMoveAvailability(LocationID *possibleMoves, int numLocations, int recentHunterMove, int *checkSea, int *checkLand)
{
    int i = 0; LocationID move;
    for (i = 0; i < numLocations; i++) {
        move = possibleMoves[i];
        if (isSea(possibleMoves[i]) && move != recentHunterMove) { *checkSea = TRUE; }    
        if (isLand(possibleMoves[i]) && move != recentHunterMove) { *checkLand = TRUE; }    
    }
}

static int isNearDrac(HunterView gameState, LocationID hunterPos)
{
    LocationID dracTrail[TRAIL_SIZE]; giveMeTheTrail(gameState, PLAYER_DRACULA, dracTrail);
    int amIClose = FALSE;
    if (hunterPos == dracTrail[0] || hunterPos == dracTrail[1]){
        amIClose = TRUE;
    }
    return amIClose;
}

// prevents hunters from going back to their last 6 recent locations, 
// this is because we don't want hunter circling around the map...
static int isBestMove(HunterView gameState, PlayerID hunter, LocationID move)
{
    LocationID hunterTrail[TRAIL_SIZE]; giveMeTheTrail(gameState, hunter, hunterTrail);
    int i;
    for (i = 0; i < TRAIL_SIZE; i++) {
        if (move == hunterTrail[i]) { return FALSE; }
    } 
    return TRUE;
}
