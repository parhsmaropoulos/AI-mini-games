#include <vector>
#include <iostream>
#include <cmath>
#include <bits/stdc++.h>

using namespace std;

// Number of population in each generation
static const int population_size = 100;

// Available genes
const vector<int> genes = {0, 1};

// Target vector that we want to achive
const vector<vector<int>>
    target = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

// Return a random number between start and end
int random_num(int start, int end)
{
    int range = (end - start) + 1;
    int random_int = start + (rand() % range);
    return random_int;
};

// Create chromosome or a vector<vector<int>> of ints
vector<vector<int>> create_chromo()
{
    vector<vector<int>> chromo;
    for (int i = 0; i < target.size(); i++)
    {
        vector<int> row;
        for (int j = 0; j < target[i].size(); j++)
        {
            row.push_back(random_num(0, 1));
        };
        chromo.push_back(row);
    };

    return chromo;
};

// Individual class representing a chromo sequence
class Individual
{
public:
    Individual(vector<vector<int>> chromosome);
    vector<Individual> Breed(Individual &parent2);
    vector<vector<int>> Get_Chromos(void) { return m_chromosome; };
    void Mutate();
    int Calculate_Fitness();
    void Print_Chromos(void);
    int Get_Fitness(void) { return m_fitness; };

private:
    int m_fitness;
    vector<vector<int>> m_chromosome;
};

// Initialize an individual class objet
Individual::Individual(vector<vector<int>> chromosome)
{
    m_chromosome = chromosome;
    m_fitness = Calculate_Fitness();
};

// Print the chromosomes of a individual
void Individual::Print_Chromos()
{
    for (int i = 0; i < m_chromosome.size(); i++)
    {
        for (int j = 0; j < m_chromosome[i].size(); j++)
        {
            cout << m_chromosome[i][j] << " ";
        }
        cout << "\n";
    }
}

void Individual::Mutate()
{
    // Get random gene to mutate
    int r = random_num(0, m_chromosome.size() - 1);
    int c = random_num(0, m_chromosome[0].size() - 1);
    m_chromosome[r][c] = random_num(0, 1);
}

//Breed an individual with an other to return their offsprings

vector<Individual> Individual::Breed(Individual &parent2)
{
    vector<Individual> kids;
    vector<vector<int>> offspring1_chrom;
    vector<vector<int>> offspring2_chrom;

    int outerlen = m_chromosome.size();
    int innerlen = m_chromosome[0].size();
    // int point = random_num(0, innerlen * outerlen);

    for (int i = 0; i < outerlen; i++)
    {
        vector<int> offspring1_genes;
        vector<int> offspring2_genes;
        // Point to break the chromosomes.
        int point = random_num(0, innerlen);

        for (int j = 0; j < innerlen; j++)
        {
            // First point chroms comes from parent 1
            // if (i * j < point)
            if (j < point)
            {
                offspring1_genes.push_back(m_chromosome[i][j]);
                offspring2_genes.push_back(parent2.m_chromosome[i][j]);
            }
            // Rest come from second parent
            else
            {
                offspring1_genes.push_back(parent2.m_chromosome[i][j]);
                offspring2_genes.push_back(m_chromosome[i][j]);
            }
        }
        offspring1_chrom.push_back(offspring1_genes);
        offspring2_chrom.push_back(offspring2_genes);
    }
    kids.push_back(Individual(offspring1_chrom));
    kids.push_back(Individual(offspring2_chrom));
    return kids;
}

// Calculate fittness score, it is the number of
// characters in map which differ from target
// map.Also check if based on color graph algo we have an invalid color.
int Individual::Calculate_Fitness()
{
    int fitness = 0;

    for (int i = 0; i < target.size(); i++)
    {
        for (int j = 0; j < target[i].size(); j++)
        {
            if (m_chromosome[i][j] != target[i][j])
            {
                fitness++;
                // Uncomment if we want to count fitness score
                // for each pair node that has value 1

                // Left node
                // if (i - 1 >= 0)
                // {
                //     if (m_chromosome[i][j] == m_chromosome[i - 1][j] == 1)
                //         fitness++;
                // }
                // // Right node
                // if (i + 1 <= target.size() - 1)
                // {
                //     if (m_chromosome[i][j] == m_chromosome[i + 1][j] == 1)
                //         fitness++;
                // }
                // // Upper Node
                // if (j - 1 >= 0)
                // {
                //     if (m_chromosome[i][j] == m_chromosome[i][j - 1] == 1)
                //         fitness++;
                // }
                // // Bellow node
                // if (j + 1 <= target[0].size() - 1)
                // {
                //     if (m_chromosome[i][j] == m_chromosome[i][j + 1] == 1)
                //         fitness++;
                // }
            }
        }
    }
    return fitness;
};

// Overloading < operator
bool operator<(Individual &ind1, Individual &ind2)
{
    return ind1.Get_Fitness() < ind2.Get_Fitness();
}

int main()
{

    srand((unsigned)(time(0)));
    // Generetaion counter;
    int generation = 0;

    vector<Individual> population;
    bool completed = false;

    // Create first population individuals
    for (int i = 0; i < population_size; i++)
    {
        vector<vector<int>> chromos = create_chromo();
        population.push_back(Individual(chromos));
    }

    while (!completed)
    {
        // Set population to incresing fitness score order
        sort(population.begin(), population.end());

        // IF the first and our best individual has fitness equal to 0
        // it means we have reached out goal.

        if (population[0].Get_Fitness() <= 0)
        {
            cout << "got fitness 0 and ended" << endl;

            completed = true;
            break;
        }

        // Otherwise generate new offsprings for new generation.
        vector<Individual> new_generation;

        // Perform Elitism, that mean 10% of fittest population
        // goes to the next generation

        // int s = (10 * population_size) / 100;
        // for (int i = 0; i < s; i++)
        //     new_generation.push_back(population[i]);

        // From 50% of fittest population, Individuals
        // will mate to produce offspring.
        int s = (50 * population_size) / 100;
        for (int i = 0; i < s; i++)
        {
            int len = population.size();
            int r = random_num(0, 50);
            Individual parent1 = population[r];
            r = random_num(0, 50);
            Individual parent2 = population[r];
            vector<Individual> offsprings = parent1.Breed(parent2);
            new_generation.push_back(offsprings[0]);
            new_generation.push_back(offsprings[1]);
        }

        population = new_generation;
        // 10% of the population mutate a gene
        s = (10 * population_size) / 100;

        for (int i = 0; i < s; i++)
        {
            int r = random_num(0, 10);
            Individual mutable_individual = population[r];
            mutable_individual.Mutate();
            mutable_individual.Calculate_Fitness();
        }

        for (int i = 0; i < population.size(); ++i)
        {
            population[i].Calculate_Fitness();
        }

        // Every 100 generations print progress
        if (generation % 5 == 1)
        {
            cout << "Generation: " << generation << "\t";
            cout << "Fitness: " << population[0].Get_Fitness() << "\n";
            cout << "Population: " << population.size() << endl;
            population[0].Print_Chromos();
            cout << "\n \n \n \n ";
        }
        generation++;
    }
    cout << "Generation: " << generation << "\t";
    cout << "Fitness: " << population[0].Get_Fitness() << "\n";
    population[0].Print_Chromos();
};