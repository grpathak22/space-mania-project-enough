#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector> // for managing asteroids
#include <cmath>  // for trigonometric calculations

// Define the spaceship's position and speed
float spaceshipX = 0.0f;
float spaceshipY = 0.0f;
float spaceshipSpeed = 0.01f;
bool isGameActive = true;
int points = 0;
double lastIncrementTime = glfwGetTime();


// Define asteroid properties
struct Asteroid {
    float x;
    float y;
    float size;
    float speed;
};

std::vector<Asteroid> asteroids; // Vector to store asteroids

// Function to generate a random number between min and max
float randomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (max - min);
}

// Function to draw a star at a random position
void drawStar() {
    glColor3f(1.0f, 1.0f, 1.0f); // White color for stars
    glBegin(GL_POINTS);
    glVertex2f(randomFloat(-1.0f, 1.0f), randomFloat(-1.0f, 1.0f));
    glEnd();
}

// Function to draw an asteroid
void drawAsteroid(float x, float y, float size) {
    glColor3f(0.5f, 0.5f, 0.5f); // Gray color for asteroids
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 30) {
        float angle = static_cast<float>(i) * 3.14159265359f / 180.0f;
        float xOffset = size * cos(angle);
        float yOffset = size * sin(angle);
        glVertex2f(x + xOffset, y + yOffset);
    }
    glEnd();
}

void updatePoints() {
    double currentTime = glfwGetTime();
    if (currentTime - lastIncrementTime >= 1.0) { // Increment points every second
        points++;
        std::cout << points;
        lastIncrementTime = currentTime;
    }
}

// Function to draw a spaceship with a cat face
void drawSpaceship(float angleDegrees) {
    glColor3f(1.0f, 0.0f, 0.0f); // Red color for spaceship

    // Apply rotation transformation
    glPushMatrix();
    glTranslatef(spaceshipX, spaceshipY, 0.0f);
    glRotatef(angleDegrees, 0.0f, 0.0f, 1.0f); 

    // Draw spaceship body (pentagon)
    glBegin(GL_POLYGON);
    glVertex2f(-0.03f, 0.0f);
    glVertex2f(-0.02f, 0.05f);
    glVertex2f(0.02f, 0.05f);
    glVertex2f(0.03f, 0.0f);
    glVertex2f(0.0f, -0.06f);
    glEnd();

    // Draw cat ears (triangles)
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.5f, 0.0f); // Black color for ears
    glVertex2f(-0.03f, 0.0f);
    glVertex2f(-0.05f, 0.03f);
    glVertex2f(-0.01f, 0.03f);

    glVertex2f(0.03f, 0.0f);
    glVertex2f(0.05f, 0.03f);
    glVertex2f(0.01f, 0.03f);
    glEnd();

    // Draw cat eyes (triangles)
    glColor3f(1.0f, 1.0f, 1.0f); // White color for eyes
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.015f, 0.02f);
    glVertex2f(-0.01f, 0.02f);
    glVertex2f(-0.0125f, 0.025f);

    glVertex2f(0.015f, 0.02f);
    glVertex2f(0.01f, 0.02f);
    glVertex2f(0.0125f, 0.025f);
    glEnd();

    // Restore the transformation matrix
    glPopMatrix();
}

// Function to initialize asteroids
void initializeAsteroids() {
    asteroids.clear();
    for (int i = 0; i < 10; ++i) {
        Asteroid asteroid;
        asteroid.x = randomFloat(-1.0f, 1.0f);
        asteroid.y = randomFloat(-1.0f, 1.0f);
        asteroid.size = randomFloat(0.02f, 0.1f);
        asteroid.speed = randomFloat(0.001f, 0.005f);
        asteroids.push_back(asteroid);
    }
}

bool isCollision(float pointX, float pointY, float circleX, float circleY, float circleRadius) {
    float distance = sqrt((pointX - circleX) * (pointX - circleX) + (pointY - circleY) * (pointY - circleY));
    return distance < circleRadius;
}

void restartGame() {
    isGameActive = false;
    points = 0;
    std::cout << "Directed by Robert. Game Restarted!" << std::endl;
    // Reset spaceship position
    spaceshipX = 0.0f;
    spaceshipY = 0.0f;
    // Reinitialize asteroids
    initializeAsteroids();
}

// Function to update asteroid positions
void updateAsteroids() {
    for (Asteroid& asteroid : asteroids) {
        asteroid.x -= asteroid.speed;
        if (asteroid.x < -1.1f) {
            asteroid.x = 1.1f;
            asteroid.y = randomFloat(-1.0f, 1.0f);
            asteroid.size = randomFloat(0.02f, 0.1f);
            asteroid.speed = randomFloat(0.001f, 0.005f);
        }
    }
}

// Function to handle keyboard input for controlling the spaceship
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_LEFT) {
            spaceshipX -= spaceshipSpeed;
        }
        else if (key == GLFW_KEY_RIGHT) {
            spaceshipX += spaceshipSpeed;
        }
        else if (key == GLFW_KEY_UP) {
            spaceshipY += spaceshipSpeed;
        }
        else if (key == GLFW_KEY_DOWN) {
            spaceshipY -= spaceshipSpeed;
        }
    }
}

int main() {
    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Spaceship", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Set the keyboard callback function
    glfwSetKeyCallback(window, keyCallback);

    // Initialize asteroids
    initializeAsteroids();

    while (!glfwWindowShouldClose(window)) {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw stars in the background
        if (isGameActive) {
            glPointSize(2.0f); // Set the point size for stars
            for (int i = 0; i < 100; ++i) {
                drawStar();
            }

            // Updating the game points
            updatePoints();

            // Draw asteroids
            for (const Asteroid& asteroid : asteroids) {
                drawAsteroid(asteroid.x, asteroid.y, asteroid.size);

                // Check for collision with the spaceship
                if (isCollision(spaceshipX, spaceshipY, asteroid.x, asteroid.y, asteroid.size)) {
                    restartGame();
                }
            }

            // Draw the spaceship
            drawSpaceship(90.0f);

            // Update asteroid positions
            updateAsteroids();
        }
        else {
            // Draw restart screen
            glColor3f(1.0f, 0.0f, 0.0f); // Black color
            glBegin(GL_QUADS);
            glVertex2f(-1.0f, -1.0f);
            glVertex2f(-1.0f, 1.0f);
            glVertex2f(1.0f, 1.0f);
            glVertex2f(1.0f, -1.0f);
            glEnd();

            // Check for Enter key press to restart the game
            if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
                isGameActive = true; // Switch back to the game state
                initializeAsteroids(); // Reinitialize asteroids
                spaceshipX = 0.0f; // Reset spaceship position
                spaceshipY = 0.0f;
            }
        }

            // Swap buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();

    return 0;
}
