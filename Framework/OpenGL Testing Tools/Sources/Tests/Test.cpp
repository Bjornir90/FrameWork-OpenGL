#include "Test.h"
#include "imgui/imgui.h"

namespace test {

	//Constructeur du menu, Initialise le test courant au test en cours (pass� en argument)
	TestMenu::TestMenu(Test *& PointeurTestEnCours):
	m_TestActuel(PointeurTestEnCours){	}


	//TestMenu n'impl�mente que l'affichage de ImGui
	void TestMenu::OnImGuiRender()	{

		for ( auto& test : m_Tests) {

			//Si on appuie sur le bouton d'un test
			if (ImGui::Button(test.first.c_str())) {

				// => On applique la fonction associ�e
				m_TestActuel = test.second();

			}

		}
	}

}