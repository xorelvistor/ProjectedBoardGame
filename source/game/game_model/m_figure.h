#pragma once
#include "_game.h"
/// trida reprezentujici figurku
class m_field;
class m_player;
class m_figure {
private:
	m_field *field;
	bool at_home;
	m_player *owner;
	int id;
public:
	m_figure();
	m_figure(m_player *,int);
//	m_figure(const m_figure &);
	~m_figure();
	void setField(m_field *);
	m_field *getField();
	bool getAtHome();
	m_player *getOwner();
	int getID();
	void move(m_field *);
	void attack(m_figure *);
	void goHome();
	static int pocet;
};
