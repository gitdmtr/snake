#include <stdint.h>
#include "Joystick.h"
#include "Snake.h"

/**
 * @brief Constructor for the Snake class.
 * @param init_dir The initial direction of the snake.
 */
Snake::Snake(direction_t init_dir) :
    current_dir(init_dir), score(0)
{
    // Initialize the snake with the provided direction and score.
}

/**
 * @brief Initialize the snake at the beginning of new round.
 */
void Snake::init()
{
    coordinates[0].x = HEAD_INIT_X;
    coordinates[0].y = 3;

    coordinates[1] = coordinates[0];
    coordinates[2] = coordinates[0];

    coordinates[1].x = HEAD_INIT_X + 1;
    coordinates[2].x = HEAD_INIT_X + 2;

    current_dir = RIGHT;
    score = 0;
    size = INIT_SNAKE_SIZE;
}

/**
 * @brief Check if the snake has eaten the food at the given coordinates.
 * @param food_coord The coordinates of the food.
 * @return True if the snake has eaten the food, false otherwise.
 */
bool Snake::has_eaten(coordinates_t food_coord)
{
    bool has_eaten = false;
    if(food_coord.x == coordinates[0].x && food_coord.y == coordinates[0].y)
    {
        has_eaten = true;
        size++;
        score++;
    }
    return has_eaten;
}

/**
 * @brief Move the snake in the specified direction.
 * @param new_dir The new direction to move the snake.
 */
void Snake::run(direction_t new_dir)
{
    // Protection against moving backwards:
    if (!(new_dir == RIGHT && current_dir == LEFT)  &&
        !(new_dir == LEFT && current_dir == RIGHT) &&
        !(new_dir == DOWN && current_dir == UP)    &&
        !(new_dir == UP && current_dir == DOWN))
    {
        current_dir = new_dir;
    }

    // Move the body of the snake.
    for (uint8_t idx = size; idx > 0; idx--)
    {
        coordinates[idx] = coordinates[idx - 1];
    }
    
    // Move the head of the snake based on the current direction.
    if (current_dir == DOWN)
    {
        coordinates[0].y = coordinates[0].y + 1;
    }
    if (current_dir == UP)
    {
        coordinates[0].y = coordinates[0].y - 1;
    }
    if (current_dir == LEFT)
    {
        coordinates[0].x = coordinates[0].x + 1;
    }
    if (current_dir == RIGHT)
    {
        coordinates[0].x = coordinates[0].x - 1;
    }
}
