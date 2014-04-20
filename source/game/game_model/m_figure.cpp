#include "m_figure.h"
#include "m_player.h"

#include "m_sector.h"
#include "m_field.h"
#include "m_home.h"


/** Bezparametricky konstruktor **/
/**
 * m_figure
 * konstruktor bez parametru
 */
m_figure::m_figure(){
	field = NULL;
	owner = NULL;
	at_home = true;
	out_of_sector = false;
	id = 42;
}

/** Konstruktor **/
/**
 * m_figure
 * konstruktor - inicializuje objekt
 *
 * @param player hrac, kteremu figura patri
 * @param id oznaceni figury
 */
m_figure::m_figure(m_player* player, int id) {
	at_home = true; /// figura zacina v domecku
	out_of_sector = false;
	owner = player;
	field = NULL;
	this->id = id;
}

/** 
 * destruktor
 * uvolni objekt
 */
m_figure::~m_figure() {
	field = NULL;
	owner = NULL;
	delete field;
	delete owner;
}

/**
 * getField
 * vrati pole na kterem figura stoji
 *
 * @return pole na kterem figura stoji
 */
m_field* m_figure::getField() {
	return field;
}

/**
 * getAtHome
 * zjisti, zda je figura ve hre
 *
 * @return true-pokud figura neni ve hre
 * @return false-pokud figura je ve hre
 */
bool m_figure::getAtHome() {
	return at_home;
}

/**
 * getOutOfSector
 * zjisti, zda jiz figura opustila svuj startovni sektor
 *
 * @return true-figura jiz opustila startovni sektor
 * @return false-figura jeste neopustila startovni sektor
 */
bool m_figure::getOutOfSector() {
	return out_of_sector;
}

/**
 * getOwner
 * vrati hrace, kteremu figura patri
 *
 * @return vlastnik figury
 */
m_player* m_figure::getOwner() {
	return owner;
}

/**
 * getID
 * vrati cislo figury
 *
 * @return oznaceni figury
 */
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

/**
 * setField
 * nastavi pole na kterem figura stoji
 *
 * @param field pole, ktere ma byt prirazeno
 */
void m_figure::setField(m_field* field) {
	this->field = field;
}

/** 
 * setAtHome
 * zmeni stav figury
 */
void m_figure::setAtHome() {
	at_home = !at_home;
}

/**
 * setOutOfSector
 * zmeni stav figury
 */
void m_figure::setOutOfSector() {
	out_of_sector = !out_of_sector;
}

/**
 * move
 * presune figuru na nove pole, pokud narazi na cizi figuru vyhodi ji
 *
 * @param new_field pole, kam se ma figura presunout
 */
void m_figure::move(m_field* new_field) {
	//cout << "---move---" << endl;
	//cout << "-> from\n" << *field;
	//cout << "<- to\n" << *new_field;
	m_figure *figure;
	//cout << "- removeFigure - from -" << endl;
	field->removeFigure();
	//cout << "- getFigure - to -" << endl;
	figure = new_field->getFigure();

	if (figure != NULL && new_field == owner->getStrokes()[id] && figure != this) {
		cout << "mam" << endl << *figure;
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

/**
 * goHome
 * vrati figuru na start
 */
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
	if(out_of_sector)
		setOutOfSector();
	//cout << "--- end goHome ---\n";
}