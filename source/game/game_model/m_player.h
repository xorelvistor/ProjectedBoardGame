#include "../m_lib.h"
#include <gl\glut.h>
#pragma once
/// trida reprezentujici hrace
class m_figure;
class m_field;
class m_sector;

typedef struct color {
	GLfloat r;
	GLfloat g;
	GLfloat b;
}TColor; /// barva hrace

class m_player {
private:
	std::list<m_figure*> my_figures; /// seznam figur hrace
	std::string name; /// jmeno hrace
	TColor color;
	m_sector *my_sector;  /// sektor hrace
	
public:
	std::vector<m_field*> strokes; /// seznam koncovych poli tahu pro kazdou figuru
	/**
	 * m_player
	 * konstruktor bez parametru
	 */
	m_player();

	/**
	 * m_player
	 * konstruktor - inicializuje objekt
	 *
	 * @param user_name jmeno hrace
	 */
	m_player(std::string);

	/** 
	 * destruktor
	 * uvolni objekt
	 */
	~m_player();

	TColor getColor() {
		return color;
	}

	/**
	 * getSector
	 * vrati sektor, ktery byl hraci prirazen
	 *
	 * @return hracuv sektor
	 */
	m_sector* getSector();

	/**
	 * getName
	 * vrati jmeno hrace
	 *
	 * @return jmeno hrace (string)
	 */
	std::string getName();

	/**
	 * getMyFigures
	 * vrati seznam figur hrace
	 *
	 * @return seznam figur
	 */
	std::list<m_figure*> getMyFigures();

	/**
	 * getStrokes
	 * vrati seznam moznych tahu
	 *
	 * @return seznam tahu 
	 */
	std::vector<m_field*> getStrokes();

	/**
	 * setSector
	 * priradi hraci sektor
	 *
	 * @param sector sektor, ktery ma byt hraci prirazen
	 * @return true - sektor je platny a prirazeni se zdarilo
	 * @return false - neplatny sektor
	 */
	bool setSector(m_sector *);

	/**
	 * setFigure
	 * vlozi figuru do seznamu figur hrace
	 *
	 * @param figure figura, ktera ma byt vlozena do seznamu
	 *
	 */
	bool setFigure(m_figure *);

	/**
	 * setHome
	 * naplni start v hracove sektoru figurami
	 *  - obalujici metoda
	 */
	void setHome();

	bool setColor(float, float, float);
	
};
