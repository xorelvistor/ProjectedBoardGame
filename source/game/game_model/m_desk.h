#include "../m_lib.h"
#pragma once

/// trida reprezentujici hraci plan
class m_sector;
class m_player;
class m_field;



class m_desk {
private:
	int desk_type; // typ planu
	std::list<m_sector*> sectors; // seznam sektoru
public:
	/**
	 * m_desk
	 * konstruktor
	 *
	 * @param number cislo urcujici typ hry (pocet sektoru)
	 */
	m_desk(int number);

	/**
	 * getDeskType
	 * vrati typ hry
	 *
	 * @return typ hry
	 */
	int getDeskType();

	/**
	 * getSectors
	 * vrati seznam sektoru
	 *
	 * @return seznam sektoru
	 */
	std::list<m_sector*> getSectors();

	/**
	 * findStrokes
	 * vyhleda a ulozi mozne tahy hrace
	 * 
	 * @param player hrac, jehoz tahy budou zjisteny
	 * @param xK6 hodnota hodu 
	 */
	bool findStrokes(m_player* player, int xK6);
};