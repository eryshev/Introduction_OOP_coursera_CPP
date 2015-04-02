#include <vector>
#include <iostream>
#include <string>
using namespace std;

/*******************************************
 * Completez le programme à partir d'ici.
 *******************************************/
class Affichable {
	virtual ostream& affiche(ostream& out) const = 0;

	friend ostream& operator<< (ostream& out, const Affichable& a) {
		return a.affiche(out);
	}
};

class OptionVoyage : public Affichable {
private:
	string _nom;
protected :
	double _prix;
public:
	int test;
	OptionVoyage(string nom, double prix) : _nom(nom), _prix(prix) {}

	inline string nom() const { return _nom; }

	virtual double prix() const { return _prix; }

	ostream& affiche(ostream& out) const {
		return out << nom() << " -> " << prix() << " CHF";
	}
};

class Sejour : public virtual OptionVoyage {
private:
	int nombre_de_nuits;
	double prix_par_nuit;
public:
	Sejour(string nom, double prix, int nombre_de_nuits, double prix_par_nuit)
		: OptionVoyage(nom, prix + nombre_de_nuits * prix_par_nuit), nombre_de_nuits(nombre_de_nuits), prix_par_nuit(prix_par_nuit)
	{}
};


class Transport : public virtual OptionVoyage {
private:
	bool est_long;
protected:
	static const double TARIF_LONG;
	static const double TARIF_BASE;
public:
	Transport(string nom, double prix, bool est_long = false)
		: OptionVoyage(nom, prix + (est_long ? TARIF_LONG : TARIF_BASE)), est_long(est_long)
	{}
};

const double Transport::TARIF_LONG(1500.0);
const double Transport::TARIF_BASE(200.0);

class OptionCombinee : public Sejour, public Transport {
private:
	bool est_long;
public:
	OptionCombinee(string nom, double prix, int nombre_de_nuits, double prix_par_nuit, bool est_long = false)
		: OptionVoyage(nom, 2*prix + nombre_de_nuits * prix_par_nuit + (est_long ? Transport::TARIF_LONG : Transport::TARIF_BASE)), 
		Sejour(nom, prix, nombre_de_nuits, prix_par_nuit), 
		Transport(nom, prix, est_long)
	{}
	
	inline double prix() const { return 0.75 * _prix; }
};

class KitVoyage : public Affichable {
private:
	vector<const OptionVoyage*> options;
	string depart, destination;
public:
	KitVoyage(string depart, string destination) : depart(depart), destination(destination) 
	{}

	double prix() const {
		double sum(0);
		for (auto opt : options) sum += opt->prix();
		return sum;
	}

	void ajouter_option(const OptionVoyage& option) {
		options.push_back(&option);
	}

	void annuler() {
		options.clear();
	}

	ostream& affiche(ostream& out) const {
		if (options.empty()) out << "Voyage de " << depart << " à " << destination << ":  vous n'avez rien réservé !" << endl;
		else {
			out << "Voyage de " << depart << " à " << destination << ", avec pour options	: " << endl;
			for (auto opt : options) {
				cout << "   - ";
				opt->affiche(out);
				cout << endl;
			}
			out << "Prix total : " << prix() << " CHF" << endl;
		}
		return out;
	}
	
};

/*******************************************
 * Ne rien modifier apres cette ligne.
 *******************************************/

int main()
{
  Transport transp1("Trajet en train", 50.0);
  Transport transp2("Trajet en avion", 50.0, true);

  Sejour sejour1("Hotel 3* : Les amandiers ", 40.0, 5, 100.0);
  Sejour sejour2("Hotel 4* : Ambassador Plazza  ", 100.0, 2, 250.0);

  OptionCombinee combi("Hotel 4* : Ambassador Plazza et trajet en avion",
                       100.0, 2, 250.0, true);

  KitVoyage kit1("Zurich", "Paris");
  kit1.ajouter_option(transp1);
  kit1.ajouter_option(sejour1);
  cout << kit1 << endl;

  KitVoyage kit2("Zurich", "New York");
  kit2.ajouter_option(transp2);
  kit2.ajouter_option(sejour2);
  cout << kit2 << endl;

  KitVoyage kit3("Zurich", "New York");
  kit3.ajouter_option(combi);
  cout << kit3 << endl;

  kit1.annuler();
  kit2.annuler();
  cout << kit1 << endl;
  cout << kit2 << endl;

  return 0;

}
