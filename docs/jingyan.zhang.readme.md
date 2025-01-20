//draft

Today we meet in TD and create together the cmakelist and our gitLab. We add our branches and learn how to commit and merge them into main.

Today we meet together in the lecture and we divide the work for borad and moves. I will work on the moves part for queen, bishop, and rook.

Today I work on the gen_move_fn.h and gen_move_fn.cpp and we implement the function for generating the 'legal' moves for rook, queen and bishop.

Today I implement the comparasion of move and chess_coordinate

Chrismas break:
today i keep implement the gen_legal_move_fn for pawn, consider the case of double move, promotion and en passant

today i finish to implement all the moves for pieces and test on if all the function for generating movements work as expected

today i implement the function wining player in board_t to help our further check for minmax search. and i implememnt the function to check if the current player is in check. (is_check function)

today i implement the function for unamke the move for the board, to do so, i create a new struct called move_info to store the information for move, and to make the board go back to the state before the move.

today i modify and debug of make move and unmake move. handling the case of castiling and en passant.

today I add into_string for moves, and i create a lot of functions for converting the pgn format of move into uci for providing our engine a better referee to the opening moves, now we gonna add some master games into the engine referee for it to search the beginning move efficiently.

today i finish all the convert process from pgn to uci, and i get more than 100000 games for our openning training.

today i create a new structure called tree_t, this structure will have inner struct called node_t, and each node will contain a map between a node and a move, for each level node, it will contain every move in the map (i.e. the first level of node_t will contain a map that maps each first move to the second nodes). by this way we can efficiently get the master's open moves.



Meet 1: We got together in the TD session to kick things off. We set up the project environment and put together the CMakeLists.txt file to manage the build system. After that, we created a GitLab repository, set up our branches, and got familiar with committing and merging changes into the main branch. It was a productive start that laid the groundwork for working as a team.

Meet 2: During the lecture, we hashed out who’d work on what. We decided to split up the work on the board logic and movement functions. I picked the movement logic for the queen, bishop, and rook. It felt good to have clear tasks for everyone, so we knew what to focus on.

Meet 3: I started diving into the gen_move_fn.h and gen_move_fn.cpp files. My job was to figure out how to generate "legal" moves for the rook, queen, and bishop. It took some time to map out how these pieces move and deal with obstacles in their paths. I made sure to stick to the official chess rules for each one. It felt like laying down the building blocks for the game’s core mechanics.

Meet 4: Next up, I added some comparison functions for moves and chess coordinates. These were key for making sure the moves we’re generating actually make sense. It was a smaller task, but it really helped tie things together and made the code more solid.

Christmas Break Work
Meet 5: Over the break, I worked on the gen_legal_move_fn for the pawn. Pawns are tricky because they’ve got so many special cases: double moves on their first go, promotion when they reach the other side, and en passant captures. It took some effort to get all of that working, but it was satisfying to see it come together.

Meet 6: I wrapped up the movement logic for all the pieces. I tested everything to make sure it worked right and handled different scenarios. It was a big milestone to see all the pieces moving as they should. Testing took some time, but it was worth it to get everything running smoothly.


