#include "..\source\game\m_lib.h"
#include "..\source\game\g_lib.h"
#include "..\source\game\game_GUI\g_desk.h"
#include "..\source\game\game_GUI\g_field.h"
#include "..\source\game\game_GUI\g_sector.h"
#include "..\source\game\game_model\model.h"

#include "..\source\aplication_tools\calibration\camera_calibration.h"
#include "..\source\aplication_tools\detectors\hand_detector\hand_detector.h"
#include "states.h"

#include <gl/glut.h>

#define CUBE_SIZE 25.0
class g_field;
class g_desk;
class g_sector;
class m_field;
class m_desk;
class m_sector;

using namespace cv;
using namespace std;

/* prototypy */
void initOpenGL(int& argc, char* argv[]);
void onResize(int w, int h);
void onMouse(int button, int state, int x, int y);
void onKeyboard(unsigned char key, int x, int y);
void onDisplay();
void idleFunc();

/* Pomocne */
string decodeState();
int throwDice();
bool isWinner(m_sector* players_sector);
bool isCorect(list<m_figure*> players_figure, m_field* hit_field, m_figure*& figure);
g_field* findMatch(cv::Point2f hit);
int findControlObject(cv::Point2f hit);
double PointDistance(cv::Point A, cv::Point B);
cv::Point2f idealPoint(vector<Point2f>& hits);
void game();
void drawGameCube(int x, int y);
void drawToss(int x, int y,int count);

/* globalni promenne */
int state = IDLE;
int id = 0; //cislo hrace
VideoCapture cap;
m_field *policko = NULL;
m_figure *figurka = NULL;
m_figure* new_figure = NULL;
m_desk* deska = NULL;
Mat homography, camera, distCoeff;
cv::vector<m_player*> players;
std::list<g_field*> graphic_fields;
vector<cv::Point> other_graphic_objects;
bool findG = false;
bool winner = false;
bool act = false;
g_desk* plan = NULL;
int okno = 600;
int global_x = 0;
int global_y = 0;
int xK6 = 0; //hodnota hodu





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

void onMouse(int button, int state, int x, int y) {
	if((button == GLUT_LEFT_BUTTON) && !act) {
		global_x = x;
		global_y = y;
		act = true;
	} else {
		global_x = 0;
		global_y = 0;
	}
}

void onKeyboard(unsigned char key, int x, int y) {
    if (key==27) exit(0);                       // pokud byla stlacena klavesa ESC, konec programu
}

void onDisplay() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
 
	
	std::list <g_field*>::iterator iter_G;
	std::list <g_sector*>::iterator iter_G_sectors;
	

	plan->draw();

	//glLoadIdentity();
	//glPushMatrix();
	//glRotatef((id+1)*45.0f,0.0f,0.0f,-1.0f);
	//glPushMatrix();
	//glTranslatef(other_graphic_objects[1].x,other_graphic_objects[1].y,0.0f);
	

	drawGameCube(other_graphic_objects[id].x,other_graphic_objects[id].y);
	drawToss(other_graphic_objects[id].x,other_graphic_objects[id].y,xK6);

	//glPopMatrix();
	//glPopMatrix();

	glFlush();
	glutSwapBuffers();


}

void initOpenGL(int& argc, char* argv[]) {
	glutInit(&argc, argv);
	glutCreateWindow("Zkousim OpenGL");
	glutReshapeWindow(800,650);
	glutPositionWindow(1366,-20);
	glutDisplayFunc(onDisplay);
	glutReshapeFunc(onResize);
	glutKeyboardFunc(onKeyboard);
	glutMouseFunc(onMouse);
	srand(time(NULL));
	glutIdleFunc(idleFunc);
	glLoadIdentity();
}

