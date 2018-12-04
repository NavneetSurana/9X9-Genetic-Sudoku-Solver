#include "SudokuSolver.h"
#include <iostream>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <ctime>
#include <random>
#include <cstdlib>
#include <direct.h>
#include <windows.h>
#include <conio.h>
#include <QCoreApplication>
using namespace std;
/**************************************MACRO STARTS******************************************/
/********************************************************************************************/
#define pb push_back
#define ff first
#define ss second
#define mp make_pair
/*********************************************************************************************/
/**************************************MACRO ENDS**********************************************/
void SudokuSolver::read(string &temp) {
    ifstream ins;
    ins.open(temp, ios::in);
    n=9;
    fv = 3LL * ((n*n*(n - 1))) / 2.0;
    pop_size = 10 * n;
    int count = 0;
    for (int i = 1; i<n + 1; i++) {
        for (int j = 1; j<n + 1; j++) {
            ins >> original[i][j];
            if (original[i][j] != 0) {
                bpm[i][j] = 1;
                count++;
            }
        }
    }
    ins.close();
    if (count >= 32) reset_size = 20;//1star,easy
    else if (count >=30) reset_size = 50;//2star,5star
    else if (count>=28) reset_size = 140;//3star , 4star
    else if (count >=25) reset_size = 350;//Challenging
    else reset_size=500;//Difficult and SuperDifficult
    return;
}
//PRINT SUDOKU
void SudokuSolver::print(vector< vector<int> >& a) {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            outputf << a[i][j] << ' ';
        }
        outputf << endl;
    }
    outputf << endl;
}
/***************************************************************************************************************/
/***************************************END OF INPUT-OUTPUT FUNCTIONS FOR SUDOKU************************************************************************/



/**************************************START OF RANDOM GENERATORS*************************************************************************/
/***************************************************************************************************************/
//RANDOM_DOUBLE GENERATOR BETWEEN 0.0 & 1.0
double SudokuSolver::randomd() {
    double k = rand() % 100 / 99.0;
    return k;
}
//RANDOM_GENERATOR USED IN UNIQUE ROW GENERATION FOR POPULATION INITIALISATION
void SudokuSolver::random(int i, int p) {
    vector<bool> itr(n + 1);
    for (int j = 1; j <= n; j++) {
        if (original[i][j] != 0) {
            itr[original[i][j]] = 1;
        }
    }
    vector<int> sh;
    for (int j = 1; j <= n; j++) {
        if (itr[j] != 1) sh.pb(j);
    }
    random_shuffle(sh.begin(), sh.end());
    int k = 0;
    for (int j = 1; j <= n; j++) {
        if (original[i][j] == 0) {
            ip[p][i][j] = sh[k];
            fp[p][i][j] = ip[p][i][j];
            k++;
        }
        else {
            ip[p][i][j] = original[i][j];
            fp[p][i][j] = original[i][j];
        }
    }
    return;
}
//end of random_generator
/***************************************************************************************************************/
/**************************************END OF RANDOM GENERATORS*************************************************************************/

/**************************************START OF POPULATION_GENERATOR AND RESET_FUNCTION*************************************************************************/
/***************************************************************************************************************/
//GENERATES POPULATION
void SudokuSolver::population_generator() {
    ip.pb(vector < vector < int > >(n + 1, vector<int>(n + 1)));
    fp.pb(vector < vector < int > >(n + 1, vector<int>(n + 1)));
    int p = ip.size() - 1;
    int t = n + 1;
    for (int i = 1; i<t; i++) {
        random(i, p);
    }
    return;
}

//RESETS POPULAITON
void SudokuSolver::reset() {
    ip.clear();
    fp.clear();
    for (int i = 0; i<pop_size; i++) {
        population_generator();
    }
}
/***************************************************************************************************************/
/***************************************END OF POPULATION_GENERATOR AND RESET_FUNCTION************************************************************************/

/**************************************START OF FITNESS_FUNCTIONS*************************************************************************/
/***************************************************************************************************************/
//COMPARISION FUNCTION
int SudokuSolver::f(int i, int j, int k, int l, vector < vector < int > >& a) {
    if (a[i][j] == a[k][l]) return 0;
    else return 1;
}

