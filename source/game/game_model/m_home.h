#pragma once
#include "_game.h"


/// trida reprezentujici start
class m_field;
class m_figure;
class m_sector;
class m_home{
private:
	int figure_count;
	m_field *fields[4];
	m_figure *figures[4];
	m_sector * sector;
public:
	m_home(m_sector*);
	int getFigureCount();
};
