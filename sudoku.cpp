#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "sudoku.h"

using namespace std;

Cellule::Cellule()
{
    this->m_listeNum.resize(9,true);
    this->m_valeur=0;
    this->m_taille=9;
}

Cellule::Cellule(int num)
{
    //vector<bool> vec(9,false);
    this->m_listeNum.resize(9,false);
    this->m_valeur=num;
    this->m_taille=1;
    this->m_listeNum[num-1]=true;
}

Cellule::Cellule(Cellule const& cellule,Carte* carte){
    this->m_listeNum=cellule.m_listeNum;
    this->m_valeur = cellule.m_valeur;
    this->m_taille = cellule.m_taille;
    this->m_carte = carte;
    this->m_x=cellule.m_x;
    this->m_y=cellule.m_y;
}

bool Cellule::setOff(int num) //enleve num; si taille =1, appeler setValeur. Si possibles =0 return false
{
    //cout<<"celO";
    if (this->m_listeNum[num-1])
    {
        this->m_listeNum[num-1]=false;
        this->m_carte->m_modifie=true;
        --(this->m_taille);
        if (this->m_taille == 0) return false;
        if (this->m_taille==1)
            for(int i(0); i<9; ++i)
                if(this->m_listeNum[i]==true)
                {
                    this->m_valeur=i+1;
                    if (!this->m_carte->setValeur(this->m_x,this->m_y,i+1))
                        return false;//appelle aussi cellule.setValeur
                    break;
                }
    }
//    cout<<num;
    return true;
}

bool Cellule::setValeur(int num)
{
    if (num==0)
        return true;
    if ((this->m_valeur!=0) && (this->m_valeur != num))
        return false;//si �a change

    if (this->m_taille > 1){
        this->m_carte->m_modifie = true;
        for(int i(0); i<9; ++i)
            this->m_listeNum[i]=false;
        this->m_listeNum[num-1]=true;
        this->m_taille = 1;
        this->m_valeur = num;
    }
    else if (this->m_taille==1){
        this->m_valeur = num;
        this->m_carte->m_modifie = true;
    }
    if (this->m_taille ==0)
        return false;

    return true;
}


Ligne::Ligne()
{}

Colonne::Colonne()
{}

Carre::Carre()
{}

Ligne::Ligne(int j, Carte* carte)  // cree la ligne
{
    this->m_carte=carte;
    this->m_cellules.resize(9,NULL);

    for(int i(0); i<9; ++i)
        this->m_cellules[i]=& (carte->m_liste_cellules[i][j]);
    this->m_y=j;

//    vector<vector<Cellule*>> vec2(9,vector<Cellule*>(0));
    this->m_liste_num.reserve(81 * sizeof(Cellule*));
    this->m_liste_num.resize(9,vector<Cellule*>()); //remplit le tableau

   // cout<<"allocation ligne"<<endl;
    for(int num(0); num<9; ++num)
        for(int i(0); i<9; ++i)
            if (this->m_cellules[i]->m_listeNum[num]==true)
                this->m_liste_num[num].push_back(this->m_cellules[i]);
}

Ligne::Ligne(Ligne const& ligne, Carte* carte){
    this->m_y =ligne.m_y;
    this->m_carte=carte;
    this->m_cellules.resize(9,NULL);
    for(int i(0);i<9;++i)
        this->m_cellules[i]=& (carte->m_liste_cellules[ligne.m_cellules[i]->m_x][ligne.m_cellules[i]->m_y]);
    this->m_liste_num.reserve(81 * sizeof(Cellule*));
    this->m_liste_num.resize(9,vector<Cellule*>()); //remplit le tableau
    for(int num(0); num<9; ++num)
        for(int k(0);k<ligne.m_liste_num[num].size();++k)
            this->m_liste_num[num].push_back(&( carte->m_liste_cellules[ligne.m_liste_num[num][k]->m_x][ligne.m_liste_num[num][k]->m_y]));
    return;
}


bool Ligne::setOff(int x,int val)
{
    //cout<<"ligO";
    for(int i(0); i<this->m_liste_num[val-1].size(); ++i)
        if (this->m_liste_num[val-1][i]->m_x==x)
        {
            this->m_liste_num[val-1].erase(this->m_liste_num[val-1].begin()+i);
            if (m_liste_num[val-1].size()==1)
                if (!(this->m_carte->setValeur(this->m_liste_num[val-1][0]->m_x,this->m_y,val)))
                    return false;
            if (this->m_liste_num[val-1].size()==0)
                return false;
            break;
        }
    return true;
}

bool Ligne::setValeur(int x,int val)
{
    //cout<<"ligV";
    for(int i(0); i<9; ++i)
        if(x!=i)
        {
            if(this->m_cellules[i]->m_listeNum[val-1]){
                if (!(this->m_cellules[i]->setOff(val)))
                    return false;
                if (!(this->m_carte->m_liste_colonnes[i].setOff(this->m_y,val)))
                    return false;
                if (!(this->m_carte->m_liste_carres[i/3 +3*(this->m_y/3)].setOff(i%3+(this->m_y%3)*3,val)))
                    return false;;
            }
        }
    this->m_liste_num[val-1].erase(this->m_liste_num[val-1].begin(),this->m_liste_num[val-1].end());
    this->m_liste_num[val-1].push_back(&(this->m_carte->m_liste_cellules[x][this->m_y]));
    return true;
}


Colonne::Colonne(int i, Carte* carte)
{
    this->m_carte=carte;
    this->m_cellules.resize(9,NULL);

    for(int j(0); j<9; ++j)
        this->m_cellules[j]=& (this->m_carte->m_liste_cellules[i][j]);
    this->m_x=i;

//    vector<vector<Cellule*>> vec2(9,vector<Cellule*>(0));
    this->m_liste_num.reserve(81 * sizeof(Cellule*));
    this->m_liste_num.resize(9,vector<Cellule*>());

    for(int num(0); num<9; ++num)
        for(int j(0); j<9; ++j)
            if (this->m_cellules[j]->m_listeNum[num]==true)
                this->m_liste_num[num].push_back(this->m_cellules[j]);
    return;
}

