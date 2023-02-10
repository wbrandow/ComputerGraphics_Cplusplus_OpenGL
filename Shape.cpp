

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Shape.h>
#include <iostream>
#include <vector>


    Shape::Shape()
    {
        vao = NULL;        
        vbos[0] = NULL;
        vbos[1] = NULL;
        nIndices = NULL; 
        nVertices = NULL;
    }

    // Implements the createCylinder function
    void Shape::createCylinder(Shape& mesh, float radiusTop, float radiusBottom, float height, int numSegments)
    {
        std::vector<GLfloat> vertices;

        float angle;
        float nextAngle;
        int nVertices = 0;

        for (int i = 0; i < numSegments; i++)
        {
            /*******************************
             *  First Triangle of Segment  *
             *******************************/
            angle = ((360.0 / numSegments) * i) * (3.14159 / 180);
            nextAngle = ((360.0 / numSegments) * (i + 1)) * (3.14159 / 180);

            // top vertex
            vertices.push_back(cos(angle) * radiusTop); // x 
            vertices.push_back(sin(angle) * radiusTop); // y
            vertices.push_back(height / 2); // z

            // top vertex normal
            vertices.push_back(cos(angle)); // x
            vertices.push_back(sin(angle)); // y
            vertices.push_back(0.0f); // z
            

            //top vertex texture coordinate
            if (i == 0 || i % 2 == 0)
            {
                vertices.push_back(0.0f);
                vertices.push_back(1.0f);
            }
            else
            {
                vertices.push_back(1.0f);
                vertices.push_back(1.0f);
            }

            // bottom vertex
            vertices.push_back(cos(angle) * radiusBottom); // x 
            vertices.push_back(sin(angle) * radiusBottom); // y
            vertices.push_back(-height / 2); // z

            // bottom vertex normal
            vertices.push_back(cos(angle)); // x
            vertices.push_back(sin(angle)); // y
            vertices.push_back(0.0f); // z
            

            //bottom vertex texture coordinate
            if (i == 0 || i % 2 == 0)
            {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }
            else
            {
                vertices.push_back(1.0f);
                vertices.push_back(0.0f);
            }

            // next top vertex
            vertices.push_back(cos(nextAngle) * radiusTop); // x 
            vertices.push_back(sin(nextAngle) * radiusTop); // y
            vertices.push_back(height / 2); // z

            // next top vertex normal
            vertices.push_back(cos(nextAngle)); // x
            vertices.push_back(sin(nextAngle)); // y
            vertices.push_back(0.0f); // z

            // next top vertex texture coordinate
            if (i == 0 || i % 2 == 0)
            {
                vertices.push_back(1.0f);
                vertices.push_back(1.0f);
            }
            else
            {
                vertices.push_back(0.0f);
                vertices.push_back(1.0f);
            }

            /********************************
             *  Second Triangle of Segment  *
             ********************************/
            
            // bottom vertex again
            // position
            vertices.push_back(cos(angle) * radiusBottom); // x 
            vertices.push_back(sin(angle) * radiusBottom); // y
            vertices.push_back(-height / 2); // z

            // bottom vertex normal
            vertices.push_back(cos(angle)); // x
            vertices.push_back(sin(angle)); // y
            vertices.push_back(0.0f); // z


            //bottom vertex texture coordinate
            if (i == 0 || i % 2 == 0)
            {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }
            else
            {
                vertices.push_back(1.0f);
                vertices.push_back(0.0f);
            }

            // next top vertex again
            // position
            vertices.push_back(cos(nextAngle) * radiusTop); // x 
            vertices.push_back(sin(nextAngle) * radiusTop); // y
            vertices.push_back(height / 2); // z

            // next top vertex normal
            vertices.push_back(cos(nextAngle)); // x
            vertices.push_back(sin(nextAngle)); // y
            vertices.push_back(0.0f); // z

            // next top vertex texture coordinate
            if (i == 0 || i % 2 == 0)
            {
                vertices.push_back(1.0f);
                vertices.push_back(1.0f);
            }
            else
            {
                vertices.push_back(0.0f);
                vertices.push_back(1.0f);
            }

            // next bottom vertex
            // position
            vertices.push_back(cos(nextAngle) * radiusBottom); // x 
            vertices.push_back(sin(nextAngle) * radiusBottom); // y
            vertices.push_back(-height / 2); // z

            // next bottom vertex normal
            vertices.push_back(cos(nextAngle)); // x
            vertices.push_back(sin(nextAngle)); // y
            vertices.push_back(0.0f); // z


            // next bottom vertex texture coordinate
            if (i == 0 || i % 2 == 0)
            {
                vertices.push_back(1.0f);
                vertices.push_back(0.0f);
            }
            else
            {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }

            nVertices += 6;
        }

        const GLuint floatsPerVertex = 3;
        const GLuint floatsPerNormal = 3;
        const GLuint floatsPerTex = 2;

        glGenVertexArrays(1, &mesh.vao);
        glBindVertexArray(mesh.vao);

        // Create 2 buffers: first one for the vertex data; second one for the indices
        glGenBuffers(2, mesh.vbos);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

        // Data for the indices
        std::vector<GLushort> indices;
        int triangleNum = 0;
        for (int i = 0; i < (nVertices * 3) - 6; i += 3)
        {
            indices.push_back(triangleNum);
            indices.push_back(triangleNum + 1);
            indices.push_back(triangleNum + 2);

            triangleNum++;
        }
        indices.push_back(nVertices - 2);
        indices.push_back(nVertices - 1);
        indices.push_back(0);
        indices.push_back(nVertices - 1);
        indices.push_back(0);
        indices.push_back(1);


        mesh.nIndices = indices.size();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(short), &indices[0], GL_STATIC_DRAW);

        GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerTex);// The number of floats before each

        // Create Vertex Attribute Pointers
        glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, floatsPerTex, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
        glEnableVertexAttribArray(2);
    }

    void Shape::createDisk(Shape& mesh, float radius, float numSegments)
    {
        std::vector<GLfloat> vertices;

        float angle;
        float nextAngle;
        nVertices = 0;

        for (int i = 0; i < numSegments; i++)
        {
            // center vertex
            // position
            vertices.push_back(0.0f);  // x
            vertices.push_back(0.0f);  // y
            vertices.push_back(0.0f);  // z
            
            // center vertex normal
            vertices.push_back(0.0f); // x
            vertices.push_back(0.0f); // y
            vertices.push_back(1.0f); // z
            
            //center vertex texture
            vertices.push_back(0.5f);  // texture x
            vertices.push_back(0.5f);  // texture y
            
            angle = ((360.0 / numSegments) * i) * (3.14159 / 180);
            nextAngle = ((360.0 / numSegments) * i) * (3.14159 / 180);

            // position of vertex
            vertices.push_back(cos(angle) * radius); // x 
            vertices.push_back(sin(angle) * radius); // y
            vertices.push_back(0.0f); // z

            // vertex normal
            vertices.push_back(0.0f); // x
            vertices.push_back(0.0f); // y
            vertices.push_back(1.0f); // z

            // vertex texture
            vertices.push_back(cos(angle));  // x
            vertices.push_back(sin(angle));  // y

            // next vertex
            // position
            vertices.push_back(cos(angle) * radius); // x 
            vertices.push_back(sin(angle) * radius); // y
            vertices.push_back(0.0f); // z

            // next vertex normal
            vertices.push_back(0.0f); // x
            vertices.push_back(0.0f); // y
            vertices.push_back(1.0f); // z

            // next vertex texture
            vertices.push_back(cos(angle));  // x
            vertices.push_back(sin(angle));  // y

            nVertices += 3;
        }

        const GLuint floatsPerVertex = 3;
        const GLuint floatsPerNormal = 3;
        const GLuint floatsPerTex = 2;

        glGenVertexArrays(1, &mesh.vao);
        glBindVertexArray(mesh.vao);

        // Create 2 buffers: first one for the vertex data; second one for the indices
        glGenBuffers(2, mesh.vbos);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

        // Data for the indices
        std::vector<GLushort> indices;
        int triangleNum = 0;
        for (int i = 0; i < numSegments - 1; i++)
        {
            indices.push_back(0);
            indices.push_back(triangleNum + 1);
            indices.push_back(triangleNum + 2);

            triangleNum++;
        }
        indices.push_back(0);
        indices.push_back(numSegments);
        indices.push_back(1);

        mesh.nIndices = indices.size();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(short), &indices[0], GL_STATIC_DRAW);

        GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerTex);// The number of floats before each

        // Create Vertex Attribute Pointers
        glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, floatsPerTex, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
        glEnableVertexAttribArray(2);
    }

    void Shape::createPlane(Shape& mesh, float width, float length)
    {
        float x = width;
        float z = length;
        nVertices = 6;

        GLfloat vertices[] = {
            -x, 0.0f, -z,  // front-left
            0.0f, 1.0f, 0.0f,  // normal
            0.0f, 0.0f,  // texture bottom-left

            x, 0.0f, -z,  // front-right
            0.0f, 1.0f, 0.0f,  // normal
            1.0f, 0.0f,  // texture bottom-right

            -x, 0.0f, z,  // back-left
            0.0f, 1.0f, 0.0f,  // normal
            0.0f, 5.0f,  // texture top-left

            x, 0.0f, -z,  // front-right
            0.0f, 1.0f, 0.0f,  // normal
            1.0f, 0.0f,  // texture bottom-right

            -x, 0.0f, z,  // back-left
            0.0f, 1.0f, 0.0f,  // normal
            0.0f, 5.0f,  // texture top-left

            x, 0.0f, z, // back-right
            0.0f, 1.0f, 0.0f,  // normal
            1.0f, 5.0f  // texture top-right
        };

        GLushort indices[] = {
            0, 1, 2, 1, 2, 3
        };

        const GLuint floatsPerVertex = 3;
        const GLuint floatsPerNormal = 3;
        const GLuint floatsPerTex = 2;

        glGenVertexArrays(1, &mesh.vao);
        glBindVertexArray(mesh.vao);

        // Create 2 buffers: first one for the vertex data; second one for the indices
        glGenBuffers(2, mesh.vbos);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

        mesh.nIndices = 6;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerTex);// The number of floats before each

        // Create Vertex Attribute Pointers
        glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, floatsPerTex, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
        glEnableVertexAttribArray(2);
    }


    void Shape::destroyShape(Shape& mesh)
    {
        glDeleteVertexArrays(1, &mesh.vao);
        glDeleteBuffers(2, mesh.vbos);
    }






