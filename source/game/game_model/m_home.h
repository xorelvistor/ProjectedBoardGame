#include "../m_lib.h"
#pragma once

/// trida reprezentujici start
class m_sector;
class m_player;
class m_field;
class m_figure;

class m_home {
private:
	int figure_count; /// aktualni pocet figur v oblasti
	std::list<m_field*> fields; /// seznam poli oblasti
	m_sector *sector; /// sektor, kteremu oblast prislusi
public:
	/**
	 * m_sector
	 * konstruktor bez parametru
	 */
	m_home();

	/**
	 * m_home
	 * konstruktor - inicializuje objekt
	 *  - vytvori pole v oblasti start
	 *
	 * @param sector sektor, kteremu oblast prislusi
	 */
	m_home(m_sector* sector);

	/** 
	 * destruktor
	 * uvolni objekt
	 */
	~m_home();

	/**
	 * getFigureCount
	 * vrati pocet figur na startu
	 *
	 * @return pocet figur
	 */
	int getFigureCount();
	
	/**
	 * getFields
	 * vrati seznam poli
	 *
	 * @return seznam poli
	 */
	std::list<m_field*> getFields();

	/**
	 * increaseCount
	 * zvysi hodnotu citace figur o 1
	 */
	void increaseCount();

	/**
	 * initHome
	 * vlozi figury na startovni pozice
	 *  - vlozi figury do seznamu figur hrace
	 *
	 * @param owner hrac, kteremu prislusi start (figury)
	 */
	void initHome(m_player *);
	
	/**
	 * crankUp
	 * nahodi figuru ze startu do hry
	 *
	 * @return prave nahozena figura
	 * @return NULL pokud je start prazdny
	 */
	m_figure* crankUp();
};
