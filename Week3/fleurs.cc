#include <iostream>
#include <vector>
#include <string>
using namespace std;

/* Classe représentant les "fleurs".
 * Une instance de cette classe hors Bouquet est un exemple / une catégorie
 * un type de de fleurs.
 * Les fleurs "réelles" sont les instances présentes dans les bouquets.
 */
class Fleur
{
  /*******************************************
   * Complétez le programme à partir d'ici.
   *******************************************/
public:

	Fleur(string nom, string couleur, double prix_base, bool parfum_ = false, bool promotion = false) 
		: nom(nom), couleur(couleur), prix_base(prix_base), parfum_(parfum_), promotion(promotion)
	{}

	inline double prix() const {
		return (promotion ? (prix_base / 2.) : prix_base);
	}

	inline bool parfum() const {
		return parfum_;
	}
	
	void affiche(ostream& sortie) const {
		sortie << nom << " " << couleur << (parfum_ ? " parfumée" : "") << ", prix : " << prix() << " CHF";
	}

	friend inline ostream& operator<<(ostream& out, Fleur f) {
		f.affiche(out);
		return out;
	}

	inline bool operator==(const Fleur&  that) const {
		return(nom == that.nom && couleur == that.couleur && parfum_ == that.parfum_);
	}

private:
	string nom;
	string couleur;
	double prix_base;
	bool parfum_;
	bool promotion;

};

class Bouquet {
private:
	vector<Fleur> bouquet;

public:
	bool parfum() const {
		for (auto f : bouquet) if (f.parfum()) return true;
		return(false);
	}

	double prix() const {
		double sum(0);
		for (auto f : bouquet) sum += f.prix();
		return parfum() ? sum * 2 : sum;
	}

	void affiche(ostream& out) const {
		if (bouquet.empty()) cout << "Encore aucune fleur dans le bouquet !"; 
		else {
			cout << "Bouquet " << (parfum() ? "parfumé " : "") << "composé de :" << endl;
			for (auto f : bouquet) cout << f << endl;
			cout << "Prix total : " << prix() << " CHF" << endl;
		}
	}

	friend inline ostream& operator<<(ostream& out, Bouquet b) {
		b.affiche(out);
		return out;
	}

	Bouquet& operator+=(Fleur& f) {
		bouquet.push_back(f);
		return *this;
	}

	Bouquet& operator-=(Fleur& f) {
		bouquet.erase(remove(bouquet.begin(), bouquet.end(), f), bouquet.end());
		return *this;
	}

	Bouquet& operator+(Fleur& f) {
		*this += f;
		return *this;
	}

	Bouquet& operator-(Fleur& f) {
		*this -= f;
		return *this;
	}

	vector<Fleur>::iterator remove(vector<Fleur>::iterator first, vector<Fleur>::iterator last, Fleur& val)
	{
		vector<Fleur>::iterator result = first;
		while (first != last) {
			if (!(*first == val)) {
				*result = move(*first);
				++result;
			}
			++first;
		}
		return result;
	}
};


/*******************************************
 * Ne rien modifier après cette ligne.
 *******************************************/

int main()
{
	Fleur r1("Rose", "jaune", 1.5); // exemple de rose jaune inodore
	cout << r1 << endl;

	Fleur r2("Rose", "jaune", 3.0, true); // exemple de rose jaune parfumée
	Fleur r3("Rose", "rouge", 2.0, true, true); // exemple de rose rouge parfumée en promo
	Bouquet b1;
	b1 += r1; // ajoute une fleur de type r1
	b1 += r1; // ajoute aurte une fleur de type r1
	b1 += r2;
	b1 += r3;
	cout << b1 << endl;

	b1 = b1 - r1; // supprime toutes les fleurs de même type que r1
	cout << b1 << endl;

	Bouquet b2;
	b2 = b1 + r1; // remet une fleur de type r1
	cout << b2 << endl;

	// supprime toutes les fleurs parfumées
	b2 -= r2;
	b2 -= r3;
	cout << b2;

	return 0;
}
