#pragma once
#include "_game.h"
/// trida reprezentujici hraci plan
class m_desk;
class m_sector;
class m_desk {
private:
	int desk_type;
	std::list<m_sector*> sectors;
public:
	m_desk(int);
//	m_desk(const m_desk &);
	int getDeskType();
	std::list<m_sector*> getSectors();
	
};