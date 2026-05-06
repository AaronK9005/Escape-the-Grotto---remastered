#include <conio.h>
#include <ctype.h>

#include "input_handle.h"

#define ARROW_PREFIX 224
#define ARROW_UP 72
#define ARROW_LEFT 75
#define ARROW_RIGHT 77
#define ARROW_DOWN 80

void input_poll(input_handle_t* ih) {
    if (!ih) return;

    if (_kbhit())
    {
        int input = tolower(_getch());
        
        if (input == ARROW_PREFIX && _kbhit())
        {
            switch (_getch())
            {
            case ARROW_UP:      ih->_char = INPUT_ARROW_UP;     break;
            case ARROW_LEFT:    ih->_char = INPUT_ARROW_LEFT;   break;
            case ARROW_DOWN:    ih->_char = INPUT_ARROW_DOWN;   break;
            case ARROW_RIGHT:   ih->_char = INPUT_ARROW_RIGHT;  break;
            }
        }
        else
        {
            ih->_char = input;
        }
    }
    else
    {
        ih->_char = INPUT_NONE;
    }
}