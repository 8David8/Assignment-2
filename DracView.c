// DracView.c ... DracView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "DracView.h"
<<<<<<< HEAD

#define DEATH 0
#define FIRST_ROUND 0
#define NO_RAIL 0

// each location potentially has 3 traps and an immature vamp :D
#define TRAP_AND_VAMP 2  
#define TRAP_INDEX 0
#define VAMP_INDEX 1
#define NUM_CHAR_ENCOUNTER_HUNTER 4
#define NUM_CHAR_PER_PLAY 7
#define NUM_CHAR_NEW_LOCATION 2
#define PAST_PLAYS_DELIMITER ' '
#define NUM_CHAR_PLAYER 1

struct dracView {
    GameView gState; //takes all the repetitive round, score, playerinfo etc. from GameView
    int trapsAndVamp[NUM_MAP_LOCATIONS][TRAP_AND_VAMP];
};
     
static PlayerID convertNameAbbrevToID(char *abbrev);

// Creates a new DracView to summarise the current state of the game
DracView newDracView(char *pastPlays, PlayerMessage messages[])
{
    assert(pastPlays != NULL); assert(messages != NULL);

    DracView dView = malloc(sizeof(struct dracView));
    dView->gState = newGameView (pastPlays, messages); //takes everything from newGameView

    int index;
    for (index = 0; pastPlays[index] != '\0'; index += NUM_CHAR_PER_PLAY) {
        if (pastPlays[index] == PAST_PLAYS_DELIMITER) { index++; }

        char playerNameAbbrev[NUM_CHAR_PLAYER+1];
        playerNameAbbrev[0] = pastPlays[index]; playerNameAbbrev[1] = '\0';

        char newLocation[NUM_CHAR_NEW_LOCATION+1];
        newLocation[0] = pastPlays[index+1]; newLocation[1] = pastPlays[index+2];
        newLocation[2] = '\0';
        
        PlayerID currCharacter = convertNameAbbrevToID(playerNameAbbrev);
        LocationID updatedLocation = abbrevToID(newLocation);
      
        // if the current turn belongs to Dracula
        if (currCharacter == PLAYER_DRACULA) {
            // Dracula placed a trap
            if (pastPlays[index+3] == 'T') {
                dView->trapsAndVamp[updatedLocation][TRAP_INDEX]++;
            } 
            // Dracula placed a vampire
            if (pastPlays[index+4] == 'V') {
                dView->trapsAndVamp[updatedLocation][VAMP_INDEX]++;
            }
            // Trap expired
            if (pastPlays[index+5] == 'M') {
                // we have to get the location of Dracula 6 moves ago...
                // to do this we grab his trail, find his location 6 moves ago
                // and remove the trap at that location
                LocationID popTrapLocation[TRAIL_SIZE];
                giveMeTheTrail(dView, PLAYER_DRACULA, popTrapLocation);
                dView->trapsAndVamp[popTrapLocation[TRAIL_SIZE-1]][TRAP_INDEX]--;
            // Vampire Matures and WREAKS HAVOK D:
            } else if (pastPlays[index+5] == 'V') {
                // similarly, we grab the Dracula's location 6 moves ago
                // and remove the (once) immature vampire of the city
                // because he probably left to cause destruction in other places?... idk
                LocationID popTrapLocation[TRAIL_SIZE];
                giveMeTheTrail(dView, PLAYER_DRACULA, popTrapLocation);
                dView->trapsAndVamp[popTrapLocation[TRAIL_SIZE-1]][VAMP_INDEX]--;
            }
        } else {
            int encounterIndex;
            for (encounterIndex = 0; encounterIndex < NUM_CHAR_ENCOUNTER_HUNTER; encounterIndex++) {
               if (pastPlays[encounterIndex+2] == 'T') {
                   dView->trapsAndVamp[updatedLocation][TRAP_INDEX]--;
               } else if (pastPlays[encounterIndex+2] == 'V') {
                   dView->trapsAndVamp[updatedLocation][VAMP_INDEX]--;
               }
            }            
        }
    }
   return dView;
}
     
