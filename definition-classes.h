
int Encavale::getPointAmi()
{
	return PointsAmi;
}
int Encavale::getPointsAdversaire()
{
	return PointsAdversaire;
}
int Encavale::getNbreJeton()
{
	return NbreJeton;
}
int Encavale::getNbreJetonAverse()
{
	return NbreJetonAdvers;
}
int Encavale::getPidFrere()
{
	return PidProcessusFrere;
}

Encavale::Encavale()
{
	srand(time(NULL));

	NbreJeton = 9; //On initialise le nombre de jeton a 9 pour chaque jour donc un totale de 18

	// On initialise la case de depart
	depart.x = 5; //depart représente le ponit de depart. Le coeur en quelque sorte
	depart.y = 0;

	//Ici On initialise les etoiles ( les cases qui contiendront les etoiles) selon la configuration donnée dans l'exemple de jeux
	coordonnees etoile1(0,0); coordonnees etoile2(0,2); 	coordonnees etoile3(0,5); 	coordonnees etoile4(2,0); coordonnees etoile5(2,2);
	coordonnees etoile6(2,5); coordonnees etoile7(4,1); 	coordonnees etoile8(4,4); 	coordonnees etoile9(5,5);
	IntialiserEtoiles(etoile1); IntialiserEtoiles(etoile2); IntialiserEtoiles(etoile3);
	IntialiserEtoiles(etoile4); IntialiserEtoiles(etoile5); IntialiserEtoiles(etoile6);
	IntialiserEtoiles(etoile7); IntialiserEtoiles(etoile8); IntialiserEtoiles(etoile9);

    //Au depart chaque joueur a 0 points
	PointsAmi = PointsAdversaire = 0;

	cout << "Bienvenue au Jeux en Cavale inter-processus. Vous allez representer un des processus :)" << endl;
	cout << "Vous jouez en quelque sorte donc contre l'ordinateur!! Bonne Chance!! :)" << endl << endl;

	affichage();

	cout << "Le Jeux va commencer dans 5 seconde!" << endl;
	sleep(3);
	system("clear");


}

void Encavale::IntialiserEtoiles(coordonnees pos)
{
	table[pos.x][pos.y].etoile = true;

}

void Encavale::InitialiserCouleur(char c)
{
	CouleurJeton = c;
	monPid = getpid(); //A l'initialisation on sauvegarde son pid
}

void Encavale::debuterJeux()
{
	//Au debut du jeu, le premier joueur pose automatiquement son jeton sur le coueur
		table[depart.x][depart.y].occuper = true;
		table[depart.x][depart.y].jet.couleur = 'O';
		NbreJeton--; // On decremente le nombre de ses jetons

		lastInsertion.x=depart.x;
		lastInsertion.y=depart.y;
}

//Communique les donnees importantes du jeux a l'adversaire par le tube
void Encavale::EnvoyerDonnee(int p[])
{
	write(p[1], &PointsAmi, sizeof(int)); // Envoi son cumule de point a l'adversaire
	write(p[1], (coordonnees *)&lastInsertion, sizeof(lastInsertion)); // Envoi les coordonnees du dernier jeton inserer
	write(p[1], &NbreJeton, sizeof(int)); // Envoi le nombre de Jeton restant;
	write(p[1], (caze*)table, 36*sizeof(caze)); // Envoi l'etat de sa grille de jeux a l'adversaire
	write(p[1], &monPid, sizeof(int)); // Envoi son pid au processus frere

}