Colonne::Colonne(Colonne const& colonne, Carte* carte){
    this->m_x =colonne.m_x;
    this->m_carte=carte;
    this->m_cellules.resize(9,NULL);
    for(int j(0);j<9;++j)
        this->m_cellules[j]=& carte->m_liste_cellules[colonne.m_cellules[j]->m_x][colonne.m_cellules[j]->m_y];

    this->m_liste_num.reserve(81*sizeof(Cellule*));
    this->m_liste_num.resize(9,vector<Cellule*>()); //remplit le tableau
    for(int num(0); num<9; ++num)
        for(int k(0);k<colonne.m_liste_num[num].size();++k)
            this->m_liste_num[num].push_back(& (carte->m_liste_cellules[colonne.m_liste_num[num][k]->m_x][colonne.m_liste_num[num][k]->m_y]));
}

bool Colonne::setOff(int y,int val)
{
    //cout<<"colO";
    for(int j(0); j<this->m_liste_num[val-1].size(); ++j)
        if (this->m_liste_num[val-1][j]->m_y==y)
        {
            this->m_liste_num[val-1].erase(this->m_liste_num[val-1].begin()+j);
            if (this->m_liste_num[val-1].size()==1)
                if (!(this->m_carte->setValeur(this->m_x,this->m_liste_num[val-1][0]->m_y,val)))
                    return false;
            if (this->m_liste_num[val-1].size()==0)
                return false;
            break;
        }
    return true;
}

bool Colonne::setValeur(int y,int val)
{
    //cout<<"colV";
    for(int j(0); j<9; ++j)
        if(y!=j)
        {
            if(this->m_cellules[j]->m_listeNum[val-1]){
                if (!(this->m_cellules[j]->setOff(val)))
                    return false;
                if (!(this->m_carte->m_liste_lignes[j].setOff(this->m_x,val)))
                    return false;
                if (!(this->m_carte->m_liste_carres[this->m_x/3+3*(j/3)].setOff((this->m_x%3)+(j%3)*3,val)))
                    return false;
            }
        }
    this->m_liste_num[val-1].erase(this->m_liste_num[val-1].begin(),this->m_liste_num[val-1].end());
    this->m_liste_num[val-1].push_back(&(this->m_carte->m_liste_cellules[this->m_x][y]));
    return true;
}


Carre::Carre(int c,Carte* carte)
{
    this->m_carte= carte;
    this->m_c =c;
    int m_i=this->m_c%3; //m_c=m_i+3*m_j
    int m_j=this->m_c/3;

//    vector<Cellule*> vec(9,NULL);
    this->m_cellules.resize(9,NULL);

    for(int i(0); i<3; ++i)
        for(int j(0); j<3; ++j)
            this->m_cellules[i+3*j] = & (this->m_carte->m_liste_cellules[3*m_i+i][3*m_j+j]);

    this->m_liste_num.reserve(81*sizeof(Cellule*));
    this->m_liste_num.resize(9,vector<Cellule*>(0));

    for(int i(0); i<3; ++i)
        for(int j(0); j<3; ++j)
            for(int num(0); num<9; ++num)
                if (this->m_cellules[i+3*j]->m_listeNum[num]==true)
                    this->m_liste_num[num].push_back(this->m_cellules[i+3*j]);

}

Carre::Carre(Carre const& carre, Carte* carte){
    this->m_c =carre.m_c;
    this->m_carte=carte;
    this->m_cellules.resize(9,NULL);
    for(int k(0);k<9;++k)
        this->m_cellules[k]=& (this->m_carte->m_liste_cellules[carre.m_cellules[k]->m_x][carre.m_cellules[k]->m_y]);

    this->m_liste_num.reserve(81*sizeof(Cellule*));
    this->m_liste_num.resize(9,vector<Cellule*>()); //remplit le tableau
    for(int num(0); num<9; ++num)
        for(int iter(0);iter<carre.m_liste_num[num].size();++iter)
            this->m_liste_num[num].push_back(& carte->m_liste_cellules[carre.m_liste_num[num][iter]->m_x][carre.m_liste_num[num][iter]->m_y]);
    return;
}

bool Carre::setOff(int k,int val)
{
    //cout<<"carO";
    for(int j(0); j<this->m_liste_num[val-1].size(); ++j)
        if (((this->m_liste_num[val-1][j]->m_x % 3) + 3 * (this->m_liste_num[val-1][j]->m_y%3))==k)
        {
            this->m_liste_num[val-1].erase(this->m_liste_num[val-1].begin()+j);
            if (this->m_liste_num[val-1].size()==1)//
                if (!(m_carte->setValeur(this->m_liste_num[val-1][0]->m_x,this->m_liste_num[val-1][0]->m_y,val)))
                    return false;
            if (this->m_liste_num[val-1].size()==0)
                return false;
            break;
        };

    return true;
}

bool Carre::setValeur(int k,int val)
{
    //cout<<"carV";
    for(int j(0); j<9; ++j)
        if(k!=j)
        {
            if(this->m_cellules[j]->m_listeNum[val-1]){
                if (!(this->m_cellules[j]->setOff(val)))
                    return false;
                if (!(this->m_carte->m_liste_lignes[(j/3) + 3* (this->m_c/3)].setOff(j%3+(this->m_c%3) *3,val)))
                    return false;
                if (!(this->m_carte->m_liste_colonnes[j%3+(this->m_c%3) *3].setOff((j/3) + 3* (this->m_c/3),val)))
                    return false;
            }
        }
    this->m_liste_num[val-1].erase(this->m_liste_num[val-1].begin(),this->m_liste_num[val-1].end());
    this->m_liste_num[val-1].push_back(&(this->m_carte->m_liste_cellules[(k%3) + 3*(this->m_c % 3)][(k/3) + 3*(this->m_c/3)]));
    return true;
}

