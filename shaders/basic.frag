#version 460 core
out vec4 FragColor;

// uniform sampler2D uTex;
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCord;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

uniform vec3 cameraPos;

uniform sampler2D material_Diffuse0;
uniform sampler2D material_Diffuse1;
// uniform sampler2D material_Lightmap0;
// uniform sampler2D material_Lightmap1;
// uniform sampler2D material_Lightmap2;
uniform sampler2D material_Normals0;

void main()
{
    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), 65);

    vec3 ambient = light.ambient * vec3(texture(material_Diffuse0, TexCord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material_Diffuse0, TexCord));
    vec3 specular = light.specular * spec * vec3(1.0, 1.0, 1.0);

    vec3 lighting = (ambient + diffuse + specular);

    // FragColor = vec4(ambientColor + diffuse + specular, 0.0) * texture(material_Diffuse0, TexCord);
    FragColor = vec4(lighting , 0.0);
}
