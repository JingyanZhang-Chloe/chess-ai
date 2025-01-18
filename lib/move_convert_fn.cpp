#include <move_convert_fn.h>

using namespace engine;

std::string engine::to_san(move_t move, board_t& board){

    std::string san;
    // handle castling
    if(board.piece(move.source).value().kind == piece_kind::king){
        if(move.source.column() - move.destination.column() == -2){
            san =  "O-O";

            move_info_t info = board.make_move(move);

            if (board.is_check()) {
                san += board.is_checkmate() ? '#' : '+';
            };

            board.unmake_move(info);

            return san;

        }
        else if (move.source.column() - move.destination.column() == 2)
        {
            san = "O-O-O";

            move_info_t info = board.make_move(move);

            if (board.is_check()) {
                san += board.is_checkmate() ? '#' : '+';
            };

            board.unmake_move(info);

            return san;
        };
    };

    auto piece = board.piece(move.source);
    if (!piece.has_value()) {
        throw "[Error in to_san: ] Invalid move: no piece at the source square";
    }

    // get the piecekind
    char piece_char;
    auto kind = piece.value().kind;
    switch (kind) {
        case piece_kind::pawn:   piece_char = 'P'; break;
        case piece_kind::knight: piece_char = 'N'; break;
        case piece_kind::bishop: piece_char = 'B'; break;
        case piece_kind::rook:   piece_char = 'R'; break;
        case piece_kind::queen:  piece_char = 'Q'; break;
        case piece_kind::king:   piece_char = 'K'; break;
        default: throw "[Error in to_san: ] Unknown piece type";
    };

    bool file_really_needed = true;

    //handle pawn
    if(kind == piece_kind::pawn){
        if(abs(move.source.row() - move.destination.row()) == abs(move.source.column() - move.destination.column())){
            // pawn is capture a piece
            san += move.source.column_as_char();
            file_really_needed = false;

            if(!(board.piece(move.destination).has_value())){
				// then it is en passant
				san += "x";
            };
        };
    }
    else
    {
        san += piece_char;
    };

    //std::cout << "[AFTER the first char] " << "san is " << san << std::endl;
    
    // Handle ambiguity: Check if multiple pieces can reach the target
    bool file_needed = false, rank_needed = false;
    std::vector<move_t> candidates = board.pseudolegal_moves();

    for (auto& candidate : candidates) {
        if (candidate.destination == move.destination && candidate != move &&
            board.piece(candidate.source)->kind == piece->kind) {
            if (candidate.source.column() != move.source.column()) {
                // the file is not the same
                file_needed = true;
            } else if (candidate.source.row() != move.source.row()) {
                // the rank is not the same
                rank_needed = true;
            };
        };
    };

    //std::cout << "[AFTER handle multiple piece reach target] " << "san is " << san << std::endl;


    //add the second elem of san
    if (file_needed && file_really_needed) {
        san += move.source.column_as_char(); // Ensure column is valid
    }
    if (rank_needed) {
        san += std::to_string(move.source.row() + 1); // Ensure row is valid
    }

    //std::cout << "[AFTER adding the second elem of san] " << "san is " << san << std::endl;


    // Add 'x' for captures
    if (board.piece(move.destination).has_value()){
        san += 'x';
    };

    //std::cout << "[AFTER adding x] " << "san is " << san << std::endl;


    // Add the target square
    san += move.destination.into_string();

    //std::cout << "[AFTER adding target square] " << "san is " << san << std::endl;


    // Handle promotions
    if (move.promotion_code.has_value()) {
        san += '=';
        switch (move.promotion_code.value())
        {
        case piece_kind::queen:
            san += "Q";
            break;

        case piece_kind::rook:
            san += "R";
            break;

        case piece_kind::bishop:
            san += "B";
            break;

        case piece_kind::knight:
            san += "N";
            break;

        default:
            break;
        };
    }

    //std::cout << "[AFTER adding promotion] " << "san is " << san << std::endl;



    move_info_t info = board.make_move(move);

    if (board.is_check()) {
        san += board.is_checkmate() ? '#' : '+';
    };
    board.unmake_move(info);

    //std::cout << "[AFTER in check/checkmate] " << "san is " << san << std::endl;


    return san;
}