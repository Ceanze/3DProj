#version 330
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 geometryPosition[];
in vec3 geometryNormal[];
in vec2 geometryTextureCoord[]; 
in vec3 geometryTangent[];

uniform mat4 camera;
uniform vec3 camPos;

out vec3 fragPosition;
out vec3 fragNormal;
out vec2 fragTextureCoord;
out vec3 fragTangent;

void main()
{
    //Calculate normal for the triangle insted of using the normals for the specific verticies.
    vec3 normal = normalize(cross(geometryPosition[1] - geometryPosition[0], geometryPosition[2] - geometryPosition[0]));
    
    for(int i = 0; i < gl_in.length(); i++)
    {
        vec3 pointToCam = camPos - geometryPosition[i];
        if(dot(pointToCam, normal) > 0)
        {
            fragPosition = geometryPosition[i];
            fragTextureCoord = geometryTextureCoord[i];
            fragNormal = geometryNormal[i];
            fragTangent = geometryTangent[i];
            gl_Position = camera*vec4(geometryPosition[i], 1);
            EmitVertex();
        }
    }
    EndPrimitive();
}