Carte::Carte(string string_sudoku)
{
    this->m_liste_cellules.resize(9,vector<Cellule>(9));
    //cout<<"ok"<<endl;
    for(int i(0); i<9; ++i)
        for(int j(0); j<9; ++j)
        {
            //this->m_cellules[i][j];
            this->m_liste_cellules[i][j]=Cellule();
            this->m_liste_cellules[i][j].m_x=i;
            this->m_liste_cellules[i][j].m_y=j;
            this->m_liste_cellules[i][j].m_carte=this;
        }
    //cout<<"allocation tab"<<endl;
    this->m_liste_lignes.resize(9);
    for(int i(0); i<9;++i)
        this->m_liste_lignes[i]=Ligne(i,this);
    //cout<<"allocation ligne"<<endl;

    this->m_liste_colonnes.resize(9);
    for(int i(0); i<9;++i)
        this->m_liste_colonnes[i]=Colonne(i,this);

    //cout<<"allocation colonne"<<endl;



    this->m_liste_carres.resize(9);
    for(int i(0); i<9;++i)
        this->m_liste_carres[i]=Carre(i,this);

    //cout<<"allocation carr�s"<<endl;


    int i(0),j(0);
    int iter(0);
    this->m_erreur_carte=true;
    do
    {
        char a=string_sudoku[iter];
        //cout<<a;
        //cout<<i<<","<<j<<" ; ";
        int numero= ((int) a)- 48;
        //cout<<numero<<endl;
        if (numero ==0)
            ++i;
        if ((1<=numero) && (numero<=9))
        {
            if((i>8)||(j>8)){
                cout<<"mauvais fichier : ne respecte pas la norme"<<endl;
                this->m_erreur_carte=false;
                break;
            }
            if (! (this->setValeur(i,j,numero))){
                this->m_erreur_carte=false;
                cout<<"erreur carte !"<<endl;
                break;
            }
            ++i;
        }
        if (a=='\n')
        {
            i=0;
            ++j;
            //cout<<"saut"<<endl;
        }
        if(i>10)
            j=9;
        ++iter;
    } while((j<=8));

    if (!this->m_erreur_carte)
    {
        cout<<"pas de lancement du jeu ..."<<endl;
        cout<<*this;
        return;
    }

    if (!this->faireCalculer()){
        this->m_erreur_carte = false;
        cout<<"erreur carte !"<<endl;
    }

    cout<<"lecture fichier ok"<<endl;

    return;

}

bool Carte::setOff(int x,int y,int val)
{
    if (!(this->m_liste_cellules[x][y].setOff(val)))
        return false;
    if (!(this->m_liste_lignes[y].setOff(x,val)))
        return false;
    if (!(this->m_liste_colonnes[x].setOff(y,val)))
        return false;
    if (!(this->m_liste_carres[(x/3) + 3* (y/3)].setOff((x%3) + 3*(y%3),val)))
        return false;

    //cout<<val;
    return true;
}

bool Carte::setValeur(int x,int y,int val)
{
    //cout<<x<<","<<y<<endl;
    if (!(this->m_liste_cellules[x][y].setValeur(val)))
        return false;
    if (!(this->m_liste_lignes[y].setValeur(x,val)))
        return false;
    if (!(this->m_liste_colonnes[x].setValeur(y,val)))
        return false;
    if (!(this->m_liste_carres[(x/3) + 3* (y/3)].setValeur((x%3) + 3*(y%3),val)))
        return false;

    //cout<<val;
    //cout<<x<<","<<y<<endl;
    return true;
}

//Ca c'est a priori OK : test� ...
bool Carte::calculerSi()
{
    for(int valeur=1; valeur<=9; ++valeur)    //pour chacune des valeurs ...
    {
        for(int c(0); c<9; ++c)                   //on rentre dans le carre :c%3,c/3
        {
            int l[3]= {0,0,0}; //    on regarde les trois lignes, si c'est sur une seule...
            for(int i(0); i<3; ++i)
                for(int j(0); j<3; j++)
                    if(this->m_liste_cellules[3*(c%3)+i][3*(c/3)+j].m_listeNum[valeur-1])
                        l[j]=1;
            if((l[0]+l[1]+l[2] )== 1) {
                int nligne = 0 + l[1] + l[2]*2; //on est sur la ligne nligne
                for(int i(0); i<9; ++i) //on enleve les valeur hors du carr�
                {
                    if (i/3 == (c%3)) //si on est dans le carr�
                        continue;
                    if (this->m_liste_cellules[i][3*(c/3)+nligne].m_listeNum[valeur-1]) //si on est hors du carr�, sur la ligne ...
                        if (!this->setOff(i,3*(c/3)+nligne,valeur))
                            return false;
                }
            }
            int l2[3]= {0,0,0}; //    on regarde les trois colonnes, dans le carr�...
            for(int i(0); i<3; ++i)
                for(int j(0); j<3; j++)
                    if(this->m_liste_cellules[3*(c%3)+i][3*(c/3)+j].m_listeNum[valeur-1])
                        l2[i]=1;
            if((l2[0]+l2[1]+l2[2]) == 1){
                int ncolonne = 0 + l2[1] + l2[2]*2;
                for(int j(0); j<9; ++j) //dans la colonne, on  regarde les j
                {
                    if (j/3 == c/3)
                        continue;
                    if (this->m_liste_cellules[3*(c%3)+ncolonne][j].m_listeNum[valeur-1])
                        if (! this->setOff(3*(c%3)+ncolonne,j,valeur))
                            return false;
                }
            }
        }

        //on rentre dans les lignes ...
        for(int ligne(0); ligne<9; ++ligne) //si dans la ligne, tout est dans le m�me carr�, on enleve dans ce carr�
        {
            int l[3]= {0,0,0}; //on regarde le carr�
            for(int i(0); i<9; ++i)
                if (this->m_liste_cellules[i][ligne].m_listeNum[valeur-1])
                    l[i/3]=1; //dans quel carr� c'est
            if ((l[0]+l[1]+l[2]) ==1){ //si dans un seul carr�
                int ncarre=0+ l[1]+2*l[2]; //x du carr� (0,1,2)
                for(int i(0); i<9; ++i) //on regarde dans ce carr�
                {
                    if ( i/3 + (ligne/3)*3 == ligne) //si on est hors de la ligne, dans le carr�
                        continue;
                    if (this->m_liste_cellules[ncarre*3 + i%3][i/3 + (ligne/3)*3].m_listeNum[valeur-1])
                        if (! this->setOff(ncarre*3 + i%3,i/3 + (ligne/3)*3,valeur))
                            return false;
                }
            }
        }


        //on rentre dans les colonnes ...
        for(int colonne(0); colonne<9; ++colonne) //la xieme colonne
        {
            int c[3]= {0,0,0};
            for(int j(0); j<9; ++j) //dans la colonne
                if (this->m_liste_cellules[colonne][j].m_listeNum[valeur-1])
                    c[j/3]=1;
            if (c[0]+c[1]+c[2] ==1){
                int ncarre=0+ c[1]+2*c[2]; //le numero du carr� dans la colonne : on enleve dans ce carr�
                for(int i(0); i<9; ++i) //on regarde dans le carr�, dont x=colonne/3, y=ncarre
                {
                    if ( i%3 + (colonne/3)*3 == colonne) //dans les autres colonnes, dans le carr�
                        continue;
                    if (this->m_liste_cellules[i%3 + (colonne/3)*3][ncarre*3 + i/3].m_listeNum[valeur-1])
                        if ( ! this->setOff(i%3 + (colonne/3)*3,ncarre*3 + i/3,valeur))
                            return false;
                }//fin du carr�
            }//fin du si
        }//fin des colonnes
    }//fin des valeurs
    return true;
} //fin fonction

