#pragma once

#include "sudoku.h"

typedef struct {
    int val1;
    int val2;
} NakedPair;

int naked_pairs_exists(NakedPair *pairs, int pair_count, int val1, int val2);
int naked_pairs(SudokuBoard *p_board);