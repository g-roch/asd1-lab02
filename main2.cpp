
#include <iostream>
#include <algorithm>

#include "pieces.h"

#define HAUT 0
#define DROITE 1
#define BAS 2
#define GAUCHE 3
#define DETAILS false

using namespace std;

ostream& operator <<(ostream& os, AttachementType rhs);
ostream& operator <<(ostream& os, const Piece& rhs);
ostream& operator <<(ostream& os, const Pieces& rhs);

bool operator ==(const Piece& lhs, const Piece& rhs);

void rotation(Piece& a);

void resolve(Pieces plateau, Pieces jeux);
void remove(Pieces& jeux, const Piece& piece);

bool testPiece(const Pieces& plateau, Piece& piece);
AttachementType complement(AttachementType a);

int main() {


   Pieces jeux(PIECES);

   for(Piece piece: PIECES)
   {
      for(int i = 0; i < 4; ++i)
      {
         Pieces jeux(PIECES);
         remove(jeux, piece);
         resolve({piece}, jeux);
         rotation(piece);
      }
   }

   return EXIT_SUCCESS;

}

void resolve(Pieces plateau, Pieces deck)
{
   //cout << plateau << endl;
   if(deck.size() == 1)
   {
      if(testPiece(plateau, deck.front()))
      {
         plateau.push_back(deck.front());
         cout << plateau << endl;
      }
      return;
   }

   for(Piece piece: deck)
   {
      if(testPiece(plateau, piece))
      {
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
   if(position % 3) // pas la premiere colonne
   {
      rechercher = complement(plateau[position-1][DROITE]);
      for(int i = 0; i < 4; ++i)
      {
         carteOK = rechercher == piece[GAUCHE];
         if(carteOK) break;
         rotation(piece);
      }
      if(!carteOK) return false;
   }
   if(position > 2)
   {
      rechercher = complement(plateau[position-3][BAS]);
      if(carteOK) carteOK = rechercher == piece[HAUT];
      else
      {
         for(int i = 0; i < 4; ++i)
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

#if DETAILS
   bool frst = true;
   os << '{';
   for(size_t i = 0; i < 4; ++i)
   {
      if(frst) frst = false;
      else os << ' ';
      os << rhs[i];
   }
   os << '}';
#endif
   
   for(size_t i = 0; i < PIECES.size(); ++i)
   {
      if(PIECES[i] == rhs)
      {
         for(size_t j = 0; j < 4; ++j)
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
ostream& operator <<(ostream& os, AttachementType rhs)
{
   switch(rhs) {
      case FILLE_HAUT: return os << "F↑";
      case FILLE_BAS: return os << "F↓";
      case DAME_HAUT: return os << "D↑";
      case DAME_BAS: return os << "D↓";
      case ARROSOIR_GAUCHE: return os << "A←";
      case ARROSOIR_DROIT: return os << "A→";
      case GATEAU_GAUCHE: return os << "G←";
      case GATEAU_DROIT: return os << "G→";
      case ARROSOIR_INVERSE: return os << "Ai";
      case NONE: return os << "Nø";
   }
   return os << "¿?";
}