bool Carte::calculerTriplet(){
    for(int j(0);j<9;++j){ //dans la ligne j
        vector<Cellule*> vecTriplet;
        vecTriplet.resize(0);
        for(int i(0);i<9;++i)
            if((this->m_liste_cellules[i][j].m_taille <= 3) && (this->m_liste_cellules[i][j].m_taille>1))  //on regarde les cellules de taille 3 ou 2
                vecTriplet.push_back(& this->m_liste_cellules[i][j]);//on les range dans le vector
//        cout<<"taille ligne :"<<vecTriplet.size();
        for(int k1(2);k1<vecTriplet.size();++k1) //si au moins 3 triplets
            for(int k2(1);k2<k1;++k2)
                for(int k3(0);k3<k2;++k3){
                    int kvec[3]={k1,k2,k3};
                    vector<bool> essai_val(9,false);
                    for(int n(0);n<3;++n)
                        for(int val(1);val<=9;++val)
                            if( vecTriplet[kvec[n]]->m_listeNum[val-1])
                                essai_val[val-1]=true; //ce nombre existe dans ce triplet de cellules
                    int nbValeur=0;
                    for(int val(1);val<=9;++val)
                        if (essai_val[val-1])
                            ++nbValeur;
                    if (nbValeur<3){ //3 triplets, de deux valeurs en tout
//                        cout<<"bug Triplet"<<endl;
                        return false;
                    }
                    if(nbValeur > 3) //impossible qu'il n'y en ait que 1 ni 2. donc pour la suite nbValeur vaut 3 !
                        continue;
                                        //la on enleve les 3 nombres de la ligne!
                                        //on commence par faire la liste des valeurs,
                                        //puis la liste des 3 coordonn�es conserv�es (vecTriplet[k]->m_x)
                    int tableauValeur[3];
                    int iter=0;
                    for(int k(0);k<9;++k)
                        if (essai_val[k]){
                            tableauValeur[iter]=k+1;
                            ++iter;
                        } //tableauValeur : les 3 val retenues;
                   // cout<<"tableau :"<<tableauValeur[0]<<","<<tableauValeur[1]<<","<<tableauValeur[2]<<endl;
                    for(int i(0);i<9;++i){ //si k1,k2,k3 est un doublet
                        bool trouveIter=false;
                        for(iter=0;iter<3;++iter)
                            if(i==vecTriplet[kvec[iter]]->m_x) //dans les autres cases
                                trouveIter=true;
                        if(!trouveIter)
                            for(int iter(0);iter<3;++iter)
                                if(this->m_liste_cellules[i][j].m_listeNum[tableauValeur[iter]-1]){
//                                    cout<<i<<","<<j<<","<<tableauValeur[iter]<<endl;
                                    if(! this->setOff(i,j,tableauValeur[iter]))
                                        return false;
                                }

                    }
                } //on finit la boucle sur les �l�ments � <3 �l�mentes !
    }//on termine la boucle sur les lignes

    for(int i(0);i<9;++i){ //dans la colonne i
        vector<Cellule*> vecTriplet;
        vecTriplet.resize(0);
        for(int j(0);j<9;++j)
            if((this->m_liste_cellules[i][j].m_taille <= 3) && (this->m_liste_cellules[i][j].m_taille > 1))  //on regarde les cellules de taille 3
                vecTriplet.push_back(&this->m_liste_cellules[i][j]);//on les range dans le vector
//        cout<<"taille colonne :"<<vecTriplet.size();
        for(int k1(2);k1<vecTriplet.size();++k1)
            for(int k2(1);k2<k1;++k2)
                for(int k3(0);k3<k2;++k3){
                    int kvec[3]={k1,k2,k3};
                    vector<bool> essai_val(9,false);
                    for(int n(0);n<3;++n)
                        for(int val(1);val<=9;++val)
                            if( vecTriplet[kvec[n]]->m_listeNum[val-1])
                                essai_val[val-1]=true; //ce nombre existe dans ce triplet de cellules
                    int nbValeur=0;
                    for(int val(1);val<=9;++val)
                        if (essai_val[val-1])
                            ++nbValeur;
                    if(nbValeur<3){
//                        cout<<"bug Triplet"<<endl;
                        return false;
                    }
                    if(nbValeur > 3) //impossible qu'il n'y en ait que 1 ni 2. donc pour la suite nbValeur vaut 3 !
                        continue;
                                        //la on enleve les 3 nombres de la ligne!
                                        //on commence par faire la liste des valeurs,
                                        //puis la liste des 3 coordonn�es conserv�es (vecTriplet[k]->m_x)
                    int tableauValeur[3];
                    int iter=0;
                    for(int k(0);k<9;++k)
                        if (essai_val[k]){
                            tableauValeur[iter]=k+1;
                            ++iter;
                        } //tableauValeur : les 3 valeurs retenues dans le triplet;*
                    //cout<<"tableau :"<<tableauValeur[0]<<","<<tableauValeur[1]<<","<<tableauValeur[2]<<endl;
                    for(int j(0);j<9;++j){ //si k1,k2,k3 est un triplet
                        bool trouveIter=false;
                        for(iter=0;iter<3;++iter)
                            if(j==vecTriplet[kvec[iter]]->m_y) //dans les autres cases
                                trouveIter=true;
                        if(!trouveIter)
                            for(int iter(0);iter<3;++iter)
                                if(this->m_liste_cellules[i][j].m_listeNum[tableauValeur[iter]-1]){
//                                    cout<<i<<","<<j<<","<<tableauValeur[iter]<<endl;
                                    if(!this->setOff(i,j,tableauValeur[iter]))
                                        return false;
                                }
                    }
                } //on finit la boucle sur les �l�ments � <3 �l�mentes !
    }//on termine la boucle sur les lignes


    for(int c(0);c<9;++c){ //dans le carr� c
        vector<Cellule*> vecTriplet;
        vecTriplet.resize(0);
        for(int k(0);k<9;++k)
//            if((this->m_liste_cellules[3*(c%3)+k%3][3*(c/3)+k/3].m_taille <= 3) && (this->m_liste_cellules[3*(c%3)+k%3][3*(c/3)+k/3].m_taille <= 3))//on regarde les cellules de taille 3
//                vecTriplet.push_back(&this->m_liste_cellules[3*(c%3)+k%3][3*(c/3)+k/3]);//on les range dans le vector
            if((this->m_liste_carres[c].m_cellules[k]->m_taille <= 3) && (this->m_liste_carres[c].m_cellules[k]->m_taille <= 3))//on regarde les cellules de taille 3
                vecTriplet.push_back(this->m_liste_carres[c].m_cellules[k]);//on les range dans le vector
//        cout<<"taille carre :"<<vecTriplet.size();
        for(int k1(2);k1<vecTriplet.size();++k1)
            for(int k2(1);k2<k1;++k2)
                for(int k3(0);k3<k2;++k3){
                    int kvec[3]={k1,k2,k3};
                    vector<bool> essai_val(9,false);
                    for(int n(0);n<3;++n)
                        for(int val(1);val<=9;++val)
                            if( vecTriplet[kvec[n]]->m_listeNum[val-1])
                                essai_val[val-1]=true; //ce nombre existe dans ce triplet de cellules
                    int nbValeur=0;
                    for(int val(1);val<=9;++val)
                        if (essai_val[val-1])
                            ++nbValeur;
                    if(nbValeur<3){
//                        cout<<"bug Triplet"<<endl;
                        return false;
                    }
                    if(nbValeur > 3) //impossible qu'il n'y en ait que 1 ni 2. donc pour la suite nbValeur vaut 3 !
                        continue;
                                        //la on enleve les 3 nombres de la ligne!
                                        //on commence par faire la liste des valeurs,
                                        //puis la liste des 3 coordonn�es conserv�es (vecTriplet[k]->m_x)
                    int tableauValeur[3];
                    int iter=0;
                    for(int k(0);k<9;++k)
                        if (essai_val[k]){
                            tableauValeur[iter]=k+1;
                            ++iter;
                        } //tableauValeur : les 3 valeurs retenues dans le triplet : val1,val2,val3. En effet nbValeur == 3
                   // cout<<"tableau :"<<tableauValeur[0]<<","<<tableauValeur[1]<<","<<tableauValeur[2]<<endl;
                    for(int c_k(0);c_k<9;++c_k){ //si k1,k2,k3 est un triplet, la coordonn�e c_k dans le carr� c
                        bool trouveIter=false;
                        for(int iter(0);iter<3;++iter)
                            if(((c_k%3)+3*(c%3)==vecTriplet[kvec[iter]]->m_x)&&((c_k/3)+3*(c/3) == vecTriplet[kvec[iter]]->m_y)) //dans les autres cases
                                trouveIter=true;
                        if(!trouveIter)
                            for(int iter(0);iter<3;++iter)
                                if(this->m_liste_cellules[(c_k%3)+3*(c%3)][(c_k/3)+3*(c/3)].m_listeNum[tableauValeur[iter]-1]){
//                                    cout<<(c_k%3)+3*(c%3)<<","<<(c_k/3)+3*(c/3)<<","<<tableauValeur[iter]<<endl;
                                    if(!this->setOff((c_k%3)+3*(c%3),(c_k/3)+3*(c/3),tableauValeur[iter]))
                                        return false;
                                }
                    } //on a finit d'enlever le triplet dans le carr� !
                } //on finit la boucle sur les �l�ments � <3 �l�mentes !
    }//on termine la boucle sur les lignes


    return true;

}

