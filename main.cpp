

#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdlib> // EXIT_SUCCESS
//#include <iomanip> // setprecision
//#include <cmath>

#include "pieces.h"

#define HAUT 0
#define DROITE 1
#define BAS 2
#define GAUCHE 3

using namespace std;

bool operator ==(const Piece& a, const Piece& b)
{
   return(a[0] == b[0]
      and a[1] == b[1]
      and a[2] == b[2]
      and a[3] == b[3])
      or (a[1] == b[0]
      and a[2] == b[1]
      and a[3] == b[2]
      and a[0] == b[3])
      or (a[2] == b[0]
      and a[3] == b[1]
      and a[0] == b[2]
      and a[1] == b[3])
      or (a[3] == b[0]
      and a[0] == b[1]
      and a[1] == b[2]
      and a[2] == b[3]);
}
ostream& operator <<(ostream& os, Piece a)
{
   for(size_t i = 0; i < PIECES.size(); ++i)
   {
      if(PIECES[i] == a)
      {
         for(char j = 'a'; j < 'a' + 4; ++j)
            if(a[j - 'a'] == PIECES[i][0])
               return os << i+1 << j;
      }
   }
   return os;
}

ostream& operator <<(ostream& os, Pieces a)
{
   for(const Piece& i : a)
   {
      os << i << ' ';
   }
   return os;
}


/**
 * Tourne une piece sur elle-même
 */
void rotation(Piece& p)
{
   AttachementType tmp = p.at(0);
   p.at(0) = p.at(3);
   p.at(3) = p.at(2);
   p.at(2) = p.at(1);
   p.at(1) = tmp;
}

/**
 * Enleve toutes les pieces ne contenant pas l'image
 */
void filtrerJeux(Pieces& jeux, AttachementType image)
{
   for(size_t i = jeux.size(); i > 0; --i)
   {
      if(jeux[i-1][0] != image 
            and jeux[i-1][1] != image
            and jeux[i-1][2] != image
            and jeux[i-1][3] != image)
      {
         jeux.erase(jeux.cbegin() + i - 1);
      }
   }

}

/** 
 * Enleve toutes les pieces impossible à placer à une position
 */
void filtrerJeux(Pieces& jeux, int position, const Pieces& plateau)
{

   vector<AttachementType> possibilite = {IMPOSSIBLE, IMPOSSIBLE, IMPOSSIBLE, IMPOSSIBLE};

   switch(position) {
      case 1:
         possibilite[GAUCHE] = plateau.at(0).at(DROITE);
         break;
      case 2:
         possibilite[GAUCHE] = plateau.at(1).at(DROITE);
         break;
      case 3:
         possibilite[HAUT] = plateau.at(0).at(BAS);
         break;
      case 4:
         possibilite[HAUT] = plateau.at(1).at(BAS);
         possibilite[GAUCHE] = plateau.at(3).at(DROITE);
         break;
      case 5:
         possibilite[HAUT] = plateau.at(2).at(BAS);
         possibilite[GAUCHE] = plateau.at(4).at(DROITE);
         break;
      case 6:
         possibilite[HAUT] = plateau.at(3).at(BAS);
         break;
      case 7:
         possibilite[HAUT] = plateau.at(4).at(BAS);
         possibilite[GAUCHE] = plateau.at(6).at(DROITE);
         break;
      case 8:
         possibilite[HAUT] = plateau.at(5).at(BAS);
         possibilite[GAUCHE] = plateau.at(7).at(DROITE);
         break;
   }

   // deux fois la meme image → impossible
   if(possibilite[HAUT] == possibilite[GAUCHE])
   {
      jeux = Pieces();
      return;
   }

   for(AttachementType& i: possibilite)
   {
      if(i == IMPOSSIBLE) continue;

      if(i%2) i = (AttachementType) ((int) i - 1);
      else i = (AttachementType) ((int) i + 1);
   }

   // Enleve les carte n'aillant pas possibilite[HAUT]
   if(possibilite[HAUT] != IMPOSSIBLE)
      filtrerJeux(jeux, possibilite[HAUT]);

   // Enleve les carte n'aillant pas possibilite[GAUCHE]
   if(possibilite[GAUCHE] != IMPOSSIBLE)
      filtrerJeux(jeux, possibilite[GAUCHE]);

   for(size_t i = jeux.size(); i > 0; --i) // pour chaque piece
   {

      if(possibilite[GAUCHE] != IMPOSSIBLE and possibilite[HAUT] != IMPOSSIBLE)
      {
         bool carteOK = false;
         for(int j = 0; j < 4; ++j)
         {
            if(possibilite[GAUCHE] == jeux[i-1][GAUCHE]
               and possibilite[HAUT] == jeux[i-1][HAUT])
            {
               carteOK = true;
               break;
            }
            rotation(jeux[i-1]);
         }
         if(!carteOK) jeux.erase(jeux.cbegin() + i - 1);
      }
      else if(possibilite[GAUCHE] != IMPOSSIBLE)
      {
         while(possibilite[GAUCHE] != jeux[i-1][GAUCHE])
            rotation(jeux[i-1]);
      }
      else if(possibilite[HAUT] != IMPOSSIBLE)
      {
         while(possibilite[HAUT] != jeux[i-1][HAUT])
            rotation(jeux[i-1]);
      }
      else
      {
         cout << "Error " << endl;
      }

      // pour chaque rotation
      //    tester si ça joue
   }



   
}

void resoudreJeuxRec(Pieces jeux, int position, Pieces plateau) {

   if(jeux.empty()) {
      cout << "Réussi " << plateau << endl;
   }

   Pieces jeuxOrig(jeux);

   filtrerJeux(jeux, position, plateau); // avec rotation

   if(jeux.empty()) {
      //cout << "echec @ " << position << " : " << jeuxOrig.size() << endl;
      return;
   }

   plateau.push_back(jeux.front());

   for(size_t i = 0; i < jeux.size(); ++i) {
      plateau.at(position) = jeux[i];

      /// TODO reprendre le jeux reçu et enlever la carte proposée
      Pieces jeuxNouveau(jeuxOrig);
      for(size_t j = 0; j < jeuxNouveau.size(); ++j) {
         if(jeuxNouveau[j] == jeux[i])
         {
            jeuxNouveau.erase(jeuxNouveau.cbegin() + j);
            break;
         }
      }
      //jeuxNouveau.erase(jeuxNouveau.cbegin() + i);
      resoudreJeuxRec(jeuxNouveau, position+1, plateau);
   }



  // cas triviaux
  //     arrivée en pos = 9
  //     un lien est faux
}

int main() {


   for(size_t i = 0; i < PIECES.size(); ++i) {
      Pieces jeux(PIECES);
      Piece piece = PIECES.at(i);
      jeux.erase(jeux.cbegin() + i);


      for(size_t i = 0 ; i < 4; ++i) {
         resoudreJeuxRec(jeux, 1, {piece});
         rotation(piece);
      }

   }
   return EXIT_SUCCESS;



}
