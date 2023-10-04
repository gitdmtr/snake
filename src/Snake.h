
#ifndef SNAKE
#define SNAKE


typedef struct 
{
  int8_t x;
  int8_t y;
} coordinates_t;

class Snake 
{
    public:       
        Snake(direction_t init_dir);
        void init();
        void run(direction_t new_dir);
        bool has_eaten(coordinates_t food_coord);
        
        static const uint8_t MAX_SNAKE_SIZE = 100;
        static const uint8_t INIT_SNAKE_SIZE = 3;
        uint8_t size = INIT_SNAKE_SIZE;
        coordinates_t coordinates[MAX_SNAKE_SIZE];
        uint8_t score; 
        
    private:
        static const uint8_t HEAD_INIT_X = 31;
        direction_t current_dir;
};

#endif