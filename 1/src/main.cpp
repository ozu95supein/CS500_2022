#include <SFML/Graphics.hpp>

#include "FrameBuffer.h"
#include "SphereObject.h"
#include "Camera.h"
#include "glm/glm.hpp"
#include <vector>
#include "SceneParser.h"

struct Ray 
{
    Ray(glm::vec3 p, glm::vec3 v)
    {
        RayOrigin_p = p;
        direction_v = v;
    }
    glm::vec3 RayOrigin_p;
    glm::vec3 direction_v;
};
//returns a color in vec3 form with ranges from 0.0 to 1.0
glm::vec3 CastRayToScene(Camera camera, Ray r, std::vector<SphereObject> scene)
{
    //t values to compare which one is the closest
    float current_t = INFINITY;
    float smallest_t = INFINITY;
    //return black by default
    glm::vec3 nearest_sphere_color = glm::vec3(0.0f, 0.0f, 0.0f);
    //iterate through the objects and find the smallest t all the while keeping track of the color
    for (auto it = scene.begin(); it != scene.end(); ++it)
    {
        glm::vec3 CP = (r.RayOrigin_p - it->GetCenter()); //Sphere to Cam vector
        float sphere_radius = it->GetScale();                     //radius of sphere
        //gain discriminant
        auto a = glm::dot(r.direction_v, r.direction_v);
        auto b = 2.0f * (glm::dot(r.direction_v, CP));
        auto c = glm::dot(CP, CP) - (sphere_radius * sphere_radius);
        float discriminant = b * b - (4.0f * a * c);
        if (discriminant < 0)
        {
            //No intersection, onto the next object
            continue;
        }
        else if (discriminant > 0)
        {
            //2 intercetions, take the smaller one (the one witht he subtraction)
            current_t = ((-2.0f * (glm::dot(r.direction_v, CP))) - glm::sqrt(discriminant)) / (2 * (glm::dot(r.direction_v, r.direction_v)));
             
        }
        else if (discriminant == 0)
        {
            current_t = -(glm::dot(r.direction_v, CP)) / (glm::dot(r.direction_v, r.direction_v));
        }
        //check if t is smaller
        if (current_t < smallest_t)
        {
            smallest_t = current_t;
            nearest_sphere_color = it->GetMaterialDiffuse();
        }
    }
    return nearest_sphere_color;
}

int main(int argc, char ** argv)
{
    const int WIDTH  = 1280;
    const int HEIGHT = 720;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");

    FrameBuffer::Init(WIDTH, HEIGHT);

    std::string inputFile = "input.txt";
    std::string inputFile1 = "/scene/A1.txt";

    ParseSceneFromFile("A1.txt");

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
    float aspect = (float)WIDTH / (float)HEIGHT;
    float f = 1;
    Camera camera(c, T, U, f);
    
    /*************************************************************************/
    //SCENE and the vector of objects
    std::vector<SphereObject> mSceneSpheres;
    //sphere1
    glm::vec3 p = glm::vec3(0.0, 0.5, 0.0);
    glm::vec3 diff = glm::vec3(0.45, 0.78, 0.66);
    float s = 0.5;
    SphereObject sphere1(p, diff, s);
    //sphere2
    p = glm::vec3(0.6, 0.5, 0.8);
    diff = glm::vec3(0.87, 0.88, 0.71);
    s = 0.5;
    SphereObject sphere2(p, diff, s);
    //sphere3
    p = glm::vec3(-0.6, 0.5, -0.8);
    diff = glm::vec3(0.88, 0.72, 0.4);
    s = 0.5;
    SphereObject sphere3(p, diff, s);

    mSceneSpheres.push_back(sphere1);
    mSceneSpheres.push_back(sphere2);
    mSceneSpheres.push_back(sphere3);

    /*************************************************************************/
    //get Cam view vec
    auto cTgt = camera.GetTPosition();
    auto cPos = camera.GetCPosition();
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
        float w_o_2 = WIDTH * 0.5f;
        float h_o_2 = HEIGHT * 0.5f;
        glm::vec3 r_o_2 = R * 0.5f;
        glm::vec3 u_o_2a = U / (2 * aspect);
        float NDC_x;
        float NDC_y;
        
        glm::vec3 PixelWorld;
        Ray ray(camera.GetCPosition(), glm::vec3(0.0f, 0.0f, 0.0f));
        //glm::vec3 result_color = CastRayToScene(DebugCamera, r, mSceneSpheres);
        glm::vec3 result_color;
        
        for (unsigned x = 0; x < WIDTH; x++)
        {
            NDC_x = ((x + 0.5f) - w_o_2) / w_o_2;
            for (unsigned y = 0; y < HEIGHT; y++)
            {
                NDC_y = (-((y + 0.5f) - h_o_2)) / h_o_2;
                //using NDC coord to create pixelWord coords
                PixelWorld = camera.GetCPosition() + (camera.GetFocalLength() * View) + (NDC_x * r_o_2) + (NDC_y * u_o_2a);
                glm::vec3 RayDir = PixelWorld - ray.RayOrigin_p;  //get ray direction from origin and Pixelworld
                RayDir = glm::normalize(RayDir);
                ray.direction_v = RayDir;   //update the ray direction according to PixelWorld Coords
                result_color = CastRayToScene(camera, ray, mSceneSpheres);  //cast the ray using camera, the ray, and the vector of sphere objects

                glm::vec3 result_color_255(result_color.x * 255.0f, result_color.y * 255.0f, result_color.z * 255.0f);
                FrameBuffer::SetPixel(x, y, result_color_255.x, result_color_255.y, result_color_255.z);
            }
        }

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