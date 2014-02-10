#pragma once
#include "_game.h"
//#include <list>
#define FIELDS_IN_SECTOR 10
/// trida reprezentujici sektor hraciho planu
class m_home;
class m_field;
class m_finish;
//typedef list<m_field> LISTFIELD;
class m_sector {
private:
	m_home *start;
	m_finish *finish;
	int id;
	std::list<m_field*> fields;
public:
	m_sector();
	m_sector(int);
	m_sector(const m_sector&);
	~m_sector();
	m_sector *getSector();
	int getID();
	m_home *getStart();
	m_finish *getFinish();
	std::list<m_field*> getFields();
	std::list<m_figure*> getFigures();
	m_sector nextSector();


};
