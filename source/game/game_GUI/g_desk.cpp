#include "g_desk.h"
#include "g_sector.h"
#include "g_field.h"
#include "../game_model/model.h"

extern cv::Mat obrazek;
extern std::list<g_field*> graphic_fields;
extern vector<cv::Point> other_graphic_objects;

#define MEZERA 10

g_desk::g_desk(int rozmer, m_desk* deska) { 
	setModel(deska);
	int sirka = rozmer/2;
	int r = (sirka-10*MEZERA)/11 ; 
	int posun = 2*r + MEZERA;
	int skok = 4*posun;
	
	int x = (800 - 10*posun - r)/2; //600 - 10*posun - 2r
	int y = (rozmer - 10*posun -2*r - skok - posun)/2 + posun + r;

	for(int i = 0; i < 4; i++) {
		if ( i == 1 ) { x += sirka;}
		else if ( i == 2) { y += sirka; }
		else if ( i == 3) { x -= sirka; }		
		graphic_sectors.push_back(new g_sector(cv::Point(x,y),sirka));	
	} 
	
	pair(); /// priradi gerafickym objektum jejich modelove protejsky
	calc(x, y, r, posun,skok); /// spocita rozlozeni policek (kriz)

	//other_graphic_objects.push_back(cv::Point(200,500));
	other_graphic_objects.push_back(cv::Point(600,500));
	other_graphic_objects.push_back(cv::Point(600,100));
	//other_graphic_objects.push_back(cv::Point(200,100));
}

void g_desk::draw() {
	std::list <g_field*>::iterator iter_G;
	iter_G = graphic_fields.begin();
	for(;iter_G != graphic_fields.end(); iter_G++) {
		(*iter_G)->draw();
	}
}


