#include <SFML/Graphics.hpp>
#include "FrameBuffer.h"
#include "SphereObject.h"
#include "Camera.h"
#include "glm/glm.hpp"
#include <vector>
#include "SceneParser.h"
#include "RayCasting.h"


int main(int argc, char ** argv)
{
    bool debug_ambience = true;     //if true, this will use the default ambience specified in raycasting.h instead of the ambience in the file
    //default values for commandline argument data
    int WIDTH = 500;
    int HEIGHT = 500;
    std::string inputFile = "A3.txt";
    std::string screenshotName = "screenshot.png";
    bool        takeScreenshot = false;
    int bounces = 5;
    //receive commandline arguments 
    if (argc > 1)
        inputFile = argv[1];
    if (argc > 2)
    {
        screenshotName = argv[2];
        takeScreenshot = true;
    }
    if (argc > 3)
    {
        WIDTH = atoi(argv[3]);
    }
    if (argc > 4)
    {
        HEIGHT = atoi(argv[4]);
    }
    if (argc > 5)
    {
        bounces = atoi(argv[5]);
    }
    //DEGUG SCENE
    //inputFile = "A_DEBUG.txt";

    //Seed random number generator
    //std::srand(std::time(nullptr));

    //inti window and frame buffer with correct data
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "cs500_oscar.s_2");
    FrameBuffer::Init(WIDTH, HEIGHT);

    //create a scene struct to store all the scene info from the txt
    SceneStruct mCurrentScene;
    ParseSceneFromFile(inputFile, mCurrentScene);
    // Generate image and texture to display
    sf::Image   image;
    sf::Texture texture;
    sf::Sprite  sprite;
    texture.create(WIDTH, HEIGHT);
    image.create(WIDTH, HEIGHT, sf::Color::Black);

    //Debug Scene, needs a camera and a sphere
    //Camera(glm::vec3 c, glm::vec3 T, glm::vec3 U, float f) : mCPosition{ c }, mTPosition{ T }, mUpVector{ U }, mFocalLength{ f }
    glm::vec3 U = glm::vec3(0, 1, 0);
    float aspect = (float)WIDTH / (float)HEIGHT;
 
    //get Cam view vec
    glm::vec3 View = mCurrentScene.mSceneCamera.GetTPosition() - mCurrentScene.mSceneCamera.GetCPosition();
    View = glm::normalize(View);
    glm::vec3 R = glm::cross(View, mCurrentScene.mSceneCamera.GetUpVector());
    R = glm::normalize(R);
    //Recompute Up
    U = glm::cross(R, View);
    U = glm::normalize(U);
    
    //boolean to render the image, this will only do this once per button press
    bool Need_To_RenderFullImage = true;
    // Init the clock
    sf::Clock clock;
    while (window.isOpen())
    {
        // Handle input
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window.close();

        // Fill framebuffer
        sf::Time elapsed = clock.getElapsedTime();
        int      time    = static_cast<int>(elapsed.asSeconds());

        //get NDC coords
        float w_o_2 = WIDTH * 0.5f;
        float h_o_2 = HEIGHT * 0.5f;
        glm::vec3 r_o_2 = R * 0.5f;
        glm::vec3 u_o_2a = U / (2 * aspect);
        float NDC_x;
        float NDC_y;
        
        glm::vec3 PixelWorld;
        Ray ray(mCurrentScene.mSceneCamera.GetCPosition(), glm::vec3(0.0f, 0.0f, 0.0f));
        //glm::vec3 result_color = CastRayToScene(DebugCamera, r, mSceneSpheres);
        glm::vec3 result_color;
        if (Need_To_RenderFullImage)
        {
            for (unsigned x = 0; x < WIDTH; x++)
            {
                NDC_x = ((x + 0.5f) - w_o_2) / w_o_2;
                for (unsigned y = 0; y < HEIGHT; y++)
                {
                    NDC_y = (-((y + 0.5f) - h_o_2)) / h_o_2;
                    //using NDC coord to create pixelWord coords
                    PixelWorld = mCurrentScene.mSceneCamera.GetCPosition() + (mCurrentScene.mSceneCamera.GetFocalLength() * View) + (NDC_x * r_o_2) + (NDC_y * u_o_2a);
                    glm::vec3 RayDir = PixelWorld - ray.RayOrigin_p;  //get ray direction from origin and Pixelworld
                    RayDir = glm::normalize(RayDir);
                    ray.direction_v = RayDir;   //update the ray direction according to PixelWorld Coords

                    //glm::vec3 CastRayRecursiveBounce(SceneStruct scene, Ray r, int RemainingBounces)
                    result_color = CastRayRecursiveBounce(mCurrentScene, ray, bounces, debug_ambience);
                    //convert to 255 format and set the pixel in the frame buffer
                    glm::vec3 result_color_255(result_color.x * 255.0f, result_color.y * 255.0f, result_color.z * 255.0f);
                    FrameBuffer::SetPixel(x, y, result_color_255.x, result_color_255.y, result_color_255.z);
                }
                ////////////////////// TAKE CONTENT IN FRAME BUFFER AND SHOW TO SCREEN ////////////////////////
                // Show image on screen
                FrameBuffer::ConvertFrameBufferToSFMLImage(image);

                texture.update(image);
                sprite.setTexture(texture);
                window.draw(sprite);
                window.display();
                //////////////////////////////////////////////
            }
            Need_To_RenderFullImage = false;
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
        {
            takeScreenshot = true;
        }
        //TODO ADD A RESTART RENDER BUTTON
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F5))
        {
            Need_To_RenderFullImage = true;
        }
        if (takeScreenshot)
        {
            image.saveToFile(screenshotName);
            takeScreenshot = false;
        }
    }
    return 0;
}