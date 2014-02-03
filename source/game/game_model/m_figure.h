#pragma once
#include "_game.h"
/// trida reprezentujici figurku
class m_field;
class m_figure {
private:
	m_field *field;
	bool at_home;
public:
	m_figure();
	m_field getField();
	bool getAtHome();
	void move(m_field);
	void attack(m_figure);
};
