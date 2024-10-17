import random

# Snake and ladder positions (as per common board settings)
snakes = {16: 6, 47: 26, 49: 11, 56: 53, 62: 19, 64: 60, 87: 24, 93: 73, 95: 75, 98: 78}
ladders = {1: 38, 4: 14, 9: 31, 21: 42, 28: 84, 36: 44, 51: 67, 71: 91, 80: 100}

# Initialize player positions
player_position = {"Player 1": 0, "Player 2": 0}
symbols = {"Player 1": "X", "Player 2": "Y"}

def print_board():
    """Function to simulate the board structure in a simple way"""
    print("--------- Snakes & Ladders Game ---------")
    print("Welcome to the game! Reach 100 to win!")
    print("If you land on a snake, you'll slide down!")
    print("If you land on a ladder, you'll climb up!")
    print("----------------------------------------")

def roll_dice():
    """Function to simulate rolling a dice"""
    return random.randint(1, 6)

def create_board():
    """Create the board with 100 cells"""
    board = list(range(1, 101))
    return board[::-1]  # Reverse to show 100 at the top

def display_board(player_position):
    """Display the board with player positions"""
    board = create_board()

    # Map player positions on the board
    for player, position in player_position.items():
        if position > 0:
            board[100 - position] = symbols[player]  # Use 100-position to reverse indexing for top-left

    # Print the board in a grid of 10x10
    for i in range(0, 100, 10):
        row = board[i:i+10]
        print("\t".join(map(str, row)))
    print("\n")

def move_player(player, position):
    """Function to move a player based on dice roll"""
    dice = roll_dice()
    print(f"{player} rolled a {dice}")
    new_position = position + dice

    if new_position > 100:
        print(f"{player} needs an exact roll to reach 100. Current position: {position}")
        return position

    # Check for snakes or ladders
    if new_position in snakes:
        print(f"Oh no! {player} hit a snake at {new_position}. Slipping down to {snakes[new_position]}")
        new_position = snakes[new_position]
    elif new_position in ladders:
        print(f"Yay! {player} hit a ladder at {new_position}. Climbing up to {ladders[new_position]}")
        new_position = ladders[new_position]

    print(f"{player} is now at {new_position}")
    return new_position

def play_game():
    """Main game logic for two players"""
    print_board()
    while True:
        for player in player_position:
            input(f"{player}'s turn. Press Enter to roll the dice...")
            player_position[player] = move_player(player, player_position[player])

            # Display the board with the updated player positions
            display_board(player_position)

            if player_position[player] == 100:
                print(f"Congratulations {player}, you've won the game!")
                print("----------- MAICA -----------")
                return

play_game()
