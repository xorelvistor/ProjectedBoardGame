#pragma once
#include "_game.h"


/// trida reprezentujici start
class m_field;
class m_figure;
class m_sector;
class m_player;
class m_home {
private:
	int figure_count;
	std::list<m_field*> fields;
	//std::list<m_figure*> figures;
	m_sector *sector;
public:
	m_home();
	m_home(m_sector *);
//	m_home(const m_home &);
	~m_home();
	int getFigureCount();
	std::list<m_field*> getFields();
	std::list<m_figure*> getFigures();
	void increaseCount();
	void initHome(m_player *);
	m_figure* crankUp();
};