// lis les donnees envoyer par l'adversaire
void Encavale::RecevoirDonneeAdversaire(int p[])
{
	read(p[0], &PointsAdversaire, sizeof(int)); // lit les point de l'adversaire
	read(p[0], (coordonnees *)&lastInsertion, sizeof(lastInsertion));
	read(p[0], &NbreJetonAdvers, sizeof(int));
	read(p[0], (caze*)&table, 36*sizeof(caze));
	read(p[0], &PidProcessusFrere, sizeof(int)); //Lis le pid du processus frere

}
void Encavale::EnoyerDonneAuPere()
{
	write(pipePere[1], &PointsAmi, sizeof(int)); // Envoi son cumule de point au père
	write(pipePere[1], &PointsAdversaire, sizeof(int)); // Envoi le cumule de point de l'adversaire au pere

	write(pipePere[1], &NbreJeton, sizeof(int)); // Envoi le nombre de Jeton restant au pere;
	write(pipePere[1], &NbreJetonAdvers, sizeof(int)); // Envoi le nombre de Jeton restant de l'adversaire au pere;

	write(pipePere[1], (caze*)&table, 36*sizeof(caze)); // Envoi l'etat actuelle de la grille de jeux au pere;

	write(pipePere[1], (coordonnees *)&lastInsertion, sizeof(lastInsertion));
}

//Lis les donnees du finales du jeux
void Encavale::ReceptionDonneesParPere()
{
	read(pipePere[0], &PointsAmi, sizeof(int));
	read(pipePere[0], &PointsAdversaire, sizeof(int));

	read(pipePere[0], &NbreJeton, sizeof(int));
	read(pipePere[0], &NbreJetonAdvers, sizeof(int));

	read(pipePere[0], (caze*)table, 36*sizeof(caze));

	read(pipePere[0], (coordonnees *)&lastInsertion, sizeof(lastInsertion));
}

//Lorsqu'un pose son jeton sur une etoile, on appel cette fonction pour enleve un jeton
//Cette fonction parcour la grille et enleve aleatoirement un jeton de sa couleur
//si celui ci n'est pas sur une etoile

void  Encavale::enleverJeton()
{
	vector <coordonnees> EmplacementJeton;
	coordonnees temp;
	//on parcours la grille pour trouver les emplacements qui contiennent les jetons du joueur
	//Et qui ne sont pas sur une etoile
	for (int i = 0 ; i < 6 ; i++)
		{for (int j = 0; j < 6 ; j++ )
			{ if (table[i][j].occuper == true
					&& table[i][j].etoile != true
					&& table[i][j].jet.couleur == CouleurJeton )
				{
					coordonnees temp;
					temp.x = i;
					temp.y = j;
					EmplacementJeton.push_back(temp);
				}
			}
		}
	int position = rand() % EmplacementJeton.size();
	temp.x = EmplacementJeton[position].x;
	temp.y = EmplacementJeton[position].y;
// On enleve le jeton

	table[temp.x][temp.y].jet.couleur = ' ';
	table[temp.x][temp.y].occuper = false;

// On incremente le nombre de jeton du joueur
	NbreJeton++;
}

//Lorsqu'un pose son jeton sur une etoile, on appel cette fonction pour enleve un jeton
//Cette fonction parcour la grille et propose a l'utilisateur de choisir un jeton a enlever parmi
//les choix pr

void  Encavale::enleverJetonHumain()
{
	coordonnees temp, temp1;
	int position;
	vector <coordonnees> EmplacementJeton;
	//on parcours la grille pour trouver les emplacements qui contiennent les jetons du joueur
	//Et qui ne sont pas sur une etoile
	for (int i = 0 ; i < 6 ; i++)
		{for (int j = 0; j < 6 ; j++ )
			{ if (table[i][j].occuper == true
					&& table[i][j].etoile != true
					&& table[i][j].jet.couleur == CouleurJeton )
				{
					coordonnees temp;
					temp.x = i;
					temp.y = j;
					EmplacementJeton.push_back(temp);
				}
			}
		}
	cout << endl << "!!!!SWEET!!!!" << endl;
	cout <<"Vous venez de poser votre jeton sur une etoile" << endl;
	cout <<"Enlever un jeton parmi la liste des choix possible : ";
	for (size_t i = 0; i< EmplacementJeton.size(); i++)
	{
		cout << " " << EmplacementJeton[i].x*6 + EmplacementJeton[i].y +1;
	}

	bool first_time = true;
	//Boucle tant que le choix d'enlevement n'est pas correcte
	do
	{
		if (first_time != true)
			{
				cout << "Position incorrect, veuillez reessayer choisir une position parmi la liste des choiz possibles!!! " << endl;
			}

		cout << endl << "Entrez la position a la quelle vous voulez enlever un jeton : ";
		cin >> position;
		temp1.x = (position-1)/6;
		temp1.y = (position-1)%6;
		first_time = false;
	}while(!VerifierChoix(EmplacementJeton,temp1));

// On enleve le jeton

	table[temp1.x][temp1.y].jet.couleur = ' ';
	table[temp1.x][temp1.y].occuper = false;

// On incremente le nombre de jeton du joueur
	NbreJeton++;
}

