#include "graphics.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(int width, int height)
{
	// Used for the linux OS
#if !defined(__APPLE__) && !defined(MACOSX)
  // cout << glewGetString(GLEW_VERSION) << endl;
	glewExperimental = GL_TRUE;

	auto status = glewInit();

	// This is here to grab the error that comes from glew init.
	// This error is an GL_INVALID_ENUM that has no effects on the performance
	glGetError();

	//Check for error
	if (status != GLEW_OK)
	{
		std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
		return false;
	}
#endif


	// Init Camera
	m_camera = new Camera();
	if (!m_camera->Initialize(width, height))
	{
		printf("Camera Failed to Initialize\n");
		return false;
	}

	//m_light = new Light(m_camera->GetView(), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
	m_light = new Light(m_camera->GetView(),
		glm::vec4(1.3f, 1.3f, 1.3f, 1.0f), //light ambient
		glm::vec4(.5f, .5f, .5f, 1.f), //light diffusion 
		glm::vec4(0.5f, 0.5f, 0.5f, 1.f), //light specular
		glm::vec4(0.15f, 0.15f, 0.15f, 1.f) //globalambient
	);
	// Set up the shaders
	m_shader = new Shader();
	if (!m_shader->Initialize())
	{
		printf("Shader Failed to Initialize\n");
		return false;
	}

	// Add the vertex shader
	if (!m_shader->AddShader(GL_VERTEX_SHADER))
	{
		printf("Vertex Shader failed to Initialize\n");
		return false;
	}

	// Add the fragment shader
	if (!m_shader->AddShader(GL_FRAGMENT_SHADER))
	{
		printf("Fragment Shader failed to Initialize\n");
		return false;
	}

	if (!m_shader->AddAsteroidShader(GL_VERTEX_SHADER)) {
		printf("Asteroid Vertex Shader failed to Intitialize\n");
	}

	if (!m_shader->AddAsteroidShader(GL_FRAGMENT_SHADER)) {
		printf("Asteroid Fragment shader failed to Initialize\n");
	}

	if (!m_shader->AddSkyBoxShader(GL_VERTEX_SHADER)) {
		printf("SkyBox Vertex Shader failed to Intitialize\n");
	}

	if (!m_shader->AddSkyBoxShader(GL_FRAGMENT_SHADER)) {
		printf("SkyBox Fragment shader failed to Initialize\n");
	}
	// Connect the program
	if (!m_shader->Finalize())
	{
		printf("Program to Finalize\n");
		return false;
	}

	// Populate location bindings of the shader uniform/attribs
	if (!collectShPrLocs()) {
		printf("Some shader attribs not located!\n");
	}

	// Starship
	m_mesh = new Mesh(glm::vec3(2.0f, 3.0f, -5.0f), ".\\assets\\SpaceShip-1.obj", ".\\assets\\SpaceShip-1.png");

	m_skybox = new SkyBox(
		".\\assets\\SkyBox\\StarSkybox041.png",
		".\\assets\\SkyBox\\StarSkybox042.png",
		".\\assets\\SkyBox\\StarSkybox043.png",
		".\\assets\\SkyBox\\StarSkybox044.png",
		".\\assets\\SkyBox\\StarSkybox045.png",
		".\\assets\\SkyBox\\StarSkybox046.png");
	

	//m_skybox = new SkyBox(".\\assets\\SkyBox\\Galaxy2.jpg");

	m_asteroid = new AsteroidMesh(glm::vec3(1.0f, -3.0f, 4.0f), ".\\assets\\rock.obj", ".\\assets\\rock.png", 2500, 50, 10);

	m_asteroid2 = new AsteroidMesh(glm::vec3(1.0f, -3.0f, 4.0f), ".\\assets\\rock.obj", ".\\assets\\rock.png", 4000, 150, 15);
	// The Sun
	m_sphere = new Sphere(64, ".\\assets\\2k_sun.jpg", ".\\assets\\2k_sun-n.jpg");
	//m_sphere->invertNormals();
	// The Earth
	//m_sphere2 = new Sphere(48, ".\\assets\\2k_earth_daymap.jpg", ".\\assets\\2k_earth_daymap-n.jpg");
	m_sphere2 = new Sphere(48, ".\\assets\\2k_earth_daymap.jpg", ".\\assets\\2k_earth_daymap-n.jpg");
	// The moon
	//m_sphere3 = new Sphere(48, ".\\assets\\2k_moon.jpg");

	Saturn = new Mesh(glm::vec3(1.0, -3.0, 4.0f), ".\\assets\\Saturn_1.obj", ".\\assets\\Saturn_(body).jpg");

	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);

	return true;
}

