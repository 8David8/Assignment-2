// GameView.c ... GameView ADT implementation

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "Map.h"

#define NUM_HUNTERS 4
#define DEATH 0
#define isCastle(place) (place == CASTLE_DRACULA)
// used to determine what the double back type is
#define DOUBLE_BACK_START (DOUBLE_BACK_1-1)

// below defines the number of characters that represents various data
// about a particular player and his/her move and actions
// mainly used for the pastPlay string
// IMPORTANT: read the data page for more information about Past Plays
#define NUM_CHAR_PER_PLAY 7
#define NUM_CHAR_PLAYER 1
#define NUM_CHAR_NEW_LOCATION 2
#define NUM_CHAR_ENCOUNTER_HUNTER 4
#define NUM_CHAR_ENCOUNTER_DRACULA 2
#define NUM_CHAR_ACTION_DRACULA 1
#define STARTING_INDEX_FOR_ENCOUNTERS 3
#define PAST_PLAYS_DELIMITER ' '

typedef struct _player {
    // trail represents a character's last 6 moves
    // which translate to a character's last 6 recent location
    // moves = locationID confirmed in the rules page
    // all Hunters and Dracula himself have their own trails
    // structure of trail array: [Most recent location][n][n][n][n][location 6 moves ago]
    LocationID trail[TRAIL_SIZE]; //TRAIL_SIZE is defined in Game.h
    int health;
    // LocationID is typedef'd as an int in the Places.h file
    // which is linked via the Game.h file
    LocationID location; //location of the hunters
} player;

struct gameView {
    player playerStats[NUM_PLAYERS]; // NUM_PLAYERS is defined in Globals.h
    int score;
    int turns;   // Can be treated as current turn or number of turn/s occurred
    int rounds;  // Can be treated as current round or number of round/s occurred
};

// ----------------------------------------------
// --- function prototypes that we created :D ---
// ----------------------------------------------

static PlayerID convertNameAbbrevToID(char *abbrev);
static void pushLocationToTrail(GameView currentView, PlayerID player, LocationID location);
static void setupGameState(GameView gView, char *pastPlays);

// ----------------------------------------------

