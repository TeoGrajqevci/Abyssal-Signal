// #version 330

// in vec2 fragTexCoord;  // The texture coordinates passed from the vertex shader
// out vec4 finalColor;    // The output color of the pixel

// uniform sampler2D texture0;  // The texture to which we are applying the postprocessing
// uniform vec2 iResolution; 

// void main() {
//     // Sample the original color from the texture
//     vec4 color = texture(texture0, fragTexCoord);
    
//     // Apply grayscale conversion
//     float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));  // Standard grayscale conversion factors

//     finalColor = vec4(vec3(gray), color.a);  // Maintain the original alpha value
// }

// #version 330

// in vec2 fragTexCoord;   // The texture coordinates passed from the vertex shader
// out vec4 finalColor;     // The output color of the pixel

// uniform sampler2D texture0;  // The texture being processed (the renderTexture)
// uniform vec2 iResolution;    // Screen resolution (width, height)

// float vignetteStrength = 0.5;  // Strength of the vignette effect

// void main() {
//     // Sample the original color from the texture
//     vec4 color = texture(texture0, fragTexCoord);

//     // Convert texture coordinates to normalized device coordinates (-1 to 1)
//     vec2 uv = (fragTexCoord * 2.0) - 1.0;
    
//     // Calculate the distance of the current pixel from the center
//     float dist = length(uv);

//     // Create the vignette effect using distance and vignette strength
//     float vignette = smoothstep(0.7, vignetteStrength, dist);

//     // Apply the vignette to the color (darken the edges)
//     finalColor = vec4(color.rgb * ( vignette), color.a);
// }


// #version 330

// in vec2 fragTexCoord;   // The texture coordinates passed from the vertex shader
// out vec4 finalColor;     // The output color of the pixel

// uniform sampler2D texture0;  // The texture being processed (the renderTexture)
// uniform vec2 iResolution;    // Screen resolution (width, height)

// float warp = 0.75; // simulate curvature of CRT monitor
// float scan = 0.75; // simulate darkness between scanlines

// void main (){

//     vec2 uv = fragTexCoord;
//     vec2 dc = abs(uv);
//     dc *= dc;

//     uv.x -= 0.5; uv.x *= 1.0+(dc.y*(0.3*warp)); uv.x += 0.5;
//     uv.y -= 0.5; uv.y *= 1.0+(dc.x*(0.4*warp)); uv.y += 0.5;

//      if (uv.y > 1.0 || uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0)
//         finalColor = vec4(0.0,0.0,0.0,1.0);
//     else
//     	{
//         // determine if we are drawing in a scanline
//         float apply = abs(sin(gl_FragCoord.y)*0.5*scan);
//         // sample the texture
//         finalColor = vec4(mix(texture(texture0,uv).rgb,vec3(0.0),apply),1.0);
//         }
// }

#version 330

in vec2 fragTexCoord;   // The texture coordinates passed from the vertex shader
out vec4 finalColor;     // The output color of the pixel

uniform sampler2D texture0;  // The texture being processed (the renderTexture)
uniform vec2 iResolution;    // Screen resolution (width, height)

void main() {
    // Sample the original color from the texture
    vec4 color = texture(texture0, fragTexCoord);

    finalColor = color;  // Maintain the original alpha value
}