bool Encavale::jouerProcessus1()
{
	int choix;
	coordonnees temp;
	vector<coordonnees> deplacement;

	//Ici, on verifie si l'adversaire n'a plus de jeton
	//alors on joue tout nos jeton en possession
	do
	{

		bool first_time = true;

		do
		{
			affichage();
			//genere les deplacements possible
			deplacement = deplacementPossible(lastInsertion);
			if (first_time != true)
			{
				cout << "Position incorrect, veuillez choisir une position parmi la liste des deplacements possibles!!! " << endl;
			}

			cout << "Voici les deplacements possibles : " ;

			//Affiche la liste des deplacments possibles
			for(vector<coordonnees>::iterator it = deplacement.begin(); it!=deplacement.end(); ++it)
			{
				cout << (it->y +1) + 6*(it->x) << " " ;
			}

			cout << endl << endl << "Entrez la position a la quelle vous voulez vous deplacer : " ;
			cin >> choix;

			temp.x = (choix-1)/6;
			temp.y = (choix-1)%6;
			first_time = false ;
		 }	while(!VerifierChoix(deplacement,temp));

		table[temp.x][temp.y].occuper = true;
		table[temp.x][temp.y].jet.couleur = CouleurJeton;
		lastInsertion = temp;
		// On decremente le nombre de ses jetons
		NbreJeton--;

		//AFFECTATION DE POINTS
		 //genere les deplacements possible
			deplacement = deplacementPossible(lastInsertion);

			if (deplacement.size() <= 0 && table[temp.x][temp.y].etoile)
			{
				PointsAmi+=30;
				affichage();
				return false;
			}

			//Premiere condition de sortie, on p plus se deplacer. On return avec un false
			else if (deplacement.size() <= 0)
			{
				PointsAmi+=20;
				affichage();
				return false;
			}
		//si il a toujours des jetons , il pose la piece courante sur une etoile
		//il gange 10 pointset enleve un jeton
			else if (table[temp.x][temp.y].etoile)
			{
				PointsAmi+=10;
				enleverJetonHumain();
			}

	}while (NbreJetonAdvers == 0 && NbreJeton > 0);

	//AFFECTATION DE POINTS

	// Si la case est une etoile, et l'adversaire n'a plus de jeton
	//et si cest le dernier jeton du joueur Ami, il gagne 30 points en enleve

	if (NbreJetonAdvers == 0 && NbreJeton == 0 && table[temp.x][temp.y].etoile)
	{
		PointsAmi+=30;
		return true;
	}
	//si c'est le dernier jeton et l'adversaire n'a plus de piece
	//mais le dernier jeton n'est pas sur une etoile, il gagne 10 points
	else if(NbreJetonAdvers == 0 && NbreJeton == 0)
	{
		PointsAmi+=20;
		return true;
	}
	return true;

}
// PROCESSUS 2 Represente l'ordinateur
//Donc joue aleatoirement selon les choix possible

