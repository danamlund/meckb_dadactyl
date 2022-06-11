/**
 * http://www.sudokuessentials.com/how_to_play_sudoku.html
 * http://www.sudokuessentials.com/sudoku_tips.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

bool is_valid_row(char board[81], int i) {
    int row = i / 9;
    bool seen[10] = { false };
    for (int j = 0; j < 9; j++) {
        int number = board[row * 9 + j];
        if (seen[number]) return false; // duplicate number
        if (number >= 1) seen[number] = true;
    }
    return true;
}

bool is_valid_column(char board[81], int i) {
    int column = i % 9;
    bool seen[10] = { false };
    for (int j = 0; j < 9; j++) {
        int number = board[column + j * 9];
        if (seen[number]) return false; // duplicate number
        if (number >= 1) seen[number] = true;
    }
    return true;
}

bool is_valid_subgrid(char board[81], int i) {
    int first = (i%9)/3*3 + i/9/3*3*9;
    bool seen[10] = { false };
    for (int j = 0; j < 9; j++) {
        int number = board[first + j%3 + j/3*9];
        if (seen[number]) return false; // duplicate number
        if (number >= 1) seen[number] = true;
    }
    return true;
}

bool is_valid(char board[81]) {
    if (!is_valid_row(board, 0)) return false;
    if (!is_valid_row(board, 9)) return false;
    if (!is_valid_row(board, 18)) return false;
    if (!is_valid_row(board, 27)) return false;
    if (!is_valid_row(board, 36)) return false;
    if (!is_valid_row(board, 45)) return false;
    if (!is_valid_row(board, 54)) return false;
    if (!is_valid_row(board, 63)) return false;
    if (!is_valid_row(board, 72)) return false;
    if (!is_valid_column(board, 0)) return false;
    if (!is_valid_column(board, 1)) return false;
    if (!is_valid_column(board, 2)) return false;
    if (!is_valid_column(board, 3)) return false;
    if (!is_valid_column(board, 4)) return false;
    if (!is_valid_column(board, 5)) return false;
    if (!is_valid_column(board, 6)) return false;
    if (!is_valid_column(board, 7)) return false;
    if (!is_valid_column(board, 8)) return false;
    if (!is_valid_row(board, 0)) return false;
    if (!is_valid_row(board, 3)) return false;
    if (!is_valid_row(board, 6)) return false;
    if (!is_valid_row(board, 28)) return false;
    if (!is_valid_row(board, 31)) return false;
    if (!is_valid_row(board, 34)) return false;
    if (!is_valid_row(board, 54)) return false;
    if (!is_valid_row(board, 57)) return false;
    if (!is_valid_row(board, 60)) return false;
    return true;
}

void print_board(char board[81]) {
    for (int i = 0; i < 81; i++) {
        printf("%d ", board[i]);
        if (i % 3 == 2) printf(" ");
        if (i % 9 == 8) {
            printf("\n");
            if ((i/9) % 3 == 2) printf("\n");
        }
    }
}

int solutions(char board[81]) {
    char copy[81];
    memcpy(copy, board, 81);

    for (int i = 0; i < 81; i++) {
        if (copy[i] == 0) {
            int output = 0;
            for (int j = 1; j <= 9; j++) {
                copy[i] = j;
                if (is_valid_row(copy, i)
                    && is_valid_column(copy, i)
                    && is_valid_subgrid(copy, i)) {
                    output += solutions(copy);
                }
            }
            return output;
        }
    }

    // all numbers filled out, check if valid
    //if (is_valid(copy)) print_board(copy);
    return is_valid(copy) ? 1 : 0;
}

int row_has_number(char board[81], int i, int number) {
    int row = i / 9;
    int found = 0;
    for (int j = 0; j < 9; j++) {
        if (board[row * 9 + j] == number) found++;
    }
    return found;
}
int column_has_number(char board[81], int i, int number) {
    int column = i % 9;
    int found = 0;
    for (int j = 0; j < 9; j++) {
        if (board[column + j * 9] == number) found++;
    }
    return found;
}
int subgrid_has_number(char board[81], int i, int number) {
    int first = (i%9)/3*3 + i/9/3*3*9;
    int found = 0;
    for (int j = 0; j < 9; j++) {
        if (board[first + j%3 + j/3*9] == number) found++;
    }
    return found;
}

bool solve_scan_lines(char board[81], int i, int number) {
    if (subgrid_has_number(board, i, number)) return false;

    char subgrid[9];
    int subgrid_first = (i%9)/3*3 + i/9/3*3*9;
    for (int j = 0; j < 9; j++) {
        int ii = subgrid_first + j%3 + j/3*9;
        subgrid[j] = board[ii];
    }
    if (row_has_number(board, subgrid_first, number)) {
        if (subgrid[0] == 0) subgrid[0] = 10;
        if (subgrid[1] == 0) subgrid[1] = 10;
        if (subgrid[2] == 0) subgrid[2] = 10;
    }
    if (row_has_number(board, subgrid_first+9, number)) {
        if (subgrid[3] == 0) subgrid[3] = 10;
        if (subgrid[4] == 0) subgrid[4] = 10;
        if (subgrid[5] == 0) subgrid[5] = 10;
    }
    if (row_has_number(board, subgrid_first+18, number)) {
        if (subgrid[6] == 0) subgrid[6] = 10;
        if (subgrid[7] == 0) subgrid[7] = 10;
        if (subgrid[8] == 0) subgrid[8] = 10;
    }

    if (column_has_number(board, subgrid_first, number)) {
        if (subgrid[0] == 0) subgrid[0] = 10;
        if (subgrid[3] == 0) subgrid[3] = 10;
        if (subgrid[6] == 0) subgrid[6] = 10;
    }
    if (column_has_number(board, subgrid_first+1, number)) {
        if (subgrid[1] == 0) subgrid[1] = 10;
        if (subgrid[4] == 0) subgrid[4] = 10;
        if (subgrid[7] == 0) subgrid[7] = 10;
    }
    if (column_has_number(board, subgrid_first+2, number)) {
        if (subgrid[2] == 0) subgrid[2] = 10;
        if (subgrid[5] == 0) subgrid[5] = 10;
        if (subgrid[8] == 0) subgrid[8] = 10;
    }
    int zeros = 0;
    for (int j = 0; j < 9; j++) {
        if (subgrid[j] == 0) zeros++;
    }
    if (zeros == 1) {
        for (int j = 0; j < 9; j++) {
            if (subgrid[j] == 0) {
                int ii = subgrid_first + j%3 + j/3*9;
                board[ii] = number;
                return true;
            }
        }
    }
    return false;
}

bool solve_scan_lines_all(char board[81]) {
    for (int number = 1; number <= 9; number++) {
        if (solve_scan_lines(board, 0, number)) return true;
        if (solve_scan_lines(board, 3, number)) return true;
        if (solve_scan_lines(board, 6, number)) return true;
        if (solve_scan_lines(board, 27, number)) return true;
        if (solve_scan_lines(board, 30, number)) return true;
        if (solve_scan_lines(board, 33, number)) return true;
        if (solve_scan_lines(board, 54, number)) return true;
        if (solve_scan_lines(board, 57, number)) return true;
        if (solve_scan_lines(board, 60, number)) return true;
    }
    return false;
}

bool solve_scan_missing_row(char board[81], int i) {
    int row = i / 9;
    int numbers_seen[10] = {0};
    for (int j = 0; j < 9; j++) {
        int ii = row * 9 + j;
        numbers_seen[board[ii]]++;
    }
    if (numbers_seen[0] == 0) return false;

    bool possibles[10][10] = {false};
    for (int j = 0; j < 9; j++) {
        int ii = row * 9 + j;
        if (board[ii] == 0) {
            for (int number = 1; number <= 9; number++) {
                if (numbers_seen[number] == 0) {
                    if (!column_has_number(board, ii, number)
                        && !subgrid_has_number(board, ii, number)) {
                        possibles[j][number] = true;
                    }
                }
            }
        }
    }
    
    for (int j = 0; j < 9; j++) {
        int ii = row * 9 + j;
        if (board[ii] == 0) {
            int single_possible_number = 0;
            for (int number = 1; number <= 9; number++) {
                if (possibles[j][number]) {
                    if (single_possible_number == 0) {
                        single_possible_number = number;
                    } else {
                        single_possible_number = -1;
                    }
                }
            }
            if (single_possible_number >= 1) {
                board[ii] = single_possible_number;
                return true;
            }
        }
    }
    
    return false;
}

bool solve_scan_missing_column(char board[81], int i) {
    int column = i % 9;
    int numbers_seen[10] = {0};
    for (int j = 0; j < 9; j++) {
        int ii = column + j * 9;
        numbers_seen[board[ii]]++;
    }
    if (numbers_seen[0] == 0) return false;

    bool possibles[10][10] = {false};
    for (int j = 0; j < 9; j++) {
        int ii = column + j * 9;
        if (board[ii] == 0) {
            for (int number = 1; number <= 9; number++) {
                if (numbers_seen[number] == 0
                    && !row_has_number(board, ii, number)
                    && !subgrid_has_number(board, ii, number)) {
                    possibles[j][number] = true;
                }
            }
        }
    }
    
    for (int j = 0; j < 9; j++) {
        int ii = column + j * 9;
        if (board[ii] == 0) {
            int single_possible_number = 0;
            for (int number = 1; number <= 9; number++) {
                if (possibles[j][number]) {
                    if (single_possible_number == 0) {
                        single_possible_number = number;
                    } else {
                        single_possible_number = -1;
                    }
                }
            }
            if (single_possible_number >= 1) {
                board[ii] = single_possible_number;
                return true;
            }
        }
    }
    
    return false;
}

bool solve_scan_missing_subgrid(char board[81], int i) {
    int first = (i%9)/3*3 + i/9/3*3*9;
    int numbers_seen[10] = {0};
    for (int j = 0; j < 9; j++) {
        int ii = first + j%3 + j/3*9;
        numbers_seen[board[ii]]++;
    }
    if (numbers_seen[0] == 0) return false;

    bool possibles[10][10] = {false};
    for (int j = 0; j < 9; j++) {
        int ii = first + j%3 + j/3*9;
        if (board[ii] == 0) {
            for (int number = 1; number <= 9; number++) {
                if (numbers_seen[number] == 0
                    && !row_has_number(board, ii, number)
                    && !column_has_number(board, ii, number)) {
                    possibles[j][number] = true;
                }
            }
        }
    }
    
    for (int j = 0; j < 9; j++) {
        int ii = first + j%3 + j/3*9;
        if (board[ii] == 0) {
            int single_possible_number = 0;
            for (int number = 1; number <= 9; number++) {
                if (possibles[j][number]) {
                    if (single_possible_number == 0) {
                        single_possible_number = number;
                    } else {
                        single_possible_number = -1;
                    }
                }
            }
            if (single_possible_number >= 1) {
                board[ii] = single_possible_number;
                return true;
            }
        }
    }
    
    return false;
}

bool solve_scan_missing_all(char board[81]) {
    for (int i = 0; i < 9; i++) {
        if (solve_scan_missing_row(board, i*9)) return true;
    }
    for (int i = 0; i < 9; i++) {
        if (solve_scan_missing_column(board, i)) return true;
    }
    if (solve_scan_missing_subgrid(board, 0)) return true;
    if (solve_scan_missing_subgrid(board, 3)) return true;
    if (solve_scan_missing_subgrid(board, 6)) return true;
    if (solve_scan_missing_subgrid(board, 27)) return true;
    if (solve_scan_missing_subgrid(board, 30)) return true;
    if (solve_scan_missing_subgrid(board, 33)) return true;
    if (solve_scan_missing_subgrid(board, 54)) return true;
    if (solve_scan_missing_subgrid(board, 57)) return true;
    if (solve_scan_missing_subgrid(board, 60)) return true;
}

bool solve_easy(char board[81]) {
    while (solve_scan_lines_all(board));
    for (int i = 0; i < 81; i++) {
        if (board[i] == 0) return false;
    }
    return true;
}
bool solve_medium(char board[81]) {
    while (solve_scan_lines_all(board) || solve_scan_missing_all(board));
    for (int i = 0; i < 81; i++) {
        if (board[i] == 0) return false;
    }
    return true;
}

static uint16_t xorshift16(uint16_t seed) {
    // http://www.retroprogramming.com/2017/07/xorshift-pseudorandom-numbers-in-z80.html
    if (seed == 0)
        seed = 1;
    seed ^= seed << 7;
    seed ^= seed >> 9;
    seed ^= seed << 8;
    return seed;
}

void random_board(char board[81], int numbers, uint16_t seed) {
    uint16_t random = seed;
    for (uint8_t i = 0; i < 81; i++)
        board[i] = 0;
    for (uint8_t i = 0; i < numbers; i++) {
        while (true) {
            uint8_t ii = random % 81;
            random = xorshift16(random);
            if (board[ii] != 0) continue;
            uint8_t number = 1 + (random % 9);
            random = xorshift16(random);
            board[ii] = number;
            if (is_valid_row(board, ii)
                && is_valid_column(board, ii)
                && is_valid_subgrid(board, ii)) {
                break;
            }
            board[ii] = 0;
        }
    }
}

void generate_valids() {
    int numbers_first = 25;
    int numbers_last = 40;
    uint16_t seens[1024];
    int seensi = 0;
    char random[81];
    int found = 0;
    uint8_t sizes[50] = {0};
    int sizesi = 0;
    printf("#define SEEDS_NUMBERS_FIRST %d\n", numbers_first);
    printf("#define SEEDS_NUMBERS_LAST %d\n", numbers_last);
    printf("uint16_t seeds[] = {\n");
    for (int numbers = 25; numbers < 40; numbers++) {
        for (int i = 0; i <= UINT16_MAX; i++) {
            bool seen = false;
            for (int j = 0; j < seensi; j++) {
                if (seens[j] == i) {
                    seen = true;
                    break;
                }
            }
            if (seen) continue;
            random_board(random, numbers, i);
            if (solve_easy(random) || solve_medium(random)) {
                random_board(random, numbers, i);
                if (solutions(random) == 1) { // verify single solution
                    printf("%d, ", i);
                    found++;
                    seens[seensi++] = i;
                    sizes[sizesi]++;
                }
            }
        }
        sizesi++;
    }
    printf("};\n");
    printf("uint16_t seeds_numbers_amount[] = {");
    for (int i = 0; i < sizesi; i++) {
        printf("%d, ", sizes[i]);
    }
    printf("};\n");
    printf("#define SEEDS_SIZE %d\n", found);
}

void generate_valids2() {
    int numbers_first = 25;
    int numbers_last = 40;
    char random[81];
    int found = 0;
    for (int numbers = 25; numbers < 40; numbers++) {
        printf("uint16_t seeds%d[] = {\n", numbers);
        int found_numbers = 0;
        for (int i = 0; i <= UINT16_MAX; i++) {
            random_board(random, numbers, i);
            if (!solve_easy(random) && solve_medium(random)) {
                random_board(random, numbers, i);
                if (solutions(random) == 1) { // verify single solution
                    printf("%d, ", i);
                    found++;
                    found_numbers++;
                }
            }
        }
        printf("};\n");
        printf("#define SEEDS_SIZE %d\n", found_numbers);
    }
}

int test() {
    {
        char test_board1[] = {0,0,3, 4,5,6, 7,8,9,
                              0,0,0, 0,0,0, 0,0,0,
                              0,0,0, 0,0,0, 0,0,0,

                              1,2,0, 0,0,0, 0,0,0,
                              0,0,0, 0,0,0, 0,0,0,
                              0,0,0, 0,0,0, 0,0,0,

                              0,0,0, 0,0,0, 0,0,0,
                              0,0,0, 0,0,0, 0,0,0,
                              0,0,0, 0,0,0, 0,0,0};
        solve_scan_missing_row(test_board1, 0);
        solve_scan_missing_row(test_board1, 0);
        if (test_board1[0] != 2 || test_board1[1] != 1) {
            printf("fail1\n");
            print_board(test_board1);
        }
    }
    {
        char b[] = {0,0,0, 1,0,0, 0,0,0,
                    0,0,0, 2,0,0, 0,0,0,
                    3,0,0, 0,0,0, 0,0,0,
                
                    4,0,0, 0,0,0, 0,0,0,
                    5,0,0, 0,0,0, 0,0,0,
                    6,0,0, 0,0,0, 0,0,0,
                
                    7,0,0, 0,0,0, 0,0,0,
                    8,0,0, 0,0,0, 0,0,0,
                    9,0,0, 0,0,0, 0,0,0};
        solve_scan_missing_column(b, 0);
        solve_scan_missing_column(b, 0);
        if (b[0] != 2 || b[9] != 1) {
            printf("fail2\n");
            print_board(b);
        }
    }
    {
        char b[] = {1,0,3, 2,0,0, 0,0,0,
                    0,5,6, 0,0,0, 0,0,0,
                    7,8,9, 0,0,0, 0,0,0,
                
                    4,0,0, 0,0,0, 0,0,0,
                    0,0,0, 0,0,0, 0,0,0,
                    0,0,0, 0,0,0, 0,0,0,
                
                    0,0,0, 0,0,0, 0,0,0,
                    0,0,0, 0,0,0, 0,0,0,
                    0,0,0, 0,0,0, 0,0,0};
        solve_scan_missing_subgrid(b, 0);
        solve_scan_missing_subgrid(b, 0);
        if (b[1] != 4 || b[9] != 2) {
            printf("fail3\n");
            print_board(b);
        }
    }
    {
        char b[] = {0,9,0, 5,0,0, 1,0,8,
                    0,0,0, 0,6,0, 9,0,0,
                    7,0,4, 0,8,0, 0,5,0,
                
                    0,2,0, 0,0,7, 0,8,3,
                    1,0,0, 0,0,8, 0,4,2,
                    0,4,8, 9,0,0, 0,0,0,
                
                    0,0,0, 0,0,0, 0,0,5,
                    0,0,0, 7,0,5, 0,0,0,
                    2,1,0, 0,9,0, 0,0,0};
        if (solve_easy(b) || solve_medium(b)) {
            printf("fail4\n");
            print_board(b);
        }
    }
}

int main(int argc, char **args) {
    struct timespec begin, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &begin);
    test();
    
    char easy[] = {0,0,0, 0,0,3, 0,5,0,
                   0,0,0, 4,1,0, 0,0,7,
                   0,8,0, 7,9,0, 3,0,0,

                   1,3,0, 9,5,0, 0,8,4,
                   0,0,2, 1,4,6, 7,0,0,
                   4,5,0, 0,3,7, 0,1,9,

                   0,0,1, 0,2,4, 0,9,0,
                   9,0,0, 0,8,1, 0,0,0,
                   0,2,0, 3,0,0, 0,0,0};

    char medium[] = {0,0,0, 1,0,0, 9,8,3,
                     0,0,0, 6,0,2, 0,1,0,
                     0,0,5, 0,0,8, 0,0,2,
                     
                     0,0,0, 8,6,0, 0,5,0,
                     5,0,8, 0,0,0, 4,0,9,
                     0,3,0, 0,1,4, 0,0,0,
                     
                     4,0,0, 3,0,0, 5,0,0,
                     0,8,0, 9,0,6, 0,0,0,
                     6,5,9, 0,0,1, 0,0,0};

    char hard[] = {0,0,0, 0,0,9, 4,0,2,
                   0,4,0, 0,5,0, 0,0,8,
                   0,9,0, 7,3,0, 0,0,0,

                   4,8,0, 1,0,0, 9,0,0,
                   0,0,2, 0,0,0, 3,0,0,
                   0,0,5, 0,0,8, 0,2,4,

                   0,0,0, 0,8,6, 0,4,0,
                   6,0,0, 0,9,0, 0,7,0,
                   8,0,4, 2,0,0, 0,0,0};

    char hard2[] = {0,0,1, 2,0,3, 4,0,0,
                    0,0,0, 6,0,7, 0,0,0,
                    5,0,0, 0,0,0, 0,0,3,

                    3,7,0, 0,0,0, 0,8,1,
                    0,0,0, 0,0,0, 0,0,0,
                    6,2,0, 0,0,0, 0,3,7,

                    1,0,0, 0,0,0, 0,0,8,
                    0,0,0, 8,0,5, 0,0,0,
                    0,0,6, 4,0,2, 5,0,0};

    char medium2[] = {0,7,0, 5,0,9, 1,6,8,
                      0,8,5, 6,1,3, 0,0,2,
                      6,0,1, 8,0,7, 0,0,3,
                     
                      0,0,0, 1,0,5, 0,3,9,
                      1,5,0, 0,0,6, 0,4,7,
                      8,9,0, 0,0,4, 0,1,5,
                     
                      0,1,0, 4,0,8, 3,0,6,
                      5,3,8, 0,6,1, 0,2,4,
                      7,6,4, 3,0,2, 0,8,1};

    char b3[] = {0,9,3, 0,0,0, 0,1,5,
                 2,0,0, 4,0,0, 0,3,0,
                 0,0,7, 0,0,0, 0,6,0,
                 
                 7,0,0, 1,0,0, 6,0,0,
                 0,1,0, 7,5,0, 0,0,0,
                 8,0,4, 0,0,2, 0,0,9,
                 
                 0,0,5, 0,8,3, 0,4,0,
                 6,0,0, 9,0,0, 0,0,0,
                 0,4,0, 0,0,0, 0,0,8};

    char b2[81];
    random_board(b2, 25, 249);

    if (false) {
        char *board = b3;
        print_board(board);
        int moves = 1;
        while (solve_scan_lines_all(board) || solve_scan_missing_all(board)) {
            printf("-- %d\n", moves++);
            print_board(board);
        }
        return 0;
    }

    //generate_valids();
    generate_valids2();

    // timings
    if (false) {
        char random[81];
        int found = 0;
        for (int i = 0; i <= UINT16_MAX; i++) {
            random_board(random, 30, i);
            //if (solutions(random) == 1) {
            //if (solve_easy(random)) {
            if (solve_medium(random)) {
                found++;
                printf("%d ", i);
                random_board(random, 30, i);
                if (solutions(random) != 1) {
                    printf("! ");
                }
            }
        }
        printf("\nfound=%d\n", found);
        // solutions(random) == 1): 115s, found=136
        // if (solve_easy(random)): 12s, found=53
        // if (solve_medium(random)): 21s, found=106
    }

    
    

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    printf ("Total time = %f seconds\n",
            (end.tv_nsec - begin.tv_nsec) / 1000000000.0 +
            (end.tv_sec  - begin.tv_sec));
    return 0;
}
