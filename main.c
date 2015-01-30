/* 
 * File:   main.c
 * Author: Paulostation
 *
 * Created on January 29, 2015, 10:52 PM
 */

#include <windows.h> /* for HANDLE type, and console functions */
#include <stdio.h> /* standard input/output */
#include <stdlib.h> /* included for rand */
#include <time.h> /* used to make delays */
#include <string.h>
#define WIDTH 70
#define HEIGHT 35
#define SIZE (WIDTH - 1)*(HEIGHT - 1)
HANDLE wHnd; /* write (output) handle */
HANDLE rHnd; /* read (input handle */

//snake 'object'
int snake[500] = {75, 76, 77, 78, 79, 80, 81, 82, 83};
//second array used to make the movement illusion
int snake2[500];
//general purpose integers
int x, y, i, cont, direction = 1, prey;


/* Method used to get array size */
int size(int array[]) {
    int i, cont = 0;
    for (i = 0; array[i] != '\0'; i++) {
        cont++;
    }
    return cont;
}

DWORD getInput(INPUT_RECORD **eventBuffer) {
    /* Variable for holding the number of current events, and a point to it */
    DWORD numEvents = 0;

    /* Variable for holding how many events were read */
    DWORD numEventsRead = 0;

    /* Put the number of console input events into numEvents */
    GetNumberOfConsoleInputEvents(rHnd, &numEvents);

    if (numEvents) /* if there's an event */ {
        /* Allocate the correct amount of memory to store the events */
        *eventBuffer = malloc(sizeof (INPUT_RECORD) * numEvents);

        /* Place the stored events into the eventBuffer pointer */
        ReadConsoleInput(rHnd, *eventBuffer, numEvents, &numEventsRead);
    }

    /* Return the amount of events successfully read */
    return numEventsRead;
}

int main(void) {

    srand(time(0));
    DWORD i;

    /* A pointer to type INPUT_RECORD, used to point to a buffer of INPUT_RECORD structs */
    INPUT_RECORD *eventBuffer;

    /* Variable for holding how many events were read */
    DWORD numEventsRead = 0;

    SMALL_RECT windowSize = {0, 0, WIDTH - 1, HEIGHT - 1};

    COORD bufferSize = {WIDTH, HEIGHT};

    COORD characterBufferSize = {WIDTH, HEIGHT};
    COORD characterPosition = {0, 0};
    SMALL_RECT consoleWriteArea = {0, 0, WIDTH - 1, HEIGHT - 1};

    CHAR_INFO consoleBuffer[WIDTH * HEIGHT];

    wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    rHnd = GetStdHandle(STD_INPUT_HANDLE);


    SetConsoleTitle("Snake-DOS");

    SetConsoleWindowInfo(wHnd, TRUE, &windowSize);

    SetConsoleScreenBufferSize(wHnd, bufferSize);

    for (y = 0; y < HEIGHT; ++y) {
        for (x = 0; x < WIDTH; ++x) {
            consoleBuffer[x + WIDTH * y].Char.AsciiChar = (unsigned char) 219;
            consoleBuffer[x + WIDTH * y].Attributes = 0;
        }
    }

    for (i = 0; i < size(snake2); ++i) {
        consoleBuffer[snake2[i]].Char.AsciiChar = (unsigned char) 219;
        consoleBuffer[snake2[i]].Attributes = 255;
    }

    prey = rand() % 2000;
    consoleBuffer[prey].Char.AsciiChar = (unsigned char) 219;
    consoleBuffer[prey].Attributes = 255;
    WriteConsoleOutputA(wHnd, consoleBuffer, characterBufferSize, characterPosition, &consoleWriteArea);
    /* Main game loop */
    while (1) {

        /* Get the input and number of events successfully obtained */
        numEventsRead = getInput(&eventBuffer);

        /* if more than 0 are read */
        if (numEventsRead) {
            /* loop through the amount of records */
            for (i = 0; i < numEventsRead; i++) {
                /* check each event */
                switch (eventBuffer[i].EventType) {
                        /* if type of event is a KEY_EVENT */
                    case KEY_EVENT:
                        switch (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode) {
                                /* if escape key is pressed*/
                            case VK_ESCAPE:
                                system("PAUSE");
                            case 0x26:
                                if (direction == WIDTH) break;
                                direction = -WIDTH;
                                break;
                            case 0x27:
                                if (direction == -1) break;
                                direction = 1;
                                break;
                            case 0x28:
                                if (direction == -WIDTH) break;
                                direction = WIDTH;
                                break;
                            case 0x25:
                                if (direction == 1) break;
                                direction = -1;
                                break;
                        }
                }
            }
        }
        /* Copy the primary array to the secondary array shifting a pixel */
        for (i = 0; i < size(snake2); i++) {
            snake2[i] = snake[i + 1];
        }
        /* Modify secondary array with the direction of the object */
        snake2[size(snake2)] = snake2[size(snake2) - 1] + direction;
        /* Write the changes on the console buffer */
        for (i = 0; i < size(snake2); ++i) {
            consoleBuffer[snake2[i]].Char.AsciiChar = (unsigned char) 219;
            consoleBuffer[snake2[i]].Attributes = 255;
        }
        /* Collision test with prey*/
        if (snake2[size(snake2) - 1] == prey) {
            snake2[size(snake2)] = snake2[size(snake2) - 1] + direction;
            /* Generate a new position for the prey */
            prey = rand() % 2346;
            consoleBuffer[prey].Char.AsciiChar = (unsigned char) 219;
            consoleBuffer[prey].Attributes = 255;
        }
        consoleBuffer[snake2[0]].Char.AsciiChar = (unsigned char) 219;
        consoleBuffer[snake2[0]].Attributes = 0;
        consoleBuffer[prey].Attributes = 255;
        /* Frame update */
        WriteConsoleOutputA(wHnd, consoleBuffer, characterBufferSize, characterPosition, &consoleWriteArea);
        /* Copy the secondary array state into the primary to make the loop continue */
        for (i = 0; i < size(snake2); i++) {
            snake[i] = snake2[i];
        }
        
        Sleep(50);
    }
}