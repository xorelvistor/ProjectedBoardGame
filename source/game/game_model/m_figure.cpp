#include "m_figure.h"
m_figure::m_figure(){
	//cout << "Obycejny konstruktor figury" << endl;
	field = NULL;
	owner = NULL;
	at_home = true;
	out_of_sector = false;
	id = 42;
}
m_figure::m_figure(m_player *player, int id) {
	//cout << "Obycejny konstruktor figury" << endl;
	at_home = true; /// figura zacina v domecku
	out_of_sector = false;
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

bool m_figure::getOutOfSector() {
	return out_of_sector;
}

m_player* m_figure::getOwner() {
	return owner;
}

int m_figure::getID() {
	return id;
}

ostream& operator<<(ostream& os,const m_figure& m_f) {
	os << "Figura c.: " << m_f.id <<  ", &" << (&m_f) << endl;
	os << "\tvlastnik: \"" << m_f.owner->getName() << "\""<< endl;
	if (m_f.at_home == true)
		os << "\tstav: na startu" << endl;
	else os << "\tstav: ve hre" << endl;
	if (m_f.out_of_sector == true)
		os << "\tpoloha: mimo startovni sektor" << endl;
	else os << "\tpoloha: stale ve startovnim sektoru" << endl;
	return os;
}

/// nastavi pole na kterem figura stoji
void m_figure::setField(m_field *field) {
	this->field = field;
}

void m_figure::setAtHome() {
	at_home = !at_home;
}

void m_figure::setOutOfSector() {
	out_of_sector = true;
}

void m_figure::move(m_field *new_field) {
	cout << "---move---" << endl;
	//cout << "-> from\n" << *field;
	//cout << "<- to\n" << *new_field;
	m_figure *figure;
	//cout << "- removeFigure - from -" << endl;
	field->removeFigure();
	//cout << "- getFigure - to -" << endl;
	figure = new_field->getFigure();

	if (figure != NULL && new_field == owner->getStrokes()[id]) {
		//cout << "mam" << endl << *figure;
		figure->goHome();
	}
	//cout << "- removeFigure - to -" << endl;
	new_field->removeFigure();
	setField(new_field);
	(new_field)->putFigure(this);
	if (owner->getSector() != field->getSector()) {
		setOutOfSector();
	}
	
}
void m_figure::attack(m_figure *figure) {

}
/// vrati figuru na start
void m_figure::goHome() {
	cout << "--- goHome ---\n";
	list<m_field*>::iterator iterF;
	list<m_field*> home_fields;
	m_home* home;
	home = owner->getSector()->getStart();
	home_fields = home->getFields();
	iterF = home_fields.begin();
	for (;iterF != home_fields.end(); iterF++) {
		if ((*iterF)->isFree()) {
			move(*iterF);
			break;
		}
	}
	setAtHome();
	home->increaseCount();
	cout << "--- end goHome ---\n";
}