void g_desk::calc(int x, int y, int r, int middle_distance, int jump) {
	/* prvni sektor */
	std::list <g_field*>::iterator iter_GF;
	iter_GF = graphic_fields.begin();
	TColor color;
	m_player* player = (*iter_GF)->getModel()->getSector()->getOwner();
	if (player != NULL)
		color = player->getColor();
	else 
		color = (*iter_GF)->getColor();

	x += jump;
	for (int i = 0; i < 4; i++) {
		(*iter_GF)->setPosition(cv::Point(x,y));
		(*iter_GF)->setColor(color.r,color.g,color.b);
		(*iter_GF)->setR_draw(r);
		std::advance(iter_GF,1);
		x -= middle_distance;
	}
	(*iter_GF)->setPosition(cv::Point(x,y));
	(*iter_GF)->setR_draw(r);
	std::advance(iter_GF,1);
	y += middle_distance;
	for (int i = 0; i < 5; i++) {
		(*iter_GF)->setPosition(cv::Point(x,y));
		(*iter_GF)->setR_draw(r);
		if (i == 0) 
			(*iter_GF)->setColor(color.r,color.g,color.b);
		std::advance(iter_GF,1);
		x += middle_distance;
	}
	x -= middle_distance;
	y += middle_distance;
	for (int i = 0; i < 4; i++) {
		(*iter_GF)->setPosition(cv::Point(x,y));
		(*iter_GF)->setR_draw(r);
		std::advance(iter_GF,1);
		y += middle_distance;
	}
	
	/* druhy sektor */
	player = (*iter_GF)->getModel()->getSector()->getOwner();
	if (player != NULL)
		color = player->getColor();
	else 
		color = (*iter_GF)->getColor();

	x += middle_distance;
	y -= jump+middle_distance;
	for (int i = 0; i < 4; i++) {
		(*iter_GF)->setPosition(cv::Point(x,y));
		(*iter_GF)->setColor(color.r,color.g,color.b);
		(*iter_GF)->setR_draw(r);
		std::advance(iter_GF,1);
		y += middle_distance;
	}
	(*iter_GF)->setPosition(cv::Point(x,y));
	(*iter_GF)->setR_draw(r);
	std::advance(iter_GF,1);
	x += middle_distance;
	for (int i = 0; i < 5; i++) {
		(*iter_GF)->setPosition(cv::Point(x,y));
		(*iter_GF)->setR_draw(r);
		if (i == 0) 
			(*iter_GF)->setColor(color.r,color.g,color.b);
		std::advance(iter_GF,1);
		y -= middle_distance;
	}
	x += middle_distance;
	y += middle_distance;
	for (int i = 0; i < 4; i++) {
		(*iter_GF)->setPosition(cv::Point(x,y));
		(*iter_GF)->setR_draw(r);
		std::advance(iter_GF,1);
		x += middle_distance;
	}
	/* treti sektor */
	player = (*iter_GF)->getModel()->getSector()->getOwner();
	if (player != NULL)
		color = player->getColor();
	else 
		color = (*iter_GF)->getColor();

	x -= jump+middle_distance;
	y -= middle_distance;
	for (int i = 0; i < 4; i++) {
		(*iter_GF)->setPosition(cv::Point(x,y));
		(*iter_GF)->setColor(color.r,color.g,color.b);
		(*iter_GF)->setR_draw(r);
		std::advance(iter_GF,1);
		x += middle_distance;
	}
	(*iter_GF)->setPosition(cv::Point(x,y));
	(*iter_GF)->setR_draw(r);
	std::advance(iter_GF,1);
	y -= middle_distance;
	for (int i = 0; i < 5; i++) {
		(*iter_GF)->setPosition(cv::Point(x,y));
		(*iter_GF)->setR_draw(r);
		if (i == 0) 
			(*iter_GF)->setColor(color.r,color.g,color.b);
		std::advance(iter_GF,1);
		x -= middle_distance;
	}
	y -= middle_distance;
	x += middle_distance;
	for (int i = 0; i < 4; i++) {
		(*iter_GF)->setPosition(cv::Point(x,y));
		(*iter_GF)->setR_draw(r);
		std::advance(iter_GF,1);
		y -= middle_distance;
	}
	/* ctvrty sektor */
	player = (*iter_GF)->getModel()->getSector()->getOwner();
	if (player != NULL)
		color = player->getColor();
	else 
		color = (*iter_GF)->getColor();

	x -= middle_distance;
	y += jump+middle_distance;
	for (int i = 0; i < 4; i++) {
		(*iter_GF)->setPosition(cv::Point(x,y));
		(*iter_GF)->setColor(color.r,color.g,color.b);
		(*iter_GF)->setR_draw(r);
		std::advance(iter_GF,1);
		y -= middle_distance;
	}
	(*iter_GF)->setPosition(cv::Point(x,y));
	(*iter_GF)->setR_draw(r);
	std::advance(iter_GF,1);
	x -= middle_distance;
	for (int i = 0; i < 5; i++) {
		(*iter_GF)->setPosition(cv::Point(x,y));
		if (i == 0) 
			(*iter_GF)->setColor(color.r,color.g,color.b);
		(*iter_GF)->setR_draw(r);
		std::advance(iter_GF,1);
		y += middle_distance;
	}
	x -= middle_distance;
	y -= middle_distance;
	for (int i = 0; i < 4; i++) {
		(*iter_GF)->setPosition(cv::Point(x,y));
		(*iter_GF)->setR_draw(r);
		std::advance(iter_GF,1);
		x -= middle_distance;
	}
	std::list <g_field*>::iterator iter_G;
	iter_G = graphic_fields.begin();
}

void g_desk::pair() {
	std::list <m_sector*> sectors;
	std::list <m_sector*>::iterator iter_Sec;
	std::list <m_field*>::iterator iter_Field;
	std::list <m_field*> model_fields;
	std::list <g_sector*>::iterator iter_G;
	std::list <g_field*>::iterator iter_GF;
	
	sectors = model_desk->getSectors();
	iter_G = graphic_sectors.begin();
	iter_Sec = sectors.begin();
	iter_GF = graphic_fields.begin();
		for (;iter_Sec != sectors.end(); iter_Sec++,iter_G++) {
			(*iter_G)->setModel(*iter_Sec);

			// nejdrive je treba priradit koncova policka
			model_fields = (*iter_Sec)->getFinish()->getFields();
			model_fields.reverse();
			//cout << (*iter_Sec)->getID() << ": " << model_fields.size() << endl;
			iter_Field = model_fields.begin();

			for (; iter_Field != model_fields.end(); iter_Field++) {
				(*iter_GF)->setModel((*iter_Field));
				std::advance(iter_GF,1);
			}

			// pote hraci pole

			model_fields = (*iter_Sec)->getFields();
			//cout << (*iter_Sec)->getID() << ": " << model_fields.size() << endl;
			iter_Field = model_fields.begin();

			for (; iter_Field != model_fields.end(); iter_Field++) {
				(*iter_GF)->setModel((*iter_Field));
				std::advance(iter_GF,1);
				if (iter_GF == graphic_fields.end()) 
					cout << "Jsem na konci graphic_fields" << endl;
			}
		}
	
}