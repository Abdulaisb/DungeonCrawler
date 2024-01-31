#include <iostream>
#include <fstream>
#include <string>
#include "logic.h"

using std::cout, std::endl, std::ifstream, std::string;

/**
 * TODO: Student implement this function
 * Load representation of the dungeon level from file into the 2D map.
 * Calls createMap to allocate the 2D array.
 * @param   fileName    File name of dungeon level.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number o-f columns in the dungeon table (aka width).
 * @param   player      Player object by reference to set starting position.
 * @return  pointer to 2D dynamic array representation of dungeon map with player's location., or nullptr if loading fails for any reason
 * @updates  maxRow, maxCol, player
 */
char** loadLevel(const string& fileName, int& maxRow, int& maxCol, Player& player) {
    //File header validation
    ifstream inputFile;
    inputFile.open(fileName);
    if (!inputFile)
        return nullptr;
    int fRow;
    inputFile >> fRow;
    if (!inputFile.good() || fRow < 1) {
        inputFile.clear(); 
        return nullptr;
    }
    maxRow = fRow;
    int fCol;
    inputFile >> fCol;
    if (!inputFile.good() || fCol < 1) {
        inputFile.clear(); 
        return nullptr;
    }
    maxCol = fCol;
    int pRow;
    inputFile >> pRow;
    if (!inputFile.good() || pRow < 0 || pRow > maxRow - 1) {
        inputFile.clear(); 
        return nullptr;
    }
    int pCol;
    inputFile >> pCol;
    if (!inputFile.good() || pCol < 0 || pCol > maxCol - 1) {
        inputFile.clear(); 
        return nullptr;
    }

    if (maxRow < 1 || maxCol < 1 ||  maxCol > INT32_MAX / maxRow || pRow > maxRow - 1 || pCol > maxCol - 1|| pRow < 0 || pCol < 0 ) {
        return nullptr;
    } 

    // PLayer Position
    player.row = pRow;
    player.col = pCol;
    // Filling map
    int escapes = 0;
    char** level =  createMap(maxRow, maxCol);
    for (int i = 0; i < maxRow; i++) {
        for (int j = 0; j < maxCol; j++) {
            inputFile >> level[i][j];
            if (!inputFile.good()) {
                inputFile.clear(); 
                return nullptr;
            }
            if(!(level[i][j] == TILE_OPEN || level[i][j] == TILE_AMULET || level[i][j] == TILE_DOOR|| level[i][j] == TILE_EXIT|| level[i][j] == TILE_MONSTER || level[i][j] == TILE_PILLAR || level[i][j] == TILE_TREASURE || level[i][j] == TILE_PLAYER)) {
                return nullptr;
            }
            
            
        }
    }
    //Checking for extra values
    char last;
    inputFile >> last;
    if (inputFile.good()) {
        return nullptr;
    }
    // Checking if player on monster
    inputFile.clear();
    if (level[pRow][pCol] == TILE_MONSTER) {
        return nullptr;
    }
    //Placing PLayer
    level[pRow][pCol] = TILE_PLAYER;
    //Checking for one escape
    for (int i = 0; i < maxRow; i++) {
        for (int j = 0; j < maxCol; j++) {
            if (level[i][j] == TILE_DOOR || level[i][j] == TILE_EXIT) {
                escapes ++;
            } 
        }
    }
    if (escapes < 1) {
        return nullptr;
    }
    return level;
}

/**
 * TODO: Student implement this function
 * Translate the character direction input by the user into row or column change.
 * That is, updates the nextRow or nextCol according to the player's movement direction.
 * @param   input       Character input by the user which translates to a direction.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @updates  nextRow, nextCol
 */
void getDirection(char input, int& nextRow, int& nextCol) {
    if (input == 'w')
        nextRow -= 1;
    if (input == 's')
        nextRow += 1;
    if (input == 'a')
        nextCol -= 1;
    if (input == 'd')
        nextCol += 1;
}

/**
 * TODO: [suggested] Student implement this function
 * Allocate the 2D map array.
 * Initialize each cell to TILE_OPEN.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @return  2D map array for the dungeon level, holds char type.
 */
