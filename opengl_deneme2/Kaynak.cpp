#include <GLFW/glfw3.h>
#include <iostream>
#include <conio.h>
#include <time.h>
#include <SOIL2/SOIL2.h>
#include <windows.h>
#include <Shlwapi.h>
#include <vector>
#pragma comment(lib, "shlwapi.lib")
using namespace std;
const int width = 21;
const int height = 21;
bool isOver = false,hasStarted = false;
enum direction { STOP = 0, UP, RIGHT, LEFT, DOWN };
float step = float(2) / float(width);
double xpos, ypos;





class dot {
    public:
        float x;
        float y;
        bool operator==(const dot& rhs) const {
            return (static_cast<int>(x * 100000.0) == static_cast<int>(rhs.x * 100000.0) && (static_cast<int>(y * 100000.0) == static_cast<int>(rhs.y * 100000.0)));
        };
};



dot startTopLeft;
dot startTopRight;
dot startBottomLeft;
dot startBottomRight;

dot randomDot() {
    srand(time(NULL));
    int x = (rand() % (width-2)) + 1 ;
    int y = (rand() % (height-2)) + 1;
    dot random;
    random.x = -1.0f + x * step;
    random.y = -1.0f + y * step;
    return random;
}


GLuint loadImage(const char* imagepath,bool inverty=false) {
    GLuint tex_2d;
    if (inverty)
    {
        tex_2d = SOIL_load_OGL_texture
        (
            imagepath,
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA | SOIL_FLAG_INVERT_Y
        );
    }
    else
    {
        tex_2d = SOIL_load_OGL_texture
        (
            imagepath,
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA
        );
    }
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
obj apple;
obj last_scored_apple;

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
    cout << topLeft.x << " " << topRight.x;
}
class snake :public obj {
public:
    int score = 0;
    int length = 2;
    vector<float> part_coords_x;
    vector<float> part_coords_y;
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
        if (parts[score].last_move == UP)
        {
            new_part = parts[score];
            new_part.topLeft.y-=step;
            new_part.topRight.y-=step;
            new_part.bottomRight.y-=step;
            new_part.bottomLeft.y-=step;
        }
        else if (parts[score].last_move == DOWN)
        {
            new_part = parts[score];
            new_part.topLeft.y += step;
            new_part.topRight.y += step;
            new_part.bottomRight.y += step;
            new_part.bottomLeft.y += step;
        }
        else if (parts[score].last_move == LEFT)
        {
            new_part = parts[score];
            new_part.topLeft.x += step;
            new_part.topRight.x += step;
            new_part.bottomRight.x += step;
            new_part.bottomLeft.x += step;
        }
        else if (parts[score].last_move == RIGHT)
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
        part_coords_x.clear();
        part_coords_y.clear();
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
                if (parts[0].topLeft.x < -1.0f)
                    isOver = true;
                if (parts[0].topLeft.y < -1.0f)
                    isOver = true;
                if (parts[0].topLeft.x > 1.0f)
                    isOver = true;
                if (parts[0].topLeft.y < -1.0f)
                    isOver = true;


                if (parts[0].topLeft == apple.topLeft && parts[0].topRight == apple.topRight && parts[0].bottomLeft == apple.bottomLeft && parts[0].bottomRight == apple.bottomRight)
                {
                    last_scored_apple = apple;
                    increase_score();
                    createApple();
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
                float center_of_part_x = (parts[i].topLeft.x + parts[i].topRight.x) / float(2);
                float center_of_part_y = (parts[i].topLeft.y + parts[i].bottomLeft.y) / float(2);
                part_coords_x.push_back(center_of_part_x);
                part_coords_y.push_back(center_of_part_y);
                float center_of_head_x = (parts[0].topLeft.x + parts[0].topRight.x) / float(2);
                float center_of_head_y = (parts[0].topLeft.y + parts[0].bottomLeft.y) / float(2);
                if (parts[i].topLeft == parts[0].topLeft && parts[i].topRight == parts[0].topRight && parts[i].bottomLeft == parts[0].bottomLeft && parts[i].bottomRight == parts[0].bottomRight)
                    isOver = true;
                temp = temp2;
            }
        }
        
    }
};



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
    newplayer.parts[0].part_dir = RIGHT;
    player = newplayer;
    createApple();
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && (xpos > 277 && xpos < 500) && (ypos > 450 && ypos < 520))
        hasStarted = true;
}



