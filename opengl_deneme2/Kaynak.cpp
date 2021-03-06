#include <GLFW/glfw3.h>
#include <iostream>
#include <conio.h>
#include <time.h>
#include <SOIL2/SOIL2.h>
#include <windows.h>
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
using namespace std;
const int width = 21;
const int height = 21;
bool isOver = false;
enum direction { STOP = 0, UP, RIGHT, LEFT, DOWN };
float step = float(2) / float(width);


class dot {
    public:
        float x;
        float y;
        bool operator==(const dot& rhs) const {
            return (rhs.x == x && rhs.y == y);
        };
};




dot startTopLeft;
dot startTopRight;
dot startBottomLeft;
dot startBottomRight;

dot randomDot() {
    srand(time(NULL));
    int x = rand() % (width-1);
    int y = rand() % (height-1);
    dot random;
    random.x = -1.0f + x * step;
    random.y = -1.0f + y * step;
    return random;
}


GLuint loadImage(const char* imagepath) {

    GLuint tex_2d = SOIL_load_OGL_texture
    (
        imagepath,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA
    );
    return tex_2d;
}

class obj {
public:
    dot topLeft;
    dot topRight;
    dot bottomRight;
    dot bottomLeft;
};

class snakepart :public obj {
public:
    direction last_move = STOP;
    direction part_dir = STOP;
};

class snake :public obj {
public:
    int score = 0;
    int length = 2;
    snakepart parts[width * height];
    snake(dot startTopLeft, dot startTopRight, dot startBottomRight, dot startBottomLeft) {
        topLeft = startTopLeft;
        topRight = startTopRight;
        bottomRight = startBottomRight;
        bottomLeft = startBottomLeft;
        snakepart first_part;
        first_part.topLeft = topLeft;
        first_part.topRight = topRight;
        first_part.bottomRight = bottomRight;
        first_part.bottomLeft = bottomLeft;
        parts[0] = first_part;
    }
    void change_direction(direction dir)
    {
        parts[0].part_dir = dir;
        move();
    }
    void increase_score()
    {
        snakepart new_part;
        new_part.part_dir = parts[score].part_dir;
        if (parts[score].part_dir == UP)
        {
            new_part = parts[score];
            new_part.topLeft.y-=step;
            new_part.topRight.y-=step;
            new_part.bottomRight.y-=step;
            new_part.bottomLeft.y-=step;
        }
        else if (parts[score].part_dir == DOWN)
        {
            new_part = parts[score];
            new_part.topLeft.y += step;
            new_part.topRight.y += step;
            new_part.bottomRight.y += step;
            new_part.bottomLeft.y += step;
        }
        else if (parts[score].part_dir == LEFT)
        {
            new_part = parts[score];
            new_part.topLeft.x += step;
            new_part.topRight.x += step;
            new_part.bottomRight.x += step;
            new_part.bottomLeft.x += step;
        }
        else if (parts[score].part_dir == RIGHT)
        {
            new_part = parts[score];
            new_part.topLeft.x -= step;
            new_part.topRight.x -= step;
            new_part.bottomRight.x -= step;
            new_part.bottomLeft.x -= step;
        }
        score++;
        length++;
        parts[score] = new_part;
    }
    void move() {
        snakepart temp;
        for (int i = 0; i < length+1; i++)
        {
            if (i == 0)
            {
                temp = parts[i];
                parts[i].last_move = parts[i].part_dir;
                switch (parts[i].part_dir)
                {
                case UP:
                    parts[i].topLeft.y += step;
                    parts[i].topRight.y += step;
                    parts[i].bottomRight.y += step;
                    parts[i].bottomLeft.y += step;
                    break;
                case DOWN:
                    parts[i].topLeft.y -= step;
                    parts[i].topRight.y -= step;
                    parts[i].bottomLeft.y -= step;
                    parts[i].bottomRight.y -= step;
                    break;
                case RIGHT:
                    parts[i].topLeft.x += step;
                    parts[i].topRight.x += step;
                    parts[i].bottomLeft.x += step;
                    parts[i].bottomRight.x += step;
                    break;
                case LEFT:
                    parts[i].topLeft.x -= step;
                    parts[i].topRight.x -= step;
                    parts[i].bottomLeft.x -= step;
                    parts[i].bottomRight.x -= step;
                    break;
                default:
                    break;
                }
                
            }
            else
            {
                snakepart temp2 = parts[i];
                parts[i] = temp;
                if (temp2.topLeft == temp.bottomLeft && temp2.topRight == temp.bottomRight)
                    parts[i].last_move = UP;
                if (temp2.bottomLeft == temp.topLeft && temp2.bottomRight == temp.topRight)
                    parts[i].last_move = DOWN;
                if (temp2.topLeft == temp.topRight && temp2.bottomRight == temp.bottomLeft)
                    parts[i].last_move = RIGHT;
                if (temp2.topRight == temp.topLeft && temp2.bottomLeft == temp.bottomRight)
                    parts[i].last_move = LEFT;
                temp = temp2;
            }
        }
        
    }
};