char** createMap(int maxRow, int maxCol) {
    if (maxRow < 1 || maxCol < 1) {
        return nullptr;
    }
    char** map = new char*[maxRow];
    for (int i = 0; i < maxRow; i ++) {
        map[i] = new char[maxCol];       
    }
    for (int i = 0; i < maxRow; i++) {
        for (int j = 0; j < maxCol; j++) {
                map[i][j] = TILE_OPEN;  
            }
    }
    return map;
}

/**
 * TODO: Student implement this function
 * Deallocates the 2D map array.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @return None
 * @update map, maxRow
 */
void deleteMap(char**& map, int& maxRow) {
    if (map != nullptr) {
        for (int i = 0; i < maxRow; i++) {
            delete[] map[i];
            map[i] = nullptr;
        }
        delete[] map;
        map = nullptr;
        }
    maxRow = 0;
}

/**
 * TODO: Student implement this function
 * Resize the 2D map by doubling both dimensions.
 * Copy the current map contents to the right, diagonal down, and below.
 * Do not duplicate the player, and remember to avoid memory leaks!
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height), to be doubled.
 * @param   maxCol      Number of columns in the dungeon table (aka width), to be doubled.
 * @return  pointer to a dynamically-allocated 2D array (map) that has twice as many columns and rows in size.
 * @update maxRow, maxCol
 */
char** resizeMap(char** map, int& maxRow, int& maxCol) {
    if (maxRow < 1 || maxCol < 1 || map == nullptr)
        return nullptr;
    int pRow = 0;
    int pCol = 0;
    for (int i = 0; i < maxRow; i++) {
        for (int j = 0; j < maxCol; j++) {
            if (map[i][j] == TILE_PLAYER) {
                pRow = i;
                pCol = j;
            }
        }
    }
    maxRow *= 2;
    maxCol *= 2;
    char** nMap = createMap(maxRow, maxCol);
    //Top Left
    for (int i = 0; i < maxRow / 2; i++) {
        for (int j = 0; j < maxCol / 2; j++) {
            nMap[i][j] = map[i][j];
        }
    }
    //Top Right
    for (int i = 0; i < maxRow / 2; i++) {
        for (int j = 0; j < maxCol / 2; j++) {
            nMap[i][j + (maxCol/2)] = map[i][j];
        }
    }
    //Bottom Left
    for (int i = 0; i < maxRow / 2; i++) {
        for (int j = 0; j < maxCol / 2; j++) {
            nMap[i + (maxRow/2)][j] = map[i][j];
        }
    }
    //Bottom Right
    for (int i = 0; i < maxRow / 2; i++) {
        for (int j = 0; j < maxCol / 2; j++) {
            nMap[i + (maxRow/2)][j + (maxCol/2)] = map[i][j];
        }
    }
    //Empty players
    for (int i = 0; i < maxRow; i++) {
        for (int j = 0; j < maxCol; j++) {
            if (nMap[i][j] == TILE_PLAYER) {
                nMap[i][j] = TILE_OPEN;
            }
        }
    }
    //Place player on map
    nMap[pRow][pCol] = TILE_PLAYER;
    int nRow = maxRow / 2;
    deleteMap(map, nRow);
    return nMap;
}

/**
 * TODO: Student implement this function
 * Checks if the player can move in the specified direction and performs the move if so.
 * Cannot move out of bounds or onto TILE_PILLAR or TILE_MONSTER.
 * Cannot move onto TILE_EXIT without at least one treasure. 
 * If TILE_TREASURE, increment treasure by 1.
 * Remember to update the map tile that the player moves onto and return the appropriate status.
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object to by reference to see current location.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @return  Player's movement status after updating player's position.
 * @update map contents, player
 */
