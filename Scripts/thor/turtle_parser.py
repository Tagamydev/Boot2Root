import pygame
import math
import re

# === Settings ===
SCREEN_SIZE = (1920, 1920)
BG_COLOR = (30, 30, 30)
LINE_COLOR = (255, 255, 255)
SCALE = 2  # pixels per space

# === Initialize Pygame ===
pygame.init()
screen = pygame.display.set_mode(SCREEN_SIZE)
pygame.display.set_caption("Pygame Turtle Parser")
screen.fill(BG_COLOR)

# Virtual turtle state
x, y = SCREEN_SIZE[0] // 2, SCREEN_SIZE[1] // 2
angle = 0  # Degrees, 0 is right

def draw_line(distance):
    global x, y
    radians = math.radians(angle)
    new_x = x + distance * math.cos(radians)
    new_y = y - distance * math.sin(radians)  # Subtract y for Pygame's inverted Y axis
    pygame.draw.line(screen, LINE_COLOR, (x, y), (new_x, new_y), 2)
    x, y = new_x, new_y

# === Load Instructions ===
with open("instructions.txt", "r", encoding="utf-8") as f:
    instructions = f.readlines()

# === Parse and Execute ===
for line in instructions:
    line = line.strip().lower()

    match = re.match(r"tourne droite de (\d+)", line)
    if match:
        angle += int(match.group(1))
        continue

    match = re.match(r"tourne gauche de (\d+)", line)
    if match:
        angle -= int(match.group(1))
        continue

    match = re.match(r"avance (\d+)", line)
    if match:
        draw_line(int(match.group(1)) * SCALE)
        continue

    match = re.match(r"recule (\d+)", line)
    if match:
        draw_line(-int(match.group(1)) * SCALE)
        continue

# === Show Drawing ===
pygame.display.flip()

# Wait until window is closed
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

pygame.quit()
