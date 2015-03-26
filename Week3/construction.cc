#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Pour simplifier
typedef string Forme   ;
typedef string Couleur ;

class Brique
{
private:
  Forme   forme   ;
  Couleur couleur ;

public:
  /*****************************************************
    Compléter le code à partir d'ici
  *******************************************************/
	Brique(const Forme f, const Couleur c) : forme(f), couleur(c) {}

	ostream& afficher(ostream& sortie) const {
		return (couleur.empty()) ? sortie << forme : sortie << "(" << forme << ", " << couleur << ")";
	}

	friend ostream& operator<<(ostream& out, const Brique& b) {
		return b.afficher(out);
	}
};

class Construction
{
	friend class Grader;

public:
	vector<vector<vector<Brique>>> contenu;

	Construction(const Brique& b) {
		vector<Brique> v_v = { b };
		vector<vector<Brique>> h_v = { v_v };
		contenu.push_back(h_v);
	}

	ostream& afficher(ostream& sortie) const {
		if (!contenu.empty()) {
			for (int i = contenu.size()-1; i >= 0; --i) {
				sortie << "Couche " << i << " :" << endl;
				for (auto j = 0; j < contenu[i].size(); ++j) {
					for (auto k = 0; k < contenu[i][j].size(); ++k)
						sortie << contenu[i][j][k] << " ";
					sortie << endl;
				}
			}
		}
		return sortie;
	}

	friend ostream& operator<<(ostream& out, const Construction& c) {
		return c.afficher(out);
	}

	Construction& operator^=(const Construction& that) {
		for (auto c : that.contenu) this->contenu.push_back(c);
		return *this;
	}

	Construction& operator-=(const Construction& that) {
		if (that.contenu.size() < contenu.size()) return *this;
		for (auto i = 0; i < contenu.size(); ++i) {
			auto j = contenu[i].size();
			for (auto t_c : that.contenu[i]) contenu[i].push_back(t_c);
		}
		return *this;
	}

	Construction& operator+=(const Construction& that) {
		if (that.contenu.size() < contenu.size()) return *this;
		for (auto i = 0; i < contenu.size(); ++i) {
			if (that.contenu[i].size() < contenu[i].size()) return *this;
			for (auto j = 0; j < contenu[i].size(); ++j)
			for (auto t_c : that.contenu[i][j]) contenu[i][j].push_back(t_c);
		}
		return *this;
	}
};

const Construction operator^(Construction a, Construction const& b) {
	Construction t_c = a ^= b;
	return t_c;
}

const Construction operator-(Construction a, Construction const& b) {
	Construction t_c = a -= b;
	return t_c;
}

const Construction operator+(Construction a, Construction const& b) {
	Construction t_c = a += b;
	return t_c;
}

const Construction operator*(unsigned int n, Construction const& a) {
	return (n == 0 || n == 1) ? a : a + (--n * a);
}

const Construction operator/(unsigned int n, Construction const& a) {
	return (n == 0 || n == 1) ? a : a ^ (--n / a);
}

const Construction operator%(unsigned int n, Construction const& a) {
	return (n == 0 || n == 1) ? a : a - (--n % a);
}

/*******************************************
* Ne rien modifier après cette ligne.
*******************************************/

int main()
{
	// Modèles de briques
	Brique toitD("obliqueD", "rouge");
	Brique toitG("obliqueG", "rouge");
	Brique toitM(" pleine ", "rouge");
	Brique mur(" pleine ", "blanc");
	Brique vide("                 ", "");

	unsigned int largeur(4);
	unsigned int profondeur(3);
	unsigned int hauteur(3); // sans le toit

	// on construit les murs
	Construction maison(hauteur / (profondeur % (largeur * mur)));

	// on construit le toit
	Construction toit(profondeur % (toitG + 2 * toitM + toitD));
	toit ^= profondeur % (vide + toitG + toitD);

	// on pose le toit sur les murs
	maison ^= toit;

	// on admire notre construction
	cout << maison << endl;

	return 0;
}