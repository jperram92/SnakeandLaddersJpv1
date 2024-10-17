import pygame
import random

# Initialize Pygame
pygame.init()

# Set up display
WIDTH, HEIGHT = 600, 600
window = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption('Snakes and Ladders')

# Colors
PURPLE = (128, 0, 128)
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
GREEN = (0, 255, 0)
RED = (255, 0, 0)
YELLOW = (255, 255, 0)

# Snake and ladder positions (as per common board settings)
snakes = {16: 6, 47: 26, 49: 11, 56: 53, 62: 19, 64: 60, 87: 24, 93: 73, 95: 75, 98: 78}
ladders = {1: 38, 4: 14, 9: 31, 21: 42, 28: 84, 36: 44, 51: 67, 71: 91, 80: 100}

# Initialize player positions
player_position = {"Player 1": 0, "Player 2": 0}
symbols = {"Player 1": "X", "Player 2": "Y"}
player_colors = {"Player 1": YELLOW, "Player 2": RED}
board_size = 10

# Load fonts
font = pygame.font.SysFont('Arial', 20)

# Create board grid
def create_board():
    board = list(range(1, 101))
    return board[::-1]

def draw_board():
    """Draws the game board with numbers, snakes, and ladders"""
    window.fill(BLACK)
    board = create_board()
    
    square_size = WIDTH // board_size
    for i in range(100):
        x = (i % board_size) * square_size
        y = (i // board_size) * square_size

        # Color alternate squares
        if (i // board_size) % 2 == 0:
            if i % 2 == 0:
                color = PURPLE
            else:
                color = WHITE
        else:
            if i % 2 == 0:
                color = WHITE
            else:
                color = PURPLE

        pygame.draw.rect(window, color, (x, y, square_size, square_size))

        # Draw board numbers
        number_text = font.render(str(board[i]), True, BLACK if color == WHITE else WHITE)
        window.blit(number_text, (x + 10, y + 10))

    # Draw snakes and ladders
    for start, end in snakes.items():
        draw_snake_ladder(start, end, RED)

    for start, end in ladders.items():
        draw_snake_ladder(start, end, GREEN)

def draw_snake_ladder(start, end, color):
    """Draws a snake or ladder on the board"""
    square_size = WIDTH // board_size
    start_pos = get_square_position(start)
    end_pos = get_square_position(end)
    
    pygame.draw.line(window, color, start_pos, end_pos, 5)

def get_square_position(square):
    """Returns the (x, y) pixel position of a square number"""
    square_size = WIDTH // board_size
    square = 100 - square  # Reverse position to display correctly
    row = square // board_size
    col = square % board_size
    x = col * square_size + square_size // 2
    y = row * square_size + square_size // 2
    return (x, y)

def move_player(player, position):
    """Function to move a player based on dice roll"""
    dice = random.randint(1, 6)
    print(f"{player} rolled a {dice}")
    new_position = position + dice

    if new_position > 100:
        return position  # Stay in place if the roll exceeds 100

    # Check for snakes or ladders
    if new_position in snakes:
        new_position = snakes[new_position]
    elif new_position in ladders:
        new_position = ladders[new_position]

    return new_position

def draw_players():
    """Draws the player pieces on the board"""
    for player, position in player_position.items():
        if position > 0:
            pos = get_square_position(position)
            pygame.draw.circle(window, player_colors[player], pos, 15)

def display_message(text):
    """Displays a message on the screen"""
    message = font.render(text, True, WHITE)
    window.blit(message, (10, 10))

# Main game loop
def play_game():
    running = True
    turn = "Player 1"
    
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        # Game logic for player turns
        if turn == "Player 1":
            player_position[turn] = move_player(turn, player_position[turn])
            if player_position[turn] == 100:
                display_message(f"Congratulations {turn}, you've won the game!")
                pygame.display.update()
                pygame.time.delay(3000)
                running = False
            turn = "Player 2"
        else:
            player_position[turn] = move_player(turn, player_position[turn])
            if player_position[turn] == 100:
                display_message(f"Congratulations {turn}, you've won the game!")
                pygame.display.update()
                pygame.time.delay(3000)
                running = False
            turn = "Player 1"

        # Draw everything on the board
        draw_board()
        draw_players()
        pygame.display.update()

        pygame.time.delay(1000)  # Delay to simulate real turn taking

    pygame.quit()

play_game()
