/*
Animación:
Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada)
Compleja: Por medio de funciones y algoritmos.
Textura Animada
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para puerta
float rotacion = 0.0f;

//variable para garaje
float rotacion2 = 0.0f;
float deslizamiento = 0.0f;
bool derrape = false;


float movOffset;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;

//variables para keyframes
float reproduciranimacion, habilitaranimacion = 0;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
// Texturas de los backyardigans - PUERTA JOSHUA
Texture TipografiaBackyardigans;

Texture MarioBros;

//Casa Un show Más

Model Casa;
Model BaseCasa;
Model InteriorCasa;
Model PuertaCasa;
Model Monticulo;
Model puertaGaraje;

// Carrito de golf
Model Carrito;
Model RuedaFI;
Model RuedaFD;
Model RuedaTI;
Model RuedaTD;

// Modelos sala
Model mesaSala;
Model Atari;
Model Cuadro;
Model tv;
Model Gabinete;
Model Sofa;

// Modelos Cuarto de Rigby y Mordecai

//Cama 
Model BaseCama;
Model Colchon;
Model Cobija;
Model Almohada;

//MesaNoche
Model MesaNoche;
Model CajonTopMN;
Model CajonBotMN;

//Lampara
Model Lampara;

//Trampolin
Model baseTrampolin;
Model bordeTrampolin;
Model elasticoTrampolin;

//Cajonera Rigby
Model Cajonera;
Model Cajon;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//función para teclado de keyframes 
void inputKeyframes(bool* keys);

//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};


	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,			0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,			0.0f, -1.0f, 0.0f,

	};

	GLfloat backyardigansVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,			0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,			0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,			0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,			0.0f, -1.0f, 0.0f,

	};



	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj5); // todos los números

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj6); // solo un número

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(backyardigansVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7); // solo un número

}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

///////////////////////////////KEYFRAMES/////////////////////


//bool animacion = false;



//NEW// Keyframes
//model = glm::translate(model, glm::vec3(-10.0f, -2.0f, 0.0f));
float posXcarro = -33.0, posYcarro = -2.4, posZcarro = -6.0;
float movcarro_x = 0.0f, movcarro_y = 0.0f, movcarro_z = 0.0f;
float girocarro = 0;

#define MAX_FRAMES 100 //Número de cuadros máximos
int i_max_steps = 100; //Número de pasos entre cuadros para interpolación, a mayor número , más lento será el movimiento
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movcarro_x;		//Variable para PosicionX
	float movcarro_y;		//Variable para PosicionY
	float movcarro_z;		//Variable para PosicionZ
	float movcarro_xInc;		//Variable para IncrementoX
	float movcarro_yInc;		//Variable para IncrementoY
	float movcarro_zInc;		//Variable para IncrementoZ
	float girocarro;		//Variable para Girocarro
	float girocarroInc;		//Variable para IncrementoGirocarro
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 7;			//El número de cuadros guardados actualmente desde 0 para no sobreescribir
bool play = false;
int playIndex = 0;


void resetElements(void) //Tecla 0
{

	movcarro_x = KeyFrame[0].movcarro_x;
	movcarro_y = KeyFrame[0].movcarro_y;
	movcarro_y = KeyFrame[0].movcarro_z;
	girocarro = KeyFrame[0].girocarro;
}

void interpolation(void)
{
	KeyFrame[playIndex].movcarro_xInc = (KeyFrame[playIndex + 1].movcarro_x - KeyFrame[playIndex].movcarro_x) / i_max_steps;
	KeyFrame[playIndex].movcarro_yInc = (KeyFrame[playIndex + 1].movcarro_y - KeyFrame[playIndex].movcarro_y) / i_max_steps;
	KeyFrame[playIndex].movcarro_zInc = (KeyFrame[playIndex + 1].movcarro_z - KeyFrame[playIndex].movcarro_z) / i_max_steps;
	KeyFrame[playIndex].girocarroInc = (KeyFrame[playIndex + 1].girocarro - KeyFrame[playIndex].girocarro) / i_max_steps;

}


void animate(void)
{
	//Movimiento del objeto con barra espaciadora
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //fin de animación entre frames?
		{
			playIndex++;
			printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)	//Fin de toda la animación con último frame?
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("termino la animacion\n");
				playIndex = 0;
				play = false;
			}
			else //Interpolación del próximo cuadro
			{

				i_curr_steps = 0; //Resetea contador
				//Interpolar
				interpolation();
			}
		}
		else
		{
			//Dibujar Animación
			movcarro_x += KeyFrame[playIndex].movcarro_xInc;
			movcarro_y += KeyFrame[playIndex].movcarro_yInc;
			movcarro_z += KeyFrame[playIndex].movcarro_zInc;
			girocarro += KeyFrame[playIndex].girocarroInc;
			i_curr_steps++;
		}

	}
}

///////////////* FIN KEYFRAMES*////////////////////////////


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(-8.0f, 2.0f, -30.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 0.5f, 0.5f);

	brickTexture = Texture("Textures/pasto.jpg");
	brickTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/PastoJoshua.jpg");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	TipografiaBackyardigans = Texture("Textures/ProyectoCGEIHC.png");
	TipografiaBackyardigans.LoadTextureA();

	//CASA UN SHOW MÁS
	Casa = Model();
	Casa.LoadModel("Models/Casa/Casa.obj");
	BaseCasa = Model();
	BaseCasa.LoadModel("Models/Casa/BaseCasa.obj");
	InteriorCasa = Model();
	InteriorCasa.LoadModel("Models/Casa/ParteInterior.obj");
	Monticulo = Model();
	Monticulo.LoadModel("Models/Casa/Monticulo.obj");
	PuertaCasa = Model();
	PuertaCasa.LoadModel("Models/Casa/Puerta.obj");
	puertaGaraje = Model();
	puertaGaraje.LoadModel("Models/Casa/puertaGaraje.obj");

	//Carrito de golf
	Carrito = Model();
	Carrito.LoadModel("Models/Casa/Carrito.obj");
	RuedaFI = Model();
	RuedaFI.LoadModel("Models/Casa/RuedaFI.obj");
	RuedaFD = Model();
	RuedaFD.LoadModel("Models/Casa/RuedaFD.obj");
	RuedaTI = Model();
	RuedaTI.LoadModel("Models/Casa/RuedaTI.obj");
	RuedaTD = Model();
	RuedaTD.LoadModel("Models/Casa/RuedaTD.obj");

	//Sala 
	mesaSala = Model();
	mesaSala.LoadModel("Models/Sala/mesaSALA.obj");
	Atari = Model();
	Atari.LoadModel("Models/Sala/atari2600.dae");
	Cuadro = Model();
	Cuadro.LoadModel("Models/Sala/MarcoGod.dae");
	tv = Model();
	tv.LoadModel("Models/Sala/tv.obj");
	Gabinete = Model();
	Gabinete.LoadModel("Models/Sala/Gabinete.obj");
	Sofa = Model();
	Sofa.LoadModel("Models/Sala/Sofa.obj");

	MarioBros = Texture("Textures/MarioBros.jpg");
	MarioBros.LoadTextureA();

	// Cuarto Rigby y Mordecai
	//Cama //
	BaseCama = Model();
	BaseCama.LoadModel("Models/cuartoMR/Cama.obj");
	Colchon = Model();
	Colchon.LoadModel("Models/cuartoMR/colchon.obj");
	Cobija = Model();
	Cobija.LoadModel("Models/cuartoMR/sabana.obj");
	Almohada = Model();
	Almohada.LoadModel("Models/cuartoMR/almohada.obj");

	//MesaNoche
	MesaNoche = Model();
	MesaNoche.LoadModel("Models/cuartoMR/MesaNoche.obj");
	CajonTopMN = Model();
	CajonTopMN.LoadModel("Models/cuartoMR/CajonTopMN.obj");
	CajonBotMN = Model();
	CajonBotMN.LoadModel("Models/cuartoMR/cajonBottomMN.obj");

	//Lampara
	Lampara = Model();
	Lampara.LoadModel("Models/CuartoMR/lamparaMR.obj");

	//Trampolin
	baseTrampolin = Model();
	baseTrampolin.LoadModel("Models/CuartoMR/baseTrampolin.obj");
	bordeTrampolin = Model();
	bordeTrampolin.LoadModel("Models/CuartoMR/bordeTrampolin.obj");
	elasticoTrampolin = Model();
	elasticoTrampolin.LoadModel("Models/CuartoMR/elasticoTrampolin.obj");

	//Cajonera
	Cajonera = Model();
	Cajonera.LoadModel("Models/CuartoMR/cajonera.obj");
	Cajon = Model();
	Cajon.LoadModel("Models/CuartoMR/Cajon.obj");
	

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/nx.jpg");
	skyboxFaces.push_back("Textures/Skybox/px.jpg");
	skyboxFaces.push_back("Textures/Skybox/ny.jpg");
	skyboxFaces.push_back("Textures/Skybox/py.jpg");
	skyboxFaces.push_back("Textures/Skybox/nz.jpg");
	skyboxFaces.push_back("Textures/Skybox/pz.jpg");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		//0.0f, 0.0f,
		0.4f, 0.3f,
		1.0f, 0.0f, 0.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		0.2f, 1.0f,
		-18.0f, 21.0f, -5.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	//Declaración de segunda luz puntual (luz, segundo piso)
	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,
		-10.5f, 21.0f, 0.0f,
		0.1f, 0.1f, 0.05f);
	pointLightCount++;

	//Declaración de segunda luz puntual (luz, segundo piso)
	pointLights[2] = PointLight(1.0f, 1.0f, 1.0f,
		0.5f, 1.0f,
		-18.0f, 12.0f, -0.0f,
		0.1f, 0.1f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 1.2f,
		2.5f, 25.0f, -3.0f,
		0.0f, -0.1f, 0.0f,
		1.0f, 0.01f, 0.0f,
		11.0f);
	spotLightCount++;

	spotLights[2] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 1.2f,
		2.5f, 25.0f, 6.0f,
		0.0f, -0.1f, -0.01f,
		1.0f, 0.01f, 0.0f,
		10.0f);
	spotLightCount++;

	spotLights[3] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 1.2f,
		-22.0f, 7.8f, -3.0f,
		0.0f, -0.1f, -0.01f,
		1.0f, 0.01f, 0.0f,
		30.0f);
	spotLightCount++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	//Movimientos base para cada transformación de las puertas
	movOffset = 6.0f;

	glm::vec3 poscarro = glm::vec3(0.0f, 0.0f, 0.0f);

	//---------PARA TENER KEYFRAMES GUARDADOS NO VOLATILES QUE SIEMPRE SE UTILIZARAN SE DECLARAN AQUÍ
	KeyFrame[0].movcarro_x = 0.0f;
	KeyFrame[0].movcarro_y = 0.0f;
	KeyFrame[0].movcarro_z = 0.0f;
	KeyFrame[0].girocarro = 0;


	KeyFrame[1].movcarro_x = 0.0f;
	KeyFrame[1].movcarro_y = 0.0f;
	KeyFrame[1].movcarro_z = -25.0f;
	KeyFrame[1].girocarro = 0;


	KeyFrame[2].movcarro_x = 0.0f;
	KeyFrame[2].movcarro_y = 0.0f;
	KeyFrame[2].movcarro_z = -25.0f;
	KeyFrame[2].girocarro = 360;


	KeyFrame[3].movcarro_x = 0.0f;
	KeyFrame[3].movcarro_y = 0.0f;
	KeyFrame[3].movcarro_z = -25.0f;
	KeyFrame[3].girocarro = 720.0f;


	KeyFrame[4].movcarro_x = 0.0f;
	KeyFrame[4].movcarro_y = 0.0f;
	KeyFrame[4].movcarro_z = -25.0f;
	KeyFrame[4].girocarro = 1080.0f;


	KeyFrame[5].movcarro_x = 0.0f;
	KeyFrame[5].movcarro_y = 0.0f;
	KeyFrame[5].movcarro_z = -25.0f;
	KeyFrame[5].girocarro = 1080.0f;

	KeyFrame[6].movcarro_x = 0.0f;
	KeyFrame[6].movcarro_y = 0.0f;
	KeyFrame[6].movcarro_z = 0.5f;
	KeyFrame[6].girocarro = 1080.0f;

	printf("\nTeclas para uso de Keyframes:\n1.-Presionar tecla G para reproducir animacion.\n2.-Presionar 0 para volver a habilitar reproduccion de la animacion\n");

	glm::vec3 lowerLight(0.0f, 0.0f, 0.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//< >


		/* Animación en Loop Se ejecuta de forma continua mientras la aplicación está activa
		Si la animación no es el loop, y se puede iniciar varias veces, el estado final y el estado inicial
		deben de ser el mismo, o agregar elementos para que no se vea que los modelos desaparecen
		o aparecen de la nada.
		*/

		// Animación que se activa al presionar la tecla F, permite abrir o cerrar las puertas.
		if (mainWindow.getAbrirCerrarPuerta())
		{
			if (rotacion > 0.0f)
			{
				rotacion -= movOffset * deltaTime;
			}
		}

		else
		{
			if (rotacion < 90.0f)
			{
				rotacion += movOffset * deltaTime;
			}
		}

		// ABRIR GARAJE
		if (mainWindow.getAbrirCerrarGaraje())
		{
			if (rotacion2 > 0.0f)
			{
				rotacion2 -= movOffset * deltaTime;
				deslizamiento -= (movOffset * deltaTime)/10;
				derrape = false;
			}
		}

		else
		{
			if (rotacion2 < 90.0f)
			{
				rotacion2 += movOffset * deltaTime;
				deslizamiento += (movOffset * deltaTime)/10;
				derrape = true;
			}
		}



		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//-------Para Keyframes
		inputKeyframes(mainWindow.getsKeys());
		animate();

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation(); // para la textura con movimiento

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		//Reinicializando variables cada ciclo de reloj
		model = glm::mat4(1.0);
		modelaux = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);

		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();


		//----------------- CASA UN SHOW MAS --------------------//
		
		//Definición de la Casa
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, -2.3f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f, 15.0f, 15.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Casa.RenderModel();

		//Definición de la Base de la casa
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BaseCasa.RenderModel();

		//Definición del monticulo de la casa
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Monticulo.RenderModel();

		//Definición del interior de la casa
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		InteriorCasa.RenderModel();

		//Definición de la puerta de casa
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-8.32f, 11.45f, -8.1f));
		model = glm::rotate(model, rotacion * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); // ABRE Y CIERRA CON F
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(6.6f, 0.3f, 6.93f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaCasa.RenderModel();

		//Definición de la puerta del garaje
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-21.45f, 5.85f, -10.4f + deslizamiento));
		//model = glm::rotate(model, rotacion * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); 
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -rotacion2  * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(9.4f, 0.3f, 9.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		puertaGaraje.RenderModel();

		// -------------------- CARRITO -------------------- \\

		//Definición de carrito
		model = glm::mat4(1.0);
		poscarro = glm::vec3(posXcarro + movcarro_x, posYcarro + movcarro_y, posZcarro + movcarro_z);
		model = glm::translate(model, poscarro);
		model = glm::scale(model, glm::vec3(15.0f, 15.0f, 15.0f));
		model = glm::rotate(model, girocarro * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carrito.RenderModel();

		//Definición de rueda Izquierda Frontal
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		RuedaFI.RenderModel();

		//Definición de rueda Derecha Frontal
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		RuedaFD.RenderModel();

		//Definición de rueda Izquierda Trasera
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		RuedaTI.RenderModel();

		//Definición de rueda Derecha Trasera
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		RuedaTD.RenderModel();


		// ---------------------- SALA ---------------------- \\
	
		//Definición de mesa de sala
		for (int i = 1; i < 3; i++) {
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(( - 6.25f * i), 8.00f, 3.0f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			mesaSala.RenderModel();
		}

		//Definición atari
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-16.5f, 10.25f, 3.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Atari.RenderModel();

		//Definición de tv
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-23.0f, 9.4f, 3.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tv.RenderModel();

		//Definición de cuadro
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.2f, 12.2f, 5.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cuadro.RenderModel();

		//Definición de sofa
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-12.0f, 8.05f, 3.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sofa.RenderModel();
		
		//Definición del gabinete
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 7.90f, 7.35f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gabinete.RenderModel();




		//MARIO BROS PARA LA TV
		// avanza un 0.00025 en la coordenada u, para hacer legibles y lentos los cambios de números
		toffsetnumerocambiau += 0.00025;

		// Es el recorrido de la primer parte del nivel
		if (toffsetnumerocambiau <= 0.75)
		{
			// Mantenemos la altura en el renglón bajo, para leer la primer parte del nivel
			toffsetnumerov = 0.0;

		}
		// Cuando la coordenada sobrepasa el 1.0 en U, subimos a la siguiente altura (segunda parte del nivel)
		else if (toffsetnumerocambiau > 1.0 and toffsetnumerocambiau <= 1.75)
		{
			// mantenemos la altura (segunda parte del nivel)
			toffsetnumerov = 0.66;
		}
		else if (toffsetnumerocambiau > 2.0 and toffsetnumerocambiau <= 2.75)
		{	
			//altura última parte de nivel
			toffsetnumerov = 0.33;
		}
		// Cuando sobrepasa el 1.75, reiniciamos la coordenada en u y en v para volver al inicio del nivel
		else if (toffsetnumerocambiau > 3.1) {
			toffsetnumerocambiau = 0.0;
			toffsetnumerov -= 0.33;
		}
		else {
			//Cuando estamos en 0.75, sumamos 0.25 para llegar a 1 y empezar a recorrer el renglón de arriba
			toffsetnumerocambiau += 0.05;
		}


		toffset = glm::vec2(toffsetnumerocambiau, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-22.0f, 10.90f, 3.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(2.55f, 2.0f, 1.82f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		MarioBros.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//---------------------- CUARTO MORDECAI RIGBY ----------------------\\

		//Definición de base de cama
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-22.0f, 15.66f, -2.6f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BaseCama.RenderModel();

		//Definición de almohada
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Almohada.RenderModel();

		//Definición de colchón
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Colchon.RenderModel();

		//Definición de cobija
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cobija.RenderModel();

		//Definición de Mesa de Noche
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-18.0f, 15.66f, -6.5f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MesaNoche.RenderModel();

		//Definición de cajones
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CajonTopMN.RenderModel();

		//Definición de cajon abajo
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CajonBotMN.RenderModel();

		//Definición de lampara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-17.5f, 19.0f, -6.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.25f, 2.25f, 2.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.RenderModel();

		//Definición de trampolin
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 16.8f, -4.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2, 0.2f, 0.2f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		baseTrampolin.RenderModel();

		//borde azul
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bordeTrampolin.RenderModel();

		//elastico del trampolin
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		elasticoTrampolin.RenderModel();

		//definición de cajonera
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.2f, 15.70f, -4.0f));
		model = glm::scale(model, glm::vec3(5.0, 5.0f, 5.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cajonera.RenderModel();

		// cajon 1
		for (int i = 0; i < 4; i++)
		{
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, 0.0f + (0.3 * i), 0.0f));
			//model = glm::scale(model, glm::vec3(5.0, 5.0f, 5.0f));
			//modelaux = model;
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Cajon.RenderModel();

		}
		


		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
} 

void inputKeyframes(bool* keys)
{
	if (derrape/*keys[GLFW_KEY_SPACE]*/)
	{
		if (reproduciranimacion < 1)
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("\n presiona 0 para habilitar reproducir de nuevo la animación'\n");
				habilitaranimacion = 0;

			}
			else
			{
				play = false;

			}
		}
	}
	if (keys[GLFW_KEY_0])
	{	
		if (habilitaranimacion < 1 && reproduciranimacion>0)
		{
			printf("Ya puedes reproducir de nuevo la animación con la tecla G'\n");
			reproduciranimacion = 0;

		}
	}
}