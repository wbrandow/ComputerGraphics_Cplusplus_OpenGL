#version 330 core

in vec3 vertexFragmentPos;
in vec3 vertexNormal;
in vec2 TexCoord;

out vec4 fragmentColor;

uniform vec3 objectColor;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 cameraPos;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform int useTexture = 0;

void main()
{
    //Calculate Ambient lighting
    float ambientStrength = 0.2f; // ambient lighting strength.
    vec3 ambient = ambientStrength * lightColor;

    //Calculate Diffuse lighting
    vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit.
    vec3 lightDirection = normalize(lightPosition - vertexFragmentPos); // Calculate distance between light source and fragments
    float impact = max(dot(norm, lightDirection), 0.0);// Calculate diffuse impact by generating dot product of normal and light.
    vec3 diffuse = impact * lightColor; 

    //Calculate Specular lighting
    float specularIntensity = 0.8f; // specular light strength.
    float highlightSize = 16.0f; // specular highlight size.
    vec3 viewDir = normalize(cameraPos - vertexFragmentPos); // Calculate view direction.
    vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector.
    //Calculate specular component.
    float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
    vec3 specular = specularIntensity * specularComponent * lightColor;

    vec4 textureColor;
    vec3 phongResult;

    // Texture color
    if (useTexture == 0){
        textureColor = texture(texture0, TexCoord);
        // Calculate Phong result.
        phongResult = (ambient + diffuse + specular) * textureColor.xyz;
    }
    else if (useTexture == 1){
        textureColor = texture(texture1, TexCoord);
        // Calculate Phong result.
        phongResult = (ambient + diffuse + specular) * textureColor.xyz;
    }
    else if (useTexture == 2){
        textureColor = texture(texture2, TexCoord);
        // Calculate Phong result.
        phongResult = (ambient + diffuse + specular) * textureColor.xyz;
    }
    else if (useTexture == 3){
        textureColor = texture(texture3, TexCoord);
        // Calculate Phong result.
        phongResult = (ambient + diffuse + specular) * textureColor.xyz;
    }
    else if (useTexture == 99){
        phongResult = (ambient + diffuse + specular) * objectColor;
    }

    fragmentColor = vec4(phongResult, 1.0f); // Send lighting results to GPU.
}
