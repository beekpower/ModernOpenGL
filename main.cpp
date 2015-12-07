// Utilized and adapted code from https://github.com/opengl-tutorials/ogl/ to help with shaders & camera movement.

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#ifdef __linux__
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#elif _WIN32
// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include "GLFW\glfw3.h"
// Include GLM
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#elif __APPLE__
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#endif

GLFWwindow* window;

using namespace glm;

#include "shader.hpp"
#include "objloader.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include "vboindexer.hpp"



int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);


	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Project 3", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	//glEnable(GL_CULL_FACE);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "StandardShading.vertexshader", "StandardShading.fragmentshader" );

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

	// Get a handle for our buffers
	GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
	GLuint vertexUVID = glGetAttribLocation(programID, "vertexUV");
	GLuint vertexNormal_modelspaceID = glGetAttribLocation(programID, "vertexNormal_modelspace");

	// Load the texture
	GLuint Texture = loadBMP_custom("bricks.bmp");

	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

    // Load the texture
    GLuint TextureWave = loadBMP_custom("wave.bmp");

    // Get a handle for our "myTextureSampler" uniform
    GLuint TextureIDWave  = glGetUniformLocation(programID, "myTextureSampler");

    // Load the texture
    GLuint TextureLine = loadBMP_custom("line.bmp");

    // Get a handle for our "myTextureSampler" uniform
    GLuint TextureIDLine  = glGetUniformLocation(programID, "myTextureSampler");

    // Load the texture
    GLuint TextureTextured = loadBMP_custom("textured.bmp");

    // Get a handle for our "myTextureSampler" uniform
    GLuint TextureIDTextured  = glGetUniformLocation(programID, "myTextureSampler");


    // Load the texture
    GLuint TexturePink = loadBMP_custom("pink.bmp");

    // Get a handle for our "myTextureSampler" uniform
    GLuint TextureIDPink  = glGetUniformLocation(programID, "myTextureSampler");

		// Load the texture
		GLuint TextureOrange = loadBMP_custom("orange.bmp");

		// Get a handle for our "myTextureSampler" uniform
		GLuint TextureIDOrange  = glGetUniformLocation(programID, "myTextureSampler");


		// Load the texture
		GLuint TextureBarrel = loadBMP_custom("barrel.bmp");

		// Get a handle for our "myTextureSampler" uniform
		GLuint TextureIDBarrel = glGetUniformLocation(programID, "myTextureSampler");

		// Load the texture
		GLuint TextureChair = loadBMP_custom("chair.bmp");

		// Get a handle for our "myTextureSampler" uniform
		GLuint TextureIDChair = glGetUniformLocation(programID, "myTextureSampler");

		GLuint TextureAbstract = loadBMP_custom("hammertone.bmp");

		GLuint TextureIDAbstract = glGetUniformLocation(programID, "myTextureSampler");

		GLuint TextureChampagne = loadBMP_custom("coolTex.bmp");

		GLuint TextureIDChampagne = glGetUniformLocation(programID, "myTextureSampler");


	// Read our .obj file
	std::vector<glm::vec3> verticesMesh;
	std::vector<glm::vec2> uvsMesh;
	std::vector<glm::vec3> normalsMesh;

	/* Insert obj file here */
	/*********************************************/

	bool res = loadOBJ("mesh.obj", verticesMesh, uvsMesh, normalsMesh);

	/*********************************************/

	// Load it into a VBO

	GLuint vertexbufferMesh;
	glGenBuffers(1, &vertexbufferMesh);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferMesh);
	glBufferData(GL_ARRAY_BUFFER, verticesMesh.size() * sizeof(glm::vec3), &verticesMesh[0], GL_STATIC_DRAW);

	GLuint uvbufferMesh;
	glGenBuffers(1, &uvbufferMesh);
	glBindBuffer(GL_ARRAY_BUFFER, uvbufferMesh);
	glBufferData(GL_ARRAY_BUFFER, uvsMesh.size() * sizeof(glm::vec2), &uvsMesh[0], GL_STATIC_DRAW);

	GLuint normalbufferMesh;
	glGenBuffers(1, &normalbufferMesh);
	glBindBuffer(GL_ARRAY_BUFFER, normalbufferMesh);
	glBufferData(GL_ARRAY_BUFFER, normalsMesh.size() * sizeof(glm::vec3), &normalsMesh[0], GL_STATIC_DRAW);

    std::vector<glm::vec3> verticesPin;
    std::vector<glm::vec2> uvsPin;
    std::vector<glm::vec3> normalsPin;

    res = loadOBJ("bowlPin.obj", verticesPin, uvsPin, normalsPin);

    /*********************************************/

    // Load it into a VBO

    GLuint vertexbufferPin;
    glGenBuffers(1, &vertexbufferPin);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbufferPin);
    glBufferData(GL_ARRAY_BUFFER, verticesPin.size() * sizeof(glm::vec3), &verticesPin[0], GL_STATIC_DRAW);

    GLuint uvbufferPin;
    glGenBuffers(1, &uvbufferPin);
    glBindBuffer(GL_ARRAY_BUFFER, uvbufferPin);
    glBufferData(GL_ARRAY_BUFFER, uvsPin.size() * sizeof(glm::vec2), &uvsPin[0], GL_STATIC_DRAW);

    GLuint normalbufferPin;
    glGenBuffers(1, &normalbufferPin);
    glBindBuffer(GL_ARRAY_BUFFER, normalbufferPin);
    glBufferData(GL_ARRAY_BUFFER, normalsPin.size() * sizeof(glm::vec3), &normalsPin[0], GL_STATIC_DRAW);

    std::vector<glm::vec3> verticesCool;
    std::vector<glm::vec2> uvsCool;
    std::vector<glm::vec3> normalsCool;

    res = loadOBJ("coolShape.obj", verticesCool, uvsCool, normalsCool);

    /*********************************************/

    // Load it into a VBO

    GLuint vertexbufferCool;
    glGenBuffers(1, &vertexbufferCool);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbufferCool);
    glBufferData(GL_ARRAY_BUFFER, verticesCool.size() * sizeof(glm::vec3), &verticesCool[0], GL_STATIC_DRAW);

    GLuint uvbufferCool;
    glGenBuffers(1, &uvbufferCool);
    glBindBuffer(GL_ARRAY_BUFFER, uvbufferCool);
    glBufferData(GL_ARRAY_BUFFER, uvsCool.size() * sizeof(glm::vec2), &uvsCool[0], GL_STATIC_DRAW);

    GLuint normalbufferCool;
    glGenBuffers(1, &normalbufferCool);
    glBindBuffer(GL_ARRAY_BUFFER, normalbufferCool);
    glBufferData(GL_ARRAY_BUFFER, normalsCool.size() * sizeof(glm::vec3), &normalsCool[0], GL_STATIC_DRAW);

    std::vector<glm::vec3> verticesSphere;
    std::vector<glm::vec2> uvsSphere;
    std::vector<glm::vec3> normalsSphere;

    res = loadOBJ("sphere2.obj", verticesSphere, uvsSphere, normalsSphere);

    /*********************************************/

    // Load it into a VBO

    GLuint vertexbufferSphere;
    glGenBuffers(1, &vertexbufferSphere);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbufferSphere);
    glBufferData(GL_ARRAY_BUFFER, verticesSphere.size() * sizeof(glm::vec3), &verticesSphere[0], GL_STATIC_DRAW);

    GLuint uvbufferSphere;
    glGenBuffers(1, &uvbufferSphere);
    glBindBuffer(GL_ARRAY_BUFFER, uvbufferSphere);
    glBufferData(GL_ARRAY_BUFFER, uvsSphere.size() * sizeof(glm::vec2), &uvsSphere[0], GL_STATIC_DRAW);

    GLuint normalbufferSphere;
    glGenBuffers(1, &normalbufferSphere);
    glBindBuffer(GL_ARRAY_BUFFER, normalbufferSphere);
    glBufferData(GL_ARRAY_BUFFER, normalsSphere.size() * sizeof(glm::vec3), &normalsSphere[0], GL_STATIC_DRAW);

    std::vector<glm::vec3> verticesSpring;
    std::vector<glm::vec2> uvsSpring;
    std::vector<glm::vec3> normalsSpring;

    res = loadOBJ("spring2.obj", verticesSpring, uvsSpring, normalsSpring);

    /*********************************************/

    // Load it into a VBO

    GLuint vertexbufferSpring;
    glGenBuffers(1, &vertexbufferSpring);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbufferSpring);
    glBufferData(GL_ARRAY_BUFFER, verticesSpring.size() * sizeof(glm::vec3), &verticesSpring[0], GL_STATIC_DRAW);

    GLuint uvbufferSpring;
    glGenBuffers(1, &uvbufferSpring);
    glBindBuffer(GL_ARRAY_BUFFER, uvbufferSpring);
    glBufferData(GL_ARRAY_BUFFER, uvsSpring.size() * sizeof(glm::vec2), &uvsSpring[0], GL_STATIC_DRAW);

    GLuint normalbufferSpring;
    glGenBuffers(1, &normalbufferSpring);
    glBindBuffer(GL_ARRAY_BUFFER, normalbufferSpring);
    glBufferData(GL_ARRAY_BUFFER, normalsSpring.size() * sizeof(glm::vec3), &normalsSpring[0], GL_STATIC_DRAW);



		std::vector<glm::vec3> verticesOrange;
		std::vector<glm::vec2> uvsOrange;
		std::vector<glm::vec3> normalsOrange;

		res = loadOBJ("orange.obj", verticesOrange, uvsOrange, normalsOrange);

		// Load it into a VBO

		GLuint vertexbufferOrange;
		glGenBuffers(1, &vertexbufferOrange);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferOrange);
		glBufferData(GL_ARRAY_BUFFER, verticesOrange.size() * sizeof(glm::vec3), &verticesOrange[0], GL_STATIC_DRAW);

		GLuint uvbufferOrange;
		glGenBuffers(1, &uvbufferOrange);
		glBindBuffer(GL_ARRAY_BUFFER, uvbufferOrange);
		glBufferData(GL_ARRAY_BUFFER, uvsOrange.size() * sizeof(glm::vec2), &uvsOrange[0], GL_STATIC_DRAW);

		GLuint normalbufferOrange;
		glGenBuffers(1, &normalbufferOrange);
		glBindBuffer(GL_ARRAY_BUFFER, normalbufferOrange);
		glBufferData(GL_ARRAY_BUFFER, normalsOrange.size() * sizeof(glm::vec3), &normalsOrange[0], GL_STATIC_DRAW);


		std::vector<glm::vec3> verticesBarrel;
 		std::vector<glm::vec2> uvsBarrel;
 		std::vector<glm::vec3> normalsBarrel;