void idleFunc() {
	game();
}
void game() {
	//cout << "IN State: " << decodeState() << " => " << players[id]->getName() << endl;
	if (winner) {
		state = WINNER;
		cout << "konec hry" << endl;
	}
	if(state == IDLE) {
		cout << "---- " << players[id]->getName() << " -----" << endl;
		state = DETECT_TOSS;
	}

	if(state == DETECT_TOSS) {
		cout << "State: " << decodeState() << " => " << players[id]->getName() << endl;
		vector<Point2f> hits1;
		Point act_hit;
		if(detectHand(cap,hits1) && !hits1.empty()) {
			//hits.push_back(cv::Point(global_x,global_y));
			//act = false;
			global_x = 0;
			global_y = 0;
			vector<Point2f> hits_toss;
			perspectiveTransform(hits1,hits_toss,homography);
			for(unsigned int j = 0; j < hits_toss.size(); j++) {
				act_hit = hits_toss[j];
				act_hit.y = okno - act_hit.y; //korekce windows OpenGL
				cout << "***** kontrolujeme shodu s other_graphic_objects *****" << endl;
				if((findControlObject(act_hit)) != -1) {
					int hod = 0;
					hod = throwDice();
					if( xK6 == 6 && new_figure == NULL) {
						xK6 += hod;
					} else {
						xK6 = hod;
					}
					state = TOSS;
					break;
				}
			}
			glColor3f(0.8f,0.8f,0.8f);
			glVertex2i(act_hit.x,act_hit.y);
			if(new_figure != NULL) {
				state = STROKES;
			} 
			glutPostRedisplay(); 
		}

	}
	
	if(state == TOSS) {
		cout << "State: " << decodeState() << endl;
		cout << "hod: " << xK6 <<endl;
		if(xK6 == 6) {
			if(players[id]->getSector()->getStart()->getFigureCount() > 0) {
				new_figure = players[id]->getSector()->getStart()->crankUp(); //nahozeni figury
				xK6 = 0;
				state = DETECT_TOSS;
				glutPostRedisplay();
			} 
			state = DETECT_TOSS; //dalsi hod - po nahozeni nebo po 6
		} else {
			m_sector* players_sector = players[id]->getSector();
			if((players_sector->getStart()->getFigureCount() + players_sector->getFinish()->getFigureCount()) == 4) {
				state = NEXT_PLAYER;
				glutPostRedisplay();
			} else {
				state = DETECT;
				glutPostRedisplay();
			}
		}
	}

	if(state == STROKES) {
		cout << "State: " << decodeState() << " <= hod: " << xK6 << endl;
		deska->findStrokes(players[id],xK6);
		//getchar();
		if((new_figure != NULL) || (figurka != NULL)) {
			state = MOVE;
		}	
	}
	
	if(state == MOVE) {
		cout << "State: " << decodeState() << endl;
		if (new_figure != NULL) {
			figurka = new_figure;
		}
		if (figurka != NULL) {
			figurka->move(players[id]->getStrokes()[figurka->getID()]);
			state = NEXT_PLAYER;
			glutPostRedisplay();
		}
	}
	if(state == NEXT_PLAYER) {
		cout << "State: " << decodeState() << endl;
		winner = isWinner(players[id]->getSector());
		xK6 = 0;
		new_figure = NULL;
		figurka = NULL;
		id++;
		if(id >= players.size())
			id = 0;
		state = IDLE;
		global_x = 0;
		global_y = 0;
	}	

	if(state == DETECT) {
		cout << "State: " << decodeState() << " => " << players[id]->getName() << " <= hod: " << xK6 << endl;
		vector<Point2f> hits2;
		Point act_hit;
		g_field* hit_field = NULL;
		if(detectHand(cap,hits2) && !hits2.empty()) {
			//hits.push_back(cv::Point(global_x,global_y));
			//act = false;
			//global_x = 0;
			//global_y = 0;
			vector<Point2f> hits_figure;
			perspectiveTransform(hits2,hits_figure,homography);
			act_hit = idealPoint(hits_figure);
			cout << "***** kontrolujeme shodu s g_field *****" << endl;
			if((hit_field = findMatch(act_hit)) != NULL) {
				cout << "***** je korektni? -> " ;
				findG = isCorect(players[id]->getMyFigures(),hit_field->getModel(),figurka);
				cout << findG << " *****" << endl;
			}
			cout << "kreslim bod do hraci plochy" <<endl;
			glPointSize(5.0);
			glColor3f(1.0f,0.0f,0.0f);
			glBegin(GL_POINTS);
				glVertex2f(act_hit.x,act_hit.y);
			glEnd();
			glFlush();
			if(findG && figurka != NULL) {
				state = STROKES;
				return;
			}
			//glutPostRedisplay();
		}
	}
	/*
		for (unsigned int id = 0; id < players.size(); id++) { //stridani hracu
			xK6 = throwDice(); //hod kostkou
			std::cout << "***** hod = " << xK6 << " ***** " << players[id]->getName() << endl;

			if(xK6 == 6) {
				std::cout << "***** resime nahozeni *****" <<endl;
				players[id]->getSector()->getStart()->crankUp();
				cout << "**** prekreslujem *****" << endl;
				glutPostRedisplay();
				xK6 = throwDice(); //hazeme znovu
				cout << "***** hod (2) = " << xK6 << " *****" << endl;
				if (xK6 == 6) {
					xK6 += throwDice();
				}
			} else if (players[id]->getSector()->getStart()->getFigureCount() == 4) {
				continue;
			}
			cout << "**** hledame moznosti *****" << endl;
			//getchar();
			deska->findStrokes(players[id],xK6);
			while(true) {
				vector<Point2f> hits;
				Point act_hit;
				g_field* hit_field = NULL;
				if(act) {
					cout << "***** vkladame bod *****" << endl << global_x << ", " <<global_y << endl;
					hits.push_back(cv::Point(global_x,global_y));
					act = false;
				} /*else {
					return;
				}
				if(detectHand(cap,hits)) {
					perspectiveTransform(hits,hits,homography);
					for(unsigned int j = 0; j < hits.size(); j++) {
						act_hit = hits[j];
						act_hit.y = okno - act_hit.y; //korekce windows OpenGL
						cout << "***** kontrolujeme shodu s g_field *****" << endl;
						if((hit_field = findMatch(act_hit)) != NULL) {
							cout << "***** je korektni? -> " ;
							findG = isCorect(players[id]->getMyFigures(),hit_field->getModel(),figurka);
							cout << findG << " *****" << endl;
						}
						if (findG) {
							break;
						}
						cout << "**** shoda nenalezena :-( *****" << endl;
					}
				}
					
				if(!findG) {
					//getchar();
					continue;
				}
				/* trefa do pole s figurkou hrace 
				cout << "**** nalezena shoda! :-) *****" << endl;
				cout << "Posun o: " << xK6 << endl;
				cout << "Figurka ID: " << figurka->getID();
				policko = (players[id]->getStrokes())[figurka->getID()];
				std::cout << "cilove pole:" << endl << *policko << endl;
				cout << "***** presun figury *****" << endl;
				figurka->move(policko);
				break;
			}
			cout << "**** prekreslujem *****" << endl;
			glutPostRedisplay();
			std::cout << "***** bude hrat dalsi hrac*****" << endl;
			getchar();
			if(isWinner(players[id]->getSector())) {
					winner = true;
					break;
			}
		}
		*/
}

