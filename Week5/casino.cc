#include <iostream>
#include <string>
#include <array>
#include <vector>
using namespace std;

/*******************************************
 * Completez le programme à partir d'ici.
 *******************************************/
class Mise {
private:
	int _mise;
public:
	Mise(int mise) : _mise(mise) {}

	virtual ~Mise() {}

	virtual inline int mise() const final {
		return _mise;
	}

	virtual int gain(int tirage) const = 0;
};

class Pleine : public Mise {
private:
	static const int FACTEUR;
	int _numero;
public:
	Pleine(int mise, int numero) : Mise(mise), _numero(numero) {}

	virtual inline int gain(int tirage) const override {
		if (tirage == _numero) return FACTEUR * mise();
		else return 0;
	}
};

const int Pleine::FACTEUR(35);

class Rouges : public Mise {
private:
	static const array<int, 18> ROUGES;
public:
	Rouges(int mise) : Mise(mise) {}

	virtual inline int gain(int tirage) const override {
		for (auto n : ROUGES) if (n == tirage) return mise();
		return 0;
	}
};

const array<int, 18> Rouges::ROUGES{ { 1, 3, 5, 7, 9, 12, 14, 16, 18, 19, 21, 23, 25, 27, 30, 32, 34, 36 } };

class Joueur {
private:
	string _nom;
	Mise* _strategie;
public:
	Joueur(string nom) : _nom(nom), _strategie(nullptr) {}

	Joueur(const Joueur&) = delete;

	//~Joueur() { delete _strategie; }

	inline string nom() const { return _nom; }

	inline void set_strategie(Mise *strategie) {
		_strategie = strategie;
	}

	inline int mise() const {
		return _strategie == nullptr ? 0 : _strategie->mise();
	}

	inline int gain(int tirage) const {
		return _strategie == nullptr ? 0 : _strategie->gain(tirage);
	}
};

class Roulette {
private:
	int num_tire;
protected:
	vector<const Joueur*> jouers;
public:
	Roulette() {}

	virtual ~Roulette() {}

	virtual inline void participe(const Joueur& joueur) {
		for (auto j : jouers) if (j == &joueur) return;
		jouers.push_back(&joueur);
	}

	inline int tirage() const {
		return num_tire;
	}

	inline int nombre_participants() const {
		return jouers.size();
	}

	inline void rien_ne_va_plus(int valeur) {
		num_tire = valeur;
	}

	virtual int perte_mise(int mise_originale) const = 0;

	int gain_maison() const {
		int sum(0);
		for (auto j : jouers) 
			if (j->gain(num_tire) == 0) sum += perte_mise(j->mise());
			else sum -= j->gain(num_tire);
		return sum;
	}

	virtual void afficher(ostream& out) const = 0;

	friend ostream& operator<<(ostream& out, Roulette& r) {
		r.afficher(out);
		return out;
	}

	void annoncer() const {
		//afficher(cout);
		cout << "Croupier : le numéro du tirage est le " << num_tire << endl;
		for (auto j : jouers) {
			cout << "Le joueur " << j->nom()
				<< " a misé " << j->mise()
				<< " et ";
			if (j->gain(num_tire) == 0) cout << "perd " << perte_mise(j->mise());
			else cout << "gagne " << j->gain(num_tire);
			cout << endl;
		}
		cout << "Gain/perte du casino : " << gain_maison() << endl;
	}

	Roulette(const Roulette&) = delete;
};

class RouletteFrancaise : public Roulette {
public:
	virtual int perte_mise(int mise_originale) const override {
		return mise_originale;
	}

	virtual void afficher(ostream& out) const override {
		out << "Roulette française";
	}
};

class RouletteAnglaise : public Roulette {
public:
	virtual int perte_mise(int mise_originale) const override {
		return mise_originale * 0.5;
	}

	virtual void afficher(ostream& out) const override {
		out << "Roulette anglaise";
	}

	virtual inline void participe(const Joueur& joueur) override {
		if (nombre_participants() < 10) Roulette::participe(joueur);
	}
};


/*******************************************
 * Ne rien modifier apres cette ligne.
 *******************************************/

void simuler_jeu(Roulette& jeu)
{
  cout << jeu << " :" << endl;
  for (int tirage : array<int, 3>({ 12, 1, 31 })) {
    jeu.rien_ne_va_plus(tirage);
    jeu.annoncer();
    cout << endl;
  }
  cout << endl;
}

int main()
{
  Joueur joueur1("Dupond");
  Joueur joueur2("Dupont");
  Joueur joueur3("Dupond"); // un AUTRE joueur, du même nom

  Pleine p1(100, 1); // miser 100 jetons sur le 1
  Rouges p2(30);     // miser  30 jetons sur les rouges
  Pleine p3(20, 13); // miser  20 jetons sur le 13

  joueur1.set_strategie(&p1);
  joueur2.set_strategie(&p2);
  joueur3.set_strategie(&p3);

  RouletteAnglaise  jeu1;
  RouletteFrancaise jeu2;

  for (auto jeu : array<Roulette*, 2>({ &jeu1, &jeu2 })) {
    jeu->participe(joueur1);
    jeu->participe(joueur2);
    jeu->participe(joueur3); // c'est un autre joueur
    jeu->participe(joueur2); // c'est déjà un joueur du jeu
    simuler_jeu(*jeu);
  }

  return 0;
}
