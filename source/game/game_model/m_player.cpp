#include "m_player.h"

m_player::m_player(){}

m_player::m_player(string user_name) {
	name = user_name;
	cout << "hrac \"" << name << "\" je pripraven\n"; 
	strokes.resize(4);
}

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

std::vector<m_field*> m_player::getStrokes() {
	return strokes;
}

bool m_player::setFigure (m_figure *figure) {
	my_figures.push_back(figure);
	cout << "// figura vlozena k hraci \"" << name << "\" //"<< endl;
	return true;
}

bool m_player::setSector(m_sector *sector) {
	my_sector = sector;
	cout << "Hraci \"" << name << "\" byl prirazen sektor c. " << my_sector->getID()  << ", &" << my_sector << endl; 
	return true;
}

void m_player::setHome() {
	my_sector->getStart()->initHome(this);
}