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

	sektory = desk->getSectors();
	iS = sektory.begin();

	hrac1 = new m_player("petr");
	hrac1->setSector(*iS);
	hrac1->setHome();

	//for (; iS!= sektory.end(); iS++) {
		cout << "sektor c.: " << (*iS)->getID() << " @ " << (*iS) << endl;
		for (int i = 0; i < 3; i++) {
			if (i == 0) {
				cout << "-- startovni pole" << endl;
				policka = (*iS)->getStart()->getFields();
			} else if (i == 1) {
				cout << "-- hraci pole" << endl;
				policka = (*iS)->getFields();
			} else if (i == 2) {
				cout << "-- cilova pole" << endl;
				policka = (*iS)->getFinish()->getFields();
			}
			for (iP = policka.begin() ; iP != policka.end(); iP++) {
				cout << "-" << *(*iP);
			}
		}
	//}

	policka = hrac1->getSector()->getFields();
	iP = policka.begin();
	std::advance(iP,5);
	figurky = hrac1->getMyFigures();
	iF = figurky.begin();
	(*iF)->move(*iP);

	for (int i = 0; i < 3; i++) {
		if (i == 0) {
			cout << "-- startovni pole" << endl;
			policka = (*iS)->getStart()->getFields();
		} else if (i == 1) {
			cout << "-- hraci pole" << endl;
			policka = (*iS)->getFields();
		} else if (i == 2) {
			cout << "-- cilova pole" << endl;
			policka = (*iS)->getFinish()->getFields();
		}
		for (iP = policka.begin() ; iP != policka.end(); iP++) {
			cout << "-" << *(*iP);
		}
	}

/*
	cout << "---main---" << endl;
	
	cout << "typ hry: " << desk->getDeskType() << endl;
	sektory = desk->getSectors();
	iS = sektory.begin();
	sektor = &(*iS); //prirazeni ukazatele na sektor
	hrac1->setSector(sektor); // predani ukazatele na sektor
	sektor->getStart()->setHome(hrac1);
	policka = sektor->getFields();
	figurky = hrac1->getMyFigures();
	iF = figurky.begin();
	figurka = &(*iF);
	iP = policka.begin();
	iP++;iP++;
	figurka->move(&(*iP));
	cout << "\n--- hledame v mainu ---" << endl;
	int pruchodu = 2;
	sektory = desk->getSectors();
	for (iS = sektory.begin(); iS != sektory.end(); iS++) {
		cout << "sektor c. " << iS->getID() << " (" << &(*iS) << ")" << endl;
		for (int i = 0; i < pruchodu; i++) {

			if (i == 0) policka = (*iS).getStart()->getFields();
			if (i == 1) policka = (*iS).getFields();
			//if (i == 2) policka = (*iS).getFinish()->getFields();

			for (iP = policka.begin() ; iP != policka.end() ; iP++) {
				cout << "na poli " << iP->getID() << " (" << &(*iP) << ")";
				if (iP->isFree()) {
					cout << " neni figura, koukni: " << iP->getFigure() << endl;
				} else {
					figurka = iP->getFigure();
					cout << " je figura: " << figurka << " na pozici " << figurka->getField()->getSector()->getID() << "-" << figurka->getField()->getID();
					cout << " (" << figurka->getField()->special << ")" << endl;
				}
			}
		}
	}
	figurky = hrac1->getMyFigures();
	figurka = NULL;
	for (iF=figurky.begin(); iF != figurky.end(); iF++) {
		figurka = &(*iF);
		if (figurka != NULL)
			cout << &(*figurka) << "->" << figurka->getField()->getSector()->getID() << "-" << figurka->getField()->getID() << " (" << figurka->getField()->special << ")" << endl;
	}
	cout << "pocet instanci: " << m_field::pocet << ", " << m_figure::pocet << endl;
*/	
	getchar();
	return 0;
}