void drawToss(int x, int y,int count) {
	//cout << "kreslim hod" <<endl;
	GLfloat size = 10.0;

	glPointSize(size);
	glColor3f(1.0f, 1.0f, 1.0f);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS);
		for(int i = 0; i < count; i++) {
			glVertex2i(x, y-50);
			x += size + 5;
			if (i == 5) x+= 10;
		}
	glEnd();
}

void drawGameCube(int x, int y) {
	float size = CUBE_SIZE;

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glColor3f(1.0f, 1.0f, 1.0f);
	
	glBegin(GL_QUADS);

		glVertex2i(x-size,y+size); // Pravý horní (horní stìna)
		glVertex2i(x-size+size/2,y+size+size/2);// Levý horní (horní stìna)
		glVertex2i(x+size+size/2,y+size+size/2);// Levý dolní (horní stìna)
		glVertex2i(x+size,y+size);// Pravý dolní (horní stìna)
		
		glVertex2i(x-size,y-size);
		glVertex2i(x-size,y+size);
		glVertex2i(x+size,y+size);
		glVertex2i(x+size,y-size);

		glVertex2i(x+size,y+size);// Pravý horní (pravá stìna)
		glVertex2i(x+size+size/2,y+size+size/2);// Levý horní (pravá stìna)
		glVertex2i(x+size+size/2,y-size+size/2);// Levý dolní (pravá stìna)
		glVertex2i(x+size,y-size);// Pravý dolní (pravá stìna)
	glEnd();

	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
		glVertex2i(x-size,y+size);
		glVertex2i(x+size,y+size);

		glVertex2i(x+size,y+size);
		glVertex2i(x+size,y-size);

		glVertex2i(x+size,y+size);
		glVertex2i(x+size+size/2,y+size+size/2);
	glEnd();

	/*
	size *= 2.5;
	TColor color = players[id]->getColor();
	glColor3f(GLfloat(color.r), GLfloat(color.g), GLfloat(color.b));

	glBegin(GL_LINE_LOOP);
		glVertex2i(x-size,y-size);
		glVertex2i(x-size,y+size);
		glVertex2i(x+size,y+size);
		glVertex2i(x+size,y-size);
	glEnd();
	*/

}
/** POMOCNE FUNKCE **/
string decodeState() {
	switch(state) {
		case (IDLE):
			return "Pocatecni stav";
		case (TOSS):
			return "Prvni hod";
		case (NEXT_TOSS): 
			return "Dalsi hod";
		case (NEXT_PLAYER): 
			return "Dalsi hrac";
		case (DETECT):
			return "Cekam na detekci";
		case (STROKES):
			return "Jake jsou moznosti";
		case (MOVE):
			return "Posun figury";
		case (DETECT_TOSS):
			return "Cekame na kostku";
		default:
			return "0";
	}
}
bool isWinner(m_sector* players_sector)  {
	int start, finish;
	start = players_sector->getStart()->getFigureCount();
	finish = players_sector->getFinish()->getFigureCount();
	return (finish - start == 4);
}
int throwDice() {
	return 1+rand()%6;
}