// IMPORTANT: newGameView gets called once and only once
// to elaborate it gets called once in HunterView.c and DracView.c
// GameView is a pointer to the gameView struct, typedef'd in GameView.h
// Creates a new GameView to summarise the current state of the game
GameView newGameView(char *pastPlays, PlayerMessage messages[])
{
    assert(pastPlays != NULL); assert(messages != NULL);  // Check parameters are valid

    GameView gView = malloc(sizeof(struct gameView));
    assert(gView != NULL); // Check if the new gameView is initialised

    setupGameState(gView, pastPlays);

    // pastPlays is a string containing a history of moves and actions
    // it represents everything that has happened so far in the game
    // Each play/turn is represented by 7 characters seperated by a space(another extra character)
    // the seven characters indicate what has happened during the player's turn
    // so to loop through each play/turn we increment index by 7
    // and check if it reaches a space, if it does increment 
    int index;
    for (index = 0; pastPlays[index] != '\0'; index += NUM_CHAR_PER_PLAY) {
     
        // the if statement below checks if it the index reaches a space
        // after successfully loop through the plays
        // if so then we know it hasn't reached the end of the string
        // if it doesn't reach a space, then don't increment as that will 
        // skip the NULL terminator which is not what we want
        if (pastPlays[index] == PAST_PLAYS_DELIMITER) { index++; }

        // get the name abbrev for the current play and store it in a seperate array
        char playerNameAbbrev[NUM_CHAR_PLAYER+1];
        playerNameAbbrev[0] = pastPlays[index]; playerNameAbbrev[1] = '\0';

        // get the location abbrev for the current play and store it in a seperate array
        char newLocation[NUM_CHAR_NEW_LOCATION+1];
        newLocation[0] = pastPlays[index+1];
        newLocation[1] = pastPlays[index+2];
        newLocation[2] = '\0';

        PlayerID currCharacter = convertNameAbbrevToID(playerNameAbbrev);
        // abbrevToID is a function within the Places.c file
        LocationID updatedLocation = abbrevToID(newLocation);

        // the current moves (the old data, aka the moves that was previously stored)
        // must be processed before the new data/moves can be updated
        // current character is Dracula
        if (currCharacter == PLAYER_DRACULA) {
            int atSea; int atCastle;
            
            // if updated location is equivalent to NOWHERE
            // then we know for certain that the pastPlays string was given to a hunter
            // we therefore do not know where dracula is...
            // Note: the function abbrevToID returns the value NOWHERE
            // if it cannot find a valid location abbreviation
            if (updatedLocation == NOWHERE) {
                // Dracula is in a city but we dont know where precisely
                if (newLocation[0] == 'C') {
                     atSea = FALSE;
                     atCastle = FALSE;
                     pushLocationToTrail(gView, PLAYER_DRACULA, CITY_UNKNOWN);
                     updatedLocation = CITY_UNKNOWN;
                // Dracula is at sea but we dont know which sea
                } else if (newLocation[0] == 'S') {
                     atSea = TRUE;
                     atCastle = FALSE;
                     pushLocationToTrail(gView, PLAYER_DRACULA, SEA_UNKNOWN);
                     updatedLocation = SEA_UNKNOWN;

                // Dracula is hiding
                // push the most recent location onto the trail
                // because hiding simply means the dracula is staying in the same place
                // as he was in his previous move
                } else if (newLocation[0] == 'H') {
                     pushLocationToTrail(gView, PLAYER_DRACULA, gView->playerStats[PLAYER_DRACULA].trail[TRAIL_SIZE-1]);
                     updatedLocation = HIDE;

                // Dracula doubled back
                // get the number of moves that Dracula has backtracked
                } else if (newLocation[0] == 'D') {
                     int numMovesBackTrack = newLocation[1] - '0';
                     LocationID backTrackDest = gView->playerStats[PLAYER_DRACULA].trail[TRAIL_SIZE+numMovesBackTrack];
                     int doubleBackType = DOUBLE_BACK_START + numMovesBackTrack;

                     // now we re-check if Dracula backtracked
                     // to a sea or castle location
                     if (backTrackDest == SEA_UNKNOWN) { atSea = TRUE; atCastle = FALSE;
                     } else if (backTrackDest == CITY_UNKNOWN) { atSea = FALSE; atCastle = FALSE;
                     // we also have to account for the -1 value
                     // which is UNKNOWN LOCATION
                     } else if (backTrackDest == UNKNOWN_LOCATION) { atSea = FALSE; atCastle = FALSE;
                     // else we know its an exact location between 0 < x < 70
                     // and therefore we can use idToType to validate places
                     } else {
                         if (isSea(backTrackDest)) { atSea = TRUE; atCastle = FALSE;
                         } else if (isCastle(backTrackDest)) { atSea = FALSE; atCastle = TRUE; }
                     }

                     pushLocationToTrail(gView, PLAYER_DRACULA, doubleBackType);
                     updatedLocation = doubleBackType;

                // Dracula teleports to Castle
                } else if (newLocation[0] == 'T') {
                     atSea = FALSE;
                     atCastle = TRUE;
                     pushLocationToTrail(gView, PLAYER_DRACULA, CASTLE_DRACULA);
                     updatedLocation = TELEPORT;
                }
            // else we precisely know where Dracula is
            // and therefore infer that the pastPlays string was passed to Dracula
            } else {
                if (isSea(updatedLocation)) { atSea = TRUE; }
                else { atSea = FALSE; }
                if (isCastle(updatedLocation)) { atCastle = TRUE; }
                else { atCastle = FALSE; }
                pushLocationToTrail(gView, PLAYER_DRACULA, updatedLocation);
            }

            gView->playerStats[PLAYER_DRACULA].location = updatedLocation;

            // Dracula despises the sea, so whenever he is located on sea
            // we reduce his blood point by 2
            if (atSea) {
                gView->playerStats[PLAYER_DRACULA].health -= LIFE_LOSS_SEA;
            // Dracula loves his castle, so whenever he is in his castle
            // he gains 10 blood points
            } else if (atCastle) {
                gView->playerStats[PLAYER_DRACULA].health += LIFE_GAIN_CASTLE_DRACULA;
            }

            if (pastPlays[index+5] == 'M') {
                //?????
                printf("hi\n");
            // a vampire has just matured
            // and therefore the score is reduced by 13
            } else if (pastPlays[index+5] == 'V') {
                gView->score -= SCORE_LOSS_VAMPIRE_MATURES;
            }

        // current character is a hunter
        } else {
            PlayerID currHunter = currCharacter;

            // if hunter was at the hospital and he was dead, revive him and restore him to full health
            if (getLocation(gView, currHunter) == ST_JOSEPH_AND_ST_MARYS &&
                getHealth(gView, currHunter) == DEATH) {
                gView->playerStats[currHunter].health = GAME_START_HUNTER_LIFE_POINTS;
            }

            // check if there were any encounters and let the game play accordingly
            // Note: encounters should only take place if the hunter or dracula is not dead
            int encounterIndex;
            for (encounterIndex = STARTING_INDEX_FOR_ENCOUNTERS; encounterIndex < NUM_CHAR_PER_PLAY &&
                gView->playerStats[currHunter].health > DEATH &&
                gView->playerStats[PLAYER_DRACULA].health > DEATH; encounterIndex++) {
                if (pastPlays[index+encounterIndex] == 'D') {
                    gView->playerStats[currHunter].health -= LIFE_LOSS_DRACULA_ENCOUNTER;
                    gView->playerStats[PLAYER_DRACULA].health -= LIFE_LOSS_HUNTER_ENCOUNTER;
                }
            }

            // check if the hunter died, and if he/she did die,
            // - set their health to zero (this is for bookeeping purposes)
            //   (we only want death to be associated with the value zero, nothing more nothing less)
            // - reduce the overall score by 6 (SCORE_LOSS_HUNTER_HOSPITAL)
            // - teleport him/her to hospital
            if (getHealth(gView, currHunter) <= DEATH) {
                gView->playerStats[currHunter].health = DEATH;
                gView->score -= SCORE_LOSS_HUNTER_HOSPITAL;
                updatedLocation = ST_JOSEPH_AND_ST_MARYS;
            }

            // if the hunter is in the same location as he was in the previous turn
            // we assume that he has rested and therefore he gains 3 life points
            if (updatedLocation == getLocation(gView, currHunter)) {
                gView->playerStats[currHunter].health += LIFE_GAIN_REST;

                // Hunter's health cannot exceed the default health it started off with (9)
                // if it does exceed we set it back to its appropriate value (9)
                if (getHealth(gView, currHunter) > GAME_START_HUNTER_LIFE_POINTS) {
                    gView->playerStats[currHunter].health = GAME_START_HUNTER_LIFE_POINTS;
                }
            }
            pushLocationToTrail(gView, currHunter, updatedLocation);
            gView->playerStats[currHunter].location = updatedLocation;
        }

    }
    return gView;
}