int doPlayerMove(char** map, int maxRow, int maxCol, Player& player, int nextRow, int nextCol) {
    //Out of bounds
    if (nextRow >= maxRow || nextCol >= maxCol || nextRow < 0 || nextCol < 0) {
        nextRow = player.row;
        nextCol = player.col;
        return STATUS_STAY;
    }
    //Open
    if (map[nextRow][nextCol] == TILE_OPEN) {
            map[player.row][player.col] = TILE_OPEN;
            map[nextRow][nextCol] = TILE_PLAYER;
            player.row = nextRow;
            player.col = nextCol;
            return STATUS_MOVE;
        }
    //Pillar or Monster
    if (map[nextRow][nextCol] == TILE_PILLAR || map[nextRow][nextCol] == TILE_MONSTER) {
        nextRow = player.row;
        nextCol = player.col;
        return STATUS_STAY;
    }
    //Treasure
    if (map[nextRow][nextCol] == TILE_TREASURE) {
        map[player.row][player.col] = TILE_OPEN;
        map[nextRow][nextCol] = TILE_PLAYER;
        player.treasure += 1;
        player.row = nextRow;
        player.col = nextCol;
        return STATUS_TREASURE;
    }
    //Amulet
    if (map[nextRow][nextCol] == TILE_AMULET) {
            map[player.row][player.col] = TILE_OPEN;
            map[nextRow][nextCol] = TILE_PLAYER;
            player.row = nextRow;
            player.col = nextCol;
            return STATUS_AMULET;
        }
    //Door
    if (map[nextRow][nextCol] == TILE_DOOR) {
            map[player.row][player.col] = TILE_OPEN;
            map[nextRow][nextCol] = TILE_PLAYER;
            player.row = nextRow;
            player.col = nextCol;
            return STATUS_LEAVE;
        }
    //Exit
    if (map[nextRow][nextCol] == TILE_EXIT) {
            if (player.treasure < 1) {
                nextRow = player.row;
                nextCol = player.col;
                return STATUS_STAY;
            }
            map[player.row][player.col] = TILE_OPEN;
            map[nextRow][nextCol] = TILE_PLAYER;
            player.row = nextRow;
            player.col = nextCol;
            return STATUS_ESCAPE;
        }
    return 1;
}

/**
 * TODO: Student implement this function
 * Update monster locations:
 * We check up, down, left, right from the current player position.
 * If we see an obstacle, there is no line of sight in that direction, and the monster does not move.
 * If we see a monster before an obstacle, the monster moves one tile toward the player.
 * We should update the map as the monster moves.
 * At the end, we check if a monster has moved onto the player's tile.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference for current location.
 * @return  Boolean value indicating player status: true if monster reaches the player, false if not.
 * @update  map contents
 */
bool doMonsterAttack(char** map, int maxRow, int maxCol, const Player& player) {
    
    //Above player
    for (int i = player.row; i >= 0; i--) {
        if (map[i][player.col] == TILE_PILLAR)
            break;
        if (map[i][player.col] == TILE_MONSTER) {
            map[i + 1][player.col] = TILE_MONSTER;
            map[i][player.col] = TILE_OPEN;
        }

    }
    //Below PLayer
    for (int i = player.row; i < maxRow; i++) {
        if (map[i][player.col] == TILE_PILLAR)
            break;
        if (map[i][player.col] == TILE_MONSTER) {
            map[i-1][player.col] = TILE_MONSTER;
            map[i][player.col] = TILE_OPEN;
        }

    }
    //Left of Player
    for (int i = player.col; i >= 0; i--) {
        if (map[player.row][i] == TILE_PILLAR)
            break;
        if(map[player.row][i] == TILE_MONSTER) {
            map[player.row][i+1] = TILE_MONSTER;
            map[player.row][i] = TILE_OPEN;
        }
    }
    //Right of Player
    for (int i = player.col; i < maxCol; i++) {
        if (map[player.row][i] == TILE_PILLAR)
            break;
        if(map[player.row][i] == TILE_MONSTER) {
            map[player.row][i-1] = TILE_MONSTER;
            map[player.row][i] = TILE_OPEN;
        }
    }
    if (map[player.row][player.col] == TILE_MONSTER) {
        return true;
    }
    return false;
}