void Graphics::HierarchicalUpdate2(double dt) {


	//cameraTransform = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -.1f));
	m_camera->Update();

	std::vector<float> speed, dist, rotSpeed, scale;
	glm::vec3 rotVector;
	glm::mat4 localTransform;
	// position of the sun	
	modelStack.push(glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0)));  // sun's coordinate
	localTransform = modelStack.top();		// The sun origin
	localTransform *= glm::rotate(glm::mat4(1.0f), (float)dt, glm::vec3(0.f, 1.f, 0.f));
	localTransform *= glm::scale(glm::vec3(5., 5., 5.));
	if (m_sphere != NULL)
		m_sphere->Update(localTransform);


	// Spaceship
	scale = { .01,.01,.01 };
	glm::vec3 offsetFromCamera = { 0, -0.5f, 1.1f }; // z cord moves ship forward
	float rotationOffset = 90.0f; // Need to rotate spaceship a bit to make it line up with camera

	localTransform = modelStack.top(); // start with sun's position
	localTransform *= glm::translate(glm::mat4(1.0f), m_camera->getPosition()); // Set position to the camera's
	// Rotations
	localTransform *= glm::rotate(glm::mat4(1.0f), glm::radians(rotationOffset), glm::vec3(0.0f, 1.0f, 0.0f)); // face forward
	localTransform *= glm::rotate(glm::mat4(1.0f), glm::radians(-m_camera->getYaw()), glm::vec3(0.0f, 1.0f, 0.0f)); // Match Yaw
	localTransform *= glm::rotate(glm::mat4(1.0f), glm::radians(-m_camera->getPitch() - 10.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Match Pitch
	// Offset from camera
	localTransform *= glm::translate(glm::mat4(1.0f), offsetFromCamera);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_mesh != NULL) {
		m_mesh->Update(localTransform);
	}
		

	// position of the first planet
	speed = { .5, .5, .5 };
	dist = { 15., 0, 15. };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 0.3, 0.3, 0.3};
	scale = { .5,.5,.5 };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(localTransform);
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_sphere2 != NULL)
		m_sphere2->Update(localTransform);

	// position of Saturn
	speed = { .3, .3, .3 };
	dist = { 100., 0., 100. };
	rotVector = { 0.,1.,0. };
	rotSpeed = { .3, .3, .3 };
	scale = { .007 ,.007, .007 };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(localTransform);
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (Saturn != NULL)
		Saturn->Update(localTransform);

	// position of the moon
	/*
	speed = { 6, 6, 6 };
	dist = { 1.25, 0., 1.25};
	rotVector = { 0.,1.,0. };
	rotSpeed = { .25, .25, .25 };
	scale = { .2f, .2f, .2f };
	localTransform = modelStack.top();
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], -sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(localTransform);			// store moon-planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));

	if (m_sphere3 != NULL)
		m_sphere3->Update(localTransform);
	*/

	modelStack.pop(); 	// back to the planet coordinate

	//modelStack.pop(); 	// back to the sun coordinate

	modelStack.pop();	// empy stack
}


void Graphics::ComputeTransforms(double dt, std::vector<float> speed, std::vector<float> dist,
	std::vector<float> rotSpeed, glm::vec3 rotVector, std::vector<float> scale, glm::mat4& tmat, glm::mat4& rmat, glm::mat4& smat) {
	tmat = glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2])
	);
	rmat = glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	smat = glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
}

