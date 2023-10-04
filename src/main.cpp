#include <Arduino.h>
#include <MD_MAX72xx.h>
#include "Joystick.h"
#include "Snake.h"

// Constants for timing and game logic
#define JOYSTICK_SAMPLE_PERIOD_MS 10
#define GAME_RUNNING_LOGIC_PERIOD_MS 450

coordinates_t food_coordinates;

// LED matrix setup
#define LED_MX_MAX_DEVICES 4
const int led_mx_max_x = LED_MX_MAX_DEVICES * 8 - 1;
const int led_mx_max_y = 7;
#define LED_MX_CLK_PIN 18
#define LED_MX_DATA_PIN 23
#define LED_MX_CS_PIN 22
MD_MAX72XX led_mx = MD_MAX72XX(MD_MAX72XX::PAROLA_HW, LED_MX_CS_PIN, LED_MX_MAX_DEVICES);

#define CHAR_SPACING 1

char GAME_OVER_MESSAGE[] = "|||||||||||||||||||";
char START_MESSAGE[] = " Start!";
char SCORE_MESSAGE[] = " Score:";

enum print_text_states_t
{
    TEXT_STATE_GET_CHAR,
    TEXT_STATE_SHOW_CHAR,
    TEXT_STATE_PREPARE_BLANK,
    TEXT_STATE_SHOW_BLANK
};

#define JOYSTICK_VERT_PIN 32
#define JOYSTICK_HORZ_PIN 35
#define JOYSTICK_SEL_PIN 34
Joystick joystick(JOYSTICK_VERT_PIN, JOYSTICK_HORZ_PIN, JOYSTICK_SEL_PIN);

direction_t direction = RIGHT;
Snake snake(direction);

// Game states
enum game_states_t
{
    GAME_STATE_START,
    GAME_STATE_RUN,
    GAME_STATE_PRINT_SCORE,
};
game_states_t game_state = GAME_STATE_START;

// Function to print text on the LED matrix
void print_text(uint8_t mod_start, uint8_t mod_end, char *message);

void setup()
{
    led_mx.begin();
    joystick.init();
}

void loop()
{
    switch (game_state)
    {
    case GAME_STATE_START:
        print_text(0, LED_MX_MAX_DEVICES - 1, START_MESSAGE);
        delay(1000);

        snake.init();
        direction = RIGHT;
        food_coordinates.x = random(0, led_mx_max_x);
        food_coordinates.y = random(0, led_mx_max_y);
        led_mx.control(MD_MAX72XX::INTENSITY, MAX_INTENSITY / 2);
        game_state = GAME_STATE_RUN;
        break;

    case GAME_STATE_RUN:
        static uint16_t run_logic_period_couter = 0;
        joystick.read_direction(direction);

        if (run_logic_period_couter > GAME_RUNNING_LOGIC_PERIOD_MS)
        {
            led_mx.clear();
            led_mx.setPoint(food_coordinates.y, food_coordinates.x, true);
            for (uint8_t idx = 0; idx < snake.size; idx++)
            {
                // Set points of the snake on the LED matrix
                led_mx.setPoint(snake.coordinates[idx].y, snake.coordinates[idx].x, true);

                // Check if snake's head hits its body or not
                if (idx > 0 &&
                    (snake.coordinates[0].x == snake.coordinates[idx].x &&
                     snake.coordinates[0].y == snake.coordinates[idx].y))
                {
                    game_state = GAME_STATE_PRINT_SCORE;
                }
            }

            // Check if snake is not out of LED matrix
            if (snake.coordinates[0].x > led_mx_max_x ||
                snake.coordinates[0].y > led_mx_max_y ||
                snake.coordinates[0].x < 0 ||
                snake.coordinates[0].y < 0)
            {
                game_state = GAME_STATE_PRINT_SCORE;
            }

            // Game ends when max foods are collected
            if (snake.size >= snake.MAX_SNAKE_SIZE)
            {
                game_state = GAME_STATE_PRINT_SCORE;
            }

            led_mx.update();

            snake.run(direction);
            
            if(snake.has_eaten(food_coordinates))
            {
                food_coordinates.x = random(0, led_mx_max_x);
                food_coordinates.y = random(0, led_mx_max_y);
            }
            run_logic_period_couter = 0;
        }

        run_logic_period_couter = run_logic_period_couter + JOYSTICK_SAMPLE_PERIOD_MS;

        delay(JOYSTICK_SAMPLE_PERIOD_MS);

        break;

    case GAME_STATE_PRINT_SCORE:

        print_text(0, LED_MX_MAX_DEVICES - 1, GAME_OVER_MESSAGE);
        delay(1500);

        print_text(0, LED_MX_MAX_DEVICES - 1, SCORE_MESSAGE);
        delay(1000);
        char score[6];

        sprintf(score, "    %u", snake.score);
        print_text(0, LED_MX_MAX_DEVICES - 1, score);
        delay(1000);
        game_state = GAME_STATE_START;
        break;

    default:
        break;
    }
}

/**
 * @brief Print text on the LED matrix.
 * @param mod_start The starting module index.
 * @param mod_end The ending module index.
 * @param message The text message to be displayed.
 */
void print_text(uint8_t mod_start, uint8_t mod_end, char *message)
{
    print_text_states_t state = TEXT_STATE_GET_CHAR;
    uint8_t cur_len;
    uint16_t show_len; // actual length of char
    uint8_t buffer[8]; // keeps data about char pixels
    int16_t col = ((mod_end + 1) * COL_SIZE) - 1;

    led_mx.control(mod_start, mod_end, MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);

    do
    {
        switch (state)
        {
        case TEXT_STATE_GET_CHAR:
            if (*message == '\0')
            {
                show_len = col - (mod_end * COL_SIZE);
                state = TEXT_STATE_PREPARE_BLANK;
                break;
            }

            show_len = led_mx.getChar(*message++, sizeof(buffer) / sizeof(buffer[0]), buffer);
            cur_len = 0;
            state = TEXT_STATE_SHOW_CHAR;
			// fall throug

        case TEXT_STATE_SHOW_CHAR: 
            led_mx.setColumn(col--, buffer[cur_len++]);

            if (cur_len == show_len)
            {
                show_len = CHAR_SPACING;
                state = TEXT_STATE_PREPARE_BLANK;
            }
            break;

        case TEXT_STATE_PREPARE_BLANK: 
            cur_len = 0;
            state = TEXT_STATE_SHOW_BLANK;
			// fall through

        case TEXT_STATE_SHOW_BLANK:
            led_mx.setColumn(col--, 0);
            cur_len++;

            if (cur_len == show_len)
            {
                state = TEXT_STATE_GET_CHAR;
            }
            break;

        default:
            col = -1;
        }
    } while (col >= (mod_start * COL_SIZE));

    led_mx.control(mod_start, mod_end, MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
}
