#ifndef INSTANCE_UNIFORM
#define INSTANCE_UNIFORM

#include "OpenGLObjectTools.h"

struct UniformInstanceDetails{
    int index;
};

struct InstanceUniformBuffer
{
public:
    unsigned int m_uniformBuffer;
    int          m_allocatedSize;
    void*        m_uniformData;
    int          m_type;
public:
    InstanceUniformBuffer() : m_uniformBuffer(0), m_allocatedSize(0), m_uniformData(nullptr), m_type(0)
    {

    }

    void AllocateUniformBuffer(int nbytes)
    {
        m_uniformBuffer = CreateUniformBuffer(nbytes, nullptr, 1);
        m_allocatedSize = nbytes;
    }

    void ResizeUniformBuffer(int nbytes)
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_uniformBuffer);
        glInvalidateBufferData(m_uniformBuffer);
        glBufferData(GL_UNIFORM_BUFFER, m_allocatedSize+nbytes, m_uniformData, GL_DYNAMIC_DRAW);
    }

    void SetTransformsData(void* data)
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_uniformBuffer);
        glBufferData(GL_UNIFORM_BUFFER, m_allocatedSize, data, GL_DYNAMIC_DRAW);
    }

    ~InstanceUniformBuffer()
    {

    }

};

#endif