void Graphics::Render()
{
	//clear the screen
	glClearColor(0.5, 0.2, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Start the correct program
	m_shader->Enable();

	// Send in the projection and view to the shader (stay the same while camera intrinsic(perspective) and extrinsic (view) parameters are the same
	glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));


	//glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sphere->GetModel()));


	//glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sphere2->GetModel()));

	// Render the objects
	/*if (m_cube != NULL){
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_cube->GetModel()));
		m_cube->Render(m_positionAttrib,m_colorAttrib);
	}*/
	
	
	if (m_mesh != NULL) {
		glUniform1i(m_hasTexture, false);
		glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(m_camera->GetView() * m_mesh->GetModel())))));
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mesh->GetModel()));
		if (m_mesh->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_mesh->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUseProgram(m_shader->GetShaderProgram());
			glUniform1i(sampler, 0);
			//m_mesh->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
			m_mesh->Render(m_positionAttrib, m_normAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	/*if (m_pyramid != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_pyramid->GetModel()));
		m_pyramid->Render(m_positionAttrib, m_colorAttrib);
	}*/

	if (Saturn != NULL) {
		glUniform1i(m_hasTexture, false);
		glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(m_camera->GetView() * Saturn->GetModel())))));
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(Saturn->GetModel()));
		if (Saturn->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, Saturn->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUseProgram(m_shader->GetShaderProgram());
			glUniform1i(sampler, 0);
			//m_mesh->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
			Saturn->Render(m_positionAttrib, m_normAttrib, m_tcAttrib, m_hasTexture);
		}
	}
	GLuint hasN = m_shader->GetUniformLocation("hasNormMap");
	if (m_sphere != NULL) {
		glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(m_camera->GetView() * m_sphere->GetModel())))));
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sphere->GetModel()));
		if (m_sphere->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sphere->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUseProgram(m_shader->GetShaderProgram());
			glUniform1i(sampler, 0);
			glUniform1i(hasN, false);
		}
		if (m_sphere->hasTexNorm) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_sphere->getTexNormalID());
			GLuint sampler = m_shader->GetUniformLocation("sp1");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 1);
			glUniform1i(hasN, true);
		}
		//m_sphere->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_texNormalAttrib, m_hasTexture, m_hasTexNorm);
		m_sphere->Render(m_positionAttrib, m_normAttrib, m_tcAttrib, m_texNormalAttrib, m_hasTexture, m_hasTexNorm);
	}


	if (m_sphere2 != NULL) {
		glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(m_camera->GetView() * m_sphere2->GetModel())))));
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sphere2->GetModel()));
		if (m_sphere2->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sphere2->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUseProgram(m_shader->GetShaderProgram());
			glUniform1i(sampler, 0);
			glUniform1i(hasN, false);
		}
		if (m_sphere2->hasTexNorm) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_sphere2->getTexNormalID());
			GLuint sampler = m_shader->GetUniformLocation("sp1");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUseProgram(m_shader->GetShaderProgram());
			glUniform1i(sampler, 1);
			glUniform1i(hasN, true);
		}
		m_sphere2->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_texNormalAttrib, m_hasTexture, m_hasTexNorm);
	}

	/*
	// Render Moon
	if (m_sphere3 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sphere3->GetModel()));
		if (m_sphere3->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sphere3->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_sphere3->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}
	*/
	m_shader->AsteroidEnable();
	
	glUniformMatrix4fv(m_astProjectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	glUniformMatrix4fv(m_astViewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

	if (m_asteroid != NULL) {
		glUniform1i(m_astHasTexture, false);

		if (m_asteroid->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_asteroid->getTextureID());
			GLuint sampler = m_shader->GetAsUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			glUseProgram(m_shader->GetAstShaderProgram());
			//m_mesh->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
			m_asteroid->Render(m_astPositionAttribLoc, m_astNormalAttribLoc,m_astTcAttribLoc, m_astHasTexture);
		}
	}
	
	if (m_asteroid2 != NULL) {
		glUniform1i(m_astHasTexture, false);
		if (m_asteroid2->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_asteroid2->getTextureID());
			GLuint sampler = m_shader->GetAsUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			glUseProgram(m_shader->GetAstShaderProgram());
			//m_mesh->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
			m_asteroid2->Render(m_astPositionAttribLoc, m_astNormalAttribLoc, m_astTcAttribLoc, m_astHasTexture);
		}
	}

	
	glDepthFunc(GL_LEQUAL);
	m_shader->SkyBoxEnable();

	glUniformMatrix4fv(m_sbViewMatrix, 1, GL_FALSE, glm::value_ptr(glm::mat4(glm::mat3(m_camera->GetView()))));
	glUniformMatrix4fv(m_sbProjectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	if (m_skybox != NULL) {
		glUniform1i(m_shader->GetSbUniformLocation("skybox"), 0);
		m_skybox->Render();
	}
	glDepthFunc(GL_LESS);

	// Get any errors from OpenGL
	auto error = glGetError();
	if (error != GL_NO_ERROR)
	{
		string val = ErrorString(error);
		std::cout << "Error initializing OpenGL! " << error << ", " << val << std::endl;
	}
}