static PlayerID convertNameAbbrevToID(char *abbrev)
{
    PlayerID currPlayer; int firstIndex = 0;
    switch (abbrev[firstIndex]) {
        case 'G': currPlayer = PLAYER_LORD_GODALMING; break;
        case 'S': currPlayer = PLAYER_DR_SEWARD; break;
        case 'H': currPlayer = PLAYER_VAN_HELSING; break;
        case 'M': currPlayer = PLAYER_MINA_HARKER; break;
        case 'D': currPlayer = PLAYER_DRACULA; break;
        default: exit(1);  // not sure if exit(1) works
    }
    return currPlayer;
}
     
// Frees all memory previously allocated for the DracView toBeDeleted
void disposeDracView(DracView toBeDeleted)
{
    free(toBeDeleted->gState); free(toBeDeleted);
    toBeDeleted = NULL;
}

//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(DracView currentView)
{ 
    return getRound(currentView->gState); //retrieves current round from GameView
}

// Get the current score
int giveMeTheScore(DracView currentView)
{
    return getScore(currentView->gState); //retrieves current score from GameView
}

// Get the current health points for a given player
int howHealthyIs(DracView currentView, PlayerID player)
{
    return getHealth(currentView->gState, player); //retrieves given player's health from GameView
}

// Get the current location id of a given player
LocationID whereIs(DracView currentView, PlayerID player)
{
    int trueLocation = getLocation(currentView->gState, player); //retrieves given player's location from GameView
   
    LocationID dracTrail[TRAIL_SIZE]; 
    giveMeTheTrail(currentView, player, dracTrail);

    switch (trueLocation) {
        case TELEPORT: trueLocation = CASTLE_DRACULA; break;
        case DOUBLE_BACK_1: trueLocation = dracTrail[1]; break;
        case DOUBLE_BACK_2: trueLocation = dracTrail[2]; break;
        case DOUBLE_BACK_3: trueLocation = dracTrail[3]; break;
        case DOUBLE_BACK_4: trueLocation = dracTrail[4]; break;
        case DOUBLE_BACK_5: trueLocation = dracTrail[5]; break;
        default: ;//Do nothing;
    }
    return trueLocation;
}

// Get the most recent move of a given player
void lastMove(DracView currentView, PlayerID player,
                 LocationID *start, LocationID *end)
{
    //get the most recent location
    LocationID mostRecentLocation = getLocation(currentView->gState, player);
    *end = mostRecentLocation;

    LocationID trail[TRAIL_SIZE];

    giveMeTheTrail(currentView, player, trail);

    //get the location before the most recent location
    LocationID mostRecentLocationBeforeMove = trail[1];
    *start = mostRecentLocationBeforeMove;

/*
    LocationID playerTrail[TRAIL_SIZE];
    giveMeTheTrail(currentView, player, playerTrail);

    int currentRound = giveMeTheRound(currentView); //retrieves the current round from the round function
    
    if (currentRound == FIRST_ROUND) { //Case 1 - round 0
        if (playerTrail[0] == UNKNOWN_LOCATION) { //the player may not have had a turn yet, so their trail is empty i.e. [-1,-1,-1,-1,-1,-1]
            *start = UNKNOWN_LOCATION;
            *end = UNKNOWN_LOCATION;   
        } else { //or else the player has had a turn, so trail would have 1 location i.e. [??,-1,-1,-1,-1,-1]
            *start = playerTrail[0];
            *end = UNKNOWN_LOCATION;
        }

    } else if (currentRound < 5) { //Case 2 - round 1-5 
        *start = playerTrail[currentRound-1]; //round 5 every player's trail is filled. round 1-4, trail has something, but is not filled
        *end = playerTrail[currentRound];     //therefore, we use the currentRound number to do this

    } else { //Case 3 - round 6 or more
        *start = playerTrail[4]; 
        *end = playerTrail[5];
        if (player == PLAYER_DRACULA) {
            *start = playerTrail[3];
            *end = playerTrail[4];
        }
    }
    */
}