bool Encavale::jouerProcessus2()
{

	coordonnees temp;
	vector<coordonnees> deplacement;
	int choix;

	do
	{
		affichage();
		//genere les deplacements possible
		deplacement = deplacementPossible(lastInsertion);

		cout <<  "Ordinateur joue........." << endl;
		sleep(2);

		//Si deplacement possible, on choisi aleatoirement un deplacement
		int choix = rand()% deplacement.size();
		temp.x = deplacement[choix].x;
		temp.y = deplacement[choix].y;
		table[temp.x][temp.y].occuper = true;
		table[temp.x][temp.y].jet.couleur = CouleurJeton;
		lastInsertion = deplacement[choix];
		// On decremente le nombre de ses jetons
		 NbreJeton--;



		//AFFECTATION DE POINTS
		 //genere les deplacements possible
		deplacement = deplacementPossible(lastInsertion);

		if (deplacement.size() <= 0 && table[temp.x][temp.y].etoile)
		{
			PointsAmi+=30;
			affichage();
			return false;
		}
		//Premiere condition de sortie, on p plus se deplacer. On return avec un false
		else if (deplacement.size() <= 0)
		{
			PointsAmi+=20;
			affichage();
			return false;
		}
		//si il a toujours des jetons , il pose la piece courante sur une etoile
		//il gange 10 pointset enleve un jeton
		else if (table[temp.x][temp.y].etoile)
		{
			PointsAmi+=10;
			enleverJeton();
		}

	}while (NbreJetonAdvers == 0 && NbreJeton > 0);

	//AFFECTATION DE POINTS

		// Si la case est une etoile, et l'adversaire n'a plus de jeton
		//et si cest le dernier jeton du joueur Ami, il gagne 30 points en enleve

	if (NbreJetonAdvers == 0 && NbreJeton == 0 && table[temp.x][temp.y].etoile)
	{
		PointsAmi+=30;
		return true;
	}
	//si c'est le dernier jeton et l'adversaire n'a plus de piece
	//mais le dernier jeton n'est pas sur une etoile, il gagne 10 points
	else if(NbreJetonAdvers == 0 && NbreJeton == 0)
	{
		PointsAmi+=20;
		return true;
	}

	return true;
}

vector<coordonnees> Encavale::deplacementPossible(coordonnees pos)
{
	int abcis[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };// vertical[i]  horizontal[i] Represente toutes les coordonnes pour effectuer
	int ordoner[8] = { -1, -2, -2, -1, 1, 2, 2, 1 };  // un deplacement en L a a partir d'un point

	vector<coordonnees> L; // Le vecteur de coordonnes qui sera retourné par rapport a la positon donnée
	coordonnees temp;
	for (int i = 0; i<8; i++)
	{   // Si la position est valide (est a linterieur de la table et la position n'est pas occupé
		//On met la position dans le vecteur de position
		if ((pos.x + abcis[i] < 6) && (pos.x + abcis[i] >= 0) && (pos.y + ordoner[i] < 6) && (pos.y + ordoner[i] >= 0)
			&& table[pos.x + abcis[i]][pos.y + ordoner[i]].occuper == false)
		{
			temp.x = pos.x + abcis[i];
			temp.y = pos.y + ordoner[i];
			L.push_back(temp);
		}
	}

	return L;
}

