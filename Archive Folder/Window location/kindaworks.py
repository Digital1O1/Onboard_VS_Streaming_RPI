import pygame
import os

os.environ['SDL_VIDEO_CENTERED'] = '1'  # Centering the window

pygame.init()

# Set up the display
window_size = (640, 480)
screen = pygame.display.set_mode(window_size)
pygame.display.set_caption("Window Location")

clock = pygame.time.Clock()

# Main loop
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Get the position of the window
    window_pos = pygame.display.get_window_position()

    # Clear the screen
    screen.fill((255, 255, 255))

    # Display the position of the window
    font = pygame.font.Font(None, 36)
    text = font.render(f"Window Position: {window_pos}", True, (0, 0, 0))
    screen.blit(text, (20, 20))

    # Update the display
    pygame.display.flip()

    # Cap the frame rate
    clock.tick(30)

pygame.quit()
