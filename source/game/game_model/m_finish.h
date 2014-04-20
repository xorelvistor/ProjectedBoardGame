#include "../m_lib.h"
#pragma once
/// trida reprezentujici domecek (cil)
class m_figure;
class m_sector;
class m_field;



class m_finish{
private:
	int figure_count; /// aktualni pocet figur v oblasti
	std::list<m_field*> fields; /// seznam poli oblasti
	m_sector *sector; /// sektor, kteremu oblast prislusi
public:
	/**
	 * m_finish
	 * konstruktor bez parametru
	 */
	m_finish();

	/**
	 * m_finish
	 * konstruktor - inicializuje objekt
	 *  - vytvori pole v oblasti cil
	 *
	 * @param sector sektor, kteremu oblast prislusi
	 */
	m_finish(m_sector*);

	/** 
	 * destruktor
	 * uvolni objekt
	 */
	~m_finish();

	/**
	 * getFigureCount
	 * vrati pocet figur v cili
	 *
	 * @return pocet figur
	 */
	int getFigureCount();

	/**
	 * increaseCount
	 * zvysi hodnotu citace figur o 1
	 */
	void increaseCount();

	/**
	 * getFields
	 * vrati seznam poli
	 *
	 * @return seznam poli
	 */
	std::list<m_field*> getFields();
};