// Affiche l'etat du jeux et a droite un guide de Jeux
void Encavale::affichage()
{
   system("clear");
   cout << "\t\t\t\t\t\t Guide de Jeux Numero des positions :\n\n"
		<< " ███████████████████████████████████" << "\t ███████████████████████████████████" << endl
		<< "|  *  |     |  *  |     |     |  *  |" << "\t|  *  |     |  *  |     |     |  *  |" << endl
		<< "|  " << table[0][0].jet.couleur << "  |  " << table[0][1].jet.couleur << "  |  " << table[0][2].jet.couleur<< "  |  " << table[0][3].jet.couleur<< "  |  " << table[0][4].jet.couleur<< "  |  " << table[0][5].jet.couleur<< "  |  \t|  " << "1 " << " |  " << "2 " << " |  " << "3 "<< " |  " << "4 "<< " |  " <<"5 "<< " |  " <<"6 "<< " |  "<< endl
		<< " ███████████████████████████████████" << "\t ███████████████████████████████████" << endl
		<< "|     |     |     |     |     |     |" << "\t|     |     |     |     |     |     |" << endl
		<< "|  " << table[1][0].jet.couleur << "  |  " << table[1][1].jet.couleur << "  |  " << table[1][2].jet.couleur<< "  |  " << table[1][3].jet.couleur<< "  |  " << table[1][4].jet.couleur<< "  |  " << table[1][5].jet.couleur<< "  |  \t|  " << "7 " << " |  " << "8 " << " |  " << "9 "<< " |  " << "10"<< " |  " <<"11"<< " |  " <<"12"<< " |  "<< endl
		<< " ███████████████████████████████████" << "\t ███████████████████████████████████" << endl
		<< "|  *  |     |  *  |     |     |  *  |" << "\t|  *  |     |  *  |     |     |  *  |" << endl
		<< "|  " << table[2][0].jet.couleur << "  |  " << table[2][1].jet.couleur << "  |  " << table[2][2].jet.couleur<< "  |  " << table[2][3].jet.couleur<< "  |  " << table[2][4].jet.couleur<< "  |  " << table[2][5].jet.couleur<< "  |  \t|  " << "13" << " |  " << "14" << " |  " << "15"<< " |  " << "16"<< " |  " <<"17"<< " |  " <<"18"<< " |  "<< endl
		<< " ███████████████████████████████████" << "\t ███████████████████████████████████" << endl
		<< "|     |     |     |     |     |     |" << "\t|     |     |     |     |     |     |" << endl
		<< "|  " << table[3][0].jet.couleur << "  |  " << table[3][1].jet.couleur << "  |  " << table[3][2].jet.couleur<< "  |  " << table[3][3].jet.couleur<< "  |  " << table[3][4].jet.couleur<< "  |  " << table[3][5].jet.couleur<< "  |  \t|  " << "19" << " |  " << "20" << " |  " << "21"<< " |  " << "22"<< " |  " <<"23"<< " |  " <<"24"<< " |  "<< endl
		<< " ███████████████████████████████████" << "\t ███████████████████████████████████" << endl
		<< "|     |  *  |     |     |  *  |     |" << "\t|     |  *  |     |     |  *  |     |" << endl
		<< "|  " << table[4][0].jet.couleur << "  |  " << table[4][1].jet.couleur << "  |  " << table[4][2].jet.couleur<< "  |  " << table[4][3].jet.couleur<< "  |  " << table[4][4].jet.couleur<< "  |  " << table[4][5].jet.couleur<< "  |  \t|  " << "25" << " |  " << "26" << " |  " << "27"<< " |  " << "28"<< " |  " <<"29"<< " |  " <<"30"<< " |  "<< endl
		<< " ███████████████████████████████████" << "\t ███████████████████████████████████" << endl
		<< "|  ♡  |     |     |     |     |  *  |" << "\t|     |     |     |     |     |  *  |" << endl
		<< "|  " << table[5][0].jet.couleur << "  |  " << table[5][1].jet.couleur << "  |  " << table[5][2].jet.couleur<< "  |  " << table[5][3].jet.couleur<< "  |  " << table[5][4].jet.couleur<< "  |  " << table[5][5].jet.couleur<< "  |  \t|  " << "31" << " |  " << "32" << " |  " << "33"<< " |  " << "34"<< " |  " <<"35"<< " |  " <<"36"<< " |  "<< endl
		<< " ███████████████████████████████████" << "\t ███████████████████████████████████" << endl;

   cout << endl << "\t\t\t\t\t Points du joueur Courant : " << PointsAmi << endl;
   cout << "\t\t\t\t\t Points de l'adversaire : " << PointsAdversaire << endl;
   cout << "\t\t\t\t\t Nombre jeton du joueur courant : " << NbreJeton << endl;
   cout << "\t\t\t\t\t Nombre jeton du joueur de l'adversaire  : " << NbreJetonAdvers << endl;
   cout << "\t\t\t\t\t Position d'insertion du dernier jeton  : " << (lastInsertion.x*6)+(lastInsertion.y +1) << endl;
}


bool Encavale::VerifierChoix(vector<coordonnees> cord, coordonnees lamda )
{
	for (vector<coordonnees>::iterator it = cord.begin(); it!=cord.end(); ++it)
	{
		if (lamda.x == it->x && lamda.y == it->y)
		{
			return true;
		}
	}
	return false;
}