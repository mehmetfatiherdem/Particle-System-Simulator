#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"


// Shaders and Rendering Pipeline

/*

	----------- 0-What is rendering pipeline? -----------
	Rendering pipeline is a series of stages that take place
	in order to render an image to the screen.

	Four stages are programmable via Shaders.

	Shaders are pieces of code written in GLSL

	----------- 1-Rendering Pipeline Stages -----------
	1.Vertex Specification
	2.Vertex Shader(programmable)
	3.Tessellation(programmable)
	4.Geometry Shader(programmable)
	5.Vertex Post-Processing
	6.Primitive Assembly
	7.Rasterization
	8.Fragment Shader(programmable)
	9.Per-sample Operations

	----------- 1.1-Vertex Specification -----------
	A vertex is a point in space(x,y,z)
	
	A primitive is a simple shape defined using one or more vertices
	
	Usually we use triangles, but we can also use points, lines, quads
	
	Vertex Specification: Setting up the data of the vertices for 
	the primitives we want to render

	Uses VAOs(Vertex Array Objects) and VBOs(Vertex Buffer Object)

	VAO defines WHAT data a vertex has (position, colour, texture,
	normals, etc.)

	VBO defines the data itself.

	Attribute Pointers define where and how shaders can access
	vertex data.

	----------- 1.1.1-Vertex Specification: Creating VAO & VBO -----------
	1.Generate a VAO ID
	2.Bind the VAO with that ID
	3.Generate a VBO ID
	4.Bind the VBO with that ID
	(now u r working on the chosen VBO attached to the chosen VAO)
	5.Attach the vertex data to that VBO
	6.Define the Attribute Pointer formatting
	7.Enable the Attribute Pointer
	8.Unbind the VAO and VBO, ready for the next object to be bound

	----------- 1.1.2-Vertex Specification: Initiating Draw -----------
	1.Activate Shader Program you want to use
	2.Bind VAO of object you want to draw
	3.Call glDrawArrays which initiates the rest of the pipeline

	----------- 1.2-Vertex Shader -----------
	Handles vertices individually.

	NOT optional

	Must store something in gl_Position as it is used by later stages

	Can specify additional outputs that can be picked up and used by 
	user-defined shaders later in pipeline

	Inputs consist of the vertex data itself

	Example:

		#version 330 //glsl version

		layout (location = 0) in vec3 pos; // optional. in means input coming to shader

		void main()
		{
			gl_Position = vec4(pos, 1.0); // second param is usually 1.0
		}


	----------- 1.3-Tessellation -----------
	Allows you to divide up data in to smaller primitives

	can be used to add higher levels of detail dynamically

	----------- 1.4-Geometry Shader -----------
	Vertex shader handles vertices, Geometry shader handles 
	primitives(groups of vertices)

	Takes prmitives then emits their vertices to create 
	the given primitive or even new primitives

	Can alter data given to it to modify given primitives
	or even create new ones.

	Can even alter the primitive type.(points lines triangles etc.)
	
	----------- 1.5-Vertex Post-Processing -----------
	Transform Feedback (if enabled)
		Result of Vertex and Geometry stages saved to buffers for later
		use.
	
	Clipping
		Primitives that wont be visible are removed(don't want to draw
		things we can't see!)
		Positions converted from "clip-space" to "window space"

	----------- 1.6-Primitive Assembly -----------
	Vertices are converted in to series of primitives

	If rendering triangles, 6 vertices would become 2 triangles

	Face culling: the removal of primitives that can't be seen
	or are facing away from the viewer. We don't want to draw smth
	if we can't see it.

	----------- 1.7-Rasterization -----------
	Converts primitives in to "Fragments"

	Fragments are pieces of data for each pixel, obtained from the 
	rasterization process

	Fragment data will be interpolated based on its position relative
	tp each vertex

	---------- 1.8-Fragment Shader -----------
	Handles data for each fragment

	Is optional but it's rare to not use it. Exceptions are cases
	where only depth or stencil data is required.

	Most important output is the colour of the pixel that the fragment 
	covers.

	Example
		
		#version 330

		out vec4 colour;

		void main()
		{
			colour = vec4(1.0, 0.0, 0.0, 1.0);
		}

	 ---------- 1.9-Per-sample Operations -----------
	 Series of tests to see if the fragment should be drawn.

	 most important test is Depth test. Determines if something is
	 in front of the point being drawn.

	 Colour blending: using defined operations, fragment colours
	 are "blended" together with overlapping fragments. Usually
	 used to handle transparent objects.

	 fragment data written to currently bound framebuffer

	 lastly in the application code the user defines a buffer swap
	 putting the newly updated framebuffer to the front.

	 the pipeline is complete.

	 ---------- Creating a Shader Program -----------
	 1. Create empty program
	 2. Create empty shaders
	 3. Attach shader source code to shaders
	 4. Compile shaders
	 5. Attach shaders to program
	 6. Link program(creates executables from shaders and links them
	 together)
	 7. Validate program(optional but highly advised cuz debugging 
	 shaders is pain)

	 ---------- Using a Shader Program -----------
	 When u create a shader, an ID is given(like VAOs and VBOs)

	 Simply call glUseProgram(shaderID)

	 All draw calls from then on will use that shader, glUseProgram
	 is used on a new shaderID, or on '0'(meaning 'no shader').




*/



