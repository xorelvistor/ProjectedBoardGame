#include "m_field.h"
m_field::m_field(){
	cout << "Obycejny konstruktor pole" << endl;
	id = 42;
	this->sector = NULL;
	figure = NULL;
	special = "nespecifikovano";
}
/// nastavi sektor a id pole
m_field::m_field(int number){
	cout << "Obycejny konstruktor pole" << endl;
		id = number;
		this->sector = NULL;
		figure = NULL;
		special = "hraci";
		pocet++;
}
m_field::m_field(m_sector *sector,int number){
//	cout << "Obycejny konstruktor pole" << endl;
		id = number;
		this->sector = sector;
		figure = NULL;
		special = "";
		pocet++;
}
m_field::m_field(m_sector *sector,int number,std::string str_special){
//	cout << "Obycejny konstruktor pole" << endl;
		id = number;
		this->sector = sector;
		figure = NULL;
		special = str_special;
		pocet++;
}

m_field::m_field(const m_field& original) {
	cout << "Kopirovaci konstruktor pole" << endl;
	
	id = original.id;
	sector = new m_sector();
	figure = new m_figure();
}
m_field::~m_field() {
	cout << "Destruktor pro " << id << endl;
	figure = NULL;
	sector = NULL;
	delete figure;
	delete sector;
}

ostream& operator<<(ostream& os,const m_field& m_f) {
	os << "Pole c.: " << m_f.id << " ("<< m_f.special << "), @ " << (&m_f) << endl;
	if (m_f.sector != NULL)
		os << "\tv sektoru c.: " << m_f.sector->getID() << ", @ " << m_f.sector << endl;
	if (m_f.figure != NULL)
		os << "\tfigura: id.: " << m_f.figure->getID() << " @ " << m_f.figure << endl;
	return os;
}
/// vrati sektor, ke kteremu pole prislusi
m_sector *m_field::getSector() {
	return sector;
}
/// vrati figuru na poli, pripadne null
m_figure *m_field::getFigure() {
	return figure;
}
/// vrati id daneho pole
int m_field::getID() {
	return id;
}

bool m_field::isFree() {
	if (this->figure != NULL)
		return false;
	else {
		return true;
	}
}
/// vlozi figuru na pole
void m_field::putFigure(m_figure *figure) {
	this->figure = figure;
	cout << "vlozena figura (" << this->figure << ") na pole (" << &(*this) <<") [" << sector << "]" << sector->getID() << "-" << id << " => volno: " << isFree() << endl;
}
/// odstrani figuru z pole a vrati ji
m_figure *m_field::removeFigure() {
	m_figure *old = NULL;
	old = figure; 
	figure = NULL;
	return old;
}

