#include "../m_lib.h"
#pragma once

/// trida reprezentujici figurku
class m_player;
class m_field;

class m_figure {
private:
	m_field* field; /// pole na kterem figura stoji
	bool at_home; /// nachazi se figura na startu?
	bool out_of_sector; /// opustila figura svuj startovni sektor?
	m_player* owner; /// vlastnik figury
	int id; /// oznaceni figury
public:
	/**
	 * m_figure
	 * konstruktor bez parametru
	 */
	m_figure();

	/**
	 * m_figure
	 * konstruktor - inicializuje objekt
	 *
	 * @param player hrac, kteremu figura patri
	 * @param id oznaceni figury
	 */
	m_figure(m_player *player, int id);

	/** 
	 * destruktor
	 * uvolni objekt
	 */
	~m_figure();
	
	/**
	 * getField
	 * vrati pole na kterem figura stoji
	 *
	 * @return pole na kterem figura stoji
	 */
	m_field* getField();

	/**
	 * getAtHome
	 * zjisti, zda je figura ve hre
	 *
	 * @return true-pokud figura neni ve hre
	 * @return false-pokud figura je ve hre
	 */
	bool getAtHome();

	/**
	 * getOutOfSector
	 * zjisti, zda jiz figura opustila svuj startovni sektor
	 *
	 * @return true-figura jiz opustila startovni sektor
	 * @return false-figura jeste neopustila startovni sektor
	 */
	bool getOutOfSector();

	/**
	 * getOwner
	 * vrati hrace, kteremu figura patri
	 *
	 * @return vlastnik figury
	 */
	m_player* getOwner();

	/**
	 * getID
	 * vrati cislo figury
	 *
	 * @return oznaceni figury
	 */
	int getID();

	/** 
	 * setAtHome
	 * zmeni stav figury
	 */
	void setAtHome();

	/**
	 * setOutOfSector
	 * zmeni stav figury na hodnotu expr
	 * 
	 * @param expr true/false 
	 */
	void setOutOfSector(bool expr);

	/**
	 * setField
	 * nastavi pole na kterem figura stoji
	 *
	 * @param field pole, ktere ma byt prirazeno
	 */
	void setField(m_field* field);

	/**
	 * move
	 * presune figuru na nove pole
	 *
	 * @param new_field pole, kam se ma figura presunout
	 */
	bool move(m_field* new_field);

	/**
	 * goHome
	 * vrati figuru na start
	 */
	void goHome();

	friend std::ostream& operator<<(std::ostream& os,const m_figure& m_f);
};
