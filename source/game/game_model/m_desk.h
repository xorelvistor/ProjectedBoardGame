#pragma once
#include "_game.h"
/// trida reprezentujici hraci plan
class m_desk;
class m_sector;
class m_desk {
private:
	int desk_type;
	m_sector *sectors[4];
public:
	m_desk(int);
	int getDeskType();
};