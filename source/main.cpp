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
void onTimer(int mode);
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
void drawHome(int x, int y, int count, m_player* player);
void drawGameCube(int x, int y);
void drawToss(int x, int y,int count);

/* globalni promenne */
int state = IDLE;
int id = 0; //cislo hrace
int timerInterval = 1; // opakovani casovace
VideoCapture cap;
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
bool possible_move = false;
bool death = false;
g_desk* plan = NULL;
g_field* hit_field = NULL;
int okno = 600;
int global_x = 0;
int global_y = 0;
int xK6 = 0; //hodnota hodu


//#define DEBUG 1


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

void onTimer (int mode) {
	int next = 0;
	if (death) {	
		if(hit_field != NULL) {
			cout << endl << "timer" << endl;
			switch (mode) {
				case (0):
					hit_field->select();
					glutPostRedisplay();
					break;
			}
		}
		
	}
	glutTimerFunc(timerInterval, onTimer, 0);
	
}
void onDisplay() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
 
	
	std::list <g_field*>::iterator iter_G;
	std::list <g_sector*>::iterator iter_G_sectors;
	

	plan->draw();
	drawGameCube(other_graphic_objects[id].x,other_graphic_objects[id].y);
	drawToss(other_graphic_objects[id].x,other_graphic_objects[id].y,xK6);
	//glLoadIdentity();
	//glPushMatrix();
	//glRotatef((id+1)*45.0f,0.0f,0.0f,-1.0f);
	//glPushMatrix();
	//glTranslatef(other_graphic_objects[1].x,other_graphic_objects[1].y,0.0f);
	
	for(int i = 0 ; i < players.size(); i++) {
		m_home* start = players[i]->getSector()->getStart();
		drawHome(other_graphic_objects[i].x,other_graphic_objects[i].y,start->getFigureCount(),players[i]);
	}

	//glPopMatrix();
	//glPopMatrix();

	glFlush();
	glutSwapBuffers();


}

void initOpenGL(int& argc, char* argv[]) {
	glutInit(&argc, argv);
	glutCreateWindow("Zkousim OpenGL");
	glutReshapeWindow(800,650);
	#ifndef DEBUG
		glutPositionWindow(1366,-20);
	#else 
		glutPositionWindow(100,-800);
		glutMouseFunc(onMouse);
	#endif
	glutDisplayFunc(onDisplay);
	glutReshapeFunc(onResize);
	glutKeyboardFunc(onKeyboard);
	srand(time(NULL));
	//glutTimerFunc(timerInterval, onTimer, 0);
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
		cout << "Vitezem se stava \'" << players[id]->getName() << "\'!" << endl << "GRATULUJEME" <<endl;
	}
	if(state == IDLE) {
		cout << "---- " << players[id]->getName() << " -----" << endl;
		state = DETECT_TOSS;
	}

	if(state == DETECT_TOSS) {
		//cout << "State: " << decodeState() << " => " << players[id]->getName() << endl;
		vector<Point2f> hits1;
		vector<Point2f> hits_toss; // transformovane body - v okne
		Point act_hit;
		#ifndef DEBUG
		if(detectHand(cap,hits1) && !hits1.empty()) {
			perspectiveTransform(hits1,hits_toss,homography);
		#else
		if(act) {
			hits_toss.push_back(cv::Point(global_x,global_y));
			act = false;
			global_x = 0;
			global_y = 0;
		#endif
			for(unsigned int j = 0; j < hits_toss.size(); j++) {
				act_hit = hits_toss[j];
				act_hit.y = okno - act_hit.y; //korekce windows OpenGL
			//	cout << "***** kontrolujeme shodu s other_graphic_objects *****" << endl;
				if((findControlObject(act_hit)) != -1) {
					int hod = 0;
					hod = throwDice();
					if(xK6 == 6 && new_figure == NULL) {
						xK6 += hod;	
					} else {
						xK6 = hod;
					}
					state = TOSS;
					glutPostRedisplay(); 
					break;
				}
			}
			return;
		}

	}
	
	if(state == TOSS) {
		cout << "State: " << decodeState() << " <= hod: " << xK6 << endl;
		if(xK6 == 6) {
			if(new_figure == NULL) {
				if (players[id]->getSector()->getStart()->getFigureCount() > 0) {
					new_figure = players[id]->getSector()->getStart()->crankUp(); //nahozeni figury
					glutPostRedisplay();
				}
				state = DETECT_TOSS; //dalsi hod po 6
			} else {
				state = STROKES;
			}
		} else if (xK6 ==0 ) {
			state = DETECT_TOSS;
		} else {
			m_sector* players_sector = players[id]->getSector();
			if((players_sector->getStart()->getFigureCount() + players_sector->getFinish()->getFigureCount()) == 4) {
				state = NEXT_PLAYER;
				glutPostRedisplay();
			} else {
				state = STROKES;
			}
		}
	}

	if(state == STROKES) {
		cout << "State: " << decodeState() << " <= hod: " << xK6 << endl;
		if(deska->findStrokes(players[id],xK6)) {
			if(new_figure != NULL)
				state = MOVE;
			else
				state = DETECT;
		} else {
			state = NEXT_PLAYER;
		}
		glutPostRedisplay();
	}
	
	if(state == MOVE) {
		cout << "State: " << decodeState() << " ";
		if (new_figure != NULL) {
			figurka = new_figure;
		}
		cout << figurka->getID() <<endl;
		if (figurka != NULL) {
			death = figurka->move(players[id]->getStrokes()[figurka->getID()]);
			glBegin(GL_LINE);
				
			state = NEXT_PLAYER;
			glutPostRedisplay();
		}

	}
	if(state == NEXT_PLAYER) {
		cout << "State: " << decodeState() << endl;
		winner = isWinner(players[id]->getSector());
		if(winner) return;
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
		//cout << "State: " << decodeState() << " => " << players[id]->getName() << " <= hod: " << xK6 << endl;
		vector<Point2f> hits2;
		vector<Point2f> hits_figure;
		Point act_hit;
		hit_field = NULL;
		#ifndef DEBUG
		if(detectHand(cap,hits2) && !hits2.empty()) {
			perspectiveTransform(hits2,hits_figure,homography);
			act_hit = idealPoint(hits_figure);
		#else
		if(act) {
			act_hit = cv::Point(global_x,okno-global_y);
			act = false;
			global_x = 0;
			global_y = 0;
		#endif
			//cout << "***** kontrolujeme shodu s g_field *****" << endl;
			if((hit_field = findMatch(act_hit)) != NULL) {
				//cout << "***** je korektni? -> " ;
				findG = isCorect(players[id]->getMyFigures(),hit_field->getModel(),figurka);
				//cout << findG << " *****" << endl;
			}
			//cout << "kreslim bod do hraci plochy" <<endl;
			glPointSize(5.0);
			glColor3f(1.0f,0.0f,0.0f);
			glBegin(GL_POINTS);
				glVertex2f(act_hit.x,act_hit.y);
			glEnd();
			glFlush();
			if(findG && figurka != NULL) {
				state = MOVE;
				//return;
			}
			//glutPostRedisplay();
		}
	}
}
void drawHome(int x, int y, int count, m_player* player) {
	TColor color = player->getColor();
	glDisable(GL_POINT_SMOOTH);
	float size = 16.0;
	glPointSize(size);
	float distance = 6.0;
	float start_y = size/2 + distance/2;
	y += start_y;
	x -= start_y;
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_POINTS);
		for(int i = 0; i < count; i++) {
			glVertex2i(x, y);
			
			if (i == 1) {
				y -= distance + size;
				x -= 2*size + 2*distance;
			}
			
			x += size + distance;
			
		}
	glEnd();
			
	glEnable(GL_POINT_SMOOTH);
} 

