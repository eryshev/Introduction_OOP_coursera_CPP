#include <iostream>
using namespace std;

/*****************************************************
* Compléter le code à partir d'ici
*****************************************************/
class Patient {
private:
	double weight;
	double height;

public:
	void init(double w, double h) {
		if (w > 0.0 && h > 0.0) {
			weight = w;
			height = h;
		}
		else {
			weight = 0.0;
			height = 0.0;
		}
	}

	void afficher() const {
		cout << "Patient : " << weight << " kg pour " << height << " m" << endl;
	}

	double poids() const {
		return weight;
	}

	double taille() const {
		return height;
	}

	double imc() const {
		if (height == 0.0) return 0.0;
		else return weight / (height * height);
	}

};

/*******************************************
* Ne rien modifier après cette ligne.
*******************************************/

int main()
{
	Patient quidam;
	double poids, taille;
	do {
		cout << "Entrez un poids (kg) et une taille (m) : ";
		cin >> poids >> taille;
		quidam.init(poids, taille);
		quidam.afficher();
		cout << "IMC : " << quidam.imc() << endl;
	} while (poids * taille != 0.0);
	return 0;
}
