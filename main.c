/* 
 * File:   test.c
 * Author: Paulo
 *
 * Created on January 29, 2015, 10:52 PM
 */

#include <windows.h> /* for HANDLE type, and console functions */
#include <stdio.h> /* standard input/output */
#include <stdlib.h> /* included for rand */
#include <time.h> /* usado para delay */
#include <string.h>
#define WIDTH 70
#define HEIGHT 35
#define SIZE (WIDTH - 1)*(HEIGHT - 1)
HANDLE wHnd; /* write (output) handle */
HANDLE rHnd; /* read (input handle */

int snake[500] = {75, 76, 77, 78, 79, 80, 81, 82, 83}, snake2[500];
int x, y, i, cont, direction = 1, rato;


//method to reset the game
int gameReset() {
    
    COORD characterBufferSize = {WIDTH, HEIGHT};
    COORD characterPosition = {0, 0};
    SMALL_RECT consoleWriteArea = {0, 0, WIDTH - 1, HEIGHT - 1};
    CHAR_INFO consoleBuffer[WIDTH * HEIGHT];

    for (i = 0; i < 500; i++) {
        snake[i] = 0;
    }
    for (i = 0; i < 500; i++) {
        snake2[i] = 0;
    }
    for (i = 0; i < tamanho2(); i++) {
        consoleBuffer[snake2[i]].Char.AsciiChar = (unsigned char) 219;
        consoleBuffer[snake2[i]].Attributes = 255;
    }
    snake[0] = 75;
    snake[1] = 76;
    snake[2] = 77;
    snake[3] = 78;
    snake[4] = 79;
    snake[5] = 80;
    snake[6] = 81;
    snake[7] = 82;
    snake[8] = 83;
    for (i = 0; i < 8; i++) {
        consoleBuffer[snake[i]].Char.AsciiChar = (unsigned char) 219;
        consoleBuffer[snake[i]].Attributes = 255;
    }
    WriteConsoleOutputA(wHnd, consoleBuffer, characterBufferSize, characterPosition, &consoleWriteArea);
    printf("Perdeu jovem =/ \n\n");
    system("PAUSE");

}

//função para pegar o tamanho do vetor secundário

int size() {
    int i, cont = 0;
    for (i = 0; snake[i] != '\0'; i++) {
        cont++;
    }
    return cont;
}

//função para pegar o tamanha do vetor principal

int tamanho2() {
    int i, cont = 0;
    for (i = 0; snake2[i] != '\0'; i++) {
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

    for (i = 0; i < tamanho2(); ++i) {
        consoleBuffer[snake2[i]].Char.AsciiChar = (unsigned char) 219;
        consoleBuffer[snake2[i]].Attributes = 255;
    }

    rato = rand() % 2000;
    consoleBuffer[rato].Char.AsciiChar = (unsigned char) 219;
    consoleBuffer[rato].Attributes = 255;
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
        // troca de estado anterior para estado atual 
        for (i = 0; i < size(); i++) {
            snake2[i] = snake[i + 1];
        }
        // direcionamento do objeto    
        snake2[tamanho2()] = snake2[tamanho2() - 1] + direction;
        // escrita de estado atual
        for (i = 0; i < tamanho2(); ++i) {
            consoleBuffer[snake2[i]].Char.AsciiChar = (unsigned char) 219;
            consoleBuffer[snake2[i]].Attributes = 255;
        }
        //teste de colisão   
        if (snake2[tamanho2() - 1] == rato) {
            snake2[tamanho2()] = snake2[tamanho2() - 1] + direction;
            rato = rand() % 2346;
            consoleBuffer[rato].Char.AsciiChar = (unsigned char) 219;
            consoleBuffer[rato].Attributes = 255;
        }
        consoleBuffer[snake2[0]].Char.AsciiChar = (unsigned char) 219;
        consoleBuffer[snake2[0]].Attributes = 0;
        consoleBuffer[rato].Attributes = 255;
        WriteConsoleOutputA(wHnd, consoleBuffer, characterBufferSize, characterPosition, &consoleWriteArea);
        //teste de colisão com o objeto
        /*
        for (i = 9; i > 0; i--) {
            if (snake2[tamanho2() - 1] == snake[i]) {
                for (y = 0; y < HEIGHT; ++y) {
                    for (x = 0; x < WIDTH; ++x) {
                        consoleBuffer[x + WIDTH * y].Char.AsciiChar = (unsigned char) 219;
                        consoleBuffer[x + WIDTH * y].Attributes = 0;
                    }
                }
                WriteConsoleOutputA(wHnd, consoleBuffer, characterBufferSize, characterPosition, &consoleWriteArea);
                resetaJogo();
            }
        }*/
        for (i = 0; i < tamanho2(); i++) {
            snake[i] = snake2[i];
        }
        Sleep(50);

    }
}