bool isCorect(list<m_figure*> players_figure, m_field* hit_field, m_figure*& figure) {
	list<m_figure*> pl_figures = players_figure;
	list<m_figure*>::iterator it_fig = pl_figures.begin();

	for(;it_fig != pl_figures.end(); it_fig++) {
		if(hit_field == (*it_fig)->getField()) {
			figure = (*it_fig);
			return true;
		}
	}
	return false;
}

g_field* findMatch(cv::Point2f hit) {
	std::list <g_field*>::iterator iter_G;

	iter_G = graphic_fields.begin();
	for(;iter_G != graphic_fields.end(); iter_G++) {
		if (PointDistance((*iter_G)->getPosition(),cv::Point(hit.x,hit.y)) < (*iter_G)->getR_Search()) {
			(*iter_G)->select();
			return (*iter_G);
		} else {
			(*iter_G)->draw();
		}
	}
	return NULL;
}

int findControlObject(cv::Point2f hit) {
		int i = -1;
	for( int j = 0; j <other_graphic_objects.size(); j++) {
		if((PointDistance(other_graphic_objects[j],hit) < CUBE_SIZE*2.5) && j == id) {
			i = j;
			break;
		}
	}
	return i;
}

double PointDistance(cv::Point A, cv::Point B) {
	double vzdalenost = sqrt(pow(A.x-B.x,2)+pow(A.y-B.y,2));
	//cout << "Vzdalenost: " << vzdalenost << endl;
	return vzdalenost;
}

cv::Point2f idealPoint(vector<Point2f>& hits) {
	Point2f hit_point;
	int sum_x = 0;
	int sum_y = 0;
	int velikost = hits.size();
	for(unsigned int j = 0; j < velikost; j++) {
				hit_point = hits[j];
				hit_point.y = okno - hit_point.y; //korekce windows OpenGL
				sum_y += hit_point.y;
				sum_x += hit_point.x;
	}
	cout << "idealni bod ma souradnice [" << sum_x/velikost << ", " << sum_y/velikost << "]" << endl;
	return Point2f(sum_x/velikost,sum_y/velikost);
}
////////////////////////////////
// MAIN
////////////////////////////////

int main (int argc, char* argv[]) {

	if(!systemCalibration(camera,distCoeff,homography)) {
		cout << "chyba kalibrace" <<endl;
		getchar();
		return -1;
	}

	cap.open(0);

	cap.set(CV_CAP_PROP_BRIGHTNESS,128.0);
	cap.set(CV_CAP_PROP_CONTRAST,85.0);
	cap.set(CV_CAP_PROP_SATURATION,32.0);
	cap.set(CV_CAP_PROP_EXPOSURE,-7.0);
	cap.set(CV_CAP_PROP_GAIN,255.0);
	

	m_player* hrac1 = new m_player("Hrac 1");
	m_player* hrac2 = new m_player("Hrac 2");
	m_player* hrac3 = new m_player("Hrac 3");
	m_player* hrac4 = new m_player("Hrac 4");

	//players.push_back(hrac1);
	players.push_back(hrac2);
	players.push_back(hrac3);
	//players.push_back(hrac4);

	hrac1->setColor(1.0,0.0,0.0);
	hrac2->setColor(0.0,1.0,0.0);
	hrac3->setColor(0.0,0.0,1.0);
	hrac4->setColor(0.2,0.2,0.2);
	deska = new m_desk(1);

	std::list<m_sector*> sektory = deska->getSectors();
	std::list<m_sector*>::iterator iS = sektory.begin();

	for(int id = 0; id < players.size(); id++,std::advance(iS,1)) {
		players[id]->setSector(*iS); 
		players[id]->setHome();
	}

	m_field *policko = NULL;
	m_figure *figurka = NULL;
	plan = new g_desk(okno,deska);
	cout << endl << "***** Zaciname hru *****" << endl;
	initOpenGL(argc,argv);
	onDisplay();
	waitKey(1000);
	glutMainLoop();
		
	return EXIT_SUCCESS;
}