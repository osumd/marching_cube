#ifndef INPUT_CONTAINER_H
#define INPUT_CONTAINER_H
#include <OpenGLToolBox.h>

#include <vector>

struct Input
{
    void* object;
    void (*ClickConversion)(void* object, GLFWwindow* window, cameraBaseClass* camera, double xpos, double ypos);

    Input() : object(nullptr), ClickConversion(nullptr)
    {

    }

};

struct InputContainer
{
    std::vector<Input> Inputs;

    void SendClick(GLFWwindow* window, cameraBaseClass* camera, double xpos, double ypos)
    {
        for(int i = 0; i < Inputs.size(); i++)
        {
            if(Inputs[i].ClickConversion != nullptr && Inputs[i].object != nullptr)
            {
                Inputs[i].ClickConversion(Inputs[i].object, window, camera, xpos, ypos);
            }
        }
    }
};

InputContainer GlobalInputContainer;

#endif