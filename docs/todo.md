![Engine v1 Diagram](images/chess-ai-v1.png)

The program flow is a summary of how the engine works.<br><br>

The tasks dependencies represent the order in which tasks should be implemented. For example, we cannot construct the board until we have a struct for moves. This is because the board should have a method that takes a move and makes that move.<br><br>

Here we put the tasks we have done:
- Parse Moves
