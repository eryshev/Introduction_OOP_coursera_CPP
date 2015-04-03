#include <iostream>
#include <vector>
#include <string>
using namespace std;

/*****************************************************
 * Compléter le code à partir d'ici
 *****************************************************/
class Piece {
private:
    string nom;
public:
    Piece(string nom) : nom(nom) {}

    inline ostream& afficher(ostream& out) const {
        return out << "\"" << nom << "\"";
    }
};

class Simple : public Piece {
private:
    string orientation;
public:
    Simple(const string nom, string o = "") : Piece(nom), orientation(o) {}

    Simple(const char* nom, string o = "") : Piece(nom), orientation(o) {}

    ostream& afficher(ostream &out) const {
        Piece::afficher(out);
        if (orientation.empty()) out << "";
        else out << " orienté vers la " << orientation;
        return out << endl;
    }

    friend ostream& operator<<(ostream& out, const Simple& p) {
        out << "pièce simple ";
        return p.afficher(out);
    }
};

class Composee : public Piece {
private:
    vector<Simple> composants;
public:
    Composee(string nom) : Piece(nom) {}

    void construire(Simple s) {
        composants.push_back(s);
    }

    ostream& afficher(ostream &out) const {
        Piece::afficher(out) << " composée de :" << endl;
        for (auto c : composants) out << "    * " << c;
        return out;
    }

    friend ostream& operator<<(ostream& out, const Composee& p) {
        out << "pièce ";
        return p.afficher(out);
    }
};
/*******************************************
 * Ne rien modifier après cette ligne.
 *******************************************/

// Sans polymorphisme, c'est difficile de faire autrement :-(...
template <typename T>
void ajouterComposant(T composant, unsigned int nb)
{
  cout << ' ' << nb << " exemplaire";
  if (nb > 1) cout << 's';
  cout << " de " << composant;
}

int main()
{
  cout << "Nous fabriquons une boîte de jeu contenant :" << endl;
  ajouterComposant(Simple("brique standard"), 59);

  // Déclare une pièce composée dont le nom est «porte»
  Composee porte("porte");

  // Une porte est constituée...
  //  ...d'un cadran de porte orienté à gauche
  porte.construire(Simple("cadran porte", "gauche"));
  //  ...et d'un battant orienté à gauche
  porte.construire(Simple("battant", "gauche"));

  // La boite de jeu a pour 2e composant la porte
  ajouterComposant(porte, 1);

  // Déclare une pièce composée dont le nom est «fenêtre»
  Composee fenetre("fenetre");

  // Une fenêtre est constituée...
  //  ...d'un cadran de fenêtre (sans orientation)
  fenetre.construire(Simple("cadran fenetre"));
  //  ...d'un volet orienté à gauche
  fenetre.construire(Simple("volet", "gauche"));
  //  ...d'un volet orienté à droite
  fenetre.construire(Simple("volet", "droite"));

  // La boite de jeu a en plus deux fenêtres
  ajouterComposant(fenetre, 2);

  return 0;
}
