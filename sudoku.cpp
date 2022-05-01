#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "sudoku.h"
#include <algorithm>
//#include <bits/stdc++.h>

using namespace std;

Cellule::Cellule()
{
    m_listeNum.resize(9,true);
    m_valeur=0;
    m_taille=9;
}

Cellule::Cellule(int num)
{
    //vector<bool> vec(9,false);
    m_listeNum.resize(9,false);
    m_valeur=num;
    m_taille=1;
    m_listeNum[num-1]=true;
}

Cellule::Cellule(Cellule const& cellule,Carte* carte){
    m_listeNum=cellule.m_listeNum;
    m_valeur = cellule.m_valeur;
    m_taille = cellule.m_taille;
    m_carte = carte;
    m_x=cellule.m_x;
    m_y=cellule.m_y;
}

Cellule::Cellule(Cellule const& cellule){
    m_listeNum=cellule.m_listeNum;
    m_valeur = cellule.m_valeur;
    m_taille = cellule.m_taille;
    m_x=cellule.m_x;
    m_y=cellule.m_y;
}


bool Cellule::setOff(int num) //enleve num; si taille =1, appeler setValeur. Si possibles =0 return false
{
    //cout<<"celO";
    if (m_listeNum[num-1])
    {
        m_listeNum[num-1]=false;
        m_carte->m_modifie=true;
        m_carte->m_modification+=1;
        --(m_taille);
        if (m_taille == 0) return false;
        if (m_taille==1)
            for(int i(0); i<9; ++i)
                if(m_listeNum[i]==true)
                {
                    m_valeur=i+1;
                    if (!m_carte->setValeur(m_x,m_y,i+1))
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
    if ((m_valeur!=0) && (m_valeur != num))
        return false;//si �a change

    if (m_taille > 1){
        m_carte->m_modifie = true;
        m_carte->m_modification += m_taille - 1;
        for(int i(0); i<9; ++i)
            m_listeNum[i]=false;
        m_listeNum[num-1]=true;
        m_taille = 1;
        m_valeur = num;
    }
    else if (m_taille==1){
        m_valeur = num;
        m_carte->m_modifie = true;
    }
    if (m_taille ==0)
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
    m_carte=carte;
    m_cellules.resize(9,NULL);

    for(int i(0); i<9; ++i)
        m_cellules[i]=& (carte->m_liste_cellules[i][j]);
    m_y=j;

//    vector<vector<Cellule*>> vec2(9,vector<Cellule*>(0));
    m_liste_num.reserve(81 * sizeof(Cellule*));
    m_liste_num.resize(9,vector<Cellule*>()); //remplit le tableau

   // cout<<"allocation ligne"<<endl;
    for(int num(0); num<9; ++num)
        for(int i(0); i<9; ++i)
            if (m_cellules[i]->m_listeNum[num]==true)
                m_liste_num[num].push_back(m_cellules[i]);
}

Ligne::Ligne(Ligne const& ligne, Carte* carte){
    m_y =ligne.m_y;
    m_carte=carte;
    m_cellules.resize(9,NULL);
    for(int i(0);i<9;++i)
        m_cellules[i]=& (carte->m_liste_cellules[ligne.m_cellules[i]->m_x][ligne.m_cellules[i]->m_y]);
    m_liste_num.reserve(81 * sizeof(Cellule*));
    m_liste_num.resize(9,vector<Cellule*>()); //remplit le tableau
    for(int num(0); num<9; ++num)
        for(int k(0);k<ligne.m_liste_num[num].size();++k)
            m_liste_num[num].push_back(&( carte->m_liste_cellules[ligne.m_liste_num[num][k]->m_x][ligne.m_liste_num[num][k]->m_y]));
    return;
}


bool Ligne::setOff(int x,int val)
{
    //cout<<"ligO";
    for(int i(0); i<m_liste_num[val-1].size(); ++i)
        if (m_liste_num[val-1][i]->m_x==x)
        {
            m_liste_num[val-1].erase(m_liste_num[val-1].begin()+i);
            if (m_liste_num[val-1].size()==1)
                if (!(m_carte->setValeur(m_liste_num[val-1][0]->m_x,m_y,val)))
                    return false;
            if (m_liste_num[val-1].size()==0)
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
            if(m_cellules[i]->m_listeNum[val-1]){
                if (!(m_cellules[i]->setOff(val)))
                    return false;
                if (!(m_carte->m_liste_colonnes[i].setOff(m_y,val)))
                    return false;
                if (!(m_carte->m_liste_carres[i/3 +3*(m_y/3)].setOff(i%3+(m_y%3)*3,val)))
                    return false;;
            }
        }
    m_liste_num[val-1].erase(m_liste_num[val-1].begin(),m_liste_num[val-1].end());
    m_liste_num[val-1].push_back(&(m_carte->m_liste_cellules[x][m_y]));
    return true;
}


Colonne::Colonne(int i, Carte* carte)
{
    m_carte=carte;
    m_cellules.resize(9,NULL);

    for(int j(0); j<9; ++j)
        m_cellules[j]=& (m_carte->m_liste_cellules[i][j]);
    m_x=i;

//    vector<vector<Cellule*>> vec2(9,vector<Cellule*>(0));
    m_liste_num.reserve(81 * sizeof(Cellule*));
    m_liste_num.resize(9,vector<Cellule*>());

    for(int num(0); num<9; ++num)
        for(int j(0); j<9; ++j)
            if (m_cellules[j]->m_listeNum[num]==true)
                m_liste_num[num].push_back(m_cellules[j]);
    return;
}

Colonne::Colonne(Colonne const& colonne, Carte* carte){
    m_x =colonne.m_x;
    m_carte=carte;
    m_cellules.resize(9,NULL);
    for(int j(0);j<9;++j)
        m_cellules[j]=& carte->m_liste_cellules[colonne.m_cellules[j]->m_x][colonne.m_cellules[j]->m_y];

    m_liste_num.reserve(81*sizeof(Cellule*));
    m_liste_num.resize(9,vector<Cellule*>()); //remplit le tableau
    for(int num(0); num<9; ++num)
        for(int k(0);k<colonne.m_liste_num[num].size();++k)
            m_liste_num[num].push_back(& (carte->m_liste_cellules[colonne.m_liste_num[num][k]->m_x][colonne.m_liste_num[num][k]->m_y]));
}

bool Colonne::setOff(int y,int val)
{
    //cout<<"colO";
    for(int j(0); j<m_liste_num[val-1].size(); ++j)
        if (m_liste_num[val-1][j]->m_y==y)
        {
            m_liste_num[val-1].erase(m_liste_num[val-1].begin()+j);
            if (m_liste_num[val-1].size()==1)
                if (!(m_carte->setValeur(m_x,m_liste_num[val-1][0]->m_y,val)))
                    return false;
            if (m_liste_num[val-1].size()==0)
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
            if(m_cellules[j]->m_listeNum[val-1]){
                if (!(m_cellules[j]->setOff(val)))
                    return false;
                if (!(m_carte->m_liste_lignes[j].setOff(m_x,val)))
                    return false;
                if (!(m_carte->m_liste_carres[m_x/3+3*(j/3)].setOff((m_x%3)+(j%3)*3,val)))
                    return false;
            }
        }
    m_liste_num[val-1].erase(m_liste_num[val-1].begin(),m_liste_num[val-1].end());
    m_liste_num[val-1].push_back(&(m_carte->m_liste_cellules[m_x][y]));
    return true;
}


Carre::Carre(int c,Carte* carte)
{
    m_carte= carte;
    m_c =c;
    int m_i=m_c%3; //m_c=m_i+3*m_j
    int m_j=m_c/3;

//    vector<Cellule*> vec(9,NULL);
    m_cellules.resize(9,NULL);

    for(int i(0); i<3; ++i)
        for(int j(0); j<3; ++j)
            m_cellules[i+3*j] = & (m_carte->m_liste_cellules[3*m_i+i][3*m_j+j]);

    m_liste_num.reserve(81*sizeof(Cellule*));
    m_liste_num.resize(9,vector<Cellule*>(0));

    for(int i(0); i<3; ++i)
        for(int j(0); j<3; ++j)
            for(int num(0); num<9; ++num)
                if (m_cellules[i+3*j]->m_listeNum[num]==true)
                    m_liste_num[num].push_back(m_cellules[i+3*j]);

}

Carre::Carre(Carre const& carre, Carte* carte){
    m_c =carre.m_c;
    m_carte=carte;
    m_cellules.resize(9,NULL);
    for(int k(0);k<9;++k)
        m_cellules[k]=& (m_carte->m_liste_cellules[carre.m_cellules[k]->m_x][carre.m_cellules[k]->m_y]);

    m_liste_num.reserve(81*sizeof(Cellule*));
    m_liste_num.resize(9,vector<Cellule*>()); //remplit le tableau
    for(int num(0); num<9; ++num)
        for(int iter(0);iter<carre.m_liste_num[num].size();++iter)
            m_liste_num[num].push_back(& carte->m_liste_cellules[carre.m_liste_num[num][iter]->m_x][carre.m_liste_num[num][iter]->m_y]);
    return;
}

bool Carre::setOff(int k,int val)
{
    //cout<<"carO";
    for(int j(0); j<m_liste_num[val-1].size(); ++j)
        if (((m_liste_num[val-1][j]->m_x % 3) + 3 * (m_liste_num[val-1][j]->m_y%3))==k)
        {
            m_liste_num[val-1].erase(m_liste_num[val-1].begin()+j);
            if (m_liste_num[val-1].size()==1)//
                if (!(m_carte->setValeur(m_liste_num[val-1][0]->m_x,m_liste_num[val-1][0]->m_y,val)))
                    return false;
            if (m_liste_num[val-1].size()==0)
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
            if(m_cellules[j]->m_listeNum[val-1]){
                if (!(m_cellules[j]->setOff(val)))
                    return false;
                if (!(m_carte->m_liste_lignes[(j/3) + 3* (m_c/3)].setOff(j%3+(m_c%3) *3,val)))
                    return false;
                if (!(m_carte->m_liste_colonnes[j%3+(m_c%3) *3].setOff((j/3) + 3* (m_c/3),val)))
                    return false;
            }
        }
    m_liste_num[val-1].erase(m_liste_num[val-1].begin(),m_liste_num[val-1].end());
    m_liste_num[val-1].push_back(&(m_carte->m_liste_cellules[(k%3) + 3*(m_c % 3)][(k/3) + 3*(m_c/3)]));
    return true;
}

Carte::Carte(string string_sudoku)
{
    m_modification=0;
    m_liste_cellules.resize(9,vector<Cellule>(9));
    //cout<<"ok"<<endl;
    for(int i(0); i<9; ++i)
        for(int j(0); j<9; ++j)
        {
            //m_cellules[i][j];
            m_liste_cellules[i][j]=Cellule();
            m_liste_cellules[i][j].m_x=i;
            m_liste_cellules[i][j].m_y=j;
            m_liste_cellules[i][j].m_carte=this;
        }
    //cout<<"allocation tab"<<endl;
    m_liste_lignes.resize(9);
    for(int i(0); i<9;++i)
        m_liste_lignes[i]=Ligne(i,this);
    //cout<<"allocation ligne"<<endl;

    m_liste_colonnes.resize(9);
    for(int i(0); i<9;++i)
        m_liste_colonnes[i]=Colonne(i,this);

    //cout<<"allocation colonne"<<endl;



    m_liste_carres.resize(9);
    for(int i(0); i<9;++i)
        m_liste_carres[i]=Carre(i,this);

    //cout<<"allocation carr�s"<<endl;


    int i(0),j(0);
    int iter(0);
    m_erreur_carte=true;
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
                m_erreur_carte=false;
                break;
            }
            if (! (setValeur(i,j,numero))){
                m_erreur_carte=false;
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
            break;
        ++iter;
        if(iter>=string_sudoku.size())
            break;
    } while((j<=8));

    if (!m_erreur_carte)
    {
        cout<<"pas de lancement du jeu ..."<<endl;
        cout<<*this;
        return;
    }


    if (!faireCalculer()){
        m_erreur_carte = false;
        cout<<"erreur carte !"<<endl;
    }

    cout<<"lecture fichier ok"<<endl;

    return;

}

bool Carte::setOff(int x,int y,int val)
{
    if (!(m_liste_cellules[x][y].setOff(val)))
        return false;
    if (!(m_liste_lignes[y].setOff(x,val)))
        return false;
    if (!(m_liste_colonnes[x].setOff(y,val)))
        return false;
    if (!(m_liste_carres[(x/3) + 3* (y/3)].setOff((x%3) + 3*(y%3),val)))
        return false;

    //cout<<val;
    return true;
}

bool Carte::setValeur(int x,int y,int val)
{
    //cout<<x<<","<<y<<endl;
    if (!(m_liste_cellules[x][y].setValeur(val)))
        return false;
    if (!(m_liste_lignes[y].setValeur(x,val)))
        return false;
    if (!(m_liste_colonnes[x].setValeur(y,val)))
        return false;
    if (!(m_liste_carres[(x/3) + 3* (y/3)].setValeur((x%3) + 3*(y%3),val)))
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
                    if(m_liste_cellules[3*(c%3)+i][3*(c/3)+j].m_listeNum[valeur-1])
                        l[j]=1;
            if((l[0]+l[1]+l[2] )== 1) {
                int nligne = 0 + l[1] + l[2]*2; //on est sur la ligne nligne
                for(int i(0); i<9; ++i) //on enleve les valeur hors du carr�
                {
                    if (i/3 == (c%3)) //si on est dans le carr�
                        continue;
                    if (m_liste_cellules[i][3*(c/3)+nligne].m_listeNum[valeur-1]) //si on est hors du carr�, sur la ligne ...
                        if (!setOff(i,3*(c/3)+nligne,valeur))
                            return false;
                }
            }
            int l2[3]= {0,0,0}; //    on regarde les trois colonnes, dans le carr�...
            for(int i(0); i<3; ++i)
                for(int j(0); j<3; j++)
                    if(m_liste_cellules[3*(c%3)+i][3*(c/3)+j].m_listeNum[valeur-1])
                        l2[i]=1;
            if((l2[0]+l2[1]+l2[2]) == 1){
                int ncolonne = 0 + l2[1] + l2[2]*2;
                for(int j(0); j<9; ++j) //dans la colonne, on  regarde les j
                {
                    if (j/3 == c/3)
                        continue;
                    if (m_liste_cellules[3*(c%3)+ncolonne][j].m_listeNum[valeur-1])
                        if (! setOff(3*(c%3)+ncolonne,j,valeur))
                            return false;
                }
            }
        }

        //on rentre dans les lignes ...
        for(int ligne(0); ligne<9; ++ligne) //si dans la ligne, tout est dans le m�me carr�, on enleve dans ce carr�
        {
            int l[3]= {0,0,0}; //on regarde le carr�
            for(int i(0); i<9; ++i)
                if (m_liste_cellules[i][ligne].m_listeNum[valeur-1])
                    l[i/3]=1; //dans quel carr� c'est
            if ((l[0]+l[1]+l[2]) ==1){ //si dans un seul carr�
                int ncarre=0+ l[1]+2*l[2]; //x du carr� (0,1,2)
                for(int i(0); i<9; ++i) //on regarde dans ce carr�
                {
                    if ( i/3 + (ligne/3)*3 == ligne) //si on est hors de la ligne, dans le carr�
                        continue;
                    if (m_liste_cellules[ncarre*3 + i%3][i/3 + (ligne/3)*3].m_listeNum[valeur-1])
                        if (! setOff(ncarre*3 + i%3,i/3 + (ligne/3)*3,valeur))
                            return false;
                }
            }
        }


        //on rentre dans les colonnes ...
        for(int colonne(0); colonne<9; ++colonne) //la xieme colonne
        {
            int c[3]= {0,0,0};
            for(int j(0); j<9; ++j) //dans la colonne
                if (m_liste_cellules[colonne][j].m_listeNum[valeur-1])
                    c[j/3]=1;
            if (c[0]+c[1]+c[2] ==1){
                int ncarre=0+ c[1]+2*c[2]; //le numero du carr� dans la colonne : on enleve dans ce carr�
                for(int i(0); i<9; ++i) //on regarde dans le carr�, dont x=colonne/3, y=ncarre
                {
                    if ( i%3 + (colonne/3)*3 == colonne) //dans les autres colonnes, dans le carr�
                        continue;
                    if (m_liste_cellules[i%3 + (colonne/3)*3][ncarre*3 + i/3].m_listeNum[valeur-1])
                        if ( ! setOff(i%3 + (colonne/3)*3,ncarre*3 + i/3,valeur))
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
            if((m_liste_cellules[i][j].m_taille <= 3) && (m_liste_cellules[i][j].m_taille>1))  //on regarde les cellules de taille 3 ou 2
                vecTriplet.push_back(& m_liste_cellules[i][j]);//on les range dans le vector
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
                                if(m_liste_cellules[i][j].m_listeNum[tableauValeur[iter]-1]){
//                                    cout<<i<<","<<j<<","<<tableauValeur[iter]<<endl;
                                    if(! setOff(i,j,tableauValeur[iter]))
                                        return false;
                                }

                    }
                } //on finit la boucle sur les �l�ments � <3 �l�mentes !
    }//on termine la boucle sur les lignes

    for(int i(0);i<9;++i){ //dans la colonne i
        vector<Cellule*> vecTriplet;
        vecTriplet.resize(0);
        for(int j(0);j<9;++j)
            if((m_liste_cellules[i][j].m_taille <= 3) && (m_liste_cellules[i][j].m_taille > 1))  //on regarde les cellules de taille 3
                vecTriplet.push_back(&m_liste_cellules[i][j]);//on les range dans le vector
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
                                if(m_liste_cellules[i][j].m_listeNum[tableauValeur[iter]-1]){
//                                    cout<<i<<","<<j<<","<<tableauValeur[iter]<<endl;
                                    if(!setOff(i,j,tableauValeur[iter]))
                                        return false;
                                }
                    }
                } //on finit la boucle sur les �l�ments � <3 �l�mentes !
    }//on termine la boucle sur les lignes


    for(int c(0);c<9;++c){ //dans le carr� c
        vector<Cellule*> vecTriplet;
        vecTriplet.resize(0);
        for(int k(0);k<9;++k)
//            if((m_liste_cellules[3*(c%3)+k%3][3*(c/3)+k/3].m_taille <= 3) && (m_liste_cellules[3*(c%3)+k%3][3*(c/3)+k/3].m_taille <= 3))//on regarde les cellules de taille 3
//                vecTriplet.push_back(&m_liste_cellules[3*(c%3)+k%3][3*(c/3)+k/3]);//on les range dans le vector
            if((m_liste_carres[c].m_cellules[k]->m_taille <= 3) && (m_liste_carres[c].m_cellules[k]->m_taille <= 3))//on regarde les cellules de taille 3
                vecTriplet.push_back(m_liste_carres[c].m_cellules[k]);//on les range dans le vector
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
                                if(m_liste_cellules[(c_k%3)+3*(c%3)][(c_k/3)+3*(c/3)].m_listeNum[tableauValeur[iter]-1]){
//                                    cout<<(c_k%3)+3*(c%3)<<","<<(c_k/3)+3*(c/3)<<","<<tableauValeur[iter]<<endl;
                                    if(!setOff((c_k%3)+3*(c%3),(c_k/3)+3*(c/3),tableauValeur[iter]))
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
            if(m_liste_cellules[i][j].m_taille == 2)   //on regarde les cellules de taille 2
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
                    if(m_liste_cellules[i][j].m_listeNum[valDoublet[0]-1])
                        if(!setOff(i,j,valDoublet[0]))
                            return false;
                    if(m_liste_cellules[i][j].m_listeNum[valDoublet[1]-1])
                        if (!setOff(i,j,valDoublet[1]))
                            return false;
                }
            }
    }

    for(int i(0);i<9;++i){ //dans la colonne i
        vector<Cellule*> vecDoublet (0);
        for(int j(0);j<9;++j)
            if(m_liste_cellules[i][j].m_taille == 2)   //on regarde les cellules de taille 2
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
                    if(m_liste_cellules[i][j].m_listeNum[valDoublet[0]-1])
                        if(!setOff(i,j,valDoublet[0]))
                            return false;
                    if(m_liste_cellules[i][j].m_listeNum[valDoublet[1]-1])
                        if (!setOff(i,j,valDoublet[1]))
                            return false;
                }
            }
    }

    for(int c(0);c<9;++c){ //dans le carr� c
        vector<Cellule*> vecDoublet (0);
        for(int k(0);k<9;++k)
            if(m_liste_cellules[(c%3)*3+(k%3)][3*(c/3)+k/3].m_taille == 2)   //on regarde les cellules de taille 2
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
                    if(m_liste_cellules[(c%3)*3+(k%3)][(c/3)*3+(k/3)].m_listeNum[valDoublet[0]-1])
                        if(!setOff((c%3)*3+(k%3),(c/3)*3+(k/3),valDoublet[0]))
                            return false;
                    if(m_liste_cellules[(c%3)*3+(k%3)][(c/3)*3+(k/3)].m_listeNum[valDoublet[1]-1])
                        if (!setOff((c%3)*3+(k%3),(c/3)*3+(k/3),valDoublet[1]))
                            return false;
                }
            }
    }

    return true;
}