obj apple;
void createApple() {
    dot topLeft = randomDot();
    dot topRight;
    dot bottomRight;
    dot bottomLeft;
    topRight.y = topLeft.y;
    topRight.x = topLeft.x + step;
    bottomRight.x = topRight.x;
    bottomRight.y = topRight.y - step;
    bottomLeft.x = topLeft.x;
    bottomLeft.y = bottomRight.y;
    apple.topLeft = topLeft;
    apple.topRight = topRight;
    apple.bottomRight = bottomRight;
    apple.bottomLeft = bottomLeft;
}
snake player(startTopLeft, startTopRight, startBottomRight, startBottomLeft);
void Start() {
    isOver = false;
    startTopLeft.x = -float(1) / float(width);
    startTopLeft.y = float(1) / float(width);
    startTopRight.x = float(1) / float(width);
    startTopRight.y = float(1) / float(width);
    startBottomRight.x = float(1) / float(width);
    startBottomRight.y = -float(1) / float(width);
    startBottomLeft.x = -float(1) / float(width);
    startBottomLeft.y = -float(1) / float(width);
    snake newplayer(startTopLeft, startTopRight, startBottomRight, startBottomLeft);
    player = newplayer;
    createApple();
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_W && action == GLFW_PRESS && player.parts[0].part_dir != DOWN)
    {
        player.change_direction(UP);
        return;
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS && player.parts[0].part_dir != UP)
    {
        player.change_direction(DOWN);
        return;
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS && player.parts[0].part_dir != LEFT)
    {
        player.change_direction(RIGHT);
        return;
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS && player.parts[0].part_dir != RIGHT)
    {
        player.change_direction(LEFT);
        return;
    }
}









