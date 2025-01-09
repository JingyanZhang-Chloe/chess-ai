Today we got to know our partners. I started to get familiar with the git repository for common project. Branches, merging, first commits etc. We also discussed about the structure of the project and how are we gonna organise and structure the project.

The weekend after we started coding the project. I implemented the very first board and piece structure wich is a crucial step for our program to work be able to actually play chess, by the rule but also to let the AI use all the possible rules that are allowed in chess and that could make a difference.

The week after, we talked about how we would organize the project so that there is enough tasks to let everyone work at the same time without causing weird thing with the merging etc. I was given the responability of implementing a test function that would be able to make our AI fight against herself and  check if blunder-ai actually play by the rules, performance, delay for a move etc.

I was sick during the two weeks before the chrismas break (i couldn't go to class and study on the project, and i have a note from the doctor). I reimplemented the structure of the board that was not on gitlab anymore. 

Chrismas break: I finally pushed the test engine function wich test the following: The ai thinks for less than 5 seconds, the move is legal, there is a piece where the engine want to do the move. I also implemented a function that get all the possible move of a certain color from the board.

Week after the christmas break: This week I fixed small issues such as variable naming. I also implemented the blunder-ai.cpp main function wich for now outputs a random legal move. I also created an injective hash function for the board state in order to be able to do dynamic programming and maybe provoque a draw if it can prevent from losing. For now, the hash function do not handle "en passant".