//ROW_FITNESS FUNCTION
int SudokuSolver::row_fitness(vector < vector < int > >& a) {
    int count = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n - 1; j++) {
            for (int l = j + 1; l <= n; l++) {
                count += f(i, j, i, l, a);
            }
        }
    }
    return count;
}

//COLUMN_FITNESS FUNCTION
int SudokuSolver::column_fitness(vector < vector < int > >& a) {
    int count = 0;
    for (int j = 1; j <= n; j++) {
        for (int i = 1; i <= n - 1; i++) {
            for (int l = i + 1; l <= n; l++) {
                count += f(i, j, l, j, a);
            }
        }
    }
    return count;
}

//SUB_SUBSQUARE_FITNESS FUNCTION
int SudokuSolver::sub_square_fitness(vector < vector < int > >& a) {
    int count = 0;
    int t = sqrt(n);
    for (int i = 1; i <= n; i++) {
        for (int q = 1; q <= t; q++) {
            for (int j = 1 + (q - 1)*t; j <= q * t - 1; j++) {
                for (int l = j + 1; l <= q * t; l++) {
                    count += f(i, j, i, l, a);
                }
            }
            if (i%t != 0) {
                for (int r = 1 + (q - 1)*t; r <= q * t; r++) {
                    for (int k = i + 1; k <= i + t - i % t; k++) {
                        for (int s = 1 + (q - 1)*t; s <= q * t; s++) {
                            count += f(i, r, k, s, a);
                        }
                    }
                }
            }
        }
    }
    return count;
}

//SUMMING_FUNCTION
int SudokuSolver::fitness(vector < vector < int > >& a) {

    int count = row_fitness(a) + column_fitness(a) + sub_square_fitness(a);
    //cout<<count<<endl;
    return count;
}

//FITNESS_CHECKER
bool SudokuSolver::find_fitness() {
    for (int j = 0; j<pop_size; j++) {
        long long temp = fitness(fp[j]);
        if (fv == temp) {
            total_solved++;
            if (total_solved == 1) {
                print(fp[j]);
            }
            return 1;
        }
    }
    return 0;
}
/***************************************************************************************************************/
/***************************************END OF FITNESS_FUNCTIONS************************************************************************/


/**************************************STRAT OF RET_GA_ALGORITHM WITH FITNESS_ENHANCEMENT*************************************************************************/
/***************************************************************************************************************/
//FUNCTION FOR ROW_WISE ALTERNATE_UNIFORM_CROSSOVER
void SudokuSolver::uniform_crossover() {

    for (int i = 0; i<pop_size; i += 2) {
        bool flag = (rand()%2);
        if (randomd() <= 0.8) {
            int j;
            if (flag == 0) j = 1;
            else j = 2;
            for (; j <= n; j += 2) {
                fp[i][j].swap(fp[i + 1][j]);
            }
        }
    }
}



// FUNCTION FOR BIT_WISE_MUTATION
void SudokuSolver::bwm() {
    for (int t = 0; t<pop_size; t++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (bpm[i][j] == 0 && randomd() <= 0.2) {
                    fp[t][i][j] = rand() % n + 1;
                }
            }
        }
    }
}