/*
		res = loadOBJ("barrel.obj", verticesBarrel, uvsBarrel, normalsBarrel);

		// Load it into a VBO

		GLuint vertexbufferBarrel;
		glGenBuffers(1, &vertexbufferBarrel);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferBarrel);
		glBufferData(GL_ARRAY_BUFFER, verticesBarrel.size() * sizeof(glm::vec3), &verticesBarrel[0], GL_STATIC_DRAW);

		GLuint uvbufferBarrel;
		glGenBuffers(1, &uvbufferBarrel);
		glBindBuffer(GL_ARRAY_BUFFER, uvbufferBarrel);
		glBufferData(GL_ARRAY_BUFFER, uvsBarrel.size() * sizeof(glm::vec2), &uvsBarrel[0], GL_STATIC_DRAW);

		GLuint normalbufferBarrel;
		glGenBuffers(1, &normalbufferBarrel);
		glBindBuffer(GL_ARRAY_BUFFER, normalbufferBarrel);
		glBufferData(GL_ARRAY_BUFFER, normalsBarrel.size() * sizeof(glm::vec3), &normalsBarrel[0], GL_STATIC_DRAW);
*/

		std::vector<glm::vec3> verticesChair;
		std::vector<glm::vec2> uvsChair;
		std::vector<glm::vec3> normalsChair;


		res = loadOBJ("chair.obj", verticesChair, uvsChair, normalsChair);

		// Load it into a VBO

		GLuint vertexbufferChair;
		glGenBuffers(1, &vertexbufferChair);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferChair);
		glBufferData(GL_ARRAY_BUFFER, verticesChair.size() * sizeof(glm::vec3), &verticesChair[0], GL_STATIC_DRAW);

		GLuint uvbufferChair;
		glGenBuffers(1, &uvbufferChair);
		glBindBuffer(GL_ARRAY_BUFFER, uvbufferChair);
		glBufferData(GL_ARRAY_BUFFER, uvsChair.size() * sizeof(glm::vec2), &uvsChair[0], GL_STATIC_DRAW);

		GLuint normalbufferChair;
		glGenBuffers(1, &normalbufferChair);
		glBindBuffer(GL_ARRAY_BUFFER, normalbufferChair);
		glBufferData(GL_ARRAY_BUFFER, normalsChair.size() * sizeof(glm::vec3), &normalsChair[0], GL_STATIC_DRAW);

		std::vector<glm::vec3> verticesBottle;
		std::vector<glm::vec2> uvsBottle;
		std::vector<glm::vec3> normalsBottle;


		res = loadOBJ("bottle.obj", verticesBottle, uvsBottle, normalsBottle);

		// Load it into a VBO

		GLuint vertexbufferBottle;
		glGenBuffers(1, &vertexbufferBottle);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferBottle);
		glBufferData(GL_ARRAY_BUFFER, verticesBottle.size() * sizeof(glm::vec3), &verticesBottle[0], GL_STATIC_DRAW);

		GLuint uvbufferBottle;
		glGenBuffers(1, &uvbufferBottle);
		glBindBuffer(GL_ARRAY_BUFFER, uvbufferBottle);
		glBufferData(GL_ARRAY_BUFFER, uvsBottle.size() * sizeof(glm::vec2), &uvsBottle[0], GL_STATIC_DRAW);

		GLuint normalbufferBottle;
		glGenBuffers(1, &normalbufferBottle);
		glBindBuffer(GL_ARRAY_BUFFER, normalbufferBottle);
		glBufferData(GL_ARRAY_BUFFER, normalsBottle.size() * sizeof(glm::vec3), &normalsBottle[0], GL_STATIC_DRAW);

		//******************************************************************************

		std::vector<glm::vec3> verticesITWP;
		std::vector<glm::vec2> uvsITWP;
		std::vector<glm::vec3> normalsITWP;

		res = loadOBJ("ITookTheWrongPills.obj", verticesITWP, uvsITWP, normalsITWP);

		// Load it into a VBO

		GLuint vertexbufferITWP;
		glGenBuffers(1, &vertexbufferITWP);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferITWP);
		glBufferData(GL_ARRAY_BUFFER, verticesITWP.size() * sizeof(glm::vec3), &verticesITWP[0], GL_STATIC_DRAW);

		GLuint uvbufferITWP;
		glGenBuffers(1, &uvbufferITWP);
		glBindBuffer(GL_ARRAY_BUFFER, uvbufferITWP);
		glBufferData(GL_ARRAY_BUFFER, uvsITWP.size() * sizeof(glm::vec2), &uvsITWP[0], GL_STATIC_DRAW);

		GLuint normalbufferITWP;
		glGenBuffers(1, &normalbufferITWP);
		glBindBuffer(GL_ARRAY_BUFFER, normalbufferITWP);
		glBufferData(GL_ARRAY_BUFFER, normalsITWP.size() * sizeof(glm::vec3), &normalsITWP[0], GL_STATIC_DRAW);

		//******************************************************************************

		std::vector<glm::vec3> verticesChampagne;
		std::vector<glm::vec2> uvsChampagne;
		std::vector<glm::vec3> normalsChampagne;

		res = loadOBJ("champagne.obj", verticesChampagne, uvsChampagne, normalsChampagne);

		// Load it into a VBO

		GLuint vertexbufferChampagne;
		glGenBuffers(1, &vertexbufferChampagne);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferChampagne);
		glBufferData(GL_ARRAY_BUFFER, verticesChampagne.size() * sizeof(glm::vec3), &verticesChampagne[0], GL_STATIC_DRAW);

		GLuint uvbufferChampagne;
		glGenBuffers(1, &uvbufferChampagne);
		glBindBuffer(GL_ARRAY_BUFFER, uvbufferChampagne);
		glBufferData(GL_ARRAY_BUFFER, uvsChampagne.size() * sizeof(glm::vec2), &uvsChampagne[0], GL_STATIC_DRAW);

		GLuint normalbufferChampagne;
		glGenBuffers(1, &normalbufferChampagne);
		glBindBuffer(GL_ARRAY_BUFFER, normalbufferChampagne);
		glBufferData(GL_ARRAY_BUFFER, normalsChampagne.size() * sizeof(glm::vec3), &normalsChampagne[0], GL_STATIC_DRAW);

	// Get a handle for our "LightPosition" uniform
	glUseProgram(programID);
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

	do{

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader,
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

		glm::vec3 lightPos = glm::vec3(0,8,0);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(vertexPosition_modelspaceID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferMesh);
		glVertexAttribPointer(
			vertexPosition_modelspaceID,  // The attribute we want to configure
			3,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(vertexUVID);
		glBindBuffer(GL_ARRAY_BUFFER, uvbufferMesh);
		glVertexAttribPointer(
			vertexUVID,                   // The attribute we want to configure
			2,                            // size : U+V => 2
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(vertexNormal_modelspaceID);
		glBindBuffer(GL_ARRAY_BUFFER, normalbufferMesh);
		glVertexAttribPointer(
			vertexNormal_modelspaceID,    // The attribute we want to configure
			3,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);

		// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, verticesMesh.size() );

        // Draw bowling pin
        computeMatricesFromInputs();
        ProjectionMatrix = getProjectionMatrix();
        ViewMatrix = getViewMatrix();
        ModelMatrix = glm::mat4(1.0);

        ModelMatrix = glm::translate(ModelMatrix, vec3(0,8,-8));
        ModelMatrix = glm::scale(ModelMatrix, vec3(6,6,6));
        ModelMatrix = glm::rotate(ModelMatrix, -55.0f, glm::vec3(0.0f, 0.0f, 1.0f));

        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

        glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureWave);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(TextureID, 0);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(vertexPosition_modelspaceID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbufferPin);
        glVertexAttribPointer(
                              vertexPosition_modelspaceID,  // The attribute we want to configure
                              3,                            // size
                              GL_FLOAT,                     // type
                              GL_FALSE,                     // normalized?
                              0,                            // stride
                              (void*)0                      // array buffer offset
                              );

        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(vertexUVID);
        glBindBuffer(GL_ARRAY_BUFFER, uvbufferPin);
        glVertexAttribPointer(
                              vertexUVID,                   // The attribute we want to configure
                              2,                            // size : U+V => 2
                              GL_FLOAT,                     // type
                              GL_FALSE,                     // normalized?
                              0,                            // stride
                              (void*)0                      // array buffer offset
                              );

        // 3rd attribute buffer : normals
        glEnableVertexAttribArray(vertexNormal_modelspaceID);
        glBindBuffer(GL_ARRAY_BUFFER, normalbufferPin);
        glVertexAttribPointer(
                              vertexNormal_modelspaceID,    // The attribute we want to configure
                              3,                            // size
                              GL_FLOAT,                     // type
                              GL_FALSE,                     // normalized?
                              0,                            // stride
                              (void*)0                      // array buffer offset
                              );

        // Draw the triangles !
        glDrawArrays(GL_TRIANGLES, 0, verticesPin.size() );

        // Draw Cool Shape
        computeMatricesFromInputs();
        ProjectionMatrix = getProjectionMatrix();
        ViewMatrix = getViewMatrix();
        ModelMatrix = glm::mat4(1.0);

        ModelMatrix = glm::translate(ModelMatrix, vec3(-12,12,-8));
        ModelMatrix = glm::scale(ModelMatrix, vec3(3,3,3));
        ModelMatrix = glm::rotate(ModelMatrix, 55.0f, glm::vec3(1.0f, 0.0f, 0.0f));


        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

        glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureLine);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(TextureIDLine, 0);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(vertexPosition_modelspaceID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbufferCool);
        glVertexAttribPointer(
                              vertexPosition_modelspaceID,  // The attribute we want to configure
                              3,                            // size
                              GL_FLOAT,                     // type
                              GL_FALSE,                     // normalized?
                              0,                            // stride
                              (void*)0                      // array buffer offset
                              );

        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(vertexUVID);
        glBindBuffer(GL_ARRAY_BUFFER, uvbufferCool);
        glVertexAttribPointer(
                              vertexUVID,                   // The attribute we want to configure
                              2,                            // size : U+V => 2
                              GL_FLOAT,                     // type
                              GL_FALSE,                     // normalized?
                              0,                            // stride
                              (void*)0                      // array buffer offset
                              );

        // 3rd attribute buffer : normals
        glEnableVertexAttribArray(vertexNormal_modelspaceID);
        glBindBuffer(GL_ARRAY_BUFFER, normalbufferCool);
        glVertexAttribPointer(
                              vertexNormal_modelspaceID,    // The attribute we want to configure
                              3,                            // size
                              GL_FLOAT,                     // type
                              GL_FALSE,                     // normalized?
                              0,                            // stride
                              (void*)0                      // array buffer offset
                              );

        // Draw the triangles !
        glDrawArrays(GL_TRIANGLES, 0, verticesCool.size() );

        // Draw Sphere
        computeMatricesFromInputs();
        ProjectionMatrix = getProjectionMatrix();
        ViewMatrix = getViewMatrix();
        ModelMatrix = glm::mat4(1.0);

        ModelMatrix = glm::translate(ModelMatrix, vec3(8,7,8));
        ModelMatrix = glm::scale(ModelMatrix, vec3(3,3,3));

        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

        glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureTextured);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(TextureIDTextured, 0);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(vertexPosition_modelspaceID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbufferSphere);
        glVertexAttribPointer(
                              vertexPosition_modelspaceID,  // The attribute we want to configure
                              3,                            // size
                              GL_FLOAT,                     // type
                              GL_FALSE,                     // normalized?
                              0,                            // stride
                              (void*)0                      // array buffer offset
                              );

        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(vertexUVID);
        glBindBuffer(GL_ARRAY_BUFFER, uvbufferSphere);
        glVertexAttribPointer(
                              vertexUVID,                   // The attribute we want to configure
                              2,                            // size : U+V => 2
                              GL_FLOAT,                     // type
                              GL_FALSE,                     // normalized?
                              0,                            // stride
                              (void*)0                      // array buffer offset
                              );

        // 3rd attribute buffer : normals
        glEnableVertexAttribArray(vertexNormal_modelspaceID);
        glBindBuffer(GL_ARRAY_BUFFER, normalbufferSphere);
        glVertexAttribPointer(
                              vertexNormal_modelspaceID,    // The attribute we want to configure
                              3,                            // size
                              GL_FLOAT,                     // type
                              GL_FALSE,                     // normalized?
                              0,                            // stride
                              (void*)0                      // array buffer offset
                              );

        // Draw the triangles !
        glDrawArrays(GL_TRIANGLES, 0, verticesSphere.size() );

        // Draw Spring
        computeMatricesFromInputs();
        ProjectionMatrix = getProjectionMatrix();
        ViewMatrix = getViewMatrix();
        ModelMatrix = glm::mat4(1.0);

        ModelMatrix = glm::translate(ModelMatrix, vec3(12,2,0));

        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

        glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TexturePink);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(TextureIDPink, 0);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(vertexPosition_modelspaceID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbufferSpring);
        glVertexAttribPointer(
                              vertexPosition_modelspaceID,  // The attribute we want to configure
                              3,                            // size
                              GL_FLOAT,                     // type
                              GL_FALSE,                     // normalized?
                              0,                            // stride
                              (void*)0                      // array buffer offset
                              );

        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(vertexUVID);
        glBindBuffer(GL_ARRAY_BUFFER, uvbufferSpring);
        glVertexAttribPointer(
                              vertexUVID,                   // The attribute we want to configure
                              2,                            // size : U+V => 2
                              GL_FLOAT,                     // type
                              GL_FALSE,                     // normalized?
                              0,                            // stride
                              (void*)0                      // array buffer offset
                              );

        // 3rd attribute buffer : normals
        glEnableVertexAttribArray(vertexNormal_modelspaceID);
        glBindBuffer(GL_ARRAY_BUFFER, normalbufferSpring);
        glVertexAttribPointer(
                              vertexNormal_modelspaceID,    // The attribute we want to configure
                              3,                            // size
                              GL_FLOAT,                     // type
                              GL_FALSE,                     // normalized?
                              0,                            // stride
                              (void*)0                      // array buffer offset
                              );

        // Draw the triangles !
        glDrawArrays(GL_TRIANGLES, 0, verticesSpring.size());

				// Draw orange
				computeMatricesFromInputs();
				ProjectionMatrix = getProjectionMatrix();
				ViewMatrix = getViewMatrix();
				ModelMatrix = glm::mat4(1.0);
                ModelMatrix = glm::translate(ModelMatrix, vec3(-10,-1,-5));
                ModelMatrix = glm::scale(ModelMatrix, vec3(1,1,1));
                //ModelMatrix = glm::rotate(ModelMatrix, -55.0f, glm::vec3(0.0f, 1.0f, 1.0f));
        
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

				// Send our transformation to the currently bound shader,
				// in the "MVP" uniform
				glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

				glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

				// Bind our texture in Texture Unit 0
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, TextureOrange);
				// Set our "myTextureSampler" sampler to user Texture Unit 0
				glUniform1i(TextureIDOrange, 0);

				// 1rst attribute buffer : vertices
				glEnableVertexAttribArray(vertexPosition_modelspaceID);
				glBindBuffer(GL_ARRAY_BUFFER, vertexbufferOrange);
				glVertexAttribPointer(
															vertexPosition_modelspaceID,  // The attribute we want to configure
															3,                            // size
															GL_FLOAT,                     // type
															GL_FALSE,                     // normalized?
															0,                            // stride
															(void*)0                      // array buffer offset
															);

				// 2nd attribute buffer : UVs
				glEnableVertexAttribArray(vertexUVID);
				glBindBuffer(GL_ARRAY_BUFFER, uvbufferOrange);
				glVertexAttribPointer(
															vertexUVID,                   // The attribute we want to configure
															2,                            // size : U+V => 2
															GL_FLOAT,                     // type
															GL_FALSE,                     // normalized?
															0,                            // stride
															(void*)0                      // array buffer offset
															);

				// 3rd attribute buffer : normals
				glEnableVertexAttribArray(vertexNormal_modelspaceID);
				glBindBuffer(GL_ARRAY_BUFFER, normalbufferOrange);
				glVertexAttribPointer(
															vertexNormal_modelspaceID,    // The attribute we want to configure
															3,                            // size
															GL_FLOAT,                     // type
															GL_FALSE,                     // normalized?
															0,                            // stride
															(void*)0                      // array buffer offset
															);

				// Draw the triangles !