bool Carte::calculerDoublet(){
    for(int j(0);j<9;++j){ //dans la ligne j
        vector<Cellule*> vecDoublet (0);
        for(int i(0);i<9;++i)
            if(this->m_liste_cellules[i][j].m_taille == 2)   //on regarde les cellules de taille 2
                vecDoublet.push_back(&m_liste_cellules[i][j]);//on les range dans le vector

        for(int k1(1);k1<vecDoublet.size();++k1)
            for(int k2(0);k2<k1;++k2){
                bool essai=true;
                vector<int> valDoublet(0);//on cherche les doublets : dans k1,k2
                for(int val(1);val<=9;++val)
                    if (vecDoublet[k1]->m_listeNum[val-1]){
                        valDoublet.push_back(val);
                        if(! vecDoublet[k2]->m_listeNum[val-1]){
                            essai=false;
                            break;
                        }
                    }
                if (!essai)
                    continue;

                for(int i(0);i<9;++i){ //si k1,k2 est un doublet
                    if((i==vecDoublet[k1]->m_x)||(i==vecDoublet[k2]->m_x)) //dans les autres cases
                        continue;
                    if(this->m_liste_cellules[i][j].m_listeNum[valDoublet[0]-1])
                        if(!this->setOff(i,j,valDoublet[0]))
                            return false;
                    if(this->m_liste_cellules[i][j].m_listeNum[valDoublet[1]-1])
                        if (!this->setOff(i,j,valDoublet[1]))
                            return false;
                }
            }
    }

    for(int i(0);i<9;++i){ //dans la colonne i
        vector<Cellule*> vecDoublet (0);
        for(int j(0);j<9;++j)
            if(this->m_liste_cellules[i][j].m_taille == 2)   //on regarde les cellules de taille 2
                vecDoublet.push_back(& m_liste_cellules[i][j]);//on les range dans le vector

        for(int k1(1);k1<vecDoublet.size();++k1)
            for(int k2(0);k2<k1;++k2){
                bool essai=true;
                vector<int> valDoublet(0);//on cherche les doublets : dans k1,k2
                for(int val(1);val<=9;++val)
                    if (vecDoublet[k1]->m_listeNum[val-1]){
                        valDoublet.push_back(val);
                        if(! vecDoublet[k2]->m_listeNum[val-1]){
                            essai=false;
                            break;
                        }
                    }
                if (!essai) //quittes si non doublet. Donc l� (k1,k2) est un doubet, de valeurs valDoublet[0], valDoublet[1];
                    continue;

                for(int j(0);j<9;++j){
                    if((j==vecDoublet[k1]->m_y)||(j==vecDoublet[k2]->m_y)) //dans les autres cases
                        continue;
                    if(this->m_liste_cellules[i][j].m_listeNum[valDoublet[0]-1])
                        if(!this->setOff(i,j,valDoublet[0]))
                            return false;
                    if(this->m_liste_cellules[i][j].m_listeNum[valDoublet[1]-1])
                        if (!this->setOff(i,j,valDoublet[1]))
                            return false;
                }
            }
    }

    for(int c(0);c<9;++c){ //dans le carr� c
        vector<Cellule*> vecDoublet (0);
        for(int k(0);k<9;++k)
            if(this->m_liste_cellules[(c%3)*3+(k%3)][3*(c/3)+k/3].m_taille == 2)   //on regarde les cellules de taille 2
                vecDoublet.push_back(& m_liste_cellules[(c%3)*3+(k%3)][3*(c/3)+k/3]);//on les range dans le vector

        for(int k1(1);k1<vecDoublet.size();++k1)
            for(int k2(0);k2<k1;++k2){
                bool essai=true;
                vector<int> valDoublet(0);//on cherche les doublets : dans k1,k2
                for(int val(1);val<=9;++val)
                    if (vecDoublet[k1]->m_listeNum[val-1]){
                        valDoublet.push_back(val);
                        if(! vecDoublet[k2]->m_listeNum[val-1]){
                            essai=false;
                            break;
                        }
                    }
                if (!essai) //quittes si non doublet. Donc l� (k1,k2) est un doubet, de valeurs valDoublet[0], valDoublet[1];
                    continue;

                for(int k(0);k<9;++k){ //si k1,k2 est un doublet, on enleve dans le carr� les k
                    if(k==(vecDoublet[k1]->m_x%3)+3*(vecDoublet[k1]->m_y%3))    //si k ne tombe pas sur un un doublet ...
                        continue;
                    if(k==(vecDoublet[k2]->m_x%3)+3*(vecDoublet[k2]->m_y%3))    //si k ne tombe pas sur un un doublet ...
                        continue;
                    if(this->m_liste_cellules[(c%3)*3+(k%3)][(c/3)*3+(k/3)].m_listeNum[valDoublet[0]-1])
                        if(!this->setOff((c%3)*3+(k%3),(c/3)*3+(k/3),valDoublet[0]))
                            return false;
                    if(this->m_liste_cellules[(c%3)*3+(k%3)][(c/3)*3+(k/3)].m_listeNum[valDoublet[1]-1])
                        if (!this->setOff((c%3)*3+(k%3),(c/3)*3+(k/3),valDoublet[1]))
                            return false;
                }
            }
    }

    return true;
}

