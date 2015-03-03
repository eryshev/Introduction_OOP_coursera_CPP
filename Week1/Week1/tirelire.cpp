#include <iostream>
using namespace std;

/*****************************************************
* Compléter le code à partir d'ici
*****************************************************/
class Tirelire {
private:
	double montant;

public:
	double getMontant() const {
		return montant;
	}

	void afficher() const {
		double montant = getMontant();
		if (montant <= 0) cout << "Vous etes sans le sous." << endl;
		else cout << "Vous avez : " << montant << " euros dans votre tirelire." << endl;
	}

	void secouer() const {
		double montant = getMontant();
		if (montant > 0) cout << "Bing bing" << endl;
	}

	void remplir(double m) {
		if ( m >= 0 ) montant = m;
	}

	void vider() {
		montant = 0.0;
	}

	void puiser(double m) {
		if (m > 0 ) {
			montant -= m;
			if (montant < 0) montant = 0.0;
		}
	}

	bool montant_suffisant(double budget, double& rest) const {
		rest = montant - (budget > 0 ? budget : 0);
		if (rest >= 0) return true;
		else {
			rest *= -1;
			return false;
		}
	}

};

/*******************************************
* Ne rien modifier après cette ligne.
*******************************************/

int main()
{
	Tirelire piggy;

	piggy.vider();
	piggy.secouer();
	piggy.afficher();

	piggy.puiser(20.0);
	piggy.secouer();
	piggy.afficher();

	piggy.remplir(550.0);
	piggy.secouer();
	piggy.afficher();

	piggy.puiser(10.0);
	piggy.puiser(5.0);
	piggy.afficher();

	cout << endl;

	// le budget de vos vacances de rève.
	double budget;

	cout << "Donnez le budget de vos vacances : ";
	cin >> budget;

	// ce qui resterait dans la tirelire après les
	// vacances
	double solde(0.0);

	if (piggy.montant_suffisant(budget, solde)) {
		cout << "Vous êtes assez riche pour partir en vacances !"
			<< endl
			<< "Il vous restera " << solde << " euros"
			<< " à la rentrée." << endl << endl;
		piggy.puiser(budget);
	}
	else {
		cout << "Il vous manque " << solde << " euros"
			<< " pour partir en vacances !" << endl << endl;
	}
	return 0;
}
