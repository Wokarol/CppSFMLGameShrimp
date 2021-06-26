uniform vec2 u_resolution;
uniform float u_t;

uniform bool u_xDirectionFlipped = true;
uniform bool u_yDirectionFlipped = true;

#define PI 3.14159265359

vec2 rotate(vec2 v, float a) {
	float s = sin(a);
	float c = cos(a);
	mat2 m = mat2(c, -s, s, c);
	return m * v;
}

void main() {
    float t = 1. - u_t;

    vec2 pos = gl_FragCoord.xy;
    
    float gridSize = 50.;
    
    // Getting in-cell coordinates
    vec2 gUV = mod(pos, gridSize);
    vec2 gID = ceil(pos / gridSize);
    
    // Bringing the range to -1..1
    gUV = ((gUV / gridSize) - 0.5) * 2.;
    
    // Getting number of cells on screen
    float maxXID = floor(u_resolution.x / gridSize);
    float maxYID = floor(u_resolution.y / gridSize);

    // Getting distance of the cell for offset timing
    float dist = 0.;
    dist += u_xDirectionFlipped 
        ? maxXID - gID.x
        : gID.x;
    dist += u_yDirectionFlipped 
        ? maxYID - gID.y
        : gID.y;
    
    // Adjusting time to fit
    float totalCellsInTime = maxXID + maxYID;
    t *= 1.1 + 0.05 * totalCellsInTime;
    t -= dist * 0.05;

    // We clamp t within 0..1 range
    t = clamp(t, 0., 1.);
    
    // Rotating the effect 45 degrees to have x and y axis in line with our cells diagonal 
    vec2 rotatedUV = rotate(gUV, PI / 4.);
    
    // We normalize the length so that -1 and 1 correspond to the corners of the cell
    rotatedUV /= sqrt(2.);
        
    // If we go on second diagonal, we rotate it again by 90 degrees to flip the axes
    if (u_xDirectionFlipped != u_yDirectionFlipped)
    	rotatedUV = rotate(rotatedUV, PI / 2.);
    
    // We bring everything into firs quadrant because of symmetry
    rotatedUV = abs(rotatedUV);
    
	// We calculate romb's size based on animation progress
    float sizeX = pow(mix(-0.01, 1.0, 1. - t), 3.);
    float sizeY = 1. - smoothstep(0.0, 1., t);
    
    // We calculate alpha to fade out towards the end
	float alpha = 1. - pow(t * 1.612, 7.);
    
    // We calculate the actual black and white mask using simple linear equation
    float mask = (sizeX * (1. - rotatedUV.x / sizeY) > rotatedUV.y) 
        ? 1.
        : 0.;

    mask *= alpha;

    gl_FragColor = vec4(vec3(0.), mask);
}