bool Carte::faireCalculer(){
    do{
        this->m_modifie=false;
        if (! this->calculerSi())
            return false;
        if(!this->calculerDoublet())
            return false;
//        if(!this->calculerTriplet()){
//            cout<<"bug triplet"<<endl;
//            return false;
//        }
        if(!this->calculerTriplet())
            return false;
        //this->forcerCalcul();
    } while (this->m_modifie);
    return true;
}


string Carte::getStringVal() const
{
    string s("");
    for (int j(0); j<9; ++j)
    {
        for(int i(0); i<9; ++i)
            s+=std::to_string(m_liste_cellules[i][j].m_valeur);
        s+="\n";
    }
    s+="\n";
    return s;
}

std::ostream& operator<<( std::ostream &flux, Carte const& carte )
{
    if(carte.m_erreur_carte)
        return flux<<carte.getStringVal()<<endl;
    else return flux<<"erreur carte afficher !"<<endl<<endl;
}

ostream& operator<<(ostream &flux, Essai const& essai ){
    if(essai.m_erreur_carte)
        return flux<<essai.getStringVal()<<endl;
    else return flux<<"erreur carte afficher !"<<endl<<endl;
}

ostream& operator<<(ostream &flux, Test const& test ){
    if(test.m_erreur_carte)
        return flux<<test.getStringVal()<<endl;
    else return flux<<"erreur carte afficher !"<<endl<<endl;
}


Carte::Carte(Carte const& carte) //constructeur de copie ! utilis� par Essai !
{
    this->m_liste_cellules.resize(9,vector<Cellule>(9));
    //cout<<"ok"<<endl;
    for(int i(0); i<9; ++i)
        for(int j(0); j<9; ++j)
            //this->m_cellules[i][j];
            this->m_liste_cellules[i][j]=Cellule(carte.m_liste_cellules[i][j],this);

    //cout<<"allocation tab"<<endl;

    this->m_liste_lignes.resize(9);
    for(int i(0); i<9;++i)
        this->m_liste_lignes[i]=Ligne(carte.m_liste_lignes[i],this);
    //cout<<"allocation ligne"<<endl;

    this->m_liste_colonnes.resize(9);
    for(int i(0); i<9;++i)
        this->m_liste_colonnes[i]=Colonne(carte.m_liste_colonnes[i],this);

    //cout<<"allocation colonne"<<endl;

    this->m_liste_carres.resize(9);
    for(int i(0); i<9;++i)
        this->m_liste_carres[i]=Carre(carte.m_liste_carres[i],this);

    this->m_modifie = false;
    //cout<<"allocation carr�s"<<endl;

    //cout<<"copier : ok"<<endl;

}

Carte::Carte(){}


Test::Test(string string_sudoku) : Carte(string_sudoku) {

}

Test::Test(Carte const& carte) : Carte(carte) {

}


Essai::Essai(Essai const& essai) : Test(essai){
    m_liste_tests.resize(0);
    m_solution=NULL;
}


Essai::Essai(){}

Test::Test(){}