// Find out what minions are placed at the specified location
void whatsThere(DracView currentView, LocationID where,
                         int *numTraps, int *numVamps)
{
    if (where == SEA || where == UNKNOWN_LOCATION) {
        *numTraps = 0;
        *numVamps = 0;
    } else {
        *numTraps = currentView->trapsAndVamp[where][TRAP_INDEX];
        *numVamps = currentView->trapsAndVamp[where][VAMP_INDEX];
    }
/*
 Helen's work
    int i = 0;
    while (i < strlen(currentView->pastPlays)) {
        if ((i % 40) > 31) {
            if ((i % 40) % 8 == 3) {
                if (currentView->pastPlays[i] == 'T') {
                    currentView->traps[whereIs(currentView, (i % 40)/8)]--;
                } else if (currentView->pastPlays[i+1] == 'V') {
                    currentView->vamps[whereIs(currentView, (i % 40) / 8)]--;
                }
            }
        } else {
            if ((i % 40) % 8 == 3) {
                if (currentView->pastPlays[i] == 'T') {
                    currentView->traps[whereIs(currentView, (i % 40)/8)]++;
                } else if (currentView->pastPlays[i+1] == 'V') {
                    currentView->vamps[whereIs(currentView, (i % 40) / 8)]++;
                }
            } else if ((i % 40)% 8 == 6) {
                if (currentView->pastPlays[i] == 'M') { //trap left trail
                    currentView->traps[abbrevToID(&currentView->pastPlays[i - 244])]-=1;
                } else if (currentView->pastPlays[i]=='V') { //mature vampire
                    currentView->vamps[abbrevToID(&currentView->pastPlays[i - 244])]-=1;
                }
            }
        }
        i++;
    
    *numTraps = currentView->traps[where];
    *numVamps = currentView->vamps[where];
    */
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(DracView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    getHistory(currentView->gState, player, trail);
}

//// Functions that query the map to find information about connectivity

// What are my (Dracula's) possible next moves (locations)
LocationID *whereCanIgo(DracView currentView, int *numLocations, int road, int sea)
{
    LocationID draculaLocation = whereIs(currentView, PLAYER_DRACULA);
    int currentRound = giveMeTheRound(currentView); //retrieves the current round from the round function

/* Helen's work
    if (currentRound == 0) { //Case 1 - Round 0, Dracula is always last to move and he cannot move to ST_JOSEPH_AND_ST_MARYS
        LocationID *place = malloc(sizeof(LocationID)*NUM_MAP_LOCATIONS);
        int i;

        while (i < NUM_MAP_LOCATIONS) {
            if (i != ST_JOSEPH_AND_ST_MARYS) {
                place[*numLocations++] = i;
                
            } 
            i++;
        }
*/
    return connectedLocations(currentView->gState, numLocations,
                              draculaLocation, PLAYER_DRACULA,
                              currentRound, road, NO_RAIL, sea);
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(DracView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea)
{
    LocationID hunterLocation = whereIs(currentView, player);
    int currentRound = giveMeTheRound(currentView); //retrieves the current round from the round function

    /*
    if (currentRound == 0) { //Case 1 - Round 0, where player has not made a move, therefore can move anywhere to start
        LocationID *place = malloc(sizeof(LocationID)*NUM_MAP_LOCATIONS);
        int i;

        while (i < NUM_MAP_LOCATIONS) {
            place[*numLocations++] = i;
            i++;
        }
        
        return place;
    } 
*/
    return connectedLocations(currentView->gState, numLocations,
                              hunterLocation, player, 
                              currentRound, road, rail, sea);
}