bool Graphics::collectShPrLocs() {
	bool anyProblem = true;
	// Locate the projection matrix in the shader
	m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
	if (m_projectionMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_projectionMatrix not found\n");
		anyProblem = false;
	}

	// Locate the view matrix in the shader
	m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
	if (m_viewMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_viewMatrix not found\n");
		anyProblem = false;
	}

	// Locate the model matrix in the shader
	m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
	if (m_modelMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_modelMatrix not found\n");
		anyProblem = false;
	}

	//Locate the normal Matrix in shader
	m_normalMatrix = m_shader->GetUniformLocation("normMatrix");
	if (m_normalMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_normalMatrix not found\n");
		anyProblem = false;
	}

	// Locate the position vertex attribute
	m_positionAttrib = m_shader->GetAttribLocation("v_position");
	if (m_positionAttrib == -1)
	{
		printf("v_position attribute not found\n");
		anyProblem = false;
	}


	// Locate the normal vertex attribute
	m_normAttrib = m_shader->GetAttribLocation("v_normal");
	if (m_normAttrib == -1)
	{
		printf("v_normal attribute not found\n");
		anyProblem = false;
	}


	// Locate the color vertex attribute
	/*
	m_colorAttrib = m_shader->GetAttribLocation("v_color");
	if (m_colorAttrib == -1)
	{
		printf("v_color attribute not found\n");
		anyProblem = false;
	}
	*/

	// Locate the color vertex attribute
	m_tcAttrib = m_shader->GetAttribLocation("v_tc");
	if (m_tcAttrib == -1)
	{
		printf("v_texcoord attribute not found\n");
		anyProblem = false;
	}

	m_texNormalAttrib = m_shader->GetAttribLocation("v_tc");
	if (m_tcAttrib == -1)
	{
		printf("v_texcoord attribute not found\n");
		anyProblem = false;
	}

	m_hasTexture = m_shader->GetUniformLocation("hasTexture");
	if (m_hasTexture == INVALID_UNIFORM_LOCATION) {
		printf("hasTexture uniform not found\n");
		anyProblem = false;
	}

	m_hasTexNorm = m_shader->GetUniformLocation("hasNormalMap");
	if (m_hasTexture == INVALID_UNIFORM_LOCATION) {
		printf("hasNormalMap uniform not found\n");
		anyProblem = false;
	}

	//===================== Asteroid Vertex Attribute Setup ======================

	m_astProjectionMatrix = m_shader->GetAsUniformLocation("projectionMatrix");
	if (m_astProjectionMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_astProjectionMatrix not found\n");
		anyProblem = false;
	}

	m_astViewMatrix = m_shader->GetAsUniformLocation("viewMatrix");
	if (m_astViewMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_astViewMatrix not found\n");
		anyProblem = false;
	}

	/*
	m_astModelMatrix = m_shader->GetAsUniformLocation("modelMatrix");
	if (m_astModelMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_astModelMatrix not found\n");
		anyProblem = false;
	}
	*/

	//Locate the normal Matrix in shader
	
	/* m_astNormalMatrix = m_shader->GetAsUniformLocation("normMatrix");
	if (m_astNormalMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_astNormalMatrix not found\n");
		anyProblem = false;
	}*/

	m_astPositionAttribLoc = m_shader->GetAsAttribLocation("v_position");
	if (m_astPositionAttribLoc == INVALID_UNIFORM_LOCATION)
	{
		printf("m_astPositionAttribLoc not found\n");
		anyProblem = false;
	}

	m_astNormalAttribLoc = m_shader->GetAsAttribLocation("v_color");
	if (m_astNormalAttribLoc == INVALID_UNIFORM_LOCATION)
	{
		printf("m_astNormalAttribLoc not found\n");
		anyProblem = false;
	}

	m_astTcAttribLoc = m_shader->GetAsAttribLocation("v_tc");
	if (m_astTcAttribLoc == INVALID_UNIFORM_LOCATION)
	{
		printf("m_astTcAttribLoc not found\n");
		anyProblem = false;
	}

	m_astInstMatrixAttribLoc = m_shader->GetAsAttribLocation("aInstanceMatrix");
	if (m_astInstMatrixAttribLoc == INVALID_UNIFORM_LOCATION)
	{
		printf("m_astInstMatrixAttribAttribLoc not found\n");
		anyProblem = false;
	}

	m_astHasTexture = m_shader->GetAsUniformLocation("hasTexture");
	if (m_astHasTexture == INVALID_UNIFORM_LOCATION)
	{
		printf("m_astTcAttribLoc not found\n");
		anyProblem = false;
	}
   
	//=================== SkyBox Setup ========================

	//m_shader->SkyBoxEnable();
	//glUniform1i(m_shader->GetSbUniformLocation("skybox"), 0);

	m_sbProjectionMatrix = m_shader->GetSbUniformLocation("projection");
	if (m_sbProjectionMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_sbProjectionMatrix not found\n");
		anyProblem = false;
	}

	m_sbViewMatrix = m_shader->GetSbUniformLocation("view");
	if (m_sbViewMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_sbViewMatrix not found\n");
		anyProblem = false;
	}

	GLuint globalAmbLoc = glGetUniformLocation(m_shader->GetShaderProgram(), "GlobalAmbient");
	if (globalAmbLoc == INVALID_UNIFORM_LOCATION)
	{
		printf("globalAmbient element not found\n");
		anyProblem = false;
	}
	glProgramUniform4fv(m_shader->GetShaderProgram(), globalAmbLoc, 1, glm::value_ptr(m_light->m_globalAmbient));

	GLuint lightAloc = glGetUniformLocation(m_shader->GetShaderProgram(), "light.ambient");
	if (lightAloc == INVALID_UNIFORM_LOCATION)
	{
		printf("Shader LightAmbient element not found\n");
		anyProblem = false;
	}
	glProgramUniform4fv(m_shader->GetShaderProgram(), lightAloc, 1, glm::value_ptr(m_light->m_lightAmbient));

	GLuint lightDLoc = glGetUniformLocation(m_shader->GetShaderProgram(), "light.diffuse");
	if (lightDLoc == INVALID_UNIFORM_LOCATION)
	{
		printf("Shader lightDloc element not found\n");
		anyProblem = false;
	}
	glProgramUniform4fv(m_shader->GetShaderProgram(), lightDLoc, 1, glm::value_ptr(m_light->m_lightDiffuse));

	GLuint lightSLoc = glGetUniformLocation(m_shader->GetShaderProgram(), "light.spec");
	if (lightSLoc == INVALID_UNIFORM_LOCATION)
	{
		printf("Shader lightSloc element not found\n");
		anyProblem = false;
	}
	glProgramUniform4fv(m_shader->GetShaderProgram(), lightSLoc, 1, glm::value_ptr(m_light->m_lightSpecular));


	GLuint lightPosLoc = glGetUniformLocation(m_shader->GetShaderProgram(), "light.position");
	glProgramUniform3fv(m_shader->GetShaderProgram(), lightPosLoc, 1, glm::value_ptr(m_light->m_lightPositionViewSpace));

	float matAmbient[4] = { 0.5, 0.5, 0.5, 1. };
	float matDiff[4] = { 0.15, 0.15, 0.15, 1.0 };
	float matSpec[4] = { 0.5, 0.5, 0.5, 1.0 };
	float matShininess = 5.;

	GLuint mAmbLoc = glGetUniformLocation(m_shader->GetShaderProgram(), "material.ambient");
	glProgramUniform4fv(m_shader->GetShaderProgram(), mAmbLoc, 1, matAmbient);

	GLuint mDiffLoc = glGetUniformLocation(m_shader->GetShaderProgram(), "material.diffuse");
	glProgramUniform4fv(m_shader->GetShaderProgram(), mDiffLoc, 1, matDiff);

	GLuint mSpecLoc = glGetUniformLocation(m_shader->GetShaderProgram(), "material.spec");
	glProgramUniform4fv(m_shader->GetShaderProgram(), mSpecLoc, 1, matSpec);

	GLuint mShineLoc = glGetUniformLocation(m_shader->GetShaderProgram(), "material.shininess");
	glProgramUniform1f(m_shader->GetShaderProgram(), mShineLoc, matShininess);


	return anyProblem;
}

std::string Graphics::ErrorString(GLenum error)
{
	if (error == GL_INVALID_ENUM)
	{
		return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
	}

	else if (error == GL_INVALID_VALUE)
	{
		return "GL_INVALID_VALUE: A numeric argument is out of range.";
	}

	else if (error == GL_INVALID_OPERATION)
	{
		return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
	}

	else if (error == GL_INVALID_FRAMEBUFFER_OPERATION)
	{
		return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
	}

	else if (error == GL_OUT_OF_MEMORY)
	{
		return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
	}
	else
	{
		return "None";
	}
}