// Vectors and Matrices

/*

	----------- 0-Vectors -----------

	A quantity with magnitude and direction

	v1=[1,2,3], v2=[2,4,6]

	v1 + v2 = [1+2, 2+4, 3+6] = [3,6,9]
	v - v2 = [1-2, 2-4, 3-6] = [-1, -2, -3]
	
	----------- 0.1-Dot Product in Vectors -----------

	v1=[1,2,3], v2=[4,5,6]

	v1.v2 = 1*4 + 2*5 + 3*6 = 32
	      = |v1|*|v2|*cos(theta)
 
	if we know the value of v1.v2, we can do
	cos^-1((v1.v2)/(|v1|x|v2|)) = theta

	----------- 1-Matrix Transforms -----------

	Identity Matrix: used as a starting point in 
	transform operations

	1000   x     x
	0100   y     y
	0010   z     z
	0001   1     1

	Translation: moves the vector. used for changing
	position of smthing

	100X   x     x+X
	010Y   y     y+Y
	001Z   z     z+Z
	0001   1     1

	Scaling: simply resizing

	SX 0  0  0   x     SXx
	0  SY 0  0   y     SYy
	0  0  SZ 0   z     SZz
	0  0  0  1   1     1

	Rotation: rotate vector around its origin
	So, to choose a point of rotationi translate
	the vector so the point to rotate around is 
	at the origin.
	there are 3 diffrent type of matrixes for rotation
	X rotation, Y rotation, Z rotations

	Combining Matrix Transforms: ORDER MATTERS

	translate:        scale:
			1001		2000     2001
			0102		0200     0202
			0013		0020     0023
			0001		0001     0001

	first scaled then translated.
	the transforms happen in reverse order
	

	----------- 2-Uniform Variables -----------

	global variables that dont change in shaders.

	Example:

		#version 330
	
		in vec3 pos;
	
		uniform mat4 model;

		void main()
		{
			gl_Position = model * vec4(pos, 1.0);
		}

	each uniform has a location ID in the shader.
	need to find the location so we can bind a value to it.

	int location = glGetUniformLocation(shaderID, "uniformVarName");
	glUniform1f(location, 3.5f);

	glUniform1f - Single floating value
	glUniform1i - Single integer value
	glUniform4f - vec4 of floating values
	glUniform4fv - vec4 of floating values, value specified
	by pointer
	glUniformMatrix4fv - mat4 of floating values, value specified
	by pointer


*/

// Interpolation, Index Draws and Projections

