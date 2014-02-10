#pragma once
#include "_game.h"
/// trida reprezentujici domecek (cil)
class m_field;
class m_figure;
class m_sector;

class m_finish{
private:
	int figure_count;
	std::list<m_field*> fields;
	//std::list<m_figure> figures;
	m_sector *sector;
public:
	m_finish();
	m_finish(m_sector*);
//	m_finish(const m_finish &);
	~m_finish();
	int getFigureCount();
	std::list<m_field*> getFields();
};
