#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <string>

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;
const int GRID_SIZE = 3;
int board[GRID_SIZE][GRID_SIZE] = {0};

bool isXTurn = true;
std::string gameStatus = "";
bool gameEnded = false;

// Function to draw an X in the specified cell
void drawX(int row, int col) {
    float cellWidth = 2.0f / GRID_SIZE;
    float xStart = -1.0f + col * cellWidth;
    float yStart = -1.0f + row * cellWidth;

    glColor3f(1.0f, 0.0f, 0.0f);
    glLineWidth(3.0f);

    glBegin(GL_LINES);
    glVertex2f(xStart + 0.1f, yStart + 0.1f);
    glVertex2f(xStart + cellWidth - 0.1f, yStart + cellWidth - 0.1f);
    glVertex2f(xStart + 0.1f, yStart + cellWidth - 0.1f);
    glVertex2f(xStart + cellWidth - 0.1f, yStart + 0.1f);
    glEnd();
}

// Function to draw an O in the specified cell
void drawO(int row, int col) {
    float cellWidth = 2.0f / GRID_SIZE;
    float xCenter = -1.0f + (col + 0.5f) * cellWidth;
    float yCenter = -1.0f + (row + 0.5f) * cellWidth;

    glColor3f(0.0f, 0.0f, 1.0f);
    glLineWidth(3.0f);

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; ++i) {
        float theta = i * 3.14159f / 180.0f;
        float x = 0.3f * cos(theta);
        float y = 0.3f * sin(theta);
        glVertex2f(xCenter + x, yCenter + y);
    }
    glEnd();
}


void drawGrid() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(5.0f);

    // Vertical grid lines
    for (int i = 1; i < GRID_SIZE; ++i) {
        float x = -1.0f + (2.0f / GRID_SIZE) * i;
        glBegin(GL_LINES);
        glVertex2f(x, -1.0f);
        glVertex2f(x, 1.0f);
        glEnd();
    }

    // Horizontal grid lines
    for (int i = 1; i < GRID_SIZE; ++i) {
        float y = -1.0f + (2.0f / GRID_SIZE) * i;
        glBegin(GL_LINES);
        glVertex2f(-1.0f, y);
        glVertex2f(1.0f, y);
        glEnd();
    }

    // Draw the current state of the board
    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE; ++col) {
            if (board[row][col] == 1)
                drawX(row, col);
            else if (board[row][col] == 2)
                drawO(row, col);
        }
    }

    // Draw the game status if the game has ended
    if (gameEnded) {
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(-0.5f, -0.1f);
        glVertex2f(0.5f, -0.1f);
        glVertex2f(0.5f, 0.1f);
        glVertex2f(-0.5f, 0.1f);
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(-0.4f, 0.0f);
        for (char c : gameStatus) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }

    glutSwapBuffers();
}

bool checkWin(int player) {
    // Check rows
    for (int i = 0; i < GRID_SIZE; i++) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) {
            return true;
        }
    }

    // Check columns
    for (int i = 0; i < GRID_SIZE; i++) {
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player) {
            return true;
        }
    }

    // Check diagonals
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player) {
        return true;
    }
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player) {
        return true;
    }

    return false;
}

// Function to handle mouse click events
void handleMouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !gameEnded) {
        // Convert mouse coordinates to grid coordinates
        int row = (WINDOW_HEIGHT - y) / (WINDOW_HEIGHT / GRID_SIZE);
        int col = x / (WINDOW_WIDTH / GRID_SIZE);

        if (board[row][col] == 0)  // If the cell is empty
        {
            if (isXTurn) {
                board[row][col] = 1;  // Place X
                std::cout << "X placed at (" << row << ", " << col << ")\n";
            } else {
                board[row][col] = 2;  // Place O
                std::cout << "O placed at (" << row << ", " << col << ")\n";
            }

            // Check for win conditions
            if (checkWin(isXTurn ? 1 : 2)) {
                gameStatus = "Winner is " + std::string(isXTurn ? "X" : "O");
                gameEnded = true;
                std::cout << gameStatus << "\n";
            } else {
                // Check for draw
                bool draw = true;
                for (int i = 0; i < GRID_SIZE; i++) {
                    for (int j = 0; j < GRID_SIZE; j++) {
                        if (board[i][j] == 0) {
                            draw = false;
                            break;
                        }
                    }
                }
                if (draw) {
                    gameStatus = "Draw";
                    gameEnded = true;
                    std::cout << gameStatus << "\n";
                }
            }

            isXTurn = !isXTurn;  // Switch turn
        }

        glutPostRedisplay();  // Redraw the screen
    }
}

void initializeGL() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Tic-Tac-Toe");


    glutDisplayFunc(drawGrid);
    glutMouseFunc(handleMouseClick);

    // Set up OpenGL state
    initializeGL();

    // Enter GLUT event loop
    glutMainLoop();
    return 0;
}
