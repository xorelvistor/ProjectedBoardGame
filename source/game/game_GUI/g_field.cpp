#include "g_field.h"
#include "../game_model/model.h"
#include "graphic_object.h"
#include "../../../source/states.h"

#include <gl/glut.h>

extern int state;
extern vector<m_player*> players;
extern int id;

void drawCircle(float cx, float cy, float r);

g_field::g_field(cv::Point position, int r):graphic_object(position) {
	model_field = NULL;
	r_draw = r;
	r_search = r-1;
	setColor(1.0,1.0,1.0);
}

void g_field::draw() {
	float r = 0;
	m_figure* figure;
	figure = model_field->getFigure();
	if (state == DETECT && figure != NULL && (figure->getOwner() == players[id])) { 
		cout << endl << endl << "detekuju" << endl << endl;
		r = 1.5*r_draw;
		r_search = r;
	} else {
		r = r_draw;
	}
	r_search = r-1.0;
	
	TColor color;
	color = getColor();
	glColor3f(GLfloat(color.r), GLfloat(color.g), GLfloat(color.b));
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	
	drawCircle(position.x,position.y,r);
	
	if (figure != NULL) {
		color = figure->getOwner()->getColor();
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
		glColor3f(GLfloat(0.0), GLfloat(0.0), GLfloat(0.0));
		drawCircle(position.x,position.y,r_draw*0.8);

		glColor3f(GLfloat(color.r+0.2), GLfloat(color.g+0.2), GLfloat(color.b+0.2));
		drawCircle(position.x,position.y,r_draw*0.7);

		glColor3f(GLfloat(color.r), GLfloat(color.g), GLfloat(color.b));
		drawCircle(position.x,position.y,r_draw*0.4);
	} 
	
	
}

void g_field::select() {
/*
	int num_segments = 20;
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_POLYGON); 
	for(int ii = 0; ii < num_segments; ii++) { 
		float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle 
		float x = r_draw/2 * cosf(theta);//calculate the x component 
		float y = r_draw/2 * sinf(theta);//calculate the y component 
		glVertex2f(GLfloat(x + position.x), GLfloat(y + position.y));//output vertex 
	} 
	glEnd(); 
	glFlush();
*/
	//cout << "kreslim na pozici x = " << position.x << ", y = " << position.y << " kde je podle me tohle modelove pole:" << endl; 
	cout << (*model_field) << "________" << endl;
	std::ostringstream str;
	//str << "Pole c.: " << model_field->getID();
	//str << ", Sektor c.: " << model_field->getSector()->getID();
	//str << ", Typ: " << model_field->special;
	//getchar();

}

void drawCircle(float cx, float cy, float r)  {
	float num_segments = 64;
	float theta = 2 * 3.1415926 / num_segments; 
	float c = cosf(theta);//precalculate the sine and cosine
	float s = sinf(theta);
	float t;

	float x = r;//we start at angle = 0 
	float y = 0; 
    
	glBegin(GL_POLYGON); 
	for(int ii = 0; ii < num_segments; ii++) 
	{ 
		glVertex2f(x + cx, y + cy);//output vertex 
        
		//apply the rotation matrix
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	} 
	glEnd(); 
}