// Frees all memory previously allocated for the GameView toBeDeleted
void disposeGameView(GameView toBeDeleted)
{
    //COMPLETE THIS IMPLEMENTATION
    free(toBeDeleted);
    toBeDeleted = NULL;
}

// ---------------------------------------------------------------------------
// --- all functions that we created and hence was not in the default file ---
// ---------------------------------------------------------------------------

// Given a character's name abbreviation     {G, S, H, M, D}
// convert it to the corresponding PlayerID  {0, 1, 2, 3, 4}
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

// push the next(most recent) location onto the trail
static void pushLocationToTrail(GameView gView, PlayerID player, LocationID location)
{
    int trailIndex;
    for (trailIndex = TRAIL_SIZE-1; trailIndex > 0; trailIndex--) {
        gView->playerStats[player].trail[trailIndex] = gView->playerStats[player].trail[trailIndex-1];
    }
    gView->playerStats[player].trail[0] = location;
}

// setup the initial game state
// declare and initialise the score, turns, rounds to the appropriate value
static void setupGameState(GameView gView, char *pastPlays) 
{
    gView->score = GAME_START_SCORE;
    // each play/turn in the pastPlays string is represented by 7 characters
    // seperated by a space(one extra character), so technically one play/turn is 8 chars long
    // the last move however does not have a space at the end so we just add one to it
    // we the divide by 8 which gives us the number of total number of turns that has passed
    gView->turns = (strlen(pastPlays)+1)/8;
    gView->rounds = gView->turns/NUM_PLAYERS;

    // initialise the trails for all players :D
    int playerCounter;
    int trailIndex;
    for (playerCounter = 0; playerCounter < NUM_PLAYERS; playerCounter++) {
        for (trailIndex = 0; trailIndex < TRAIL_SIZE; trailIndex++) {
            gView->playerStats[playerCounter].trail[trailIndex] = NOWHERE;
        }
    }
    // initialise all the hunter player's stats
    int hunterCount;
    for (hunterCount = 0; hunterCount < NUM_HUNTERS; hunterCount++) {
        gView->playerStats[hunterCount].health = GAME_START_HUNTER_LIFE_POINTS; // sets all player health to default
        // we set the location to nowhere when newGameView is called initially
        // this however will be updated if pastPlays contains previous history about players' moves and actions
        gView->playerStats[hunterCount].location = NOWHERE;
    }

    // initialise Dracula's stats
    gView->playerStats[PLAYER_DRACULA].health = GAME_START_BLOOD_POINTS; // sets dracs blood points in the beginning
    gView->playerStats[PLAYER_DRACULA].location = NOWHERE;
}

