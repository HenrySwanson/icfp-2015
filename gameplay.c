// Note, this expects the units to already be centered.
int score_seq(board* b, unit* units, int num_units, dir_t* moves, int num_moves)
{
    int score = 0;

    int active_unit = 0;

    for(int i = 0; i < num_moves; i++)
    {
        unit* u = units + active_unit;
        move_unit(u, moves[i]);

        // Did we lock our piece?
        if(!can_be_placed(u, b))
        {
            move_unit(u, opposite_dir(moves[i]));
            place(u, b);

            // TODO row scoring

            active_unit++;
            if(active_unit == num_units)
                // That should have been our final move
                assert(i == num_moves - 1);
                break;
        }
    }

    // TODO power scoring

    return score;
}