// FUNCTION FOR FITNESS_ENHANCING
//SUB_SQUARE_REPETITION_REMOVING FUNCTION
void SudokuSolver::sqr(int &t) {
    int len = sqrt(n);
    for (int j = 1; j <= n; j += len) {
        for (int i = 1; i <= n; i += len) {
            vector<int> temp;
            vector<int> flag1(n + 1);
            for (int k = 0; k < len; k++) {
                for (int l = 0; l < len; l++) {
                    flag1[fp[t][i + k][j + l]] += 1;
                }
            }
            for (int k = 1; k <= n; k++) {
                if (flag1[k] == 0) temp.pb(k);
            }
            random_shuffle(temp.begin(), temp.end());
            int ptr = 0;
            for (int k = 0; k < len; k++) {
                for (int l = 0; l < len; l++) {
                    if (flag1[fp[t][i + k][j + l]] > 1 && bpm[i + k][j + l] != 1) {
                        flag1[fp[t][i + k][j + l]]--;
                        fp[t][i + k][j + l] = temp[ptr];
                        ptr++;
                    }
                }
            }
            temp.clear();
            flag1.clear();
        }
    }
}
//ROW_REPETITION_REMOVING FUNCTION
void SudokuSolver::ror(int &t) {
    for (int i = 1; i <= n; i++) {
        vector<int> temp;
        vector<int> flag1(n + 1);
        for (int j = 1; j <= n; j++) {
            flag1[fp[t][i][j]]++;
        }
        for (int j = 1; j <= n; j++) {
            if (flag1[j] == 0) temp.pb(j);
        }
        random_shuffle(temp.begin(), temp.end());
        int k = 0;
        for (int j = 1; j <= n; j++) {
            if (flag1[fp[t][i][j]] > 1 && bpm[i][j] != 1) {
                flag1[fp[t][i][j]]--;
                fp[t][i][j] = temp[k];
                k++;
            }
        }
    }
}
//COLUMN_REPETITION_REMOVING FUNCTION
void SudokuSolver::cor(int &t) {
    for (int j = 1; j <= n; j++) {
        vector<int> temp;
        vector<int> flag1(n + 1);
        for (int i = 1; i <= n; i++) {
            flag1[fp[t][i][j]]++;
        }
        for (int i = 1; i <= n; i++) {
            if (flag1[i] == 0) temp.pb(i);
        }
        random_shuffle(temp.begin(), temp.end());
        int k = 0;
        for (int i = 1; i <= n; i++) {
            if (flag1[fp[t][i][j]] > 1 && bpm[i][j] != 1) {
                flag1[fp[t][i][j]]--;
                fp[t][i][j] = temp[k];
                k++;
            }
        }

    }
}
//CALLER OF THE ABOVE THREE FUNCTON
void SudokuSolver::fitness_enhancer() {
    for (int t = 0; t < pop_size; t++) {
        sqr(t);

        ror(t);

        cor(t);
    }
}




//ELITISM
void SudokuSolver::elitism() {
    for (int i = 0; i<pop_size; i++) {
        sp.pb(mp(-fitness(ip[i]), mp(1, i)));
        sp.pb(mp(-fitness(fp[i]), mp(0, i)));
    }
    sort(sp.begin(), sp.end());
    vector < vector < vector < int > > > temp;
    for (int i = 0; i< pop_size; i += 1) {
        if (sp[i].ss.ff == 1) {
            temp.pb(ip[sp[i].ss.ss]);
        }
        else temp.pb(fp[sp[i].ss.ss]);
    }
    ip.clear();
    fp.clear();
    ip = temp;
    fp = temp;
    sp.clear();
    temp.clear();
}




//GA_INITIALIZER
bool SudokuSolver::t_Ret_GA(string& input,string& output,QList<QLabel *>& list,qint32& maxitr,QLabel* itrNum, QLabel* fitnessAch,bool* Stop) {
    //qDebug()<<"done";
    read (input);
    outputf.open(output);
    bool flag = 0;
    for (int i = 0; i<maxitr; i++) {
        if(*Stop==true){
            *Stop=false;
            return false;
        }
        itr_no = i + 1;
        itrNum->setText(QString::number(itr_no));
        if ((i%reset_size) == 0) {
            reset();
        }
        uniform_crossover();
        fitness_enhancer();
        flag = find_fitness();
        if (flag == 1) break;
        bwm();
        fitness_enhancer();
        flag = find_fitness();
        if (flag == 1) break;
        elitism();
        for(int p=0;p<list.size();p++){
            if(list[p]->objectName()[0]!='L') continue;
            quint32 j=static_cast<quint32>(static_cast<QString>(list[p]->objectName()[1]).toUInt());
            quint32 k=static_cast<quint32>(static_cast<QString>(list[p]->objectName()[2]).toUInt());
            if(0<j && j<10 && 0<k && k<10){
                if(list[p]->text()=="") list[p]->setStyleSheet("QLabel { background-color : rgb(215, 235, 203) }");
                list[p]->setText(QString::number(fp[0][j][k]));
            }

        }
        fitnessAch->setText(QString::number(fitness(fp[0]))+"/972");
        QCoreApplication::processEvents();
        //qDebug()<<itr_no<<endl;
    }
    outputf.close();
    return flag;
}
/***************************************************************************************************************/
/**********************************END OF RET_GA_ALGORITHM WITH FITNESS_ENHANCEMENT*****************************************************************************/


/***************************************************************************************************************/
/**********************************END OF FILE MANAGING FUNCTIONS AND STATISTICAL FUNCTIONS*****************************************************************************/
