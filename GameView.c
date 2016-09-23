// GameView.c ... GameView ADT implementation

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"

#define NUM_HUNTERS 4
#define DEATH 0

// below defines the number of characters that represents various data
// about a particular player and his/her move and actions
// mainly used for the pastPlay string 
// read the data page for more information about Past Plays
#define NUM_CHAR_PER_PLAY 7
#define NUM_CHAR_PLAYER 1
#define NUM_CHAR_NEW_LOCATION 2
#define NUM_CHAR_ENCOUNTER_HUNTER 4
#define NUM_CHAR_ENCOUNTER_DRACULA 2
#define NUM_CHAR_ACTION_DRACULA 1
#define STARTING_INDEX_FOR_ENCOUNTERS 3

// #include "Map.h" ... if you decide to use the Map ADT

typedef struct _player {
    LocationID trail[TRAIL_SIZE]; //TRAIL_SIZE is defined in Game.h
    int health;
    // LocationID is typedef'd as an int in the Places.h file 
    // which is linked via the Game.h file
    LocationID location; //location of the hunters 
} player;

struct gameView {
    player playerStats[NUM_PLAYERS]; // NUM_PLAYERS is defined in Globals.h
    // the trail array stores dracula last 6 moves
    // which translate to dracula's last 6 recent location, 
    // moves = locationID confirmed in the rules page
    int score;
    int turns;   // Can be treated as current turn or number of turn/s occurred 
    int rounds;  // Can be treated as current round or number of round/s occurred 
};

// ----------------------------------------------
// --- function prototypes that we created :D --- 
// ----------------------------------------------

static PlayerID convertPlayerNameAbbrevToID(char *abbrev);

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
   
    gView->score = GAME_START_SCORE;
    // each play/turn in the pastPlays string is represented by 7 characters
    // seperated by a space(one extra character)
    // the last move however does not have a space at the end so we just add one to it
    // we the divide by 8 which gives us the number of total number of turns that has passed
    gView->turns = (strlen(pastPlays)+1)/8; 
    gView->rounds = gView->turns/NUM_PLAYERS;

    // initialise all the hunter player's stats
    int hunterCount;  
    for (hunterCount = 0; hunterCount < NUM_HUNTERS; hunterCount++) {
        gameView->playerStats[hunterCounter].health = GAME_START_HUNTER_LIFE_POINT; // sets all player health to default
        // we set the location to nowhere when newGameView is called initially
        // this however will be updated if pastPlays contains previous history about players' moves and actions
        gameView->playerStats[hunterCounter].location = NOWHERE; 
    }

    // initialise Dracula's stats
    gameView->playerStats[PLAYER_DRACULA].health = GAME_START_BLOOD_POINTS; // sets dracs blood points in the beginning
    gameView->playerStats[PLAYER_DRACULA].location = NOWHERE; 

    // pastPlays is a string containing a history of moves and actions
    // it represents everything that has happened so far in the game
    // Each play/turn is represented by 7 characters seperated by a space(another extra character)
    // the seven characters indicate what has happened during the player's turn
    // so to loop through each play/turn we increment index by 8(NUM_CHAR_PER_PLAY(7)+1)
    int index;
    for (index = 0; pastPlays[index] != '\0'; index += NUM_CHAR_PER_PLAY+1) {
        // get the name abbrev for the current play and store it in a seperate array
        char playerNameAbbrev[NUM_CHAR_PLAYER+1];
        playerNameAbbrev[0] = pastPlays[index]; player[1] = '\0';
        
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
                
            //injure or heal Dracula, it gets performed here  

            d

        //current character is a hunter   
        } else {
            Player currHunter = currCharacter;
          
            // if hunter was at the hospital and he was dead, revive him and restore him to full health
            if (getLocation(gView, currHunter) == ST_JOSEPH_AND_ST_MARYS &&
                getHealth(gView, currHunter) == DEATH) {
                gView->playerStats[currHunter].health = GAME_START_HUNTER_LIFE_POINTS;
            }
            
            // check if there were any encounters and let the game play accordingly
            // Note: encounters should only take place if the hunter or dracula is not dead
            int encounterIndex;
            for (encounterIndex = STARTING_INDEX_FOR_ENCOUNTERS; startingIndex < NUM_CHARS_PER_PLAY &&
                gView->playerStats[currHunter] > DEATH && 
                gView->playerStats[PLAYER_DRACULA] > DEATH; encounterIndex++;) {
                if (pastPlays[index+encounterIndex] == 'D') {
                    gView->playerStats[currHunter] -= LIFE_LOSS_DRACULA_ENCOUNTER; 
                    gView->playerStats[PLAYER_DRACULA] -= LIFE_LOSS_HUNTER_ENCOUNTER;
                } 
            }
            
            // check if the hunter died, and if they did die, 
            // - set their health to zero (this is for bookeeping purposes)
            //   (we only want death to be associated with the value zero, nothing more nothing less)
            // - reduce the overall score by 6 (SCORE_LOSS_HUNTER_HOSPITAL)
            // - transport them to hospital
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

// ----------------------------------------------------------------------------------
// --- Functions to return simple information about the current state of the game ---
// ----------------------------------------------------------------------------------

// Get the current round
Round getRound(GameView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return currentView->rounds;
}

// Get the id of current player - ie whose turn is it?
PlayerID getCurrentPlayer(GameView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return currentView->turns;
}

// Get the current score
int getScore(GameView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return currentView->score;
}

// Get the current health points for a given player
int getHealth(GameView currentView, PlayerID player)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return currentView->playerStats[player].health;
}

// Get the current location id of a given player
LocationID getLocation(GameView currentView, PlayerID player)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return currentView->playerStats[player].location;
}

// -----------------------------------------------------------------------
// --- Functions that return information about the history of the game ---
// -----------------------------------------------------------------------

// Fills the trail array with the location ids of the last 6 turns
void getHistory(GameView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
}

// ---------------------------------------------------------------------------
// --- Functions that query the map to find information about connectivity ---
// ---------------------------------------------------------------------------

// Returns an array of LocationIDs for all directly connected locations
LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return NULL;
}
