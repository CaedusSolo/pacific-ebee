#include "battlefield.h"
#include "cell.h"
#include "game_states.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>

// Helper function to get ship name
const char* get_ship_name(enum Ship ship) {
    switch(ship) {
        case CARRIER: return "Carrier";
        case BATTLESHIP: return "Battleship";
        case CRUISER: return "Cruiser";
        case DESTROYER: return "Destroyer";
        case PATROL_BOAT: return "Patrol Boat";
        default: return "Unknown";
    }
}

// Print battlefield showing which players have ships in each cell
void print_battlefield_simple(Battlefield* bf) {
    printf("\nBattlefield Ship Placement (showing player count per cell):\n");
    printf("   ");
    for (int x = 0; x < Battlefield_getWidth(bf); x++) {
        printf("%2d ", x);
    }
    printf("\n");
    
    for (int y = 0; y < Battlefield_getHeight(bf); y++) {
        printf("%2d ", y);
        for (int x = 0; x < Battlefield_getWidth(bf); x++) {
            Cell* cell = Battlefield_getCell(bf, x, y);
            int count = 0;
            for (int p = 0; p < PLAYER_NUM; p++) {
                if (Cell_hasPlayerShip(cell, p)) {
                    count++;
                }
            }
            if (count == 0) {
                printf(" . ");
            } else {
                printf(" %d ", count);
            }
        }
        printf("\n");
    }
}

// Print detailed ship information for a specific player
void print_player_ships(Battlefield* bf, int player_index) {
    printf("\n=== Player %d Ships ===\n", player_index);
    int ship_counts[5] = {0}; // Count cells for each ship type
    
    for (int y = 0; y < Battlefield_getHeight(bf); y++) {
        for (int x = 0; x < Battlefield_getWidth(bf); x++) {
            Cell* cell = Battlefield_getCell(bf, x, y);
            if (Cell_hasPlayerShip(cell, player_index)) {
                enum Ship ship = cell->ship_types[player_index];
                ship_counts[ship]++;
            }
        }
    }
    
    printf("Carrier:     %d cells\n", ship_counts[CARRIER]);
    printf("Battleship:  %d cells\n", ship_counts[BATTLESHIP]);
    printf("Cruiser:     %d cells\n", ship_counts[CRUISER]);
    printf("Destroyer:   %d cells\n", ship_counts[DESTROYER]);
    printf("Patrol Boat: %d cells\n", ship_counts[PATROL_BOAT]);
    printf("Total cells: %d\n", ship_counts[0] + ship_counts[1] + ship_counts[2] + ship_counts[3] + ship_counts[4]);
}

// Check for overlapping cells between players
void print_overlaps(Battlefield* bf) {
    printf("\n=== Cell Overlaps ===\n");
    int overlap_count = 0;
    
    for (int y = 0; y < Battlefield_getHeight(bf); y++) {
        for (int x = 0; x < Battlefield_getWidth(bf); x++) {
            Cell* cell = Battlefield_getCell(bf, x, y);
            int players_here = 0;
            int player_list[PLAYER_NUM];
            
            for (int p = 0; p < PLAYER_NUM; p++) {
                if (Cell_hasPlayerShip(cell, p)) {
                    player_list[players_here++] = p;
                }
            }
            
            if (players_here > 1) {
                overlap_count++;
                printf("Cell (%d, %d): Players ", x, y);
                for (int i = 0; i < players_here; i++) {
                    printf("%d", player_list[i]);
                    if (i < players_here - 1) printf(", ");
                }
                printf(" (");
                for (int i = 0; i < players_here; i++) {
                    printf("%s", get_ship_name(cell->ship_types[player_list[i]]));
                    if (i < players_here - 1) printf(" + ");
                }
                printf(")\n");
            }
        }
    }
    
    if (overlap_count == 0) {
        printf("No overlapping cells found.\n");
    } else {
        printf("Total overlapping cells: %d\n", overlap_count);
    }
}

int main(int argc, char *argv[]) {
    int size = 10;  // Smaller size for easier visualization
    
    if (argc > 1) {
        size = atoi(argv[1]);
        if (size < 5 || size > 25) {
            printf("Size must be between 5 and 25. Using default: 10\n");
            size = 10;
        }
    }
    
    printf("=== Battleship Random Placement Test ===\n");
    printf("Battlefield size: %dx%d\n", size, size);
    printf("Number of players: %d\n", PLAYER_NUM);
    
    // Create battlefield
    Battlefield* bf = Battlefield_new(size, size);
    
    // Place ships for all players
    printf("\nPlacing ships for each player...\n");
    for (int p = 0; p < PLAYER_NUM; p++) {
        Battlefield_placeShipsRandomly(bf, p);
        printf("Player %d ships placed.\n", p);
    }
    
    // Print results
    print_battlefield_simple(bf);
    
    for (int p = 0; p < PLAYER_NUM; p++) {
        print_player_ships(bf, p);
    }
    
    print_overlaps(bf);
    
    // Cleanup
    Battlefield_delete(bf);
    
    printf("\n=== Test Complete ===\n");
    return 0;
}
