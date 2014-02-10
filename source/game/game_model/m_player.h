#pragma once
#include "_game.h"
/// trida reprezentujici hrace
class m_figure;
class m_sector;
class m_player {
private:
	std::list<m_figure*> my_figures;
	std::string name;
	std::string color;
	m_sector *my_sector; 
public:
	m_player();
	m_player(std::string);
//	m_player(const m_player &);
	~m_player();
	m_sector *getSector();
	std::string getName();
	std::list<m_figure*> getMyFigures();
	bool setColor(std::string);
	bool setSector(m_sector *);
	bool setFigure(m_figure *);
	void setHome();
};