// ----------------------------------------------------------------------------------
// --- Functions to return simple information about the current state of the game ---
// ----------------------------------------------------------------------------------

// Get the current round
Round getRound(GameView currentView)
{
    return currentView->rounds;
}

// Get the id of current player - ie whose turn is it?
PlayerID getCurrentPlayer(GameView currentView)
{
    return currentView->turns % NUM_PLAYERS;
}

// Get the current score
int getScore(GameView currentView)
{
    return currentView->score;
}

// Get the current health points for a given player
int getHealth(GameView currentView, PlayerID player)
{
    return currentView->playerStats[player].health;
}

// Get the current location id of a given player
LocationID getLocation(GameView currentView, PlayerID player)
{
    return currentView->playerStats[player].location;
}

// -----------------------------------------------------------------------
// --- Functions that return information about the history of the game ---
// -----------------------------------------------------------------------

// Fills the trail array with the location ids of the last 6 turns
void getHistory(GameView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    //check if input is valid
    assert(currentView != NULL);
    assert(player >= 0 && player <= NUM_PLAYERS);
    assert(trail != NULL);

    int i;
    for (i = 0; i < TRAIL_SIZE; i++)
        trail[i] = currentView->playerStats[player].trail[i];
}

// ---------------------------------------------------------------------------
// --- Functions that query the map to find information about connectivity ---
// ---------------------------------------------------------------------------

// Returns an array of LocationIDs for all directly connected locations
LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea)
{
    //Check if all inputs are valid
    assert(currentView != NULL); assert(numLocations != NULL);
    assert(player >= 0 && player < NUM_PLAYERS);
    assert(validPlace(from)); //function from places.h
   
    LocationID *reachable = malloc(sizeof(int) * NUM_MAP_LOCATIONS);
    reachable = getConnections(from, player, round, road, rail, sea); 

    int i;
    for (i = 0; i < NUM_MAP_LOCATIONS; i++){
        if(reachable[i] == TRUE) (*numLocations)++;     
        //printf("%d: %d\n",i ,reachable[i]);
    }

    return reachable; 
}

/*
// Returns an array of LocationIDs for all directly connected locations
LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea)
{
    //Check if all inputs are valid
    assert(currentView != NULL);
    assert(numLocations != NULL);
    assert(player >= 0 && player < NUM_PLAYERS);
    assert(validPlace(from)); //function from places.h

    //initialise an array to return and hold the possible locations
    LocationID *reachable = malloc(sizeof(int) * NUM_MAP_LOCATIONS);
    int j;
    for (j = 0; j < NUM_MAP_LOCATIONS; j++)
        reachable[j] = 0;
    reachable[from] = TRUE;

    Map g = newMap();

    int i = (round + player)%4; //sum of roudns + player mod 4
    if (i == 0 || player == PLAYER_DRACULA){
        rail = FALSE;
    }
    VList curr = g->connections[from];
    while (curr != NULL){
        if (rail == TRUE && curr->type == RAIL){
            VList second = g->connections[curr->v];
            switch(i){
                case 1: reachable[curr->v] = 1; break;
                default:
                    //Find the cities located 2 links away if i = 2
                    while(second != NULL){
                        if (second->type == RAIL)
                            reachable[second->v] = 2;
                    }
                    //Find cities located 3 links away if i = 3
                    if (i == 3){
                        second = g->connections[curr->v];
                        VList third = g->connections[second->v];
                        while(third != NULL){
                            if (third->type == RAIL)
                                reachable[third->v] = 3;
                        }
                    }
                    break;
                }
        }
        if (sea == TRUE && curr->type == BOAT)
            reachable[curr->v] = TRUE;
        if (road == TRUE && curr->type == ROAD)
            reachable[curr->v] = TRUE;
        curr = curr->next;
        }
    }
    return 0;
    //return reachable;
}
*/
