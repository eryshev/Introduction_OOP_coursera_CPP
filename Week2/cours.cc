#include <iostream>
#include <iomanip> // pour setfill
#include <string>
#include <vector>
using namespace std;

typedef string Day; // type pour définir les jous

void print_time(double); // fonction utile pour afficher les heures

class Time
/* Représente le jour et l'heure d'un évènement.
 * Les heures sont représentées en double depuis minuit.
 * Par exemple 14:30 est représenté 14.5.
 * La fonction-outil print_time permet d'afficher de telles heures.
 */
{
public:
  // Constructeur à partir du jour et de l'heure
  Time(Day jour, double heure)
    : day_(jour), hour_(heure) {
  }

  // Affichage
  void print() const {
    cout << day_ << " à ";
    print_time(hour_);
  }

  // Pour connaître le jour
  Day day() const {
    return day_;
  }

  // Pour connaître l'heure
  double hour() const {
    return hour_;
  }

private:
  Day day_;
  double hour_;
};

/* Fonction outil pour afficher les temps représentés en double
 * comme indiqué ci-dessus.
 */
void print_time(double t)
{
  cout << setfill('0') << setw(2) << int(t)
       << setfill('0') << setw(1) << ":"
       << setfill('0') << setw(2) << int(60.0 * (t - int(t)));
}

/* Type utilisé pour identifier les cours.
 * Chaque cours aura en effet un identificateur unique ; par exemple "CS-101".
 */
typedef string CourseId;

/*******************************************
 * Complétez le programme à partir d'ici.
 *******************************************/
class Activity {
public:
    Activity(string salle, Day jour, double heure, double duration) :
        salle(salle), horaire(jour, heure), duration(duration)
    {}

    Activity(string salle, Time time, double duration) :
        salle(salle), horaire(time), duration(duration)
    {}

    //
    Activity(Activity const &) = delete;

    inline string getLocation() const {
        return salle;
    }

    inline double getDuration() const {
        return duration;
    }

    inline Time getTime() const {
        return horaire;
    }

    bool conflicts(Activity const& that) const {
        Time this_time = this->horaire;
        Time that_time = that.horaire;
        double b1 = this_time.hour(), b2 = that_time.hour();
        double e1 = this_time.hour() + this->duration, e2 = that_time.hour() + that.duration;
        return (this_time.day() == that_time.day()
                && ((b1 <= b2 && b2 < e1) || (b2 <= b1 && b1 < e2)));
    }

    void print() const {
        cout << "le ";
        horaire.print();
        cout << " en " << salle << ", durée ";
        print_time(duration);
    }

private:
    string salle;
    double duration;
    Time horaire;
};

class Course {
public:
    Course (CourseId const&_course_id, string const& _nom, const Activity & _lecture, const Activity & _seance, int const& _credits)
        : course_id(_course_id), nom(_nom),
          lecture(_lecture.getLocation(), _lecture.getTime(), _lecture.getDuration()),
          seance(_seance.getLocation(), _seance.getTime(), _seance.getDuration()), credits(_credits)
    {
        /*course_id = _course_id;
        nom = _nom;
        credits = _credits;
        Activity new_lecture(_lecture.getLocation(), _lecture.getTime(), _lecture.getDuration());
        lecture = new_lecture;
        Activity new_seance(_seance.getLocation(), _seance.getTime(), _seance.getDuration());
        seance = new_seance;*/
        cout << "Nouveau cours : " << course_id << endl;
    }

    Course(Course const &) = delete;

    ~Course () {
        cout << "Suppression du cours : " << course_id << endl;
    }

    inline CourseId getId() const { return course_id; }
    inline string getTitle() const { return nom; }
    inline int getCredits() const { return credits; }

    inline double workload() const { return (lecture.getDuration() + seance.getDuration()); }
    inline bool conflicts(Activity const& that) const { return (lecture.conflicts(that) || seance.conflicts(that));}
    inline bool conflicts(Course const& that) const { return (that.conflicts(lecture) || that.conflicts(seance));}

    void print() const {
        cout << course_id << ": " << nom;
        cout << "- cours "; lecture.print();
        cout << ", exercices "; seance.print();
        cout << ". crédits : " << credits;
    }


private:
    CourseId course_id;
    string nom;
    Activity lecture;
    Activity seance;
    int credits;
};

class StudyPlan {
public:
    inline void add_course(Course const& that) {plan.push_back(const_cast<Course *>(&that));}

    bool conflicts(CourseId cid, vector<CourseId> list) const {
        Course* cours = exists(cid);
        if (exists(cid) == NULL) return false;

        unsigned int size = list.size();
        for (unsigned int i = 0; i < size; ++i) {
            Course* maybe_cours;
            maybe_cours = exists(list[i]);
            if(maybe_cours != NULL && maybe_cours->conflicts(*cours)) return true;
        }

        return false;
    }

