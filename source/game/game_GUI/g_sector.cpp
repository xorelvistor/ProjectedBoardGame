#include "g_sector.h"
#include "g_field.h"
#include "../game_model/model.h"
#include "graphic_object.h"
extern cv::Mat obrazek;
extern std::list<g_field*> graphic_fields;

g_sector::g_sector(cv::Point p, int sirka):graphic_object(p) { 
	int rozestup = 10;
	int r = (sirka-10*rozestup)/11 ; 
	int x = p.x;
	int y = p.y;
	int posun = 2*r + rozestup;
	
	g_field* g_pole;
	// koncova policka
	for (int i = 0; i < 4; i++) {	
		g_pole = new g_field(cv::Point(500,100),r);
		g_pole->setColor(0.5f,0.5f,0.5f);
		graphic_fields.push_back(g_pole);
	}
	
	// hraci policka
	
	for (int i = 0; i < 10; i++) {
		g_pole = new g_field(cv::Point(550,100),r);
		if (i == 1) {
			g_pole->setColor(0.5f,0.5f,0.5f);
		}
		graphic_fields.push_back(g_pole);
	}
	draw(sirka);
}

void g_sector::draw(int sirka) {
//cout << "kreslim sektor" << endl;
	glPointSize(10.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS);
	glVertex2i(GLint(position.x),GLint(position.y)); 
	glEnd();
}

void g_sector::select() {
	cout << "jsem v sektoru c.: " << getModel()->getID() << endl;
}