int main(void)
{
    TCHAR temp[MAX_PATH];
    DWORD length = GetModuleFileName(NULL, temp, MAX_PATH);
    PathRemoveFileSpec(temp);
    wstring temp2(&temp[0]); 
    string path(temp2.begin(), temp2.end()); 
    Start();
    GLFWwindow* window;
    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(1080,1080, "Snake", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    float last_move_time = 0;
    obj last_scored_apple;
    last_scored_apple.topLeft.x = 0;
    last_scored_apple.topLeft.y = 0;
    last_scored_apple.topRight.x = 0;
    last_scored_apple.topRight.y = 0;
    last_scored_apple.bottomLeft.x = 0;
    last_scored_apple.bottomLeft.y = 0;
    last_scored_apple.bottomRight.x = 0;
    last_scored_apple.bottomRight.y = 0;
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSetKeyCallback(window, key_callback);
        if (glfwGetTime() - last_move_time > 0.05)
        {
            player.move();
            last_move_time = glfwGetTime();
            if (sqrt(pow(player.parts[0].topLeft.x - apple.topLeft.x, 2) + pow(player.parts[0].topLeft.y - apple.topLeft.y, 2)) < 0.001 && last_scored_apple.topLeft.x != apple.topLeft.x)
            {
                last_scored_apple = apple;
                player.increase_score();
                createApple();
            }
            for (int i = 0; i < player.length; i++)
            {
                cout << player.parts[i].part_dir << " ";
            }
            cout << endl;
        }
        string backgroundImagePath = path + "/Graphics/back.png";
        glBindTexture(GL_TEXTURE_2D, loadImage(backgroundImagePath.c_str()));
        glEnable(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(-1, -1);
        glTexCoord2f(80, 0); glVertex2f(1, -1);
        glTexCoord2f(80, 80); glVertex2f(1, 1);
        glTexCoord2f(0, 80); glVertex2f(-1, 1);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        for (int i = 0; i < player.length; i++)
        {
            string partPath;
            if (i == 0)
            {
                
                switch(player.parts[i].part_dir)
                {
                case UP:
                    partPath = path + "/Graphics/head_up.png";
                    break;
                case DOWN:
                    partPath = path + "/Graphics/head_down.png";
                    break;
                case LEFT:
                    partPath = path + "/Graphics/head_left.png";
                    break;
                case RIGHT:
                    partPath = path + "/Graphics/head_right.png";
                    break;
                default:
                    break;
                }
            }
            else if (i == player.length-1)
            {

                switch (player.parts[i].part_dir)
                {
                case UP:
                    partPath = path + "/Graphics/tail_down.png";
                    break;
                case DOWN:
                    partPath = path + "/Graphics/tail_up.png";
                    break;
                case LEFT:
                    partPath = path + "/Graphics/tail_right.png";
                    break;
                case RIGHT:
                    partPath = path + "/Graphics/tail_left.png";
                    break;
                default:
                    break;
                }
            }
            else
            {
                if((player.parts[i].last_move == UP && player.parts[i-1].last_move == UP) || (player.parts[i].last_move == DOWN && player.parts[i-1].last_move == DOWN))
                    partPath = path + "/Graphics/body_vertical.png";
                if ((player.parts[i].last_move == RIGHT && player.parts[i - 1].last_move == RIGHT) || (player.parts[i].last_move == LEFT && player.parts[i - 1].last_move == LEFT))
                    partPath = path + "/Graphics/body_horizontal.png";
                if ((player.parts[i].last_move == UP && player.parts[i - 1].last_move == RIGHT) || (player.parts[i].last_move == LEFT && player.parts[i-1].last_move == DOWN))
                    partPath = path + "/Graphics/body_bottomright.png";
                if ((player.parts[i].last_move == UP && player.parts[i - 1].last_move == LEFT) || (player.parts[i].last_move == RIGHT && player.parts[i-1].last_move == DOWN))
                    partPath = path + "/Graphics/body_bottomleft.png";
                if ((player.parts[i].last_move == DOWN && player.parts[i - 1].last_move == RIGHT) || (player.parts[i].last_move == LEFT && player.parts[i-1].last_move == UP)) 
                    partPath = path + "/Graphics/body_topright.png";
                if ((player.parts[i].last_move == DOWN && player.parts[i - 1].last_move == LEFT) || (player.parts[i].last_move == RIGHT && player.parts[i-1].last_move == UP))
                    partPath = path + "/Graphics/body_topleft.png";
            }
            glBindTexture(GL_TEXTURE_2D, loadImage(partPath.c_str()));
            glEnable(GL_TEXTURE_2D);
            glEnable(GL_BLEND); //Enable blending.
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set blending function.
            glBegin(GL_POLYGON);
            glTexCoord2f(0, 0); glVertex3d(player.parts[i].topLeft.x, player.parts[i].topLeft.y, 0.0f);
            glTexCoord2f(1, 0); glVertex3d(player.parts[i].topRight.x, player.parts[i].topRight.y, 0.0f);
            glTexCoord2f(1, 1); glVertex3d(player.parts[i].bottomRight.x, player.parts[i].bottomRight.y, 0.0f);
            glTexCoord2f(0, 1); glVertex3d(player.parts[i].bottomLeft.x, player.parts[i].bottomLeft.y, 0.0f);
            glColor3d(1.0f, 1.0f, 1.0f);
            glEnd();
        }

        string applePath = path + "/Graphics/apple.png";
        glBindTexture(GL_TEXTURE_2D, loadImage(applePath.c_str()));
        glEnable(GL_TEXTURE_2D);
        glBegin(GL_POLYGON);
        glTexCoord2f(0, 0); glVertex3d(apple.topLeft.x, apple.topLeft.y, 0.0f);
        glTexCoord2f(1, 0); glVertex3d(apple.topRight.x, apple.topRight.y, 0.0f);
        glTexCoord2f(1, 1); glVertex3d(apple.bottomRight.x, apple.bottomRight.y, 0.0f);
        glTexCoord2f(0, 1); glVertex3d(apple.bottomLeft.x, apple.bottomLeft.y, 0.0f);
        glEnd();




        glfwSwapBuffers(window);
        

        glfwPollEvents();
    }



    glfwTerminate();
    return 0;
}