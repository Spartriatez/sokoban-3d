/*
Niniejszy program jest wolnym oprogramowaniem; mo�esz go
rozprowadza� dalej i / lub modyfikowa� na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundacj� Wolnego
Oprogramowania - wed�ug wersji 2 tej Licencji lub(wed�ug twojego
wyboru) kt�rej� z p�niejszych wersji.

Niniejszy program rozpowszechniany jest z nadziej�, i� b�dzie on
u�yteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domy�lnej
gwarancji PRZYDATNO�CI HANDLOWEJ albo PRZYDATNO�CI DO OKRE�LONYCH
ZASTOSOWA�.W celu uzyskania bli�szych informacji si�gnij do
Powszechnej Licencji Publicznej GNU.

Z pewno�ci� wraz z niniejszym programem otrzyma�e� te� egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
je�li nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "myCube.h"
#include "board.h"
#include <fstream>
#include <conio.h>
#include <iostream>
#include "model2.h"

float speed = 0; // [radiany/s]
float l_high = 30.0f;
float l_angle = 10.0f;
int speed_x = 0, speed_y = 0;

Models::Sphere mySphere(0.5f, 18, 18);
Model2 model1, model2, circles, ring;
const int levels = 8;
const int s_plansza = 20; //rozmiar planszy
int plansza[levels][s_plansza][s_plansza];

int poziom = 0;
//skrz_str *skrzynka;
void move_up(int *);
int tsx, tsy;
int tempruch = 0, ruch = 0;
int reakcja1 = 0, reakcja2 = 0, reakcja3 = 0;
int restart = 0;
void key_callback(GLFWwindow* window, int key,
	int scancode, int action, int mods) {
	reakcja1 = reakcja2 = 1;
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) { tsy = speed_y; tsx = speed_x; speed_x = 1; };
		if (key == GLFW_KEY_RIGHT) { tsy = speed_y; tsx = speed_x; speed_x = -1; };
		if (key == GLFW_KEY_UP) { tsy = speed_y; tsx = speed_x; speed_y = -1; };
		if (key == GLFW_KEY_DOWN) { tsy = speed_y; tsx = speed_x; speed_y = 1; };
		if (key == GLFW_KEY_R) { restart = 1; }
		if (key == GLFW_KEY_J)speed = -3.14;
		if (key == GLFW_KEY_L)speed = +3.14;
		if (key == GLFW_KEY_I && l_high>10.0f)l_high -= 1.0f;
		if (key == GLFW_KEY_K && l_high<40.0f)l_high += 1.0f;
		if (key == GLFW_KEY_U && l_angle >0.0f)l_angle -= 1.0f;
		if (key == GLFW_KEY_O && l_angle <20.0f)l_angle += 1.0f;


	}

	if (action == GLFW_RELEASE) {
		speed = 0;
		speed_x = 0;
		speed_y = 0;
	}
}

//Procedura obs�ugi b��d�w
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}
GLuint tex0, tex1, tex2, tex3, tex4, tex5,tex6,tex7,tex8,tex9;

GLuint readTexture(char* filename) {

	GLuint	tex;
	glActiveTexture(GL_TEXTURE0);
	//Wczytanie do pami�ci komputera
	std::vector<unsigned char>image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do kt�rych wczytamy wymiary obrazka
							  //Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);
	//Import do pami�ci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
									   //Wczytaj obrazek do pami�ci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return tex;
}


//Procedura inicjuj�ca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, kt�ry nale�y wykona� raz, na pocz�tku programu************
	glClearColor(0, 0, 0, 1); //Czy�� ekran na czarno
							  glEnable(GL_LIGHTING); //W��cz tryb cieniowania
	//glEnable(GL_LIGHT0); //W��cz domyslne �wiat�o
	
	glEnable(GL_DEPTH_TEST); //W��cz u�ywanie Z-Bufora
	glEnable(GL_COLOR_MATERIAL); //glColor3d ma modyfikowa� w�asno�ci materia�u
	//glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	tex0 = readTexture("textures/wall2.png"); // sciany blokuj�ce
	tex1 = readTexture("textures/podest.png"); //
	tex2 = readTexture("textures/plansza.png");
	tex3 = readTexture("textures/podest3.png");
	tex4 = readTexture("textures/przes.png");
	tex5 = readTexture("textures/bricks.png");
	tex6 = readTexture("textures/planetoida.png");
	tex7 = readTexture("textures/rock2.png");
	tex8 = readTexture("textures/saturn.png");
	tex9 = readTexture("textures/ring.png");
	glfwSetKeyCallback(window, key_callback);

}
int poczatek = 1;
int *a, *b;
int skrzynka_id, s_id;
int skx;
int sky;
int **tabx = new int *[levels], **taby = new int *[levels];
// Tworzenie gry
int tmpa, tmpb;
int tmpskx, tmpsky;
int suma = 0;
int *ilzal;
int *ilosc;
int *restarta, *restartb, **restartska = new int *[levels], **restartskb = new int *[levels];
int **tabska = new int *[levels], **tabskb = new int *[levels];
int sprawdz(int x, int y, int ilosc)
{
	for (int i = 0; i<ilosc; i++)
	{
		if ((tabska[poziom][i] == x) && (tabskb[poziom][i] == y))
		{
			return i;
		}
	}
	return -1;
}
int tmp = 0;
char *nazw4;
int sum = 0;

int poziommm = 1;
//Procedura rysuj�ca zawarto�� sceny
void drawScene(GLFWwindow* window, float angle) {
	//************Tutaj umieszczaj kod rysuj�cy obraz******************l
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);	glEnable(GL_TEXTURE_2D);
	float swiatlo_otoczenia[] = { 0.1f, 0.2f, 0.2f,1.0f };
	float swiatlo_rozproszone[] = { 0.2f, 0.2f, 0.6f, 1.0f };
	float swiatlo_odbite[] = { 1.0f, 0.2f, 0.2f,1.0f };
	float swiatlo_pozycja[] = { 3.0f, 3.0f, 5.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, swiatlo_otoczenia);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, swiatlo_rozproszone);
	glLightfv(GL_LIGHT0, GL_SPECULAR, swiatlo_odbite);
	glLightfv(GL_LIGHT0, GL_POSITION, swiatlo_pozycja);

	float material[] = { 0.0, 0.0, 0.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material);
	float material2[] = { 0.7, 0.5, 0.5, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material2);
	float material3[] = { 0.5, 0.5, 0.5, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material3);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50);

	//glDisable(GL_LIGHTING);
	
	std::fstream plik2("plik.txt", std::ios::out);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wykonaj czyszczenie bufora kolor�w

	std::ifstream plik;

	glm::mat4 V = glm::lookAt( //Wylicz macierz widoku
		glm::vec3(0.0f, l_angle, l_high),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f));

	glm::mat4 P = glm::perspective(50 * PI / 180, 1.0f, 1.0f, 50.0f); //Wylicz macierz rzutowania

																	  //Za�aduj macierz rzutowania do OpenGL
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(P));

	//Przejd� w tryb pracy z macierz� Model-Widok
	glMatrixMode(GL_MODELVIEW);

	//Wylicz macierz obrotu o k�t angle wok� osi (0,1,1)
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::rotate(M, angle, glm::vec3(0, 0, 1));
	glLoadMatrixf(glm::value_ptr(V*M)); //Za�aduj wyliczon� macierz do OpenGL
	
	
/*
										//Narysuj model
	glEnableClientState(GL_VERTEX_ARRAY); //W��cz u�ywanie tablicy wierzcho�k�w
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glBindTexture(GL_TEXTURE_2D, tex2);
	glVertexPointer(3, GL_FLOAT, 0, boardVertices); //Wska� tablic� wierzcho�k�w
	glTexCoordPointer(2, GL_FLOAT, 0, geomTexCoords);
	//glColorPointer(3, GL_FLOAT, 0, boardColors); //Wska� tablic� kolor�w
	glDrawArrays(GL_QUADS, 0, boardCount); //Wykonaj rysowanie

	glVertexPointer(3, GL_FLOAT, 0, myCubeVertices); //Wska� tablic� wierzcho�k�w
	*/

	glBindTexture(GL_TEXTURE_2D, tex2);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY); //W��cz u�ywanie tablicy kolor�w
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	//glColorPointer(3, GL_FLOAT, 0, myCubeColors1); //Wska� tablic� kolor�w
	glVertexPointer(3, GL_FLOAT, 0, boardVertices);
	glTexCoordPointer(2, GL_FLOAT, 0, boardTex);
	glNormalPointer(GL_FLOAT, 0, geomNormals);
	glDrawArrays(GL_QUADS, 0, boardCount); //Wykonaj rysowanie
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	//----------------------------------------------------------------------------------------------------------------------//

	//-------------------------------------------------------------------------------------------
	if (poziommm == 1) {
		fstream plik, plik2, plik3;
		plik.open("test11.txt", ios::out);
		plik2.open("test22.txt", ios::out);
		plik3.open("test33.txt", ios::out);
		model1.loadOBJ("rock.obj", model1.Vertices, model1.Textures, model1.NormalVertices, &model1.numVertices, &model1.numTextures, &model1.normalNumVertices);
		model2.loadOBJ("rock2.obj", model2.Vertices, model2.Textures, model2.NormalVertices, &model2.numVertices, &model2.numTextures, &model2.normalNumVertices);
		if (circles.loadOBJ("sphere2.obj", circles.Vertices, circles.Textures, circles.NormalVertices, &circles.numVertices, &circles.numTextures, &circles.normalNumVertices) != false)
			cout << "nie zaladowano pliku" << endl;
		if (ring.loadOBJ2("rings1.obj", ring.Vertices, ring.NormalVertices, &ring.numVertices, &ring.normalNumVertices) != false)
			cout << "nie zaladowano pliku ring" << endl;

		plik << "Vertices " << model1.numVertices << endl;
		for (int i = 0; i < model1.numVertices * 3; i++)
			plik << "i " << i << "vertices: " << model1.Vertices[i] << endl;

		plik2 << "Textures " << model1.numTextures << endl;
		for (int i = 0; i < model1.numVertices * 2; i++)
			plik2 << "i " << i << "textures " << model1.Textures[i] << endl;
		plik3 << "Natur Vertices " << model1.normalNumVertices << endl;
		for (int i = 0; i < model1.numVertices * 3; i++)
			plik3 << "i " << i << " normal " << model1.NormalVertices[i] << endl;

		plik.close();
		plik2.close();
		plik3.close();
		poziommm = 0;
	}
	
	//-----------------------------------------------------------------------


	if (poczatek == 1) {
		restarta = new int[levels], restartb = new int[levels];
		ilosc = new int[levels];
		a = new int[levels], b = new int[levels];
		for (int i = 0; i < levels; i++)
			ilosc[i] = 0;
		for (int s = 0; s < levels; s++) {
			switch (s) {
			case 0: nazw4 = "levels/poziom1.txt"; break;
			case 1: nazw4 = "levels/poziom2.txt"; break;
			case 2: nazw4 = "levels/poziom3.txt"; break;
			case 3: nazw4 = "levels/poziom4.txt"; break;
			case 4: nazw4 = "levels/poziom5.txt"; break;
			case 5: nazw4 = "levels/poziom6.txt"; break;
			case 6: nazw4 = "levels/poziom7.txt"; break;
			case 7: nazw4 = "levels/poziom8.txt"; break;
			}
			plik.open(nazw4);
			for (int i = 0; i < s_plansza; i++) {
				for (int j = 0; j < s_plansza; j++)
					plik >> plansza[s][i][j];
			}
			plik.close();
		}

		
		for (int i = 0; i < levels; i++)
		{
			plik2 << "plansza " << i << std::endl;
			for (int j = 0; j < s_plansza; j++)
			{
				for (int k = 0; k < s_plansza; k++)
					plik2 << plansza[i][j][k];
				plik2 << std::endl;
			}
			plik2 << std::endl;
		}


		// Wstawianie ruchomych obiektow
		for (int s = 0; s < levels; s++) {
			for (int i = 0; i < s_plansza; i++)
			{
				for (int j = 0; j < s_plansza; j++)
				{
					if (plansza[s][i][j] == 3)
						sum++;
				}
			}
			ilosc[s] = sum;
			sum = 0;
		}
		for (int i = 0; i < levels; i++)
		{
			plik2 << "ilosc skrzynek " << ilosc[i] << std::endl;
		}
		for (int i = 0; i < levels; i++) {
			tabx[i] = new int[ilosc[i]];
			taby[i] = new int[ilosc[i]];
			restartska[i] = new int[ilosc[i]];
			restartskb[i] = new int[ilosc[i]];
			tabska[i] = new int[ilosc[i]];
			tabskb[i] = new int[ilosc[i]];
		}


		for (int s = 0;s<levels;s++) {
			for (int i = 0; i < s_plansza; i++)
			{
				for (int j = 0; j < s_plansza; j++)
				{
					if (plansza[s][i][j] == 4)
					{
						restarta[s] = a[s] = i;
						restartb[s] = b[s] = j;
					}
					if (plansza[s][i][j] == 3)
					{
						restartska[s][tmp] = tabska[s][tmp] = j;
						restartskb[s][tmp] = tabskb[s][tmp] = i;
						plik2 << "s " << s << "tmp " << tmp << "tabska " << tabska[s][tmp] << " tabskb " << tabskb[s][tmp] << std::endl;
						tmp++;
					}
				}
			}
			tmp = 0;
		}

		poczatek = 0;
	}
	else {
		suma = 0;
		tmpa = a[poziom];
		tmpb = b[poziom];
		ilzal = new int[ilosc[poziom]];
		for (int i = 0; i < ilosc[poziom]; i++)
			ilzal[i] = 0;
		if (reakcja1 == 1) {
			a[poziom] = a[poziom] + speed_y;
			b[poziom] = b[poziom] + speed_x;
			reakcja1 = 0;
		}
		//std::cout << "x " << speed_x << " y  " << speed_y << " tsx " << tsx << " tsy " << tsy << std::endl;

		if (plansza[poziom][a[poziom]][b[poziom]] == 1 || plansza[poziom][a[poziom]][b[poziom]] == 0 /*|| plansza[a][b]==3*/)
		{
			a[poziom] = tmpa;
			b[poziom] = tmpb;//speed_x i speed_y zrobic
			speed_x = tsx;
			speed_y = tsy;
		}
		tmpskx = skx;
		tmpsky = sky;

		skrzynka_id = sprawdz(b[poziom], a[poziom], ilosc[poziom]);
		//std::cout << "sprawdz " << skrzynka_id << std::endl;

		sky = b[poziom] + speed_x;
		skx = a[poziom] + speed_y;
		std::cout << "skx " << skx << " sky " << sky << std::endl;

		if (skrzynka_id != -1)
		{
			if (plansza[poziom][skx][sky] == 1 || plansza[poziom][skx][sky] == 0)
			{
				skx = tmpskx;
				sky = tmpsky;
				a[poziom] = tmpa;
				b[poziom] = tmpb;
			}
			else if (sprawdz(sky, skx, ilosc[poziom]) != -1) {
				s_id = sprawdz(sky, skx, ilosc[poziom]);
				skx = tmpskx;
				sky = tmpsky;
				a[poziom] = tmpa;
				b[poziom] = tmpb;
			}
			else {
				tabska[poziom][skrzynka_id] = sky;
				tabskb[poziom][skrzynka_id] = skx;
			}
		}

		// rysowanie postaci gracza
		glm::mat4 W = glm::mat4(1.0f);
		W = glm::translate(W, glm::vec3(-10 + b[poziom] * 1, -10 + a[poziom] * 1, 0.5f));
		//W = glm::scale(W, glm::vec3(0.2f, 0.2f, 0.2f));
		glLoadMatrixf(glm::value_ptr(V*M*W));
		// ----------------------------


		//glColorPointer(3, GL_FLOAT, 0, myCubeColors4); //Wska� tablic� kolor�w
		glBindTexture(GL_TEXTURE_2D, tex3);
		mySphere.drawSolid();
		/*
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glVertexPointer(3, GL_FLOAT, 0, myCubeVertices);
		glTexCoordPointer(2, GL_FLOAT, 0, geomTexCoords);
		glDrawArrays(GL_QUADS, 0, myCubeVertexCount); //Wykonaj rysowanie
		*/
		for (int i = 0; i < ilosc[poziom]; i++)
		{
			/*plik2 << "poziom " << poziom << std::endl;
			plik2 << "i" << i << "tabska " << tabska[i] << " tabskb " << tabskb[i] << std::endl;*/
			// ----------------------------------------------

			// rysowanie skrzynek		

			if (plansza[poziom][tabskb[poziom][i]][tabska[poziom][i]] == 2)
			{
				//glEnableClientState(GL_VERTEX_ARRAY);
				//glEnableClientState(GL_COLOR_ARRAY);
				glm::mat4 W = glm::mat4(1.0f);
				W = glm::translate(W, glm::vec3(-10 + tabska[poziom][i] * 1, -10 + tabskb[poziom][i] * 1, 0.5f));
				W = glm::scale(W, glm::vec3(0.8f, 0.8f, 1.0f));
				glLoadMatrixf(glm::value_ptr(V*M*W));

				//glColorPointer(3, GL_FLOAT, 0, myCubeColors3); //Wska� tablic� kolor�w
				glBindTexture(GL_TEXTURE_2D, tex4);
				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glVertexPointer(3, GL_FLOAT, 0, myCubeVertices);
				glTexCoordPointer(2, GL_FLOAT, 0, geomTexCoords);
				glDrawArrays(GL_QUADS, 0, myCubeVertexCount); //Wykonaj rysowanie*/
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				ilzal[i] = 1;
			}
			else {
				//glEnableClientState(GL_VERTEX_ARRAY);
				//glEnableClientState(GL_COLOR_ARRAY);
				glm::mat4 W = glm::mat4(1.0f);
				W = glm::translate(W, glm::vec3(-10 + tabska[poziom][i] * 1, -10 + tabskb[poziom][i] * 1, 0.5f));
				W = glm::scale(W, glm::vec3(0.8f, 0.8f, 1.0f));
				glLoadMatrixf(glm::value_ptr(V*M*W));
				glBindTexture(GL_TEXTURE_2D, tex4);
				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glVertexPointer(3, GL_FLOAT, 0, myCubeVertices);
				glTexCoordPointer(2, GL_FLOAT, 0, geomTexCoords);
				//glColorPointer(3, GL_FLOAT, 0, myCubeColors3); //Wska� tablic� kolor�w
				glDrawArrays(GL_QUADS, 0, myCubeVertexCount); //Wykonaj rysowanie
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				ilzal[i] = 0;
			}
		}
		// --------------------------------------------------


		for (int i = 0; i < ilosc[poziom]; i++)
			suma += ilzal[i];
		if (suma == ilosc[poziom])
		{
			suma = 0;
			poziom++;
		}
		if (restart == 1)
		{
			a[poziom] = restarta[poziom];
			b[poziom] = restartb[poziom];
			for (int i = 0; i < ilosc[poziom]; i++)
			{
				tabska[poziom][i] = restartska[poziom][i];
				tabskb[poziom][i] = restartskb[poziom][i];
			}
			suma = 0;
			restart = 0;
		}

		// ----------------------------------------------------
		// rysowanie reszty element�w


		std::cout << "suma fdfsfsfs   " << suma << "ilosc" << ilosc[poziom] << std::endl;
		for (int i = 0; i < s_plansza; i++) {
			for (int j = 0; j < s_plansza; j++) {
				//glDisableClientState(GL_COLOR_ARRAY);
				if (plansza[poziom][i][j] == 1) { // �ciany blokuj�ce

					glm::mat4 W = glm::mat4(1.0f);
					W = glm::translate(W, glm::vec3(-10 + j * 1, -10 + i * 1, 0.5f));
					//W = glm::scale(W, glm::vec3(0.9f, 0.9f, 0.9f));
					glLoadMatrixf(glm::value_ptr(V*M*W));
					glBindTexture(GL_TEXTURE_2D, tex0);
					glEnableClientState(GL_VERTEX_ARRAY);
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					glEnableClientState(GL_NORMAL_ARRAY);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


					//glColorPointer(3, GL_FLOAT, 0, myCubeColors1); //Wska� tablic� kolor�w
					glVertexPointer(3, GL_FLOAT, 0, myCubeVertices);
					glNormalPointer(GL_FLOAT, 0, normals);
					glTexCoordPointer(2, GL_FLOAT, 0, geomTexCoords);
					glDrawArrays(GL_QUADS, 0, myCubeVertexCount); //Wykonaj rysowanie
					glDisableClientState(GL_VERTEX_ARRAY);
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
					glDisableClientState(GL_NORMAL_ARRAY);
				}
				if (plansza[poziom][i][j] == 2) { // podest

					glm::mat4 W = glm::mat4(1.0f);
					W = glm::translate(W, glm::vec3(-10 + j * 1, -10 + i * 1, 0.1f));
					//W = glm::translate(W, glm::vec3(1,1,1));
					W = glm::scale(W, glm::vec3(1.0f, 1.0f, 0.2f));
					glLoadMatrixf(glm::value_ptr(V*M*W));
					glBindTexture(GL_TEXTURE_2D, tex1);
					glEnableClientState(GL_VERTEX_ARRAY);
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					glEnableClientState(GL_NORMAL_ARRAY);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


					//glColorPointer(3, GL_FLOAT, 0, myCubeColors1); //Wska� tablic� kolor�w
					glVertexPointer(3, GL_FLOAT, 0, myCubeVertices);
					glTexCoordPointer(2, GL_FLOAT, 0, geomTexCoords);
					glDrawArrays(GL_QUADS, 0, myCubeVertexCount); //Wykonaj rysowanie
					glDisableClientState(GL_VERTEX_ARRAY);
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
					glDisableClientState(GL_NORMAL_ARRAY);
				}

			}
		}
	//-----------------------------------------------------------------------------
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindTexture(GL_TEXTURE_2D, tex6);
		glVertexPointer(3, GL_FLOAT, 0, model1.Vertices);
		glTexCoordPointer(2, GL_FLOAT, 0, model1.Textures);
		glm::mat4 M1 = glm::mat4(1.0f);
		M1 = glm::translate(M1, glm::vec3(5, 5, 5));
		glLoadMatrixf(glm::value_ptr(V*M1*M));
		glNormalPointer(GL_FLOAT, sizeof(float) * 3, model1.NormalVertices);
		glDrawArrays(GL_TRIANGLES, 0, model1.numVertices);
		glBindTexture(GL_TEXTURE_2D, tex7);
		glm::mat4 M2 = glm::mat4(1.0f);
		M2 = glm::translate(M2, glm::vec3(7, 7, 6));
		M2 = glm::scale(M2, glm::vec3(2.0f, 2.0f, 2.0f));
		glLoadMatrixf(glm::value_ptr(V*M2*M));
		glVertexPointer(3, GL_FLOAT, 0, model2.Vertices);
		glTexCoordPointer(2, GL_FLOAT, 0, model2.Textures);

		glNormalPointer(GL_FLOAT, sizeof(float) * 3, model2.NormalVertices);
		glDrawArrays(GL_TRIANGLES, 0, model2.numVertices);

		glBindTexture(GL_TEXTURE_2D, tex8);
		glm::mat4 M3 = glm::mat4(1.0f);
		
		M3 = glm::translate(M3, glm::vec3(-15, -15, -5));
		M3 = glm::scale(M3, glm::vec3(2.0f, 2.0f, 2.0f));
		glLoadMatrixf(glm::value_ptr(V*M3));
		glVertexPointer(3, GL_FLOAT, 0, circles.Vertices);
		glTexCoordPointer(2, GL_FLOAT, 0, circles.Textures);

		glNormalPointer(GL_FLOAT, sizeof(float) * 3, circles.NormalVertices);
		glDrawArrays(GL_TRIANGLES, 0, circles.numVertices);

		glBindTexture(GL_TEXTURE_2D, tex9);
		glVertexPointer(3, GL_FLOAT, 0, ring.Vertices);


		glNormalPointer(GL_FLOAT, sizeof(float) * 3, ring.NormalVertices);
		glDrawArrays(GL_QUADS, 0, ring.numVertices);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	//---------------------------------------------------------------------------
	}
	plik2.close();
	//if (poziom == 8)
	//	poziom = 0;
	//std::cout << "poziom " << poziom << std::endl;
	//-----------------------------------------------------------------------------------------------------------------------//	
	glDisableClientState(GL_VERTEX_ARRAY); //Wy��cz u�ywanie tablicy wierzcho�k�w
	glDisableClientState(GL_COLOR_ARRAY); //Wy��cz u�ywanie tablicy kolor�w
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);


	//Przerzu� tylny bufor na przedni
	glfwSwapBuffers(window);

}


