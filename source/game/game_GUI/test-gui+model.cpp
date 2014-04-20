#include "../m_lib.h"
#include "../g_lib.h"
#include "g_desk.h"
#include "g_field.h"
#include "g_sector.h"
#include "../game_model/model.h"

#include "..\..\aplication_tools\calibration\camera_calibration.h"

#include <gl/glut.h>

#define OPEN_GL

void CallBackFunc(int event, int x, int y, int flags, void* userdata);
void onDisplay();

double PointDistance(cv::Point A, cv::Point B) {
	double vzdalenost = sqrt(pow(A.x-B.x,2)+pow(A.y-B.y,2));
	//cout << "Vzdalenost: " << vzdalenost << endl;
	return vzdalenost;
}
cv::Mat obrazek;
std::list<g_field*> graphic_fields;
g_desk* plan = NULL;

int okno = 600;
//---------------------------------------------------------------------
// Nastaveni souradneho systemu v zavislosti na velikosti okna
//---------------------------------------------------------------------
void onResize(int w, int h)  {                   // w a h reprezentuje novou velikost okna
cout << "w = " << w << ", h = " << h << endl;
    glViewport(0, 0, w, h);                     // viditelna oblast pres cele okno
    glMatrixMode(GL_PROJECTION);                // zacatek modifikace projekcni matice
    glLoadIdentity();                           // vymazani projekcni matice (=identita)
    glOrtho(0, w, 0, h, -1, 1);                 // mapovani abstraktnich souradnic do souradnic okna
	okno = h;
}

void onKeyboard(unsigned char key, int x, int y) {
    if (key==27) exit(0);                       // pokud byla stlacena klavesa ESC, konec programu
}
	
void onMouse(int x, int y) {
	
	std::list <g_field*>::iterator iter_G;
	std::list <g_sector*>::iterator iter_G_sectors;
/*	
	glColor3f(0.2f, 0.8f, 0.5f);
	glPointSize(10.0f);
	glBegin(GL_POINTS);
	glVertex2i(x,600-y);
	glEnd();
	glFlush();
*/
	iter_G = graphic_fields.begin();
	for(;iter_G != graphic_fields.end(); iter_G++) {
		if (PointDistance((*iter_G)->getPosition(),cv::Point(x,okno-y)) < (*iter_G)->getR_Search()) {
			cout << "x = " << x << "; y = " << y << endl;
			(*iter_G)->select();
			
			break;
		} else {
			(*iter_G)->draw();
		}
	}
}
int main (int argc, char* argv[]) {

	m_player* hrac1 = new m_player("Hrac 1");
	m_player* hrac2 = new m_player("Hrac 2");
	m_player* hrac3 = new m_player("Hrac 3");
	m_player* hrac4 = new m_player("Hrac 4");

	hrac1->setColor(1.0,0.0,0.0);
	hrac2->setColor(0.0,1.0,0.0);
	hrac3->setColor(0.0,0.0,1.0);
	hrac4->setColor(0.2,0.2,0.2);
	m_desk* deska = new m_desk(1);

	std::list<m_sector*> sektory = deska->getSectors();
	std::list<m_sector*>::iterator iS = sektory.begin();

	hrac1->setSector((*iS));
	hrac1->setHome();
	std::advance(iS,1);
	hrac2->setSector((*iS));
	hrac2->setHome();

	m_field *policko = NULL;
	m_figure *figurka = NULL;
	//std::advance(iS,1);
	//hrac3->setSector((*iS));
	//std::advance(iS,1);
	//hrac4->setSector((*iS));

	cout << "--- nahozeni prvni figury ---\n";
	figurka = hrac1->getSector()->getStart()->crankUp();
	policko = deska->findStrokes(hrac1,5);
	figurka->move(policko);
	cout << "--- nahozeni druhe figury ---\n";
	figurka = hrac1->getSector()->getStart()->crankUp();
	policko = deska->findStrokes(hrac1,7);
	figurka->move(policko);
	policko = deska->findStrokes(hrac1,2);

	plan = new g_desk(okno,deska);

	glutInit(&argc, argv);
	glutCreateWindow("Zkousim OpenGL");
	glutReshapeWindow(800,600);
	glutPositionWindow(200,200);
	glutDisplayFunc(onDisplay);
	glutReshapeFunc(onResize);
	glutKeyboardFunc(onKeyboard);
	glutPassiveMotionFunc(onMouse);
	glutMainLoop();
	
	return EXIT_SUCCESS;
}

void onDisplay() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	std::list <g_field*>::iterator iter_G;
	std::list <g_sector*>::iterator iter_G_sectors;
	
	plan->draw();
}