/*
	----------- 0-Interpolation -----------

	Per-vertex attributes passed on are "interpolated" 
	using the other values on the primitive.

	In other words, a weighted average of the three vertices
	on a triangle is passed on

	Fragment shader picks up the interpolated value and uses
	that.

	The value is effectively an estimate of what the value
	should be at that position.
	
	example: RGB colored triangle, Red=>x-axis, Green=>y-axis, 
	Blue=>z-axis

	----------- 1-Index Draws -----------

	Assume we want to draw a cube. We would be using triangles.
	We would have 12 triangles 2 per faces. That means 12x3=36
	vertices. However, the cube has 8 vertices so we have 
	unnecessary vertices.

	what we can use instead is defining 8 vertices numberin them
	1 to 8(0to7) and refer them by their number.

	just bind them to an Element Array Buffer in the VAO

	example:

		glBindBuffer(GL_ELEMENT_ARRAY, IBO) //IBO: index buffer obj

	can still be a bit of pain :_(
	we can use 3D modelling software and load in models

	----------- 2-Projections -----------

	Used to convert from View Space -> Clip Space

	Local Space: raw position of each vertex drawn relative to origin
	
	multiply by model matrix to get:

	World Space: position of vertex in the world itself if camera
	is assumed to be positioned at the origin.
	
	multiply by view matrix to get:

	View Space: position of vertex in the world, relative to the
	camera position and orientation. 
	
	multiply by projection matrix to get:

	Clip Space: position of vertex in the world, relative to the 
	camera position and orientation, as viewed in the area not to be
	"clipped" from the final output.

	Screen Space: After clipping takes place, the final image is
	created and placed on the co-ordinate system of window itself.

	to create clip space we define an area(frustum) of what is 
	not to be clipped with a projection matrix

	2 commonly used types of projection:
		- Orthographic (most common in 2D apps)
		- Perspective (most common in 3D apps)
	for images see theory video 12.46

	Orthographic: the one furthest back looks to be the same size
	as the one at the front, implying it's larger
	Perspective: the one at the back looks smaller than the one at
	the front, due to it being more distant, as it should

	----------- 2.1-Projections with GLM and OpenGL -----------

	example: 

		glm::mat4 proj = glm::perspective(fov, aspect, near, far);

	fov = field-of-view, the angle of the frustum(in y-axis)
	aspect = aspect ratio of the viewport(usually its width divided
	by its height)
	near = distance of the near plane
	far = distance of the far plane

	Bind the given matrix to a uniform in the shader.

	example:

					// order must be EXACTLY THIS
		gl_Position = projection * view * model * vec4(pos, 1.0);



*/

//note about glm::mat4 Identity matrix syntax change
/*

IMPORTANT!!


In the following lesson, when using GLM, I identify an Identity Matrix as follows:

glm::mat4 model;



However, this no longer works in newer versions of GLM! The new way to initialise an Identity Matrix is as follows:

glm::mat4 model(1.0f);

Or alternatively:

glm::mat4 model = glm::mat4(1.0f);



This will also need to be performed on subsequent reinitialisations, like so:

model = glm::mat(1.0f);



If you use the old way, you will find your objects don't appear because they are being given garbage transform data! Every time you need to initialise an Identity Matrix, make sure you use this new way of initialising to ensure your program works.

Apologies for any confusion this may have caused!


*/


// Camera/View Space
/*
	- Camera processes the scene as seen in "View Space"
	- View Space is the co-ordinate system with each vertex as seen
	from the camera
	- Use a View Matrix to convert from World Space to View Space.
	- View Matrix requires 4 values:
		- Camera Position
		
		- Direction (~= z): the direction camera is looking in. 
		direction vector is points in opposite direction of the
		intuitive direction cuz actually the world moves around
		our camera as opposed to the camera being moved

		- Right (~= x): can calculate by doing cross product of
		Direction and 'up' vector [0, 1, 0]
		
		- Up (~= y): can calculate by doing cross product of
		Direction and 'right' vector

	- View Matrix applied to a vertex will convert it to View Space
	glm::mat4 viewMatrix = glm::lookAt(position, target, up);

	- target = camera_pos + direction => look in front

	- up = upwards direction of the world not the camera. lookAt uses this
	to calculate 'right' and 'up' relative to the camera

	----------- Using the View Matrix -----------

	- Bind the View Matrix to a uniform on the shader.
	- Apply it btw the projection and model matrices.
	- gl_Position = projection * view * model * vec4(pos, 1.0);

	REMEMBER: ORDER MATTERS: multiplying the projection, view and model
	matrices in a different order will not work.

	----------- Input: Moving the Camera -----------

	GLFW: glfwGetKey(window, GLFW_KEY_W);
	SDL: check for event check if KEYDOWN event, check which key pressed

	----------- Input: Turning -----------

	- three types of angle.

	1- Pitch: looking up and down => rotate view up-down using an 
	axis relative to yaw
	2- Yaw: looking left and right => rotate around up axis(y)
	3- Roll: like a plane doing a barrel roll (we don't be using this)

	1- Pitching axis will depend on yaw.. need to update x, y and z
	
	y = sin(pitch)
	x = cos(pitch)
	z = cos(pitch)
	
	REMEMBER: we are updating x and z cuz the yaw could have the camera
	facing along a combination of them.

	2- Yaw: we could base yaw on pitch too but would be unrealistic. so
	we only update x and z

	x = cos(yaw)
	z = sin(yaw)

	Input: Turning - Pitch and Yaw: combine the values from pitch and 
	yaw to get a DIRECTION VECTOR with those props.

	x = cos(pitch) x cos(yaw)
	y = sin(pitch)
	z = cos(pitch) x sin(yaw)

	Update camera direction with new vector.

	GLFW: glfwSetCursorPosCallback(window, callback);
	Store old mouse position, compare to new position. use difference
	to decide pitch/yaw change.

	SDL: Check for SDL_MOUSEMOTION event.
	Call SDL_GetMouseState(&x, &y);
	then do the same as above.

*/


