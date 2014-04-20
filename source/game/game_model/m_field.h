#include "../m_lib.h"
#pragma once

/// reprezentuje policko hraciho planu
class m_figure;
class m_sector;
class m_field {
private:
	int id; /// cislo pole v ramci sektoru
	m_figure* figure; /// figura na poli
	m_sector* sector; /// sektor do ktereho pole patri
	
public:
	std::string special; // typ pole

	/**
	 * m_field
	 * konstruktor bez parametru
	 */
	m_field();

	/**
	 * m_field
	 * konstruktor - inicializuje objekt
	 *
	 * @param sector sektor, kteremu pole prislusi
	 * @param number identifikator pole
	 * @param str_special typ pole
	 */
	m_field(m_sector* sector,int number,std::string str_special);
	
	/** 
	 * destruktor
	 * uvolni objekt
	 */
	~m_field(void);
	
	/**
	 * getID
	 * vrati identifikator pole
	 * 
	 * @return identifikator pole
	 */
	int getID();

	/**
	 * getSector
	 * vrati sektor, kteremu pole prislusi
	 *
	 * @return sektor, kteremu pole prislusi
	 */  
	m_sector* getSector();

	/**
	 * getFigure
	 * vrati figuru, ktera je na poli nebo NULL
	 *
	 * @return figura na poli nebi NULL 
	 */
	m_figure* getFigure();

	/**
	 * isFree
	 * zjisti, zda je na poli figura
	 * 
	 * @return true nebo false
	 */
	bool isFree();

	/**
	 * leftUntilEnd
	 * vrati pocet poli do konce sektoru
	 *
	 * @return pocet poli 
	 */
	int leftUntilEnd();

	/**
	 * putFigure
	 * vlozi figuru na pole
	 *
	 * @param figure figura, ktera ma byt vlozena na pole
	 */
	void putFigure(m_figure* figure);

	/**
	 * removeFigure
	 * odstrani figuru z pole
	 *
	 * @return odstranena figura nebo NULL
	 */
	m_figure* removeFigure();

	friend std::ostream& operator<<(std::ostream& os,const m_field& m_f);
};
