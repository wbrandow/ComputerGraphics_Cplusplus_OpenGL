#ifndef SHAPE_H
#define SHAPE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Shape
{
public:
    GLuint vao;         // Handle for the vertex array object
    GLuint vbos[2];     // Handles for the vertex buffer objects
    GLuint nIndices;    // Number of indices of the mesh
    GLuint nVertices;   // Number of vertices of the mesh

    Shape();
    void createCylinder(Shape& mesh, float radiusTop, float radiusBottom, float height, int numSegments);
    void createDisk(Shape& mesh, float radius, float numSegments);
    void createPlane(Shape& mesh, float width, float length);
    void createCube(Shape& mesh, float width, float length, float height);
    void createSphere(Shape& mesh, float radius, int numSegments, int numStacks);
    void createTorus(Shape& mesh, float bigRadius, float smallRadius, int numSides, int numSegments);
    void destroyShape(Shape& mesh);
};
#endif
