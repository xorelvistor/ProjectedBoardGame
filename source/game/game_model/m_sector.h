#pragma once
#include "_game.h"
#define FIELDS_IN_SECTOR 10

/// trida reprezentujici sektor hraciho planu
class m_home;
class m_field;
class m_finish;
class m_sector {
public:
	m_sector(int);
	int getID();
	m_home getStart();
	m_finish getFinish();
	m_field *getFields();
	m_sector nextSector();
private:
	m_home * start;
	m_finish * finish;
	int id;
	m_field *fields[FIELDS_IN_SECTOR];

};
