#include <SFML/Graphics.hpp>

#include "FrameBuffer.h"
#include "SphereObject.h"
#include "Camera.h"
#include "glm/glm.hpp"
#include <vector>

glm::vec3 CastRayToScene(Camera camera, glm::vec3 PW, std::vector<SphereObject> scene)
{
    //select a sphere object
    //using eq of a ray we need to find t, 
    // find the discriminant, check the 3 possibilities
    //check the t values and keep the smaller one
    //go to next sphere object and see if the resulting new t is smaller than previous

}

int main(int argc, char ** argv)
{
    const int WIDTH  = 1280;
    const int HEIGHT = 720;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");

    FrameBuffer::Init(WIDTH, HEIGHT);

    std::string inputFile = "input.txt";
    std::string screenshotName = "screenshot.png";
    bool        takeScreenshot = false;
    
    if (argc > 1)
        inputFile = argv[1];
    if (argc > 2)
    {
        screenshotName = argv[2];
        takeScreenshot = true;
    }
	
    // Generate image and texture to display
    sf::Image   image;
    sf::Texture texture;
    sf::Sprite  sprite;
    texture.create(WIDTH, HEIGHT);
    image.create(WIDTH, HEIGHT, sf::Color::Black);

    //Debug Scene, needs a camera and a sphere
    //Camera(glm::vec3 c, glm::vec3 T, glm::vec3 U, float f) : mCPosition{ c }, mTPosition{ T }, mUpVector{ U }, mFocalLength{ f }
    glm::vec3 c = glm::vec3(0.0, 1.5, 3.5);
    glm::vec3 T = glm::vec3(0, 0, 0);
    glm::vec3 U = glm::vec3(0, 1, 0);
    float aspect = WIDTH / HEIGHT;
    float f = 1;
    Camera camera(c, T, U, f);
    
    /*************************************************************************/
    //SCENE and the vector of objects
    glm::vec3 p = glm::vec3(-0.5, 0.3, 0.8);
    glm::vec3 diff = glm::vec3(0.4, 0.7, 0.32);
    float s = 0.3;
    SphereObject sphere1(p, diff, s);
    std::vector<SphereObject> mSceneSpheres;
    mSceneSpheres.push_back(sphere1);
    /*************************************************************************/
    //get Cam view vec
    glm::vec3 View = camera.GetTPosition() - camera.GetCPosition();
    View = glm::normalize(View);
    glm::vec3 R = glm::cross(View, camera.GetUpVector());
    R = glm::normalize(R);
    //Recompute Up
    U = glm::cross(R, View);
    U = glm::normalize(U);

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
        float w_o_2 = WIDTH / 2.0f;
        float h_o_2 = HEIGHT / 2.0f;
        glm::vec3 r_o_2 = R / 2.0f;
        glm::vec3 u_o_2a = U / (2 * aspect);
        float NDC_x;
        float NDC_y;
        glm::vec3 PixelWorld;
        for (unsigned x = 0; x < WIDTH; x++)
        {
            NDC_x = ((x + 0.5f) - w_o_2) / w_o_2;
            for (unsigned y = 0; y < HEIGHT; y++)
            {
                NDC_y = (-((y + 0.5f) - h_o_2)) / h_o_2;
                //using NDC coord to create pixelWord coords
                PixelWorld = camera.GetCPosition() + (camera.GetFocalLength() * View) + (NDC_x * r_o_2) + (NDC_y * u_o_2a);
                
            }
        }
        //for (unsigned x = 0; x < WIDTH; x++)
        //{
        //    for (unsigned y = 0; y < HEIGHT; y++)
        //    {
        //        if (time % 2 == 0)
        //        {
        //            if (y % 50 < 25 && x % 50 < 25)
        //                FrameBuffer::SetPixel(x, y, 255, 0, 0);
        //            else
        //                FrameBuffer::SetPixel(x, y, 0, 255, 0);
        //        }
        //        else
        //        {
        //            if (y % 50 < 25 && x % 50 < 25)
        //                FrameBuffer::SetPixel(x, y, 0, 255, 0);
        //            else
        //                FrameBuffer::SetPixel(x, y, 255, 0, 0);
        //        }
        //    }
        //}




        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
            takeScreenshot = true;
		
        // Show image on screen
        FrameBuffer::ConvertFrameBufferToSFMLImage(image);

        texture.update(image);
        sprite.setTexture(texture);
        window.draw(sprite);
        window.display();
		
		
        if (takeScreenshot)
        {
            image.saveToFile(screenshotName);
            takeScreenshot = false;
        }
    }

    return 0;
}