#include "m_player.h"
#include "m_sector.h"
#include "m_figure.h"
#include "m_home.h"

/** Bezparametricky konstruktor **/
/**
 * m_player
 * konstruktor bez parametru
 */
m_player::m_player(){
	my_sector = NULL;
	name = "unsetting";
	color.r = color.g = color.b = 1.0f;
}

/** Konstruktor **/
/**
 * m_player
 * konstruktor - inicializuje objekt
 *
 * @param user_name jmeno hrace
 */
m_player::m_player(string user_name) {
	name = user_name;
	//cout << "hrac \"" << name << "\" je pripraven\n"; 
	strokes.resize(4);
}

/** 
 * destruktor
 * uvolni objekt
 */
m_player::~m_player() {
	my_sector = NULL;
	delete my_sector;
}

/**
 * getSector
 * vrati sektor, ktery byl hraci prirazen
 *
 * @return hracuv sektor
 */
m_sector* m_player::getSector() {
	return my_sector;
}

/**
 * getName
 * vrati jmeno hrace
 *
 * @return jmeno hrace (string)
 */
std::string m_player::getName() {
	return name;
}

/**
 * getMyFigures
 * vrati seznam figur hrace
 *
 * @return seznam figur
 */
list<m_figure*> m_player::getMyFigures() {
	return my_figures;
}

/**
 * getStrokes
 * vrati seznam moznych tahu
 *
 * @return seznam tahu 
 */
std::vector<m_field*> m_player::getStrokes() {
	return strokes;
}

/**
 * setFigure
 * vlozi figuru do seznamu figur hrace
 *
 * @param figure figura, ktera ma byt vlozena do seznamu
 *
 */
bool m_player::setFigure (m_figure* figure) {
	if (figure == NULL)
		return false;
	my_figures.push_back(figure);
	//cout << "// figura c.:" <<  figure->getID() << " vlozena k hraci \"" << name << "\" //"<< endl;
	return true;
}

/**
 * setSector
 * priradi hraci sektor
 *
 * @param sector sektor, ktery ma byt hraci prirazen
 * @return true - sektor je platny a prirazeni se zdarilo
 * @return false - neplatny sektor
 */
bool m_player::setSector(m_sector* sector) {
	my_sector = sector;
	if (my_sector == NULL)
		return false;
	//cout << "Hraci \"" << name << "\" byl prirazen sektor c.: " << my_sector->getID()  << ", &" << my_sector << endl; 
	my_sector->setOwner(this);
	return true;
}

/**
 * setHome
 * naplni start v hracove sektoru figurami
 *  - obalujici metoda
 */
void m_player::setHome() {
	my_sector->getStart()->initHome(this);
}

bool m_player::setColor(float r, float g, float b) {
	color.r = r;
	color.g = g;
	color.b = b;
	return true;
}