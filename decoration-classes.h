int pipeProcessus1[2];	//descripteur du tube
int pipeProcessus2[2];	//descripteur du tube
int pipePere[2];		//descripteur du tube

class coordonnees
{
public:
	int x, y;
	coordonnees(int x, int y)
	{
	this->x = x;
	this->y = y;
	}
	coordonnees()
	{};
};

struct jeton
{
	char couleur;


	jeton (char couleur)
	{
	this->couleur = couleur;
	};

	jeton()
	{
		couleur = ' ';
	};
};

/*
Represente une case dans le table de jeux
*/
struct caze
{
	jeton jet;     // Chaque case peut contenir un jeton
	bool occuper; // False = vide, true = occuper
	bool etoile; // Si la case est une etoile etoile = true ; sinon etoile = false

	caze()
	{
	 occuper = false; // Par default une case en vide
	 etoile = false;
	};

};

class Encavale
{
	private:
		caze table[6][6];
		coordonnees lastInsertion; // Contient la position du dernier jeton qui a été inserer
		coordonnees depart; //depart représente le ponit de depart. Le coeur en quelque sorte
		int PointsAmi;
		int PointsAdversaire;
		int NbreJeton;
		int NbreJetonAdvers;
		char CouleurJeton;
		int monPid;
		int PidProcessusFrere;

	public:
		Encavale();
		void InitialiserCouleur(char c);
		void affichage();

		bool jouerProcessus1();
		bool jouerProcessus2();

		vector<coordonnees> deplacementPossible(coordonnees pos); //Prend en parametre une position et retourne un vecteur avec les deplacements possible
		bool placer(coordonnees cord);
		void IntialiserEtoiles(coordonnees cord);
		bool VerifierChoix(vector<coordonnees> cord, coordonnees lamda );

		void EnoyerDonneAuPere();
		void ReceptionDonneesParPere();
		void EnvoyerDonnee(int p[]);
		void RecevoirDonneeAdversaire(int p[]);

		void enleverJeton();
		void enleverJetonHumain();

		void debuterJeux();
		//Consulteur
		int getPointAmi() ;
		int getPointsAdversaire() ;
		int getNbreJeton() ;
		int getNbreJetonAverse();
		int getPidFrere() ;

};
