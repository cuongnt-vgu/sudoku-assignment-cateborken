#include "hidden_singles.h"
#include <stdlib.h>

// find hidden single values (the value that is unique among all the candidates for a row, a collumn, or a box)
int find_hidden_single_values(Cell **p_cells, int *hidden_single_values)
{
    int counter = 0;
    int counts_candidate[BOARD_SIZE] = {0}; 
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        int *candidates = get_candidates(p_cells[i]);
        if (p_cells[i]->num_candidates > 1)
        {
            for (int j = 0; j < p_cells[i]->num_candidates; j++)
            {
                counts_candidate[candidates[j]-1]++;
            }
        }
        free(candidates);
    }
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (counts_candidate[i] == 1)
        {
            hidden_single_values[counter++] = i + 1;
        }
    }        
    return counter;
}

// find hidden single cells in a row, in a collumn or in a box
void find_hidden_single(Cell **p_cells, HiddenSingle *p_hidden_singles, int *p_counter)
{
    int hidden_single_values[BOARD_SIZE];
    int num_hidden_single_values = find_hidden_single_values(p_cells, hidden_single_values);

    for (int i = 0; i < num_hidden_single_values; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (p_cells[j]->num_candidates > 1)
            {
                int *candidates = get_candidates(p_cells[j]);
                for (int k = 0; k < p_cells[j]->num_candidates; k++)
                {
                    if (candidates[k] == hidden_single_values[i])
                    {
                        p_hidden_singles[*p_counter].p_cell = p_cells[j];
                        p_hidden_singles[*p_counter].value = hidden_single_values[i];
                        (*p_counter)++;
                        break;
                    }
                }
                free(candidates);
            }
        }
    }
}

// find hidden single cells in a board, return the number of hidden single cells
int hidden_singles(SudokuBoard *p_board)
{
    HiddenSingle hidden_singles[BOARD_SIZE * BOARD_SIZE];
    int counter = 0;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        find_hidden_single(p_board->p_rows[i], hidden_singles, &counter);
        find_hidden_single(p_board->p_cols[i], hidden_singles, &counter);
        find_hidden_single(p_board->p_boxes[i], hidden_singles, &counter);
    }

    int solved_cells = 0; // Counts cells that are solved (i.e 1 candidate)
    for (int i = 0; i < counter; i++)
    {
        if (hidden_singles[i].p_cell->num_candidates == 1) {solved_cells++;}
        Cell *cell = hidden_singles[i].p_cell;
        int value = hidden_singles[i].value;
        set_candidates(cell, &value, 1);
    }
    return counter - solved_cells;
}
