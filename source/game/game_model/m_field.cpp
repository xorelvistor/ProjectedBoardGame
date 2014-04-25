#include "m_field.h"
#include "m_sector.h"
#include "m_figure.h"

/** Bezparametricky konstruktor **/
/**
 * m_field
 * konstruktor bez parametru
 */
m_field::m_field(){
	id = 42;
	this->sector = NULL;
	figure = NULL;
	special = "nespecifikovano";
}

/** Konstruktor **/
/**
 * m_field
 * konstruktor - inicializuje objekt
 *
 * @param sector sektor, kteremu pole prislusi
 * @param number identifikator pole
 * @param str_special typ pole
 */
m_field::m_field(m_sector* sector,int number,std::string str_special){
		id = number;
		this->sector = sector;
		figure = NULL;
		special = str_special;
}

/** Destruktor **/
/** 
 * destruktor
 * uvolni objekt
 */
m_field::~m_field() {
	figure = NULL;
	sector = NULL;
	delete figure;
	delete sector;
}

/** vypis instance **/
ostream& operator<<(ostream& os,const m_field& m_f) {
	os << "Pole c.: " << m_f.id <<  ", &" << (&m_f) << endl;
	os << "\ttyp: " << m_f.special << endl;
	if (m_f.sector != NULL)
		os << "\tv sektoru c.: " << m_f.sector->getID() << ", &" << m_f.sector << endl;
	if (m_f.figure != NULL)
		os << "\t" << *(m_f.figure);
	return os;
}

/**
 * getSector
 * vrati sektor, kteremu pole prislusi
 *
 * @return sektor, kteremu pole prislusi
 */ 
m_sector *m_field::getSector() {
	return sector;
}

/**
 * getFigure
 * vrati figuru, ktera je na poli nebo NULL
 *
 * @return figura na poli nebi NULL 
 */
m_figure *m_field::getFigure() {
	return figure;
}

/**
 * getID
 * vrati identifikator pole
 * 
 * @return identifikator pole
 */
int m_field::getID() {
	return id;
}

/**
 * isFree
 * zjisti, zda je na poli figura
 * 
 * @return true nebo false
 */
bool m_field::isFree() {
	if (this->figure != NULL)
		return false;
	else {
		return true;
	}
}

/**
 * leftUntilEnd
 * vrati pocet poli do konce sektoru
 *
 * @return pocet poli 
 */
int m_field::leftUntilEnd() {
	if(special == "hraci")
		return 9-id;
	else
		return 3-id;
}

/**
 * putFigure
 * vlozi figuru na pole
 *
 * @param figure figura, ktera ma byt vlozena na pole
 */
void m_field::putFigure(m_figure *figure) {
	//cout << "- putFigure -" << endl;
	this->figure = figure;
	//cout << *figure;

}

/**
 * removeFigure
 * odstrani figuru z pole
 *
 * @return odstranena figura nebo NULL
 */
m_figure *m_field::removeFigure() {
	m_figure *old = NULL;
	old = figure; 
	figure = NULL;
	return old;	
}