// [IMPORTANT FIX] Camera Facing Random Direction
/*
* 
A common issue people have with the following lesson, is that 
the camera sometimes starts off facing in a random direction.

This is actually a bug in the code in the lesson video! 
I neglected to set the "xChange" and "yChange" to 0.0f in the Window 
constructor, so it starts off with a garbage value for the x/y change 
and instantly offsets the camera.

To fix this, simply set xChange and yChange to 0.0f in the two constructors 
for the Window class (in Window.cpp) like so:

xChange = 0.0f;
yChange = 0.0f;
This should fix any camera issues that crop up.



Sorry for the confusion this may have caused you! 
The downloadable code has been altered to reflect this fix.

*/

// Textures and Image Loading
/*
	- points on the textures are called texels
	- creating a texture is similar to creating a VBOs and VAOs
	
	- glGenTextures(1, &texture);
	- glBindTexture(GL_TEXTURE_2D, texture);

	- There are different types of textures:
		- GL_TEXTURE_1D
		- GL_TEXTURE_3D
		- GL_TEXTURE_CUBE_MAP

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, 
	GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	1st param: texture target
	2nd param: mipmap level
	3rd param: format of the stored data
	4th and 5th param: width and height of the texture
	6th param: this value must always be 0 border of the texture 
	7th param: format of the pixel data being loaded(as opposed to
	stored on the 3rd param)
	8th param: data type of the pixel data(int, float, byte, etc.)
	9th param: the actual image data


	----------- Mipmaps -----------

	- resolution limitations for textures
	- the closer u get to an object, the more pixelated it becomes.
	further it attempts to render multiple pixels as one pixel.

	- mipmaps are a series of textures with decreasing resolutions.
	u switch btw them based on the distance from the camera.

	----------- Filters -----------

	- what if we try to render off center of a texel?
	2 possible filters:
		- GL_NEAREST: nearest texel is used(creates a pixelated fx)
		- GL_LINEAR: linear interpolation is used(blends pixel 
		boundaries)

	Linear is more common in most cases.

*/

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

GLuint shader, uniformModel, uniformProjection;

// Vertex Shader
static const char *vShader = "Shaders/shader.vert";

// Fragments Shader
static const char* fShader = "Shaders/shader.frag";

void CreateObject()
{
	unsigned int indices[] = {
		0,3,1,
		1,3,2,
		2,3,0,
		0,1,2
	};

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f, //bottomleft
		0.0f, -1.0f, 1.0f, //background
		1.0f, -1.0f, 0.0f, //bottomright
		0.0f, 1.0f, 0.0f //top
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj2);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();

	CreateObject();
	CreateShaders();
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
	glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth()/mainWindow.getBufferHeight(), 0.1f, 100.0f);


	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		// Get + Handle user input events
		glfwPollEvents();

		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear window
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Red
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[1]->RenderMesh();


		glUseProgram(0);

		
		mainWindow.swapBuffers();
	}

}
