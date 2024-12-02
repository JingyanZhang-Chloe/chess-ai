struct piece_t {
    char piece;    //¨ The piece type ('K', 'Q', 'R', 'B', 'N', 'P' for white, lowercase for black)
    bool is_white; 
};


struct board_t {
	piece_t squares[8][8];
    bool white_to_move;     // True if it's white's turn
    int move_number;        // Current move number in the game
    bool castling_rights[2];
};