void drawToss(int x, int y,int count) {
	//cout << "kreslim hod" <<endl;
	GLfloat size = 10.0;

	glPointSize(size);
	if(count > 5) {
		glColor3f(1.0f, 0.0f, 0.0f);
	}
	else
		glColor3f(1.0f, 1.0f, 1.0f);
	
	x -= count/2 * size - (count/2-1)*5;
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS);
	
		for(int i = 0; i < count; i++) {
			glVertex2i(x, y-50);
			x += size + 5;
			if (i == 5) {
				x+= 10;
				glColor3f(1.0f, 1.0f, 1.0f);
			}
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
	cout << "isWinner? => " << endl;
	cout << finish << " - "<< start << " =" << finish - start << endl;
	return ((finish - start) == 4);
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
			//(*iter_G)->select();
			return (*iter_G);
		} else {
			//(*iter_G)->draw();
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
	#ifndef DEBUG
	if(!systemCalibration(camera,distCoeff,homography)) {
		cout << "chyba kalibrace" <<endl;
		getchar();
		return -1;
	}
	cap.open(0);
	cap.set(CV_CAP_PROP_BRIGHTNESS,128.0);
	cap.set(CV_CAP_PROP_CONTRAST,85.0);
	cap.set(CV_CAP_PROP_SATURATION,32.0);
	#endif
	//cap.set(CV_CAP_PROP_EXPOSURE,-7.0);
	//cap.set(CV_CAP_PROP_GAIN,255.0);
	int sum_players = 2;
	string names [4] = {"Hrac 1 - cerveny", "Hrac 2 - zeleny", "Hrac 3 - modry", "Hrac 4 - sedy"};
	m_player* hrac;
	for(int i = 0; i < sum_players; i++) { 
		hrac = new m_player(names[i]);
		switch (i) {
			case (0):
				hrac->setColor(1.0,0.0,0.0);
				other_graphic_objects.push_back(cv::Point(200,500));
				break;
			case (1):
				hrac->setColor(0.0,1.0,0.0);
				other_graphic_objects.push_back(cv::Point(600,500));
				break;
			case (2):
				hrac->setColor(0.0,0.0,1.0);
				other_graphic_objects.push_back(cv::Point(600,100));
				break;
			case (3):
				hrac->setColor(0.2,0.2,0.2);
				other_graphic_objects.push_back(cv::Point(200,100));
				break;
			default:
				break;
		}
		players.push_back(hrac);
	}
	deska = new m_desk(1);

	std::list<m_sector*> sektory = deska->getSectors();
	std::list<m_sector*>::iterator iS = sektory.begin();

	for(int id = 0; id < players.size(); id++,std::advance(iS,1)) {
		players[id]->setSector(*iS); 
		players[id]->setHome();
	}
 
	plan = new g_desk(okno,deska);
	cout << endl << "***** Zaciname hru *****" << endl;
	initOpenGL(argc,argv);
	//onDisplay();
	//waitKey(1000);
	glutMainLoop();
		
	return EXIT_SUCCESS;
}