bool Carte::faireCalculer(){
    do{
        m_modifie=false;
        if (! calculerSi())
            return false;
        if(!calculerDoublet())
            return false;
//        if(!calculerTriplet()){
//            cout<<"bug triplet"<<endl;
//            return false;
//        }
        if(!calculerTriplet())
            return false;
        //forcerCalcul();
    } while (m_modifie);
    return true;
}


string Carte::getStringVal() const
{
    string s("");
    for (int j(0); j<9; ++j)
    {
        for(int i(0); i<9; ++i){
            s+=std::to_string(m_liste_cellules[i][j].m_valeur);
            s+=" ";
        }
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
    m_modification = carte.m_modification;
    m_liste_cellules.resize(9,vector<Cellule>(9));
    //cout<<"ok"<<endl;
    for(int i(0); i<9; ++i)
        for(int j(0); j<9; ++j)
            //m_cellules[i][j];
            m_liste_cellules[i][j]=Cellule(carte.m_liste_cellules[i][j],this);

    //cout<<"allocation tab"<<endl;

    m_liste_lignes.resize(9);
    for(int i(0); i<9;++i)
        m_liste_lignes[i]=Ligne(carte.m_liste_lignes[i],this);
    //cout<<"allocation ligne"<<endl;

    m_liste_colonnes.resize(9);
    for(int i(0); i<9;++i)
        m_liste_colonnes[i]=Colonne(carte.m_liste_colonnes[i],this);

    //cout<<"allocation colonne"<<endl;

    m_liste_carres.resize(9);
    for(int i(0); i<9;++i)
        m_liste_carres[i]=Carre(carte.m_liste_carres[i],this);

    m_modifie = false;

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
    m_aleatoire = essai.m_aleatoire;
}


Essai::Essai(){}

Test::Test(){}


bool Essai::tester(int x,int y,int val){
    //cout<<"tester :"<<x<<","<<y<<","<<val<<endl;
    m_x=x;
    m_y=y;
    m_val=val;
    m_erreur=true;

    if(! m_liste_cellules[x][y].m_listeNum[val-1]){
        m_erreur = false;
        return false;
    }
    if (! (setValeur(x,y,val))){ //Essaie dans la copie TEST
        //cout<<"test faux"<<endl;
        m_erreur =false; //renvoit que la combinaison est impossible
        return false;
    }
    if ( ! (faireCalculer())){
        //cout<<"test faux"<<endl;
        m_erreur =false; //idem : combinaison impossible
        return false;
    }

    //cout<<"test vrai"<<endl;
    return true;
}


bool Essai::enlever(int x,int y,int val){ //retourne faux si probleme dans la carte de base : vrai contradiction du jeu
                                            //si contradiction locale, on l'enleve avec cette m�me fonction ...
    //cout<<"enlever :"<<x<<","<<y<<","<<val<<endl;
    if ( ! setOff(x,y,val) ){ //on l'enleve de la carte principale ..
        //cout<<"probleme carte principale"<<endl;//si contradiction
        return false;
    }
    if(! faireCalculer()){
        //cout<<"probleme carte principale"<<endl;
        return false;
    }
    for (int i(0);i<m_liste_tests.size();++i){ //on parcourt la liste des essais, pour en enlever l'essai faux !
        if (m_liste_tests[i]->m_erreur == true){ //si il n'y a pas d�j� d'erreur ! car on supprime � la fin
            if (! (m_liste_tests[i]->setOff(x,y,val))){ //si le setOff tombe sur une erreur
                m_liste_tests[i]->m_erreur=false; //on l'enleve de la liste
                if (! (enlever(m_liste_tests[i]->m_x,m_liste_tests[i]->m_y,m_liste_tests[i]->m_val))){ //et on calcule !
                    m_erreur = false;
                    return false;
                }
            }
            else if(!(m_liste_tests[i]->faireCalculer())){
                m_liste_tests[i]->m_erreur=false; //on l'enleve de la liste
                if (! (enlever(m_liste_tests[i]->m_x,m_liste_tests[i]->m_y,m_liste_tests[i]->m_val))){ //et on calcule !
                    m_erreur = false;
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
    while(i < m_liste_tests.size()){ //tant qu'on n'est pas hors de la liste !
        if (! m_liste_tests[i]->m_erreur) { //� retirer !
            delete m_liste_tests[i]; //destructeur ...
            m_liste_tests.erase(m_liste_tests.begin()+i); //mise a jour de la liste
        }
        else ++i; //ou incrementer
    }

        //si l'hypoth�se est d�j� v�rifi�e dans la carte de base (carte m�re) : on enl�ve l'hypoth�se !
    i=0;
    while(i<m_liste_tests.size()){
        if (m_liste_cellules[m_liste_tests[i]->m_x][m_liste_tests[i]->m_y].m_valeur == m_liste_tests[i]->m_val){
            delete m_liste_tests[i];
            m_liste_tests.erase(m_liste_tests.begin()+i);
        }
        else ++i;
    }

}

bool Essai::ajouterTest(int x,int y,int val){

    //cout<<"ajouter Test :"<<x<<","<<y<<","<<val<<endl;

    Essai* ajout = new Essai(*this);
    ajout->m_mere=this;
    m_liste_tests.push_back(ajout);


    if (! (m_liste_tests[m_liste_tests.size()-1]->tester(x,y,val)) ){ //si interdit : � modifier .... si true il est gard�, sinon on retirer (enlever)
        m_liste_tests[m_liste_tests.size()-1]->m_erreur = false;
        if ( ! enlever(x,y,val)) //on enleve (x,y,val), et r�cursivement ...
            return false; //si faux : c'est que la Carte elle-m�me est fausse !

                            //une fois tous les "enlever" termin�s, on nettoie
        nettoyerListe();
    }

    return true; //n'est faux que si la CARTE est _strictement_ impossible !
}

Essai::Essai(string string_sudoku) : Test(string_sudoku){
    m_liste_tests.reserve(81*sizeof(Test*));
    m_liste_tests.resize(0);
    m_erreur=true; //pas d'erreur globale
    m_solution=NULL;
    m_mere=NULL;
//    cout<<"construction Essai r�ussie ..."<<endl;
}

bool Essai::iterer(){ //ajouter les essais ! tous
    m_erreur = true;
    for(int i(0);i<9;++i)
        for(int j(0);j<9;++j)
            for(int val(1);val<=9;++val)
                if (m_liste_cellules[i][j].m_listeNum[val-1])
                    if (m_liste_cellules[i][j].m_taille > 1){
//                        //cout<<"iterer :"<<i<<","<<j<<","<<val<<"; "<<endl;
                        if (! ajouterTest(i,j,val)){ //probleme carte principale
                            m_erreur = false;
//                            //cout<<"erreur carte iterer"<<endl;
                            for(int i(0);i< m_liste_tests.size(); ++i)
                                if (m_liste_tests[i]){
                                    delete m_liste_tests[i];
                                    m_liste_tests[i]=NULL;
                                }
                            m_liste_tests.clear();
                            return false;
                        }
                    }
    //cout<<"fin du calcul !"<<endl;

    nettoyerListe();



    //si l'hypoth�se A=>B et l'hypoth�se B=>A : alors on enl�ve l'une des deux ! nettoie efficacement :-)
    for(int i(0);i<m_liste_tests.size();++i){
        int j = i+1;
        while(j<m_liste_tests.size()){
            if ((m_liste_tests[i]->m_liste_cellules[m_liste_tests[j]->m_x][m_liste_tests[j]->m_y].m_valeur == m_liste_tests[j]->m_val)&&
                    (m_liste_tests[j]->m_liste_cellules[m_liste_tests[i]->m_x][m_liste_tests[i]->m_y].m_valeur == m_liste_tests[i]->m_val)){
//                //cout<<"carte doublon"<<endl<<*m_liste_tests[i]<<endl<<*m_liste_tests[j]<<endl<<endl;
                delete m_liste_tests[j];
                m_liste_tests.erase(m_liste_tests.begin()+j);
            }
            else ++j;
        }
    }

    //on trie selon le nombre de moficiation de chaque carte
    if (m_aleatoire == false)
        std::sort(m_liste_tests.begin(),m_liste_tests.end(),comparaison_modification());

    return true;

}


Essai::~Essai(){
    for(int i(0);i< m_liste_tests.size(); ++i)
        if (m_liste_tests[i]){
            delete m_liste_tests[i];
            m_liste_tests[i]=NULL;
        }
    //destruction recursive
}

bool Essai::niveauDeux(){
    if (! iterer())
        return false;
    //cout<<"taille liste niveau deux : "<<m_liste_tests.size()<<endl;
    int i(0);
    if (m_aleatoire == false)
        std::sort(m_liste_tests.begin(),m_liste_tests.end(),comparaison_modification());
    while(i<m_liste_tests.size()){
        if(! m_liste_tests[i]->iterer()){ //si probleme dans l'it�ration : enlever, et donc aussi nettoyer !
            if(! enlever(m_liste_tests[i]->m_x,m_liste_tests[i]->m_y,m_liste_tests[i]->m_val)){ //enleve dans la carte principale ET dans toute la liste
                m_erreur = false; //erreur de la carte principale
                return false;
            }
            nettoyerListe();
            delete m_liste_tests[i];
            m_liste_tests.erase(m_liste_tests.begin()+i);
        }
        else ++i;
    }
    return true;
}

int Essai::nombrePossible(){
    vector<vector<int>> cartePossible(9,vector<int>(9,0));
    for(int i(0);i<m_liste_tests.size();++i)
        cartePossible[m_liste_tests[i]->m_x][m_liste_tests[i]->m_y]+=1;
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
    //cout<<"taille "<<m_liste_tests.size()<<endl;
    m_solution=NULL;
    m_erreur =true;
    if(m_liste_tests.size()==0){
        if (! iterer()){ //si probleme dans la carte this
    //        cout<<" probleme iterer";
            m_erreur =false;
            return false;
        }
    }
    if(m_liste_tests.size() == 0) //si on a trouv� un unique essai ! fin de la pile
        return true;
//    cout<<"taille "<<m_liste_tests.size()<<endl;

    if (m_aleatoire == true)
        melangerListe(m_liste_tests); //on insere de l'al�atoire
    while(m_liste_tests.size()>0){
    //for(int i(0);i<m_liste_tests.size();++i){ //on parcourt les essais qu'on a it�r�s
        if (m_liste_tests[0]->generer()){ //si on a trouv� ! on sauvegarde le chemin
            m_solution=m_liste_tests[0];
            return true; //et on dit qu'on a trouv�
        }
        else{
            delete m_liste_tests[0];
            m_liste_tests.erase(m_liste_tests.begin());
        }
    }

    m_erreur = false;
    return false; //si aucun de valides ...
}

int Essai::getSolution(vector<vector<int>>& tableau){
    if (m_mere)
        if(tableau[m_x][m_y]==0)
            tableau[m_x][m_y]=m_val;

    if(m_solution)
        return 1+m_solution->getSolution(tableau);
    else return 1;
}

inline bool comparaison_modification::operator() (const Essai* struct1, const Essai* struct2)
{
    return (struct1->m_modification > struct2->m_modification);
}






Profondeur::Profondeur(Profondeur& parent) : Carte(parent)
{
    m_niveauSup=NULL;
    m_modification=parent.m_modification;
    //m_possible.erase(m_possible.begin());
    //melangerPossible();
}

Profondeur::Profondeur(Carte& carte) : Carte(carte){
    m_niveauSup=NULL;
    m_modification = carte.m_modification;
}

Profondeur::Profondeur(string string_sudoku) : Carte(string_sudoku){
    m_niveauSup=NULL;
    m_possible.m_valeur=0;
    //    faireCalculer();

}

bool Profondeur::choisirPossible(){
    int x = rand()%9;
    int y = rand()%9;
    int val = rand()%9;

    int nombreBoucle=0;
    boucle:
    while( x <9){
        while( y <9 ){
            while (val<9){
                if ( (m_liste_cellules[x][y].m_listeNum[val] == true) && (m_liste_cellules[x][y].m_valeur ==0) ){
                    m_possible.m_x=x;
                    m_possible.m_y=y;
                    m_possible.m_valeur = val+1;
                    return true;
                }
                val +=1;
                }
            val=0;
            y+=1;
            }
        y=0;
        x+=1;
    }
    //on reboucle depuis le début;
    nombreBoucle+=1;
    x=0;
    y=0;
    val=0;

    if(nombreBoucle ==1 )
        goto boucle;
    cout << "erreur choisirPossible"<<endl;

    return false;
}


bool Profondeur::essayer(){

    m_niveauSup = new Profondeur(*this);
    if (! m_niveauSup->choisirPossible())
        return false;

    if ( (m_niveauSup->setValeur(m_niveauSup->m_possible.m_x,m_niveauSup->m_possible.m_y,m_niveauSup->m_possible.m_valeur)) == false){
        return false;
    }
    if ((m_niveauSup->faireCalculer()) == false)
        return false;

    return true;
}

bool Profondeur::boucle(){
    a:

//    cout << m_modification <<" nombre modification " <<endl;
    if (m_modification >= 648) //alors la carte est pleine.
        return true;

    if ( essayer() == false){ // essayer le m_possible du niveauSup
        if ( ! setOff(m_niveauSup->m_possible.m_x,m_niveauSup->m_possible.m_y,m_niveauSup->m_possible.m_valeur) ){ //si bug dans la carte testée ...
            delete m_niveauSup;   // si c'est faux
            m_niveauSup = NULL;
            return false;
        }

        delete m_niveauSup;   // si c'est faux
        m_niveauSup = NULL;

        goto a;
    } // donc la le m_possible est vrai (pour l'instant)


    if (m_niveauSup->boucle() == false){ //au début de l'appel, on teste la  taille du niveauSup :ok
        if (! setOff(m_niveauSup->m_possible.m_x,m_niveauSup->m_possible.m_y,m_niveauSup->m_possible.m_valeur) ){
            delete m_niveauSup;   // si c'est faux
            m_niveauSup = NULL;
            return false;
        }

        delete m_niveauSup;
        m_niveauSup = NULL;

        goto a; // comme la boucle est fausse, on cherche un autre essai (vrai) à boucler.
    }

    return true; // la boucle est vraie, donc provient d'une boucle vraie, qui provient d'une m_modification >= 728 vraie ...

}


Profondeur::~Profondeur(){
    if (m_niveauSup)
        delete m_niveauSup;
    m_niveauSup=NULL;
}

int Profondeur::recuperer(vector<vector<int>>& tableau){
    if (m_possible.m_valeur != 0)
        tableau[m_possible.m_x][m_possible.m_y]= m_possible.m_valeur;
    if (m_niveauSup)
        return m_niveauSup->recuperer(tableau) +1;
    else return 0;

}

Profondeur* Profondeur::derniereCarte(){
    if (m_niveauSup == NULL)
        return this;
    else return m_niveauSup->derniereCarte();
}


