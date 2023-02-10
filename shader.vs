#version 330 core

layout (location = 0) in vec3 position; // VAP position 0 for vertex position data
layout (location = 1) in vec3 normal; // VAP position 1 for normals
layout (location = 2) in vec2 textureCoord;

out vec3 vertexNormal; // For outgoing normals to fragment shader
out vec3 vertexFragmentPos; // For outgoing color or pixels to fragment shader
out vec2 TexCoord;

//Uniform or Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f); // Transforms vertices into clip coordinates

    vertexFragmentPos = vec3(model * vec4(position, 1.0f)); // Gets fragment or pixel position in world space only (excludes view and projection)

    vertexNormal = mat3(transpose(inverse(model))) * normal; // Gets normal vectors in world space only and excludes normal translation properties

    TexCoord = textureCoord;
}