    void print(CourseId cid) const {
        Course* cours = exists(cid);
        if (cours != NULL) cours->print();
    }

    unsigned int credits(CourseId cid) const {
        Course* cours = exists(cid);
        return (cours != NULL) ? cours->getCredits() : 0;
    }

    double workload(CourseId cid) const {
        Course* cours = exists(cid);
        //cout << "cid = " << cid << " workload = " << cours->workload() << endl;
        return (cours != NULL) ? cours->workload() : 0;
    }

    void printCourseSuggestions(vector<CourseId> list) const {
        unsigned int size = plan.size();
        bool compatible_found = false;
        for (unsigned int i = 0; i < size; ++i) {
            if (!conflicts(plan[i]->getId(), list)) {
                plan[i]->print();
                cout << endl;
                compatible_found = true;
            }
        }
        if(!compatible_found) cout << "Aucun cours n'est compatible avec la sélection de cours." << endl;
    }

private:

    Course* exists(CourseId cid) const {
        unsigned int size = plan.size();
        for (unsigned int i = 0; i < size; ++i) {
            if(plan[i]->getId() == cid) return plan[i];
        }
        return NULL;
    }

    vector<Course*> plan;
};

class Schedule {
public:
    Schedule(StudyPlan const& study_plan)
        : study_plan(study_plan)
    {}

    bool add_course(CourseId cid) {
        bool is_conflict = study_plan.conflicts(cid, user_plan);
        if (!is_conflict) {
            user_plan.push_back(cid);
            return true;
        }
        else return false;
    }

    double computeDailyWorkload() const {
        unsigned int size = user_plan.size();
        double workload(0.);
        for (unsigned int i = 0; i < size; ++i) {
            workload += study_plan.workload(user_plan[i]);
        }
        //cout << "workload = " << workload << endl;
        return workload / 5.;
    }

    int computeTotalCredits() const {
        unsigned int size = user_plan.size();
        int credits(0);
        for (unsigned int i = 0; i < size; ++i) {
            credits += study_plan.credits(user_plan[i]);
        }
        return credits;
    }

    void print() const {
        unsigned int size = user_plan.size();
        for (unsigned int i = 0; i < size; ++i) {
            study_plan.print(user_plan[i]);
            cout << endl;
        }
        cout << "Total de crédits   : " << computeTotalCredits() << endl;
        cout << "Charge journalière : ";
        print_time(computeDailyWorkload());
        cout << endl;
        cout << "Suggestions :" << endl;
        study_plan.printCourseSuggestions(user_plan);
        cout << endl;
    }

private:
    StudyPlan const& study_plan;
    vector<CourseId> user_plan;
};


/*******************************************
 * Ne rien modifier après cette ligne.
 *******************************************/

int main()
{
  // Quelques activités
  Activity physicsLecture  ("Central Hall", "lundi",  9.25, 1.75);
  Activity physicsExercises("Central 101" , "lundi", 14.00, 2.00);

  Activity historyLecture  ("North Hall", "lundi", 10.25, 1.75);
  Activity historyExercises("East 201"  , "mardi",  9.00, 2.00);

  Activity financeLecture  ("South Hall",  "vendredi", 14.00, 2.00);
  Activity financeExercises("Central 105", "vendredi", 16.00, 1.00);

  // On affiche quelques informations sur ces activités
  cout << "L'activité physicsLecture a lieu ";
  physicsLecture.print();
  cout << "." << endl;

  cout << "L'activité historyLecture a lieu ";
  historyLecture.print();
  cout << "." << endl;

  if (physicsLecture.conflicts(historyLecture)) {
    cout << "physicsLecture est en conflit avec historyLecture."
         << endl;
  } else {
    cout << "physicsLecture n'est pas en conflit avec historyLecture."
         << endl;
  }
  cout << endl;

  // Création d'un plan d'étude
  StudyPlan studyPlan;
  Course physics("PHY-101", "Physique", physicsLecture, physicsExercises, 4);
  studyPlan.add_course(physics);
  Course history("HIS-101", "Histoire", historyLecture, historyExercises, 4);
  studyPlan.add_course(history);
  Course finance("ECN-214", "Finance" , financeLecture, financeExercises, 3);
  studyPlan.add_course(finance);

  // Première tentative d'emploi du temps
  Schedule schedule1(studyPlan);
  schedule1.add_course(finance.getId());
  cout << "Emploi du temps 1 :" << endl;
  schedule1.print();

  /* On ne sait pas encore très bien quoi faire : on essaye donc
   * sur une copie de l'emploi du temps précédent.
   */
  Schedule schedule2(schedule1);
  schedule2.add_course(history.getId());
  cout << "Emploi du temps 2 :" << endl;
  schedule2.print();

  // Un troisième essai
  Schedule schedule3(studyPlan);
  schedule3.add_course(physics.getId());
  cout << "Emploi du temps 3 :" << endl;
  schedule3.print();

  return 0;
}
