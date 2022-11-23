heightmap has three textures combined by a splatmap in bumpFragment

ambient, diffuse and specular lighting are implemented in combineFragment and a directional light is traditionally rendered in bumpFragment

heightmap has three bump maps, also combined by a splatmap in bumpFragment

skybox is rendered in combineFragment wherever depth buffer is 1

real time reflections are rendered on water in waterFragment and are combined with refraction and a standard water texture

camera moves and rotates automatically

animated robot guy is the child of a scene graph (Renderer.cpp line 332)

robot guy mesh and spider mesh are placed in scene (Renderer.cpp line 332 and 82)

extra textures are loaded for the heightmap (Renderer.cpp line 26) and combined in bumpFragment

a direction light is rendered on the heightMap in bumpFragment

deferred rendering is included, pretty much the same as the tutorial, although i significantly bumped up the ambient

physically correct attenuation can be turned on in the UI and is handled in pointLightFragment but when enabled the edges of light volumes can be seen

particle system is included, before the first frame MAX_PARTICLES are initiliased as dead (Renderer.cpp line 39). Particles are organised in an array and shifted around at runtime so that all alive particles are at the front, 
followed by all the dead ones, with particleIndex variable to track where the boundary is. UpdateParticles runs UpdateParticle on every alive particle and returns false when it is time for a particle to die (Renderer.cpp line 583).
When a particle dies it swaps places in the array with the last alive particle (Renderer.cpp line 591). Particles are renderred using instanced rendering, with model matrices and colour information stored in VBOs. Particles must 
always face camera so modelView matrix is used containing transposed view matrix (Renderer.cpp line 677). Due to VBO size limitiations modelView matrix is split into 4 columns, with one VBO for each. Every frame particleIndex 
particles are instanced renderred (Renderer.cpp line 731).

Perlin noise is used to slightly alter the terrain each time the game is run (HeightMap.cpp line 36), this can be done mid game by pressing the 0 key. I found a perlin noise library online, 
I didn't make it myself (nclgl/PerlinNoise.hpp).

Instanced rendering used for particle system.

Particles are sorted each frame for correct transparent rendering (Renderer.cpp line 649)

tesselation and geometry shaders used for grass, grassDomain, grassHull and grassGeometry

UBO used to store model, view and projection matrices (Renderer.cpp line 248) (also added a vec4 for fogColour and a bool for drawing normals as frag colours right at the end of the coursework, these should really be somewhere else)
VBOs used for particle instanced rendering

skeletal animation on robot guy and spider (the spider animation can be hard to see but it is sort of breathing)

skeletal animation shader (skinningVertex) calculates normals for lighting calculations, these can be seen by checking the "Draw Normals as Colours" option in the UI.

animated geometry is achieved in the grassGeometry shader, moving each blade of grass with a slightly different sin function (i dont know if this was really necessary but I kept adding one to my random seed each time I used 
it to get different values so thats why you see things like base.xz + 1, base.xz + 2 etc. Each blade of grass is a line primitive, the first vertex emitted is the peak, which sways using a sin function, the second vertex
is the base, with a base position with a random offset from the point received from the tesselation shaders. I found the rand function on stackoverflow, I have absolutely no idea how it works other than it takes a vec2 
as a seed and returns a float.

A fog effect is achieved on the terrain in bumpFragment (I probably should have done the same thing to the rest of the objects) by interpolating fragColour between the fogless colour and a fog colour that can be set in the UI based on 
distance from the camera. fogFactor is calculated in bumpVertex and passed to bumpFragment, it increased exponentially with distance instead of linearly for increased realism. This is why I made the camera go so high at the end of 
the automatic sequence so you can see the whole terrain turn to the fog colour.

Projective texturing is used for the water reflection and refraction in the waterFragment shader.


I tried to make a portal effect using a similar technique but once I implemented deferred rendering it pretty much just broke. If you move the camera to about 3000,3000,11000 (you can manually move the camera through the UI), 
you will see two quads, what you see through the left quad should be what you would see through the right quad were you at the same position relative to the right quad but you can see that the lighting is just all out of whack.

I should probably elaborate a little on how the water works. Each frame, everything that should be seen in the water (this can be changed in the UI) is renderred 3 times. Once normally with nothing to do with the water, once to an FBO 
with the camera underwater to obtain a reflection texture (Renderer.cpp line 1172) and once to another FBO with the camera in its normal position to obtain a refraction texture (Renderer.cpp line 1255). These 2 textures are then
combined and projected onto the water quad. The whole water renderring sequence is handled by RenderReflection, I really should have renamed this to RenderWater.