#pragma once
#include "_game.h"
/// trida reprezentujici domecek (cil)
class m_field;
class m_figure;
class m_sector;

class m_finish{
private:
	int figure_count;
	m_field *fields[4];
	m_figure *figures[4];
	m_sector * sector;
public:
	m_finish(m_sector*);
	int getFigureCount();
};
