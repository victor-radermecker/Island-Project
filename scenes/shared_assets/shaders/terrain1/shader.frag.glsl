#version 330 core

in struct fragment_data
{
    vec4 position;
    vec4 normal;
    vec4 color;
    vec2 texture_uv;
} fragment;

uniform sampler2D sand_sampler;
uniform sampler2D grass_sampler;

uniform float transition_height_sg = 4.0;
uniform float transition_height_g = 12.0;

out vec4 FragColor;

uniform vec3 camera_position;
uniform vec3 color     = vec3(0.99f, 0.83f, 0.50f);
uniform float color_alpha = 1.0;
float ambiant;
float diffuse;
uniform float specular = 0.5;
uniform int specular_exponent = 128;

uniform vec3 light;
uniform vec3 sun_color;
uniform float ambiant_sun;
uniform float diffuse_sun;


void main()
{
    ambiant = ambiant_sun;
    diffuse = diffuse_sun;

    float sandTextureAmount = 0.0;
    float grassTextureAmount = 0.0;


    float height = fragment.position[2];
    if(height < transition_height_sg)
    {
        sandTextureAmount = 1.0f;
    } else if(height < transition_height_g)
    {
        float a = 1.0 / (transition_height_sg - transition_height_g);
        float b = - a * transition_height_g;
        sandTextureAmount = a * height + b;
        grassTextureAmount = 1.0 - sandTextureAmount;
    } else {
        grassTextureAmount = 1.0;
    }

    vec4 sandTextureColor = texture(sand_sampler, fragment.texture_uv) * sandTextureAmount;
    vec4 grassTextureColor = texture(grass_sampler, fragment.texture_uv) * grassTextureAmount;

    vec4 color_texture = sandTextureColor + grassTextureColor;


    vec3 n = normalize(fragment.normal.xyz);
    vec3 u = normalize(light-fragment.position.xyz);
    vec3 r = reflect(u,n);
    vec3 t = normalize(fragment.position.xyz-camera_position);


    float diffuse_value  = diffuse * clamp( dot(u,n), 0.0, 1.0);
    float specular_value = specular * pow( clamp( dot(r,t), 0.0, 1.0), specular_exponent);


    vec3 c = (ambiant+diffuse_value)*color.rgb*fragment.color.rgb*color_texture.rgb + specular_value*sun_color;

    FragColor = vec4(c, color_texture.a*fragment.color.a*color_alpha);
}
