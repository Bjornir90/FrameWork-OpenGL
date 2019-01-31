#include "MonTest.h"

//Pour les calculs matriciels
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

/**
* Constructeur de MonTest 
* C'est ici que l'initialise tout ce dont notre application aura besoin
*/
test::MonTest::MonTest():
	//On initialise les matrices de vue et de projection
	m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
	m_Vue(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))) {

	//Tableau contenant les positions des points (4 pour un rectangle) 
	float positions[] = {
		100.0f, 100.0f,	//0
		200.0f, 100.0f, //1
		200.0f, 200.0f, //2
		100.0f, 200.0f  //3
	};

	//Pour �viter la redondance (cr�er 2x ou plus le m�me point) on cr�e un IndiceBuffer (tab d'indice)
	//qui va stocker les indices des points � tracer
	unsigned int indices[] = {
		0,1,2,
		2,3,0
	};

	//On cr�e le VertexArray
	m_VAO = std::make_unique<VertexArray>();

	//On stocke nos donn�es (les positions)
	//										nb de pts * nombre de floats/pt * size(float)
	m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 2 * sizeof(float));

	//D�finition de notre layout pour notre VertexArray
	VertexBufferLayout layout;
	//On utilse 2 floats par points (positions) => layout regroupe ce qui va ensemble
	//layout location 0 (cf vertex shader)
	layout.Push<float>(2);

	//On regroupe tout dans notre vertex Array (on donne du sens � nos donn�es)
	m_VAO->AddBuffer(*m_VertexBuffer, layout);

	//On stocke et lie � l'index buffer les indices
	m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

	//Cr�ation du shader
	m_Shader = std::make_unique<Shader>("Ressources/Shaders/MonShader.shader");

	m_Shader->Bind();

	//Uniform --> envoi de donn�es du CPU vers les shaders pour �tre utilis� comme une variable dans le shader
	//Utilisation uniform apr�s bind avec le shader qui utilise l'uniform
	//Uniform appel� � chaque dessin (draw call)
	m_Shader->SetUniform4f("u_Color", 0.0f, 0.0f, 1.0f, 1.0f);
}

/**
* Destructeur de MonTest (c'est normalement ici que l'on lib�re la m�moire)
*/
test::MonTest::~MonTest(){

	//Rien � faire ici => smarts pointeurs (unique_ptr) sont d�truits lorsqu'ils sont hors de port�e

}

/**
* Fonction de mise � jour (si l'on veut faire des choses qui d�pendent du temps)
* Pas utilis�e dans ce test...
*/
void test::MonTest::OnUpdate(float delta){

}

/**
* Fonction qui dessine (fait le rendu) de ce que l'on souhaite afficher
* Appel�e � chaque dessin dans la boucle de rendu (rendering loop)
*/
void test::MonTest::OnRender(){

	GLCALL(glClearColor(0.0f,0.0f,0.0f, 1.0f));
	GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	//Ici notre renderer ne stocke rien => on peut se permettre de le cr�er � chaque frame
	Renderer renderer;

	//Dessin 1
	{
		//Definition d'une matrice du mod�le => Consid�re toutes les transformations 
		//aka translation, rotation, scaling
		//� appliquer � notre mod�le...
		//Ici on le d�place vers en haut � droite
		glm::mat4 modele = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));

		//Matrice finale (que l'on va multiplier par chaque point pour qu'il s'affiche correctement)
		// /!\ ORDRE IMPORTANT ICI /!\ 
		glm::mat4 mvp = m_Proj * m_Vue * modele;

		//Seul moyen de ne pas g�rer les uniform � la main => materials
		//materials = shader + des donnees utiles au dessin
		m_Shader->Bind();

		//On envoit la matrice de projection au shader
		m_Shader->SetUniformMat4f("u_MVP", mvp);

		//en tant normal, un renderer prend un vertexArray + IndexBuffer + Materials (pas shader)
		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
	}

}

void test::MonTest::OnImGuiRender(){

}
