Meeting 1:
The team met during the TD session to collaborate on the initial setup of the project. Together, we created the CMakeLists file to manage the build process and set up a GitLab repository for version control. Each member created their own branches, and we went through the process of learning how to commit changes and merge them into the main branch. This session established the groundwork for our collaboration and ensured that everyone was aligned on version control practices.

Meeting 2:
We met during the lecture to discuss and divide the project tasks. The focus was on splitting responsibilities for implementing the board and moves logic. The team agreed on the scope of each task, and I was assigned the responsibility of implementing the moves for the queen, bishop, and rook. This meeting ensured that all team members had clear objectives and that the workload was evenly distributed.

Meeting 3:
The team continued making progress, and I started working on the gen_move_fn.h and gen_move_fn.cpp files. These files were critical for implementing the move generation functionality. I focused on writing logic to generate legal moves for the rook, queen, and bishop, ensuring the rules for their movement patterns were correctly implemented. This session was a key step in building the foundation for piece movement.

Meeting 4:
In this meeting, I implemented a comparison function for moves and chess coordinates. The purpose of this functionality was to enable the system to validate moves by comparing them against the expected positions on the board. By introducing this feature, we ensured that the generated moves would be accurate and aligned with the rules of chess, adding an extra layer of reliability to the move generation process.

Meeting 5:
During the Christmas break, I focused on extending the gen_legal_move_fn to handle pawn movements. This involved addressing unique cases for pawns, such as their ability to perform a double move from their starting position, promoting to another piece upon reaching the back rank, and executing en passant captures. These additions made the pawn move generation more comprehensive, covering all edge cases and special rules associated with the piece.

Meeting 6:
During this meeting, I completed the implementation of move generation for all chess pieces. I also tested the functions to verify that the movements generated for each piece worked as expected. This step was crucial for confirming the correctness and reliability of the logic written so far, ensuring that the core functionality of the chess engine was on track.

Meeting 7:
In this session, I implemented a function to determine the winning player based on the current state of the board. Additionally, I developed the is_check function to detect if the current player is in check. These functions are essential for adding game state evaluation and will play a significant role in implementing advanced features such as AI decision-making using the minimax algorithm.

Meeting 8:
The focus of this meeting was on implementing the unmake_move function for the board. To achieve this, I designed a new struct called move_info to store the details of each move. This struct allows the board to revert to its state before a move was made. This implementation is critical for undoing moves during searches and evaluations, which is especially important for techniques like backtracking in AI algorithms.

Meeting 9:
Today, I worked on debugging and refining the make_move and unmake_move functions. Special cases like castling and en passant were addressed to ensure that these complex scenarios were handled correctly. This meeting focused on improving the reliability of the board's state transitions, which is fundamental to the chess engine's overall functionality.

Meeting 10:
I implemented the to_string function for moves, allowing moves to be easily converted into a readable format. Additionally, I developed a series of functions to convert moves from the PGN (Portable Game Notation) format to the UCI (Universal Chess Interface) format. This was done to enhance the engine’s ability to process and refer to opening moves. The focus was now on gathering master games to build a strong opening repertoire for the chess engine.

Meeting 11:
During this session, I completed the entire conversion process from PGN to UCI format and successfully gathered a dataset of over 100,000 games for training the engine's opening phase. This extensive collection of games will allow the engine to make more informed decisions during the initial stages of a chess match.

Meeting 12:
Today, I designed and implemented a new structure called tree_t, which includes an inner struct called node_t. Each node contains a map that links a move to another node, allowing the tree to represent the progression of moves. For the first level of nodes, each node contains a map of all possible opening moves leading to subsequent levels. This structure provides an efficient way to store and retrieve master opening moves for the engine.

Meeting 13:
I began implementing methods to turn the gathered opening games into a tree structure for more efficient searching. Additionally, I developed a merge function to combine multiple trees. Each node in the tree now also records win, loss, and draw counts, enabling the engine to evaluate opening lines based on their historical success rates. This enhancement ensures that the opening repertoire is not only efficient but also strategically sound.

Meeting 14:
During this meeting, I focused on fixing bugs in the tree merging functionality. I also implemented a print function for the tree structure to visualize and debug the complex relationships within it. These improvements were essential for ensuring that the opening tree worked as intended and could be effectively utilized by the chess engine.