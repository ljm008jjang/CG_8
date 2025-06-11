#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

// 카메라 위치 (필요하긴 하나 specular 없으므로 사용 안함)
uniform vec3 viewPos;

// Directional light direction (normalized, from 위 요구사항)
uniform vec3 lightDir;

// 조명 파라미터
uniform vec3 Ia;          // Ambient light intensity (global ambient)
uniform vec3 lightAmbient;   // Directional light ambient color (0,0,0)
uniform vec3 lightDiffuse;   // Directional light diffuse color (1,1,1)
uniform vec3 lightSpecular;  // Directional light specular color (0,0,0)

// 재질 파라미터
uniform vec3 ka;  // Ambient reflectivity (1,1,1)
uniform vec3 kd;  // Diffuse reflectivity (1,1,1)
uniform vec3 ks;  // Specular reflectivity (0,0,0)
uniform float shininess;  // 0, unused

void main()
{
    vec3 N = normalize(Normal);
    vec3 L = normalize(-lightDir);  // 빛이 오는 방향이 -lightDir임

    // Ambient: 전역 ambient * 재질 ka
    vec3 ambient = Ia * ka;

    // Diffuse: kd * lightDiffuse * max(dot(N,L),0)
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = kd * lightDiffuse * diff;

    // Specular 없음 (ks=0 or shininess=0)
    vec3 specular = vec3(0.0);

    vec3 color = ambient + diffuse + specular;

    FragColor = vec4(color, 1.0);
}
