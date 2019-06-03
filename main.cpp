/**
 * @file main.cpp
 * @brief LABO 2: PUZZLE IMPOSSIBLE
 *
 * GROUPE: ASD1_B_A
 * 
 * Résoudre le jeu puzzle impossible
 *
 * @author Gwendoline Dössegger
 * @author Quentin Saucy
 * @author Gabriel Roch
 * @date 2019-03-06 au 2019-03-13
 */

#include <iostream>
#include <algorithm>

#include "pieces.h"

#define HAUT 0
#define DROITE 1
#define BAS 2
#define GAUCHE 3
#define NB_FACE 4
#define NB_COL 3

using namespace std;

/**
 * @brief Afficher une piece (numéro + lettre)
 * @example 4a
 * @param os le flux de sortie
 * @param rhs La piece à afficher
 * @return le flux
 */
ostream& operator <<(ostream& os, const Piece& rhs);

/**
 * @brief Afficher une série de pièces
 * @example 4a 1b 3d 7a …
 * @param os le flux de sortie
 * @param rhs Le vecteur de pieces à afficher
 * @return le flux
 */
ostream& operator <<(ostream& os, const Pieces& rhs);

/**
 * @brief Compare deux pièces
 * @param lhs Une pièce à comparer
 * @param rhs Une deuxième pièce à comparer
 * @retval true si c'est la même carte (à une rotation près)
 * @retval false si ce n'est pas la même carte (à une rotation près)
 */
bool operator ==(const Piece& lhs, const Piece& rhs);

/**
 * @brief Tourne un pièce de 45°
 * @param a La pièce à tourner
 */
void rotation(Piece& a);

/**
 * @brief Supprimer un carte d'un jeu de carte
 * @param jeu Vecteur de pièce à modifier
 * @param piece La pièce à supprimer du jeu
 */
void remove(Pieces& jeu, const Piece& piece);

/**
 * @brief Test si une piece peut rentrer dans la position suivante
 * dans sur le plateau
 * @param plateau Le plateau de jeu
 * @param piece La pièce à insérer
 * @note La piece est tournée pour tester la pièce, la rotation terminal est la
 *    rotation permettant l'insertion, ou la rotation initiale.
 * @retval true Si la pièce peux être insérée
 */
bool testPiece(const Pieces& plateau, Piece& piece);

/**
 * @brief Retourne l'image complémentaire de l'image fournie
 * @param a L'image à inversé
 * @return L'image complémentaire
 */
AttachementType complement(AttachementType a);

/**
 * @brief Résoud le jeux de manière récursive.
 * @param plateau Le plateau de jeu actuelle
 * @param deck Le deck comportant les pièces encore à poser
 */
void resolve(Pieces& plateau, Pieces deck);

int main() {
   Pieces jeux(PIECES);
   Pieces plateau;

   for(Piece piece: PIECES)
   {
      Pieces deck(PIECES);
      remove(deck, piece);
      for(int i = 0; i < NB_FACE; ++i) 
      {
         // Pour chaque orientation de chaque pièce, on la place en 
         // premiere position
         plateau = {piece};
         resolve(plateau, deck);
         rotation(piece);
      }
   }
   return EXIT_SUCCESS;
}

void resolve(Pieces& plateau, Pieces deck)
{
   if(deck.size() == 1) // s'il n'y a plus qu'une pièce
   {
      if(testPiece(plateau, deck.front()))
      {
         plateau.push_back(deck.front());
         cout << plateau << endl;
         plateau.pop_back();
      }
      return;
   }

   for(Piece piece: deck)
   {
      if(testPiece(plateau, piece)) 
      { 
         // si la pièce rentre sur le plateau, elle est
         // tournée correctement par testPiece.

         plateau.push_back(piece);

         Pieces nouveauJeux(deck);
         remove(nouveauJeux, piece);

         resolve(plateau, nouveauJeux);

         plateau.pop_back();
      }
   }
}

bool testPiece(const Pieces& plateau, Piece& piece)
{
   size_t position = plateau.size();
   bool carteOK = false;
   AttachementType rechercher;
   if(position % NB_COL) // pas la premiere colonne
   {
      rechercher = complement(plateau[position-1][DROITE]);
      for(int i = 0; i < NB_FACE; ++i)
      {
         carteOK = rechercher == piece[GAUCHE];
         if(carteOK) break;
         rotation(piece);
      }
      if(!carteOK) return false;
   }
   if(position >= NB_COL) // pas la premiere ligne
   {
      rechercher = complement(plateau[position-3][BAS]);
      if(carteOK) carteOK = rechercher == piece[HAUT];
      else
      {
         for(int i = 0; i < NB_FACE; ++i)
         {
            carteOK = rechercher == piece[HAUT];
            if(carteOK) break;
            rotation(piece);
         }
         if(!carteOK) return false;
      }
   }
   return carteOK;

}

AttachementType complement(AttachementType a)
{
   if(a%2) return (AttachementType) ((int) a - 1);
   else return (AttachementType) ((int) a + 1);
}

void remove(Pieces& jeux, const Piece& piece)
{
   jeux.erase(remove(jeux.begin(), jeux.end(), piece), jeux.cend());
}


void rotation(Piece& a)
{
   AttachementType tmp = a[0];
   a[0] = a[3];
   a[3] = a[2];
   a[2] = a[1];
   a[1] = tmp;
}

bool operator ==(const Piece& lhs, const Piece& rhs)
{
   return (lhs[0] == rhs[0] and lhs[1] == rhs[1] and lhs[2] == rhs[2] and lhs[3] == rhs[3])
       or (lhs[1] == rhs[0] and lhs[2] == rhs[1] and lhs[3] == rhs[2] and lhs[0] == rhs[3])
       or (lhs[2] == rhs[0] and lhs[3] == rhs[1] and lhs[0] == rhs[2] and lhs[1] == rhs[3])
       or (lhs[3] == rhs[0] and lhs[0] == rhs[1] and lhs[1] == rhs[2] and lhs[2] == rhs[3]);
}

ostream& operator <<(ostream& os, const Pieces& rhs)
{
   bool frst = true;
   for(const Piece& i : rhs)
   {
      if(frst) frst = false;
      else os << ' ';
      os << i;
   }
   return os ;
}

ostream& operator <<(ostream& os, const Piece& rhs)
{
   for(size_t i = 0; i < PIECES.size(); ++i)
   {
      if(PIECES[i] == rhs)
      {
         for(size_t j = 0; j < NB_FACE; ++j)
         {
            if(rhs[j] == PIECES[i][0])
               switch(j)
               {
                  case 0: return os << i+1 << 'a';
                  case 1: return os << i+1 << 'd';
                  case 2: return os << i+1 << 'c';
                  case 3: return os << i+1 << 'b';
               }
         }
      }
   }
   return os;
}


