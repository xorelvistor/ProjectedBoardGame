#include "_game.h"
class m_desk;
int m_field::pocet = 0;
int m_figure::pocet = 0;

m_field predani(m_field neco) {
	cout << "Predani" << endl;
	return neco;
}
int main () {
	
	m_desk *desk;
	desk = new m_desk(1);
	int n=3;
	m_sector *sektor = NULL;
	m_field *policko = NULL;
	m_player *hrac1 = NULL;
	m_figure *figurka = NULL;
	std::list<m_sector*> sektory;
	std::list<m_sector*>::iterator iS;
	std::list<m_field*> policka;
	std::list<m_figure*> figurky;
	std::list<m_figure*>::iterator iF;
	std::list<m_field*>::iterator iP;
	std::vector <m_field*> moznosti;
	std::vector <m_field*>::iterator iM;

	sektory = desk->getSectors();
	iS = sektory.begin();
	//std::advance(iS,3);

	hrac1 = new m_player("petr");
	hrac1->setSector(*iS);
	hrac1->setHome();

	policka = hrac1->getSector()->getFields();
	iP = policka.begin();
	std::advance(iP,1);
	figurky = hrac1->getMyFigures();
	iF = figurky.begin();
	
	cout << "--- nahozeni prvni figury ---\n";
	figurka = hrac1->getSector()->getStart()->crankUp();
	policko = desk->findStrokes(hrac1,5);
	figurka->move(policko);
	cout << "--- nahozeni druhe figury ---\n";
	figurka = hrac1->getSector()->getStart()->crankUp();
	policko = desk->findStrokes(hrac1,7);
	figurka->move(policko);
	policko = desk->findStrokes(hrac1,2);
	moznosti = hrac1->getStrokes();
	cout << "**** moznosti ****" << endl;
	
	for (int i = 0; i < moznosti.size() ; i++) {
		cout << "pro figuru c.: " << i << endl;
		if(moznosti[i] != NULL)
			cout << *moznosti[i] << endl;
	}
	
	//figurka=hrac1->getSector()->getStart()->crankUp();
	//figurka=hrac1->getSector()->getStart()->crankUp();
	//figurka=hrac1->getSector()->getStart()->crankUp();
	//figurka=hrac1->getSector()->getStart()->crankUp();
	/*
	cout << "--- posun prvni figury ---\n";
	std::advance(iP,1);
	figurka->move(*iP);
	cout << "--- nahozeni druhe figury ---\n";
	figurka=hrac1->getSector()->getStart()->crankUp();
	cout << "--- posun druhe figury ---\n";
	std::advance(iP,1);
	figurka->move(*iP);
	cout << "--- nahozeni treti figury ---\n";
	figurka=hrac1->getSector()->getStart()->crankUp();
	cout << "--- posun treti figury ---\n";
	std::advance(iP,1);
	figurka->move(*iP);
	cout << "--- nahozeni ctvrte figury ---\n";
	figurka=hrac1->getSector()->getStart()->crankUp();
	cout << "--- posun ctvrte figury ---\n";
	std::advance(iP,1);
	figurka->move(*iP);
	*/
	/*
	iS = sektory.begin();
	
	cout << "------------- pruchod v mainu -------------" << endl;
	int oblasti = 2;
	//for (iS = sektory.begin(); iS!= sektory.end(); iS++) {
		cout << "sektor c.: " << (*iS)->getID() << ", &" << (*iS) << endl;
		for (int i = 0; i < oblasti; i++) {
			if (i == 0) {
				cout << "------------------------- startovni pole --------------------------" << endl;
				cout << "Pocet figur: " << (*iS)->getStart()->getFigureCount() << endl;
				policka = (*iS)->getStart()->getFields();
			} else if (i == 1) {
				cout << "------------------------- hraci pole --------------------------" << endl;
				policka = (*iS)->getFields();
			} 
			//else if (i == 2) {
			//	cout << "------------------------- cilova pole --------------------------" << endl;
			//	cout << (*iS)->getFinish()->getFigureCount() << endl;
			//	policka = (*iS)->getFinish()->getFields();
			//}
			for (iP = policka.begin() ; iP != policka.end(); iP++) {
				cout << "-" << *(*iP);
			}
		}
	//}
*/
	cout << "xxxxx konec mainu xxxxx" << endl;
	getchar();
	return 0;
}