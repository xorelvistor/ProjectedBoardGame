#include "m_player.h"
m_player::m_player(){cout << "Obycejny konstruktor hrace" << endl;}
m_player::m_player(string user_name) {
	//cout << "Obycejny konstruktor hrace" << endl;
	name = user_name;
	cout << "hrac \"" << name << "\" je pripraven\n"; 
}
/*
m_player::m_player(const m_player &original) {
	cout << "Kopirovaci konstruktor hrace" << endl;
	
	my_sector = new m_sector();
	*my_sector = *original.my_sector;
	std::list<m_figure>::const_iterator iter_orig;
	iter_orig = original.my_figures.begin();
	name = original.name;
	color = original.color;
	for (;iter_orig != original.my_figures.end();iter_orig++) {
		my_figures.push_back(*iter_orig);
	}
	
}
*/
m_player::~m_player() {
	my_sector = NULL;
	delete my_sector;
}
m_sector* m_player::getSector() {
	return my_sector;
}
std::string m_player::getName() {
	return name;
}
list<m_figure*> m_player::getMyFigures() {
	return my_figures;
}

bool m_player::setFigure (m_figure *figure) {
	my_figures.push_back(figure);
	cout << ", vlozena figura " << figure << " k hraci" << endl;
	return true;
}

bool m_player::setSector(m_sector *sector) {
	my_sector = sector;
	cout << "Hraci \"" << name << "\" byl prirazen sektor c. " << my_sector->getID()  << " [" << my_sector << "]" << endl; 
	return true;
}

void m_player::setHome() {
	my_sector->getStart()->initHome(this);
}