#ifndef SUDOKUSOLVER_H
#define SUDOKUSOLVER_H
#include <vector>
#include <string>
#include <fstream>
#include <QList>
#include <QLabel>

class SudokuSolver{

private:
    std::ofstream outputf;
    int total_solved;
    std::vector<double> calc;
    int original[101][101];// ORIGINAL INPUT SUDOKU WITH GIVENS ONLY
    int bpm[101][101];// BLUE PRINT MATRIX
    int n;// size of sudoku
    long long fv;// fitness value
    int pop_size, reset_size, itr_no;//INITIALS
    std::vector< std::vector < std::vector<int > > > ip;// INITIAL POPULATION
    std::vector< std::vector < std::vector<int > > > fp;// FINAL POPULATION
    std::vector< std::pair < long long, std::pair< bool, int > > > sp;// USED IN ELITISM (SIMPLIFIED POPULATION)

    //READS SUDOKU
     void read(std::string &);
    //PRINT SUDOKU
     void print(std::vector< std::vector<int> >&);
    //RANDOM_DOUBLE GENERATOR BETWEEN 0.0 & 1.0
     double randomd();
    //RANDOM_GENERATOR USED IN UNIQUE ROW GENERATION FOR POPULATION INITIALISATION
     void random(int, int);
    //GENERATES POPULATION
     void population_generator();
    //RESETS POPULAITON
     void reset();
    //COMPARISION FUNCTION
     int f(int , int , int , int , std::vector < std::vector < int > >&);
    //ROW_FITNESS FUNCTION
     int row_fitness(std::vector < std::vector < int > >&);
    //COLUMN_FITNESS FUNCTION
     int column_fitness(std::vector < std::vector < int > >&);
    //SUB_SUBSQUARE_FITNESS FUNCTION
     int sub_square_fitness(std::vector < std::vector < int > >&);
    //SUMMING_FUNCTION
     int fitness(std::vector < std::vector < int > >&);
    //FITNESS_CHECKER
     bool find_fitness();
    //FUNCTION FOR ROW_WISE ALTERNATE_UNIFORM_CROSSOVER
     void uniform_crossover();
    // FUNCTION FOR BIT_WISE_MUTATION
     void bwm();
    // FUNCTION FOR FITNESS_ENHANCING
    //SUB_SQUARE_REPETITION_REMOVING FUNCTION
     void sqr(int &);
    //ROW_REPETITION_REMOVING FUNCTION
     void ror(int &);
    //COLUMN_REPETITION_REMOVING FUNCTION
     void cor(int &);
    //CALLER OF THE ABOVE THREE FUNCTON
     void fitness_enhancer();
    //ELITISM
     void elitism();
public:
    //GA_INITIALIZER
     bool t_Ret_GA(std::string&,std::string&,QList<QLabel *>&,qint32&,QLabel*, QLabel*,bool*);
};
#endif //SUDOKUSOLVER_H
