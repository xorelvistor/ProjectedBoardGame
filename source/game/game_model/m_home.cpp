#include "m_home.h"
m_home::m_home(){cout << "Obycejny konstruktor home" << endl;}
m_home::m_home(m_sector *sector) {
	//cout << "Obycejny konstruktor home" << endl;
	//cout << "\n\tstartovni pozice" << endl;
	this->sector = sector;
	figure_count = 4;
	list<m_field*>::iterator iterF;
	m_field *pole;
	for(int i = 0; i < figure_count ; i++) {
		pole = new m_field(sector,i,"start");
		fields.push_back(pole);
		if (fields.empty()) {
			cout << "koncim" << endl;
			return;
		}
	}
}
/*
m_home::m_home(const m_home &original) {
	cout << "Kopirovaci konstruktor home" << endl;
	
	figure_count = original.figure_count;
	sector = new m_sector();
	*sector = *original.sector;
	std::list<m_field>::const_iterator iter_orig;
	iter_orig = original.fields.begin();
	for (;iter_orig != original.fields.end();iter_orig++) {
		fields.push_back(*iter_orig);
	}
	
}
*/
m_home::~m_home() {
	sector = NULL;
	delete sector;
}
int m_home::getFigureCount () {
	return figure_count;
}

std::list<m_field*>  m_home::getFields() {
	return fields;
}

void m_home::increaseCount() {
	figure_count++;
}
void m_home::initHome(m_player *owner) {
	m_figure *figure;
	m_field *field;
	std::string zprava="";
	list<m_field*>::iterator iterF;
	int i = 0;
	cout << "-- initHome --" << endl;
	cout << "V sektoru (" << owner->getSector()->getID() << ") hrace \"" << owner->getName() << "\" byly vytvoreny tyto figury:\n";
	for (iterF = fields.begin();iterF != fields.end(); iterF++) {
		figure = new m_figure(owner,i);
		field = (*iterF);
		field->putFigure(figure);
		figure->setField(field);
		owner->setFigure(figure);
		i++;
	}
}
/// nahodi figuru do hry
m_figure* m_home::crankUp() {
	cout << "-- crankUp --" << endl;
	std::list<m_field*>::iterator iterF;
	std::list<m_field*> fields;
	m_figure* figure;
	for(iterF = this->fields.begin(); iterF != this->fields.end(); iterF++) {
		if ((figure = (*iterF)->getFigure()) != NULL) {
			cout << *figure;
			break;
		}
	}
	if (figure == NULL) {
		cout << "zadna figura na startu" << endl;
		return NULL;
	}
	if (figure->getAtHome()) {
		fields = sector->getFields();
		iterF = fields.begin();
		figure->move(*(++iterF));
		figure->setAtHome();
	}
	figure_count--;
	return figure;
}
