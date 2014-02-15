#pragma once
#include "_game.h"
/// reprezentuje policko hraciho planu
class m_figure;
class m_sector;
class m_field {
private:
	int id; /// cislo pole v ramci sektoru
	m_figure *figure; /// figura na poli
	m_sector *sector; /// sektor do ktereho pole patri
	
public:
	std::string special;
	m_field();
	m_field(int number);
	m_field(m_sector *,int);
	m_field(m_sector *,int,std::string);
	m_field(const m_field &);
	friend std::ostream& operator<<(std::ostream& os,const m_field& m_f);
	~m_field(void);
	int getID();
	m_sector *getSector();
	m_figure *getFigure();
	bool isFree();
	int leftUntilEnd();
	void putFigure(m_figure *);
	m_figure *removeFigure();
	static int pocet;

};