int main(void)
{


	GLFWwindow* window; //Wska�nik na obiekt reprezentuj�cy okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedur� obs�ugi b��d�w

	if (!glfwInit()) { //Zainicjuj bibliotek� GLFW
		fprintf(stderr, "Nie mo�na zainicjowa� GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(750, 750, "OpenGL", NULL, NULL);  //Utw�rz okno 500x500 o tytule "OpenGL" i kontekst OpenGL. 

	if (!window) //Je�eli okna nie uda�o si� utworzy�, to zamknij program
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje si� aktywny i polecenia OpenGL b�d� dotyczy� w�a�nie jego.
	glfwSwapInterval(1); //Czekaj na 1 powr�t plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotek� GLEW
		fprintf(stderr, "Nie mo�na zainicjowa� GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjuj�ce

	float angle = 0; //K�t obrotu torusa
	glfwSetTime(0); //Wyzeruj licznik czasu

					//G��wna p�tla
	while (!glfwWindowShouldClose(window)) //Tak d�ugo jak okno nie powinno zosta� zamkni�te
	{
		angle += speed*glfwGetTime(); //Zwi�ksz k�t o pr�dko�� k�tow� razy czas jaki up�yn�� od poprzedniej klatki
		glfwSetTime(0); //Wyzeruj licznik czasu
		drawScene(window, angle); //Wykonaj procedur� rysuj�c�
		glfwPollEvents(); //Wykonaj procedury callback w zalezno�ci od zdarze� jakie zasz�y.
	}
	glDeleteTextures(1, &tex0);
	glDeleteTextures(1, &tex1);
	glDeleteTextures(1, &tex2);
	glDeleteTextures(1, &tex3);
	glDeleteTextures(1, &tex4);
	glDeleteTextures(1, &tex5);
	glDeleteTextures(1, &tex6);
	glDeleteTextures(1, &tex7);
	glDeleteTextures(1, &tex8);
	glDeleteTextures(1, &tex9);
	glfwDestroyWindow(window); //Usu� kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zaj�te przez GLFW
	exit(EXIT_SUCCESS);
}

void move_up(int)
{
}
