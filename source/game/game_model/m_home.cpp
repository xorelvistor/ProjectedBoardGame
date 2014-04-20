#include "m_sector.h"
#include "m_home.h"
#include "m_field.h"

#include "m_figure.h"
#include "m_player.h"
/** Bezparametricky konstruktor **/
/**
 * m_sector
 * konstruktor bez parametru
 */
m_home::m_home(){}

/** Konstruktor **/
/**
 * m_home
 * konstruktor - inicializuje objekt
 *  - vytvori pole v oblasti start
 *
 * @param sector sektor, kteremu oblast prislusi
 */
m_home::m_home(m_sector* sector) {
	this->sector = sector;
	figure_count = 4; // pocatecni pocet figur na startu
	list<m_field*>::iterator iterF;
	m_field *pole;
	for(int i = 0; i < figure_count ; i++) {
		pole = new m_field(sector,i,"start");
		fields.push_back(pole);
		if (fields.empty()) {
			cout << "koncim" << endl;
			return;
		}
	}
}

/** 
 * destruktor
 * uvolni objekt
 */
m_home::~m_home() {
	sector = NULL;
	delete sector;
}

/**
 * getFigureCount
 * vrati pocet figur na startu
 *
 * @return pocet figur
 */
int m_home::getFigureCount () {
	return figure_count;
}

/**
 * increaseCount
 * zvysi hodnotu citace figur o 1
 */
void m_home::increaseCount() {
	figure_count++;
}

/**
 * getFields
 * vrati seznam poli
 *
 * @return seznam poli
 */
std::list<m_field*>  m_home::getFields() {
	return fields;
}

/**
 * initHome
 * vlozi figury na startovni pozice
 *  - vlozi figury do seznamu figur hrace
 *
 * @param owner hrac, kteremu prislusi start (figury)
 */
void m_home::initHome(m_player* owner) {
	m_figure* figure;
	m_field* field;
	std::string zprava="";
	list<m_field*>::iterator iterF;
	int i = 0;
	cout << "-- initHome --" << endl;

	for (iterF = fields.begin();iterF != fields.end(); iterF++) {
		figure = new m_figure(owner,i);
		field = (*iterF);
		field->putFigure(figure);
		figure->setField(field); /// nastaveni pozice figury
		owner->setFigure(figure); /// vlozeni figury k hraci
		i++;
	}
}

/**
 * crankUp
 * nahodi figuru ze startu do hry
 *
 * @return prave nahozena figura
 * @return NULL pokud je start prazdny
 */
m_figure* m_home::crankUp() {
	cout << "-- crankUp --" << endl;
	std::list<m_field*>::iterator iterF;
	std::list<m_field*> fields;
	m_figure* figure;
	for(iterF = this->fields.begin(); iterF != this->fields.end(); iterF++) {
		if ((figure = (*iterF)->getFigure()) != NULL) {
			//cout << *figure;
			break;
		}
	}
	if (figure == NULL) {
		cout << "zadna figura na startu" << endl;
		return NULL;
	}
	if (figure->getAtHome()) {
		fields = sector->getFields();
		iterF = fields.begin();
		figure->move(*(++iterF));
		figure->setAtHome();
	}
	figure_count--;
	return figure;
}