bool Essai::tester(int x,int y,int val){
    //cout<<"tester :"<<x<<","<<y<<","<<val<<endl;
    this->m_x=x;
    this->m_y=y;
    this->m_val=val;
    this->m_erreur=true;

    if(! this->m_liste_cellules[x][y].m_listeNum[val-1]){
        this->m_erreur = false;
        return false;
    }
    if (! (this->setValeur(x,y,val))){ //Essaie dans la copie TEST
        //cout<<"test faux"<<endl;
        this->m_erreur =false; //renvoit que la combinaison est impossible
        return false;
    }
    if ( ! (this->faireCalculer())){
        //cout<<"test faux"<<endl;
        this->m_erreur =false; //idem : combinaison impossible
        return false;
    }

    //cout<<"test vrai"<<endl;
    return true;
}


bool Essai::enlever(int x,int y,int val){ //retourne faux si probleme dans la carte de base : vrai contradiction du jeu
                                            //si contradiction locale, on l'enleve avec cette m�me fonction ...
    //cout<<"enlever :"<<x<<","<<y<<","<<val<<endl;
    if ( ! this->setOff(x,y,val) ){ //on l'enleve de la carte principale ..
        cout<<"probleme carte principale"<<endl;//si contradiction
        return false;
    }
    if(! this->faireCalculer()){
        cout<<"probleme carte principale"<<endl;
        return false;
    }
    for (int i(0);i<this->m_liste_tests.size();++i){ //on parcourt la liste des essais, pour en enlever l'essai faux !
        if (this->m_liste_tests[i]->m_erreur == true){ //si il n'y a pas d�j� d'erreur ! car on supprime � la fin
            if (! (this->m_liste_tests[i]->setOff(x,y,val))){ //si le setOff tombe sur une erreur
                this->m_liste_tests[i]->m_erreur=false; //on l'enleve de la liste
                if (! (this->enlever(this->m_liste_tests[i]->m_x,this->m_liste_tests[i]->m_y,this->m_liste_tests[i]->m_val))){ //et on calcule !
                    this->m_erreur = false;
                    return false;
                }
            }
            else if(!(this->m_liste_tests[i]->faireCalculer())){
                this->m_liste_tests[i]->m_erreur=false; //on l'enleve de la liste
                if (! (this->enlever(this->m_liste_tests[i]->m_x,this->m_liste_tests[i]->m_y,this->m_liste_tests[i]->m_val))){ //et on calcule !
                    this->m_erreur = false;
                    return false;
                }
            }
        }
            //et on boucle !
    }
    return true; //si aucune erreur remarqu�e sur *this!
}

//Essai::Carte(Essai const& essai){

void Essai::nettoyerListe(){
    int i(0); //on retire les elements morts ...
    while(i < this->m_liste_tests.size()){ //tant qu'on n'est pas hors de la liste !
        if (! this->m_liste_tests[i]->m_erreur) { //� retirer !
            delete this->m_liste_tests[i]; //destructeur ...
            this->m_liste_tests.erase(this->m_liste_tests.begin()+i); //mise a jour de la liste
        }
        else ++i; //ou incrementer
    }

        //si l'hypoth�se est d�j� v�rifi�e dans la carte de base (carte m�re) : on enl�ve l'hypoth�se !
    i=0;
    while(i<this->m_liste_tests.size()){
        if (this->m_liste_cellules[this->m_liste_tests[i]->m_x][this->m_liste_tests[i]->m_y].m_valeur == this->m_liste_tests[i]->m_val){
            delete this->m_liste_tests[i];
            this->m_liste_tests.erase(this->m_liste_tests.begin()+i);
        }
        else ++i;
    }

    //si l'hypoth�se A=>B et l'hypoth�se B=>A : alors on enl�ve l'une des deux ! nettoie efficacement :-)
    for(int i(0);i<this->m_liste_tests.size();++i){
        int j = i+1;
        while(j<this->m_liste_tests.size()){
            if ((this->m_liste_tests[i]->m_liste_cellules[this->m_liste_tests[j]->m_x][this->m_liste_tests[j]->m_y].m_valeur == this->m_liste_tests[j]->m_val)&&
                    (this->m_liste_tests[j]->m_liste_cellules[this->m_liste_tests[i]->m_x][this->m_liste_tests[i]->m_y].m_valeur == this->m_liste_tests[i]->m_val)){
//                cout<<"carte doublon"<<endl<<*this->m_liste_tests[i]<<endl<<*this->m_liste_tests[j]<<endl<<endl;
                delete this->m_liste_tests[j];
                this->m_liste_tests.erase(this->m_liste_tests.begin()+j);
            }
            else ++j;
        }
    }
}

bool Essai::ajouterTest(int x,int y,int val){

    //cout<<"ajouter Test :"<<x<<","<<y<<","<<val<<endl;

    Essai* ajout = new Essai(*this);
    ajout->m_mere=this;
    this->m_liste_tests.push_back(ajout);


    if (! (this->m_liste_tests[this->m_liste_tests.size()-1]->tester(x,y,val)) ){ //si interdit : � modifier .... si true il est gard�, sinon on retirer (enlever)
        this->m_liste_tests[this->m_liste_tests.size()-1]->m_erreur = false;
        if ( ! this->enlever(x,y,val)) //on enleve (x,y,val), et r�cursivement ...
            return false; //si faux : c'est que la Carte elle-m�me est fausse !

                            //une fois tous les "enlever" termin�s, on nettoie
        this->nettoyerListe();
    }

    return true; //n'est faux que si la CARTE est _strictement_ impossible !
}

Essai::Essai(string string_sudoku) : Test(string_sudoku){
    this->m_liste_tests.reserve(81*sizeof(Test*));
    this->m_liste_tests.resize(0);
    this->m_erreur=true; //pas d'erreur globale
    this->m_solution=NULL;
    this->m_mere=NULL;
//    cout<<"construction Essai r�ussie ..."<<endl;
}

