#include <iostream>
#include <string>
#include <vector>

#include <limits.h>
#include <stdio.h>
#include <stdexcept>

#include <ctime>

#include <unistd.h>
#include <signal.h>
#include <cstdlib>
#include <sys/wait.h>
#include <sys/types.h>

using namespace std;

#include "decoration-classes.h"
#include "definition-classes.h"

int main()
{
	char rejouer;
	do
	{
	   int fils1, fils2; //les deux processus fils. fils1 represente l'humain et fils 2 est l'ordinateur
	   //Creation des pipes et verification de la validité
	   int p1 = pipe (pipeProcessus1);
	   int p2 = pipe(pipeProcessus2);
	   int p3 = pipe(pipePere);

	   Encavale J;

	   if(p1==-1)
	   		{
	   			perror("pipe");
	   			exit(10);
	   		}
	   	if(p2==-1)
	   		{
	   			perror("pipe");
	   			exit(10);
	   		}
	   	if(p3==-1)
	   		{
	   			perror("pipe");
	   			exit(10);
	   		}

	   fils1 = fork(); //  On cree le fils 1

	   if (fils1 != 0 && fils1 != -1) //Si le fils1 a ete correctement
	   {
			fils2 = fork(); // On cree un autre fils dans le pere

			if ( fils2 == -1)  // Si le fils deux ne n'est pas cree correctement
				{
					cout << "Erreur creqtion fils 2" <<endl;
					exit(9);
				}
			 if (fils2!= 0) // On est dans le code du pere
			   {
				 close(pipeProcessus1[0]); //Ferme en lecture le tube du fils 1 au pere
				 close(pipeProcessus1[1]); //Ferme en ecriture le tube du fils 1 au pere

				 close (pipeProcessus2[0]); //Ferme en lecture le tube du fils 2 au pere
				 close (pipeProcessus2[1]); //Ferme en ecriture le tube du fils 2 au pere

				 close(pipePere[1]); // Ferme en ecriture le tube du pere


				int a, b;
				int x =	wait(&a); // Attend la fin d'un des fils
				int y = wait(&b); // Attend la fin de l'autre fils

				//Lis les resultat du jeu entre les deux fils
				J.ReceptionDonneesParPere();

				//Affiche l'etat final
				J.affichage();

				// Compile les resultats et l'affiche
				if (y == fils1) //C'est le fils 1 (L'utilisateur) qui a envoyer les donnees au pere
				{
					cout << endl << "Vous avez mis fin a la parti" << endl;
					// si le code de retour est exit(2),
					//Les deux joueurs ont epuisés leur jeton
					if (b == 512)
					{
						cout << endl << "Les jetons sont epuises!!!" << endl;
						cout << "Vous avez accumuler " << J.getPointAmi()<< " points " << endl;
						cout << "L'ordinateur a accumuler " << J.getPointsAdversaire()<< " points " << endl;

						if(J.getPointAmi() > J.getPointsAdversaire())
						{
							cout << "Felicitation vous avez gagnez ☺ ☺ !!!!" << endl;
						}
						else if (J.getPointAmi() < J.getPointsAdversaire())
						{
							cout << "L'ordinatuer vous a gagner. Dur a accepte.Ressayer !!" << endl;
						}
						else
						{
							cout << "Deux Grand Esprit ce sont rencontrer. Consequence... Match null" << endl;
						}
					}
					else if (b == 768) // Avec un code de retour 768 exit (3), l'utilisateur a gagner avec 100 points
					{
						cout << "Felicitation vous avez gagnez avec un cumule de "<< J.getPointAmi()<< " points ☺ ☺ !!!!" << endl;
					}
					else if (b == 1280) // Avec un code de retour 1280 , exit (5), l'utilisateur n'avait plus de deplacement possible
					{
						cout << endl << "Plus de deplacement possible!!!" << endl;
						cout << "Vous avez accumuler " << J.getPointAmi()<< " points " << endl;
						cout << "L'ordinateur a accumuler " << J.getPointsAdversaire()<< " points " << endl;
						if(J.getPointAmi() > J.getPointsAdversaire())
						{
							cout << "Felicitation vous avez gagnez ☺ ☺ !!!!" << endl;
						}
						else if (J.getPointAmi() < J.getPointsAdversaire())
						{
							cout << "L'ordinatuer vous a gagner. Dur a accepte.Ressayer !!" << endl;
						}
						else
						{
							cout << "Deux Grand Esprit ce sont rencontrer. Consequence... Match null" << endl;
						}
					}
				}

				else if (y == fils2)// C'est le fils 2 (L'ordinateur) qui a envoyé les donnees au pere
				{
					cout << endl << "L'ordinateur a mis fin a la partie" << endl;

					// si le code de retour est exit(2),
					//Les deux joueurs ont epuisés leur jeton
					if (b == 512)
					{
						cout << endl << "Les jetons sont epuises!!!" << endl;
						cout << "Vous avez accumuler " << J.getPointsAdversaire()<< " points " << endl;
						cout << "L'ordinateur a accumuler " << J.getPointAmi()<< " points " << endl;

						if(J.getPointAmi() > J.getPointsAdversaire())
						{
							cout << "L'ordinatuer vous a gagner. Dur a accepte.Ressayer !!" << endl;
						}
						else if (J.getPointAmi() < J.getPointsAdversaire())
						{
							cout << "Felicitation vous avez gagnez ☺ ☺ !!!!" << endl;
						}
						else
						{
							cout << "Deux Grand Esprit ce sont rencontrer. Consequence... Match null" << endl;
						}
					}
					else if (b == 768) // Avec un code de retour 768 exit (3), l'ordianteur a gagner avec 100 points
					{
						cout << "L'ordinatuer vous a gagner avec "<< J.getPointAmi()<< " points . Dur a accepte.Ressayer !!!!" << endl;
					}
					else if (b == 1280) // Avec un code de retour 1280 , exit (5), l'ordinateur n'avait plus de deplacement possible
					{
						cout << endl << "Plus de deplacement possible!!!" << endl;
						cout << "Vous avez accumuler " << J.getPointsAdversaire()<< " points " << endl;
						cout << "L'ordinateur a accumuler " <<J.getPointAmi()<< " points " << endl;
						if(J.getPointAmi() > J.getPointsAdversaire())
						{
							cout << "L'ordinatuer vous a gagner. Dur a accepte.Ressayer !!" << endl;
						}
						else if (J.getPointAmi() < J.getPointsAdversaire())
						{
							cout << "Felicitation vous avez gagnez ☺ ☺ !!!!" << endl;
						}
						else
						{
							cout << "Deux Grand Esprit ce sont rencontrer. Consequence... Match null" << endl;
						}
					}
				}
				cout << endl << endl ;
				cout << "Voulez vous jouer a nouveau?" << endl;
				cout << "O pour continuer et N pour sortir : "  ;
				cin >> rejouer;
				}

		}
	   //Arbitrairement Le fils 1 commence le Jeux1. Il va donc poser sa premiere piece sur
	   // le coeur
	   if (fils1 == 0)// On est dans le code du fils 1
	   {
		   close(pipeProcessus1[0]); //Ferme en lecture le tube du fils 1 au fils1
		   close (pipeProcessus2[1]); //Ferme en ecriture le tube du fils 2 au fils 1
		   close(pipePere[0]); // Ferme en lecture le tube du pere au fils 1

		   J.InitialiserCouleur('O'); //le fils1 joue avec '0'
		   J.debuterJeux();	   //Le fils initie le Jeux
		   J.EnvoyerDonnee(pipeProcessus1); // Il envoi ses donnees au fils 2 pour qu'a son tour, il joue

		   while(1)
		   {
			   J.RecevoirDonneeAdversaire(pipeProcessus2);

			   if (J.jouerProcessus1()) //Le fils 1 Joue
			   {  //APRES AVOIR JOUER

				   // Si aucun joueur n'a de jeton on envoie les donnees au pere et on termine
				   if((J.getNbreJeton() == 0) && (J.getNbreJetonAverse() == 0))
				   {

					   J.EnoyerDonneAuPere();
					   kill(J.getPidFrere(),9);// Tue le processus frere avant de se suicider
					   // Envoie donnees et se suicide avec un exit(2)
					   exit(2);
				   }
				   //si la joueur a 100. Il est proclamer vainqueur, il envoi les donnees de
				   //jeux au pere et il fini
				   else if (J.getPointAmi() == 100)
				   {
					    // Envoie donnees et se suicide avec un exit(2)
					   	  J.EnoyerDonneAuPere();
					   	  kill(J.getPidFrere(),9);// Tue le processus frere avant de se suicider
					  	  exit(3);

				   }
				   else // Sinon il envoi les donnees a son frere pour qu'il joue
				   {
					   J.EnvoyerDonnee(pipeProcessus1);
				   }
			   }
			   //IL N'A PAS PU JOUER
			   // Il n'ya plus de mouvement possible
			   //Envoi les donnees au pere et se suicide avec un  exit (5)
			   else
			   {

				   J.EnoyerDonneAuPere();
				   kill(J.getPidFrere(),9);// Tue le processus frere avant de se suicider
				   exit(5);
			   }
		   }
	   }

	    if (fils2 == 0)// On est dans le code du fils 2
	    {
	    	J.InitialiserCouleur('X'); //le fils1 joue avec 'X'

	    	close(pipeProcessus2[0]); //Ferme en lecture le tube du fils 2 au fils2
	    	close (pipeProcessus1[1]); //Ferme en ecriture le tube du fils 1 au fils 2
	    	close(pipePere[0]); // Ferme en lecture le tube du pere au fils 2

	    	while(1)
	    	{
	 		   J.RecevoirDonneeAdversaire(pipeProcessus1);

	 		   if (J.jouerProcessus2()) //Le fils 2 Joue
	 		   {  //APRES AVOIR JOUER

	 			   // Si aucun joueur n'a de jeton on envoie les donnees au pere et on termine
	 			   if((J.getNbreJeton() == 0) && (J.getNbreJetonAverse() == 0))
	 			   {

	 				   // Envoie donnees et se suicide avec un exit(2)
	 				   J.EnoyerDonneAuPere();
	 				   kill(J.getPidFrere(), 9) ;// Tue le processus frere avant de se suicider
	 				   exit(2);
	 			   }
	 			   //si la joueur a 100. Il est proclamer vainqueur, il envoi les donnees de
	 			   //jeux au pere et il fini
	 			   else if (J.getPointAmi() == 100)
	 			   {


	 				   // Envoie donnees et se suicide avec un exit(2)
	 				   	 J.EnoyerDonneAuPere();
	 				   	 kill(J.getPidFrere(),9) ;// Tue le processus frere avant de se suicider
	 				   	 exit(3);

	 			   }
	 			   else // Sinon il envoi les donnees a son frere pour qu'il joue
	 			   {
	 				   J.EnvoyerDonnee(pipeProcessus2);
	 			   }

	 		   }
	 		   //IL N'A PAS PU JOUER
	 		   // Il n'ya plus de mouvement possible
	 		   //Envoi les donnees au pere et se suicide avec un  exit (5)
	 		   else
	 		   {
	 			   J.EnoyerDonneAuPere();
	 			   kill(J.getPidFrere(),9) ;// Tue le processus frere avant de se suicider
	 			   exit(5);
	 		   }

	 	   }

	    }
	}while(rejouer == 'O' || rejouer == 'o' );
  exit(0);
}