static void cursor_position_callback(GLFWwindow* window, double xpos1, double ypos1)
{
    xpos = xpos1;
    ypos = ypos1;
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
    DWORD length = GetModuleFileName(NULL, temp, MAX_PATH); //Get Full EXE path 
    PathRemoveFileSpec(temp); //Remove Filename
    wstring temp2(&temp[0]);  
    string path(temp2.begin(), temp2.end());  //Get path to string
    Start();
    GLFWwindow* window;
    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(800,800, "Snake", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    float last_move_time = 0;
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
        if (hasStarted)
        {
            glfwSetKeyCallback(window, key_callback);
            if (glfwGetTime() - last_move_time > 0.07 && !isOver)
            {
                player.move();
                last_move_time = glfwGetTime();
            }
            string backgroundImagePath = path + "/Graphics/back.png";
            GLuint backgroundTex = loadImage(backgroundImagePath.c_str());
            glBindTexture(GL_TEXTURE_2D, backgroundTex);
            glEnable(GL_TEXTURE_2D);
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

                    switch (player.parts[i].part_dir)
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
                    case STOP:
                        partPath = path + "/Graphics/head_up.png";
                        break;
                    default:
                        break;
                    }
                }
                else if (i == player.length - 1)
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
                    if ((player.parts[i].last_move == UP && player.parts[i - 1].last_move == UP) || (player.parts[i].last_move == DOWN && player.parts[i - 1].last_move == DOWN))
                        partPath = path + "/Graphics/body_vertical.png";
                    if ((player.parts[i].last_move == RIGHT && player.parts[i - 1].last_move == RIGHT) || (player.parts[i].last_move == LEFT && player.parts[i - 1].last_move == LEFT))
                        partPath = path + "/Graphics/body_horizontal.png";
                    if ((player.parts[i].last_move == UP && player.parts[i - 1].last_move == RIGHT) || (player.parts[i].last_move == LEFT && player.parts[i - 1].last_move == DOWN))
                        partPath = path + "/Graphics/body_bottomright.png";
                    if ((player.parts[i].last_move == UP && player.parts[i - 1].last_move == LEFT) || (player.parts[i].last_move == RIGHT && player.parts[i - 1].last_move == DOWN))
                        partPath = path + "/Graphics/body_bottomleft.png";
                    if ((player.parts[i].last_move == DOWN && player.parts[i - 1].last_move == RIGHT) || (player.parts[i].last_move == LEFT && player.parts[i - 1].last_move == UP))
                        partPath = path + "/Graphics/body_topright.png";
                    if ((player.parts[i].last_move == DOWN && player.parts[i - 1].last_move == LEFT) || (player.parts[i].last_move == RIGHT && player.parts[i - 1].last_move == UP))
                        partPath = path + "/Graphics/body_topleft.png";
                }
                GLuint partTex = loadImage(partPath.c_str());
                glBindTexture(GL_TEXTURE_2D, partTex);
                glEnable(GL_TEXTURE_2D);
                glEnable(GL_BLEND); //Enable blending.
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set blending function.
                glBegin(GL_POLYGON);
                glTexCoord2f(0, 0); glVertex3d(player.parts[i].topLeft.x, player.parts[i].topLeft.y, 0.0f);
                glTexCoord2f(1, 0); glVertex3d(player.parts[i].topRight.x, player.parts[i].topRight.y, 0.0f);
                glTexCoord2f(1, 1); glVertex3d(player.parts[i].bottomRight.x, player.parts[i].bottomRight.y, 0.0f);
                glTexCoord2f(0, 1); glVertex3d(player.parts[i].bottomLeft.x, player.parts[i].bottomLeft.y, 0.0f);
                glColor3d(1.0f, 1.0f, 1.0f);
                glDisable(GL_TEXTURE_2D);
                glEnd();
                glDeleteTextures(1, &partTex);
            }

            string applePath = path + "/Graphics/apple.png";
            GLuint appleTex = loadImage(applePath.c_str());
            glBindTexture(GL_TEXTURE_2D, appleTex);
            glEnable(GL_TEXTURE_2D);
            glBegin(GL_POLYGON);
            glTexCoord2f(0, 0); glVertex3d(apple.topLeft.x, apple.topLeft.y, 0.0f);
            glTexCoord2f(1, 0); glVertex3d(apple.topRight.x, apple.topRight.y, 0.0f);
            glTexCoord2f(1, 1); glVertex3d(apple.bottomRight.x, apple.bottomRight.y, 0.0f);
            glTexCoord2f(0, 1); glVertex3d(apple.bottomLeft.x, apple.bottomLeft.y, 0.0f);
            glEnd();
            glDisable(GL_TEXTURE_2D);
            glDeleteTextures(1, &appleTex);
            glDeleteTextures(1, &backgroundTex);

            if (isOver)
            {
                glColor4f(1.0, 1.0, 1.0, 0.5);

                glBegin(GL_QUADS);
                glVertex3f(-1.0, +1.0, 0.0); // top left
                glVertex3f(-1.0, -1.0, 0.0); // bottom left
                glVertex3f(+1.0, -1.0, 0.0); // bottom right
                glVertex3f(+1.0, +1.0, 0.0); // top right
                glEnd();
                glColor4f(1.0, 1.0, 1.0, 1.0);
                string gameoverpath = path + "/Graphics/gameover.png";
                GLuint goTex = loadImage(gameoverpath.c_str(), true);
                glEnable(GL_TEXTURE_2D);

                glBegin(GL_QUADS);
                glTexCoord2f(0, 0); glVertex2f(-0.5f, 0.0f);
                glTexCoord2f(1, 0); glVertex2f(0.5f, 0.0f);
                glTexCoord2f(1, 1); glVertex2f(0.5f, 0.5f);
                glTexCoord2f(0, 1); glVertex2f(-0.5f, 0.5f);
                glEnd();
                glDeleteTextures(1, &goTex);

            }


        }
        else
        {
        string menubackpath = path + "/Graphics/menu_back.jpg";
        GLuint menuback = loadImage(menubackpath.c_str(),true);
        glBindTexture(GL_TEXTURE_2D, menuback);
        glEnable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(-1, -1);
        glTexCoord2f(1, 0); glVertex2f(1, -1);
        glTexCoord2f(1,1); glVertex2f(1, 1);
        glTexCoord2f(0, 1); glVertex2f(-1, 1);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glDeleteTextures(1, &menuback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glEnable(GL_BLEND); //Enable blending.
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set blending function.
        glEnable(GL_TEXTURE_2D);
        glfwSetCursorPosCallback(window, cursor_position_callback);
        cout << ypos << endl;
        string startpath;
        if((xpos>277 && xpos<500) && (ypos>450 && ypos<520))
           startpath = path + "/Graphics/start_active.png",true;
        else
           startpath = path + "/Graphics/start_inactive.png";
        GLuint startTex = loadImage(startpath.c_str(),true);
        glEnable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(-0.5f, -0.5f);
        glTexCoord2f(1, 0); glVertex2f(0.5f, -0.5f);
        glTexCoord2f(1, 1); glVertex2f(0.5f, 0.0f);
        glTexCoord2f(0, 1); glVertex2f(-0.5f, 0.0f);
        glEnd();
        glDeleteTextures(1, &startTex);
        }

        glfwSwapBuffers(window);
        
        glfwPollEvents();
    }



    glfwTerminate();
    return 0;
}