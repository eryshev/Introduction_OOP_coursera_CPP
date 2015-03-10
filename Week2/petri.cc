#include <iostream>
#include <utility> // pour la fonction swap
#include <string>
#include <vector>
using namespace std;

class Cellule
{
    /*****************************************************
   * Compléter le code à partir d'ici
   *****************************************************/
public:
    Cellule(string nom = "Pyrobacculum",
            double taille = 10.,
            int energie = 5,
            string couleur = "verte") :
        nom(nom), taille(taille), energie(energie), couleur(couleur)
    {}

    void affiche() const {
        cout << nom;
        cout << ", taille = " << taille;
        cout << " microns, énergie = " << energie;
        cout << ", couleur = " << couleur;
        cout << endl;
    }

    Cellule division() {
        Cellule new_c = *this;
        energie--;
        new_c.perform_mutation();
        return new_c;
    }

    inline int _energie() const { return energie; }

private:
    string nom;
    double taille;
    int energie;
    string couleur;

    inline void perform_mutation() {
        couleur = mutation();
    }

    string mutation() const {
        if (couleur == "verte") return "blue";
        if (couleur =="bleu") return "rouge";
        if (couleur =="rouge") return "rose bonbon";
        if (couleur == "violet") return "verte";
        return couleur + " fluo";
    }
};

class Petri {
public:
    inline void ajouter(Cellule c) {
        population.push_back(c);
    }

    void affiche() {
        for (vector<Cellule>::iterator i=population.begin(); i != population.end(); ++i) {
            (*i).affiche();
        }
    }

    void evolue() {
        unsigned int size = population.size();
        for (unsigned int i = 0; i < size; ++i) {
            population.push_back(population[i].division());
        }

        /*for (vector<Cellule>::iterator i=population.begin(); i != population.end(); ++i) {
            if((*i)._energie() <= 0) {
                population.erase(i);
            }
        }*/

        size = population.size();
        for (unsigned int i = 0; i < size; ++i) {
            if( population[i]._energie() <= 0) {
                swap(population[i], population.back());
                population.pop_back();
                size--;
            }
        }
    }

private:
    vector<Cellule> population;
};

/*******************************************
 * Ne rien modifier après cette ligne.
 *******************************************/

int main()
{
  Petri assiette;

  assiette.ajouter(Cellule());
  assiette.ajouter(Cellule("PiliIV", 4, 1, "jaune"));
  cout << "Population avant évolution :" << endl;
  assiette.affiche();

  assiette.evolue();
  cout << "Population après évolution :" << endl;
  assiette.affiche();

  return 0;
}
