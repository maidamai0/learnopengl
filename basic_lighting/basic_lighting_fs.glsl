#version 400
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 ViewPos;

void main() {
    // calculate the ambient component
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // calculate diffuse component
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // calculate specular component
    float specular_strength = 0.5;
    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specular_strength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}