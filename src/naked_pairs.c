#include "naked_pairs.h"
#include <stdlib.h>


int naked_pairs_exists(NakedPair *pairs, int pair_count, int val1, int val2) {
    for (int i = 0; i < pair_count; i++) {
        if ((pairs[i].val1 == val1 && pairs[i].val2 == val2) || (pairs[i].val1 == val2 && pairs[i].val2 == val1)) {
            return 1;
        }
    }
    return 0;
}

int naked_pairs(SudokuBoard *p_board) {
    NakedPair pairs[BOARD_SIZE * BOARD_SIZE];
    int pair_count = 0;

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            Cell *cell = &(p_board->data[i][j]);
            if (cell->num_candidates != 2)
                continue;

            int *candidates = get_candidates(cell);

            // Check row, column, and box
            for (int unitType = 0; unitType < 3; unitType++) {
                Cell **unit;
                switch (unitType) {
                    case 0:
                        unit = p_board->p_rows[i];
                        break;
                    case 1:
                        unit = p_board->p_cols[j];
                        break;
                    case 2:
                        unit = p_board->p_boxes[(i / 3) * 3 + j / 3];
                        break;
                }

                for (int k = 0; k < BOARD_SIZE; k++) {
                    if (unit[k] == cell)
                        continue;

                    Cell *other = unit[k];
                    if (other->num_candidates != 2)
                        continue;

                    int *other_candidates = get_candidates(other);
                    if (candidates[0] == other_candidates[0] && candidates[1] == other_candidates[1]) {
                        if (!naked_pairs_exists(pairs, pair_count, candidates[0], candidates[1])) {
                            pairs[pair_count].val1 = candidates[0];
                            pairs[pair_count].val2 = candidates[1];
                            pair_count++;
                        }
                        for (int l = 0; l < BOARD_SIZE; l++) {
                            if (unit[l] == cell || unit[l] == other)
                                continue;

                            Cell *remove = unit[l];
                            if (is_candidate(remove, candidates[0])) {
                                unset_candidate(remove, candidates[0]);
                            }
                            if (is_candidate(remove, candidates[1])) {
                                unset_candidate(remove, candidates[1]);
                            }
                        }
                    }
                    free(other_candidates);
                }
            }
            free(candidates);
        }
    }
    return pair_count;
}
