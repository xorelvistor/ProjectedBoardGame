#include "../m_lib.h"
#include "m_player.h"
#pragma once

//#include <list>
#define FIELDS_IN_SECTOR 10
/// trida reprezentujici sektor hraciho planu
class m_home;
class m_finish;
class m_field;


class m_sector {
private:
	m_home *start; /// oblast start
	m_finish *finish; /// oblast cil
	std::list<m_field*> fields; /// seznam hernich poli
	int id; /// oznaceni sektoru
	m_player* owner; /// hrac, kteremu je tento sektor prirazen
	
public:
	/**
	 * m_sector
	 * konstruktor bez parametru
	 */
	m_sector();

	/**
	 * m_sector
	 * konstruktor - inicializuje objekt
	 *  - vytvori oblast pro cil a start
	 *  - vytvori hraci pole v sektoru
	 *
	 * @param number oznaceni sektoru
	 */
	m_sector(int number);

	/** 
	 * destruktor
	 * uvolni objekt
	 */
	~m_sector();

	/**
	 * getID
	 * vrati oznaceni sektoru
	 *
	 * @return cislo sektoru
	 */
	int getID();

	/**
	 * getStart
	 * vrati oblast start
	 *
	 * @return start
	 */
	m_home* getStart();

	/**
	 * getFinish
	 * vrati oblast cil
	 *
	 * @return cil
	 */
	m_finish* getFinish();

	/**
	 * getFields
	 * vrati seznam hernich policek
	 *
	 * @return seznam policek
	 */
	std::list<m_field*> getFields();

	m_player* getOwner() {
		return owner;
	}

	void setOwner(m_player* ow) {
		owner = ow;
	}
};
