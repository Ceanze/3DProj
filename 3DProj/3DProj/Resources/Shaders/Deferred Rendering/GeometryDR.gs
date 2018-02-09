#version 330
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 geometryPosition[];
in vec3 geometryNormal[];
in vec3 geometryTangent[];
in vec2 geometryTextureCoord[]; 

uniform mat4 camera;
uniform vec3 camPos;
uniform vec4 normal_ortho;

out vec3 fragPosition;
out vec3 fragNormal;
out vec3 fragTangent;
out vec2 fragTextureCoord;

void main()
{
    //Calculate normal for the triangle insted of using the normals for the specific verticies.
    vec3 normal = normalize(cross(geometryPosition[1] - geometryPosition[0], geometryPosition[2] - geometryPosition[0]));
    vec3 p = (geometryPosition[0] + geometryPosition[1] + geometryPosition[2])/3.0;
    vec3 pcp = p - camPos;
    vec3 pp = -dot(pcp, normal_ortho.xyz)*normal_ortho.xyz;

    //if(normal_ortho.w == 0.0 || dot(pp, normal) > 0)
    //{
        for(int i = 0; i < gl_in.length(); i++)
        {
            vec3 pointToCam = camPos - geometryPosition[i];
            if(dot(pointToCam, normal) > 0 || normal_ortho.w == 1.0)
            {
                fragPosition = geometryPosition[i].xyz;
                fragTextureCoord = geometryTextureCoord[i];
                fragNormal = geometryNormal[i];
                fragTangent = geometryTangent[i];
                gl_Position = camera*vec4(fragPosition, 1.0);
                EmitVertex();
            }
        }
    //}
    EndPrimitive();

    


}