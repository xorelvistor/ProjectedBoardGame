#include "m_finish.h"
#include "m_sector.h"
#include "m_field.h"
#include "m_home.h"

/** Bezparametricky konstruktor **/
/**
 * m_finish
 * konstruktor bez parametru
 */
m_finish::m_finish(){}

/** Konstruktor **/
/**
 * m_finish
 * konstruktor - inicializuje objekt
 *  - vytvori pole v oblasti cil
 *
 * @param sector sektor, kteremu oblast prislusi
 */
m_finish::m_finish(m_sector *sector) {
	this->sector = sector;
	figure_count = 0;
	list<m_field*>::iterator iterF;
	m_field *pole;
	for(int i = 0; i < sector->getStart()->getFigureCount(); i++) {
		pole = new m_field(sector,i,"finish");
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
m_finish::~m_finish() {
	sector = NULL;
	delete sector;
}

/**
 * getFields
 * vrati seznam poli
 *
 * @return seznam poli
 */
std::list<m_field*>  m_finish::getFields() {
	return fields;
}

/**
 * getFigureCount
 * vrati pocet figur v cili
 *
 * @return pocet figur
 */
int m_finish::getFigureCount () {
	return figure_count;
}

/**
 * increaseCount
 * zvysi hodnotu citace figur o 1
 */
void m_finish::increaseCount() {
	figure_count++;
}