bool Essai::iterer(){ //ajouter les essais ! tous
    this->m_erreur = true;
    for(int i(0);i<9;++i)
        for(int j(0);j<9;++j)
            for(int val(1);val<=9;++val)
                if (this->m_liste_cellules[i][j].m_listeNum[val-1])
                    if (this->m_liste_cellules[i][j].m_taille > 1){
//                        cout<<"iterer :"<<i<<","<<j<<","<<val<<"; "<<endl;
                        if (! this->ajouterTest(i,j,val)){ //probleme carte principale
                            this->m_erreur = false;
//                            cout<<"erreur carte iterer"<<endl;
                            for(int i(0);i< this->m_liste_tests.size(); ++i)
                                if (this->m_liste_tests[i]){
                                    delete this->m_liste_tests[i];
                                    this->m_liste_tests[i]=NULL;
                                }
                            this->m_liste_tests.clear();
                            return false;
                        }
                    }
    cout<<"fin du calcul !"<<endl;

    this->nettoyerListe();

    return true;

}

Essai::~Essai(){
    for(int i(0);i< this->m_liste_tests.size(); ++i)
        if (this->m_liste_tests[i]){
            delete this->m_liste_tests[i];
            this->m_liste_tests[i]=NULL;
        }
    //destruction recursive
}

bool Essai::niveauDeux(){
    if (! this->iterer())
        return false;
    cout<<"taille liste niveau deux : "<<this->m_liste_tests.size()<<endl;
    int i(0);
    while(i<this->m_liste_tests.size()){
        if(! this->m_liste_tests[i]->iterer()){ //si probleme dans l'it�ration : enlever, et donc aussi nettoyer !
            if(! this->enlever(this->m_liste_tests[i]->m_x,this->m_liste_tests[i]->m_y,this->m_liste_tests[i]->m_val)){ //enleve dans la carte principale ET dans toute la liste
                this->m_erreur = false; //erreur de la carte principale
                return false;
            }
            this->nettoyerListe();
            delete this->m_liste_tests[i];
            this->m_liste_tests.erase(this->m_liste_tests.begin()+i);
        }
        else ++i;
    }
    return true;
}

int Essai::nombrePossible(){
    vector<vector<int>> cartePossible(9,vector<int>(9,0));
    for(int i(0);i<this->m_liste_tests.size();++i)
        cartePossible[this->m_liste_tests[i]->m_x][this->m_liste_tests[i]->m_y]+=1;
    for(int i(0);i<9;++i)
        for(int j(0);j<9;++j)
            if(cartePossible[i][j] == 0)
                cartePossible[i][j]=1;
    int nombre=1;
    for(int i(0);i<9;++i)
        for(int j(0);j<9;++j)
                nombre*=cartePossible[i][j];

    return nombre;

}

void melangerListe(vector<Essai*>& mon_vecteur){
    vector<int> liste_1(mon_vecteur.size());
    vector<int> liste_2(mon_vecteur.size());
    for(int i(0);i<mon_vecteur.size();++i)
        liste_1[i]=i;
    for(int i(0);i<mon_vecteur.size();++i){
        int tirage= rand()% liste_1.size();
        liste_2[i]=liste_1[tirage];
        liste_1.erase(liste_1.begin()+tirage);
    }
    //ici, liste_1 est vide, et liste_2 m�lang�e
    vector<Essai*> nouveau(mon_vecteur.size());
    for(int i(0);i<mon_vecteur.size();++i)
        nouveau[i]=mon_vecteur[liste_2[i]];

    mon_vecteur=nouveau;
}

bool Essai::generer(){ //return true si trouv�, false si erreur : alors on boucle
    cout<<"taille "<<this->m_liste_tests.size()<<endl;
    this->m_solution=NULL;
    this->m_erreur =true;
    if(this->m_liste_tests.size()==0){
        if (! this->iterer()){ //si probleme dans la carte this
    //        cout<<" probleme iterer";
            this->m_erreur =false;
            return false;
        }
    }
    if(this->m_liste_tests.size() == 0) //si on a trouv� un unique essai ! fin de la pile
        return true;
//    cout<<"taille "<<this->m_liste_tests.size()<<endl;
    melangerListe(this->m_liste_tests); //on insere de l'al�atoire
    while(this->m_liste_tests.size()>0){
    //for(int i(0);i<this->m_liste_tests.size();++i){ //on parcourt les essais qu'on a it�r�s
        if (this->m_liste_tests[0]->generer()){ //si on a trouv� ! on sauvegarde le chemin
            this->m_solution=this->m_liste_tests[0];
            return true; //et on dit qu'on a trouv�
        }
        else{
            delete this->m_liste_tests[0];
            this->m_liste_tests.erase(this->m_liste_tests.begin());
        }
    }

    this->m_erreur = false;
    return false; //si aucun de valides ...
}

int Essai::getSolution(vector<vector<int>>& tableau){
    if (this->m_mere)
        if(tableau[this->m_x][this->m_y]==0)
            tableau[this->m_x][this->m_y]=this->m_val;
//    cout<<this->m_x<<","<<this->m_y<<","<<this->m_val<<";"<<endl;
    if(this->m_solution)
        return 1+this->m_solution->getSolution(tableau);
    else return 1;
}

/*
void resoudreGenererTester(string adresse){

    Essai monSudoku(adresse);
    monSudoku.iterer();
    cout<<"resolution sudoku"<<endl;
    cout<<monSudoku<<endl;

    //generer le sudoku r�ponse;
    monSudoku.generer();
    //Recuperer le sudoku g�n�r�
    vector<vector<int>> tableau(9,vector<int> (9,0));
    for(int i(0);i<9;++i)
        for(int j(0);j<9;++j)
            tableau[i][j]=monSudoku.m_liste_cellules[i][j].m_valeur;

    int compteur = monSudoku.getSolution(tableau);
    cout<<"nombre de valeurs ajout�es : "<<compteur<<endl;
    cout<<"afficher g�n�r� :"<<endl;
    for(int j(0);j<9;++j){
        for(int i(0);i<9;++i)
            cout<<tableau[i][j];
        cout<<endl;
    }

    ofstream ecrireFichier("C:/Users/33658/OneDrive/Documents/projets/sudoku/test genere.txt");
    for(int j(0);j<9;++j){
        for(int i(0);i<9;++i)
            ecrireFichier<<tableau[i][j];
        ecrireFichier<<'\n';
    }
    ecrireFichier.close();

    cout<<"r�soudre le g�n�r�"<<endl;
    Essai monSudokuTest("C:/Users/33658/OneDrive/Documents/projets/sudoku/test genere.txt");
    monSudokuTest.iterer();
    cout<<monSudokuTest<<endl;

}
*/
