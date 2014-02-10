#include "m_figure.h"
m_figure::m_figure(){
	cout << "Obycejny konstruktor figury" << endl;
	field = NULL;
	owner = NULL;
	at_home = true;
}
m_figure::m_figure(m_player *player, int id) {
	cout << "Obycejny konstruktor figury" << endl;
	at_home = true; /// figura zacina v domecku
	owner = player;
	field = NULL;
	this->id = id;
	pocet++;
}
/*
m_figure::m_figure(const m_figure &original) {
	cout << "Kopirovaci konstruktor figury" << endl;
	
	owner = new m_player();
	field = new m_field();
	*owner = *(original.owner);
	*field = *(original.field);
	at_home = original.at_home;
	
}
*/
m_figure::~m_figure() {
	field = NULL;
	owner = NULL;
	delete field;
	delete owner;
}
/// vrati pole na kterem se figura nachazi
m_field *m_figure::getField() {
	return field;
}
///
bool m_figure::getAtHome() {
	return at_home;
}

m_player* m_figure::getOwner() {
	return owner;
}

int m_figure::getID() {
	return id;
}
/// nastavi pole na kterem figura stoji
void m_figure::setField(m_field *field) {
	this->field = field;
}

void m_figure::move(m_field *new_field) {
	cout << "---move---" << endl;
	cout << "from " << *field << endl;
	cout << "to " << *new_field << endl;
	m_figure *figure;
	figure = field->removeFigure();
	cout << "figura: " << figure << endl;
	if (figure->getField() != NULL)
		cout << "ID pole vracene figury: " << figure->getField()->getID() <<endl;
	(new_field)->putFigure(this);
	setField(new_field);

}
void m_figure::attack(m_figure *figure) {

}
/// vrati figuru na start
void m_figure::goHome() {
	list<m_field*>::iterator iterF;
	list<m_field*> home_fields;
	home_fields = owner->getSector()->getStart()->getFields();
	iterF = home_fields.begin();
	for (;iterF != home_fields.end(); iterF++) {
		if ((*iterF)->isFree()) {
			move(*iterF);
		}
	}
	at_home = !at_home;
}