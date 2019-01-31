#pragma once

#include <functional>
#include <vector>

namespace test {

	/**
	* Classe qui va permettre de la g�n�ricit� dans les tests
	* On va factoriser tout le code possible pour que l'on puisse cr�er des mini applications tests
	*/
	class Test {

	public :

		Test() {};
		virtual ~Test() {};


		//Fonction a redef par nos tests qui h�ritent de Test
		virtual void OnUpdate(float delta) {};
		virtual void OnRender() {};
		virtual void OnImGuiRender() {};


	};


	//Classe qui va g�rer notre menu
	class TestMenu : public Test {

	public : 

		TestMenu(Test*& PointeurTestEnCours);

		void OnImGuiRender() override;

		template<typename T>
		void AjouteTest(const std::string NomTest) {

			std::cout << "Ajout du test : " << NomTest << '\n';

			//On ajoute le test � la liste des tests
			//										lambda expression (pour le constructeur ici)
			m_Tests.push_back(std::make_pair(NomTest, []() { return new T(); }));
		}


	private:

		//R�f�rence sur le pointeur de test actuel (dans le main) et le modifie en temps r�el
		Test*& m_TestActuel;

		//Tableau de paires (compos�e du nom du test et une fonction qui renvoit un pointeur sur ce test)
		//en g�n�ral un constructeur de test
		std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;

	};



}