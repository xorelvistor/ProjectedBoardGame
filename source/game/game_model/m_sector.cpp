#include "m_sector.h"
#include "m_field.h"
#include "m_home.h"
#include "m_finish.h"

/** Bezparametricky konstruktor **/
/**
 * m_sector
 * konstruktor bez parametru
 */
m_sector::m_sector(){
	start = NULL;
	finish = NULL;
	id = 42;
}

/** Konstruktor **/
/**
 * m_sector
 * konstruktor - inicializuje objekt
 *  - vytvori oblast pro cil a start
 *  - vytvori hraci pole v sektoru
 *
 * @param number oznaceni sektoru
 */
m_sector::m_sector(int number) {
	id = number;
	start = new m_home(this);
	finish = new m_finish(this);
	owner = NULL;
	list<m_field>::iterator iter;
	m_field *pole;
	for(int i = 0; i < FIELDS_IN_SECTOR ; i++) {
		pole = new m_field(this,i,"hraci");
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
m_sector::~m_sector() {
	start = NULL;
	finish = NULL;
	delete start;
	delete finish;
}

/**
 * getID
 * vrati oznaceni sektoru
 *
 * @return cislo sektoru
 */
int m_sector::getID() {
	return id;	
}

/**
 * getStart
 * vrati oblast start
 *
 * @return start
 */ 
m_home* m_sector::getStart() {
	return start;	
}

/**
 * getFinish
 * vrati oblast cil
 *
 * @return cil
 */
m_finish* m_sector::getFinish() {
	return finish;
}

/**
 * getFields
 * vrati seznam hernich policek
 *
 * @return seznam policek
 */
std::list<m_field*>  m_sector::getFields() {
	return fields;
}