<<<<<<< HEAD
				glDrawArrays(GL_TRIANGLE_FAN, 0, verticesOrange.size() );
=======
				glDrawArrays(GL_TRIANGLE_STRIP, 0, verticesOrange.size() );
>>>>>>> origin/master

/*

				// Draw barrel
				computeMatricesFromInputs();
				ProjectionMatrix = getProjectionMatrix();
				ViewMatrix = getViewMatrix();
				ModelMatrix = glm::mat4(1.0);

				ModelMatrix = glm::translate(ModelMatrix, vec3(3,3,0));

				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

				// Send our transformation to the currently bound shader,
				// in the "MVP" uniform
				glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

				glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

				// Bind our texture in Texture Unit 0
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, TextureBarrel);
				// Set our "myTextureSampler" sampler to user Texture Unit 0
				glUniform1i(TextureIDBarrel, 0);

				// 1rst attribute buffer : vertices
				glEnableVertexAttribArray(vertexPosition_modelspaceID);
				glBindBuffer(GL_ARRAY_BUFFER, vertexbufferBarrel);
				glVertexAttribPointer(
															vertexPosition_modelspaceID,  // The attribute we want to configure
															3,                            // size
															GL_FLOAT,                     // type
															GL_FALSE,                     // normalized?
															0,                            // stride
															(void*)0                      // array buffer offset
															);

				// 2nd attribute buffer : UVs
				glEnableVertexAttribArray(vertexUVID);
				glBindBuffer(GL_ARRAY_BUFFER, uvbufferBarrel);
				glVertexAttribPointer(
															vertexUVID,                   // The attribute we want to configure
															2,                            // size : U+V => 2
															GL_FLOAT,                     // type
															GL_FALSE,                     // normalized?
															0,                            // stride
															(void*)0                      // array buffer offset
															);

				// 3rd attribute buffer : normals
				glEnableVertexAttribArray(vertexNormal_modelspaceID);
				glBindBuffer(GL_ARRAY_BUFFER, normalbufferBarrel);
				glVertexAttribPointer(
															vertexNormal_modelspaceID,    // The attribute we want to configure
															3,                            // size
															GL_FLOAT,                     // type
															GL_FALSE,                     // normalized?
															0,                            // stride
															(void*)0                      // array buffer offset
															);

				// Draw the triangles !
				glDrawArrays(GL_TRIANGLES, 0, verticesBarrel.size() );
*/


				// Draw chair
				computeMatricesFromInputs();
				ProjectionMatrix = getProjectionMatrix();
				ViewMatrix = getViewMatrix();
				ModelMatrix = glm::mat4(1.0);

				ModelMatrix = glm::translate(ModelMatrix, vec3(-7,3,10));

				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

				// Send our transformation to the currently bound shader,
				// in the "MVP" uniform
				glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

				glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

				// Bind our texture in Texture Unit 0
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, TextureChair);
				// Set our "myTextureSampler" sampler to user Texture Unit 0
				glUniform1i(TextureIDChair, 0);

				// 1rst attribute buffer : vertices
				glEnableVertexAttribArray(vertexPosition_modelspaceID);
				glBindBuffer(GL_ARRAY_BUFFER, vertexbufferChair);
				glVertexAttribPointer(
															vertexPosition_modelspaceID,  // The attribute we want to configure
															3,                            // size
															GL_FLOAT,                     // type
															GL_FALSE,                     // normalized?
															0,                            // stride
															(void*)0                      // array buffer offset
															);

				// 2nd attribute buffer : UVs
				glEnableVertexAttribArray(vertexUVID);
				glBindBuffer(GL_ARRAY_BUFFER, uvbufferChair);
				glVertexAttribPointer(
															vertexUVID,                   // The attribute we want to configure
															2,                            // size : U+V => 2
															GL_FLOAT,                     // type
															GL_FALSE,                     // normalized?
															0,                            // stride
															(void*)0                      // array buffer offset
															);

				// 3rd attribute buffer : normals
				glEnableVertexAttribArray(vertexNormal_modelspaceID);
				glBindBuffer(GL_ARRAY_BUFFER, normalbufferChair);
				glVertexAttribPointer(
															vertexNormal_modelspaceID,    // The attribute we want to configure
															3,                            // size
															GL_FLOAT,                     // type
															GL_FALSE,                     // normalized?
															0,                            // stride
															(void*)0                      // array buffer offset
															);

				// Draw the triangles !
				glDrawArrays(GL_TRIANGLES, 0, verticesChair.size() );

				// Draw bottle
				computeMatricesFromInputs();
				ProjectionMatrix = getProjectionMatrix();
				ViewMatrix = getViewMatrix();
				ModelMatrix = glm::mat4(1.0);

				ModelMatrix = glm::translate(ModelMatrix, vec3(-12,4,3));
                ModelMatrix = glm::scale(ModelMatrix, vec3(3,2,3));
                ModelMatrix = glm::rotate(ModelMatrix, -55.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

				// Send our transformation to the currently bound shader,
				// in the "MVP" uniform
				glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

				glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

				// Bind our texture in Texture Unit 0
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, TexturePink);
				// Set our "myTextureSampler" sampler to user Texture Unit 0
				glUniform1i(TextureIDPink, 0);

				// 1rst attribute buffer : vertices
				glEnableVertexAttribArray(vertexPosition_modelspaceID);
				glBindBuffer(GL_ARRAY_BUFFER, vertexbufferBottle);
				glVertexAttribPointer(
															vertexPosition_modelspaceID,  // The attribute we want to configure
															3,                            // size
															GL_FLOAT,                     // type
															GL_FALSE,                     // normalized?
															0,                            // stride
															(void*)0                      // array buffer offset
															);

				// 2nd attribute buffer : UVs
				glEnableVertexAttribArray(vertexUVID);
				glBindBuffer(GL_ARRAY_BUFFER, uvbufferBottle);
				glVertexAttribPointer(
															vertexUVID,                   // The attribute we want to configure
															2,                            // size : U+V => 2
															GL_FLOAT,                     // type
															GL_FALSE,                     // normalized?
															0,                            // stride
															(void*)0                      // array buffer offset
															);

				// 3rd attribute buffer : normals
				glEnableVertexAttribArray(vertexNormal_modelspaceID);
				glBindBuffer(GL_ARRAY_BUFFER, normalbufferBottle);
				glVertexAttribPointer(
															vertexNormal_modelspaceID,    // The attribute we want to configure
															3,                            // size
															GL_FLOAT,                     // type
															GL_FALSE,                     // normalized?
															0,                            // stride
															(void*)0                      // array buffer offset
															);

				// Draw the triangles !
				glDrawArrays(GL_TRIANGLES, 0, verticesBottle.size() );


				// Draw ITookTheWrongPills
				computeMatricesFromInputs();
				ProjectionMatrix = getProjectionMatrix();
				ViewMatrix = getViewMatrix();
				ModelMatrix = glm::mat4(1.0);

				ModelMatrix = glm::translate(ModelMatrix, vec3(-10, 5, 6));
				ModelMatrix = glm::scale(ModelMatrix, vec3(3, 2, 3));
				ModelMatrix = glm::rotate(ModelMatrix, -55.0f, glm::vec3(0.0f, 0.0f, 1.0f));

				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

				// Send our transformation to the currently bound shader,
				// in the "MVP" uniform
				glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

				glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

				// Bind our texture in Texture Unit 0
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, TextureAbstract);
				// Set our "myTextureSampler" sampler to user Texture Unit 0
				glUniform1i(TextureIDAbstract, 0);

				// 1rst attribute buffer : vertices
				glEnableVertexAttribArray(vertexPosition_modelspaceID);
				glBindBuffer(GL_ARRAY_BUFFER, vertexbufferITWP);
				glVertexAttribPointer(
					vertexPosition_modelspaceID,  // The attribute we want to configure
					3,                            // size
					GL_FLOAT,                     // type
					GL_FALSE,                     // normalized?
					0,                            // stride
					(void*)0                      // array buffer offset
					);

				// 2nd attribute buffer : UVs
				glEnableVertexAttribArray(vertexUVID);
				glBindBuffer(GL_ARRAY_BUFFER, uvbufferITWP);
				glVertexAttribPointer(
					vertexUVID,                   // The attribute we want to configure
					2,                            // size : U+V => 2
					GL_FLOAT,                     // type
					GL_FALSE,                     // normalized?
					0,                            // stride
					(void*)0                      // array buffer offset
					);

				// 3rd attribute buffer : normals
				glEnableVertexAttribArray(vertexNormal_modelspaceID);
				glBindBuffer(GL_ARRAY_BUFFER, normalbufferITWP);
				glVertexAttribPointer(
					vertexNormal_modelspaceID,    // The attribute we want to configure
					3,                            // size
					GL_FLOAT,                     // type
					GL_FALSE,                     // normalized?
					0,                            // stride
					(void*)0                      // array buffer offset
					);

				// Draw the triangles !
				glDrawArrays(GL_TRIANGLES, 0, verticesITWP.size());

				//**************************************************************************************

				// Draw Champagne Glass
				computeMatricesFromInputs();
				ProjectionMatrix = getProjectionMatrix();
				ViewMatrix = getViewMatrix();
				ModelMatrix = glm::mat4(1.0);

				ModelMatrix = glm::translate(ModelMatrix, vec3(2, 7, 2));
				ModelMatrix = glm::scale(ModelMatrix, vec3(3, 2, 3));
				ModelMatrix = glm::rotate(ModelMatrix, -55.0f, glm::vec3(0.0f, 0.0f, 1.0f));

				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

				// Send our transformation to the currently bound shader,
				// in the "MVP" uniform
				glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

				glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

				// Bind our texture in Texture Unit 0
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, TextureChampagne);
				// Set our "myTextureSampler" sampler to user Texture Unit 0
				glUniform1i(TextureIDChampagne, 0);

				// 1rst attribute buffer : vertices
				glEnableVertexAttribArray(vertexPosition_modelspaceID);
				glBindBuffer(GL_ARRAY_BUFFER, vertexbufferChampagne);
				glVertexAttribPointer(
					vertexPosition_modelspaceID,  // The attribute we want to configure
					3,                            // size
					GL_FLOAT,                     // type
					GL_FALSE,                     // normalized?
					0,                            // stride
					(void*)0                      // array buffer offset
					);

				// 2nd attribute buffer : UVs
				glEnableVertexAttribArray(vertexUVID);
				glBindBuffer(GL_ARRAY_BUFFER, uvbufferChampagne);
				glVertexAttribPointer(
					vertexUVID,                   // The attribute we want to configure
					2,                            // size : U+V => 2
					GL_FLOAT,                     // type
					GL_FALSE,                     // normalized?
					0,                            // stride
					(void*)0                      // array buffer offset
					);

				// 3rd attribute buffer : normals
				glEnableVertexAttribArray(vertexNormal_modelspaceID);
				glBindBuffer(GL_ARRAY_BUFFER, normalbufferChampagne);
				glVertexAttribPointer(
					vertexNormal_modelspaceID,    // The attribute we want to configure
					3,                            // size
					GL_FLOAT,                     // type
					GL_FALSE,                     // normalized?
					0,                            // stride
					(void*)0                      // array buffer offset
					);

				// Draw the triangles !
				glDrawArrays(GL_TRIANGLES, 0, verticesChampagne.size());

		glDisableVertexAttribArray(vertexPosition_modelspaceID);
		glDisableVertexAttribArray(vertexUVID);
		glDisableVertexAttribArray(vertexNormal_modelspaceID);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbufferMesh);
	glDeleteBuffers(1, &uvbufferMesh);
	glDeleteBuffers(1, &normalbufferMesh);
    glDeleteBuffers(1, &vertexbufferPin);
    glDeleteBuffers(1, &uvbufferPin);
    glDeleteBuffers(1, &normalbufferPin);
    glDeleteBuffers(1, &vertexbufferSphere);
    glDeleteBuffers(1, &uvbufferSphere);
    glDeleteBuffers(1, &normalbufferSphere);
    glDeleteBuffers(1, &vertexbufferSpring);
    glDeleteBuffers(1, &uvbufferSpring);
    glDeleteBuffers(1, &normalbufferSpring);
    glDeleteBuffers(1, &vertexbufferCool);
    glDeleteBuffers(1, &uvbufferCool);
    glDeleteBuffers(1, &normalbufferCool);
	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
