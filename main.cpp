//
//  main.cpp
//
//  Project: UALR - Programming 2 - Spring 22 - Power Play 2
//  Created by: Mark McCorkle on 20220128
//  Based on: Power Play 1 by Sean Orme
//  IDE: CLion 2021.2.3     - VERIFIED
//  IDE: XCode              - VERIFIED/UNVERIFIED
//  IDE: Visual Studio 2022 - VERIFIED/UNVERIFIED
//  IDE: Linux via g++      - VERIFIED/UNVERIFIED
//
/*

    For this PP we will be separating our code out into functions. We will also be changing up two pieces of
    functionality:

    1)  Damage dealt on attacks are based on a normal distribution with the mean being the object’s strength and a
        standard deviation of 2.
    2)  We are loading the monsters from a file. I have a “monsters.txt” file that looks like this:

        3
        Dragon
        5
        50
        Imp
        1
        3
        Orc
        4
        25

    First line is the number of monsters then name, strength and health for each.

    We will be dividing up the code into the following functions (NOTE:: the declarations should be at the top of the
    file and the definitions at the end) (NOTE2:: You must decide return types and parameters!

    This function loads the monsters from the passed in file and returns the vector. You’ll need a for loop.
    loadMonsters();

    Almost a copy and paste from PP1
    displayBattle();

    Almost a copy and paste from PP1
    monsterAttack();

    Almost a copy and paste from PP1
    playerAttack();

    returns damage done using normal distribution and the passed in object’s strength. I also output the object’s name,
    along with “ deals “ mean: attacker.strength, stdev: 2.0
    attack();

    takes the defending object and the damage being done. subtracts the damage from the defender’s health. outputs:
    damage << " damage to " << object.name << "!!!" << std::endl;
    The code for a fight looks like this for the player attacking a monster:
     defend(monsters[monsterNum - 1], attack(player));
    defend();

    uses a normal distribution with median of strength*2 and standard deviation of 3. adds random amount to object’s
    health.
    heal();

 */
// TODO: Alphabetize/order the includes
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <fstream>

struct Object
{
    std::string name;
    int strength{ 0 };
    int health{ 0 };
};

// Function Prototypes:
std::vector<Object> loadMonsters( const std::string& );
void displayBattle(  );
void monsterAttack(  );
void playerAttack(  );
void attack(  );
void defend(  );
void heal(  );

int main()
{
    std::random_device seed;
    std::default_random_engine engine(seed());
    std::normal_distribution<double> randomHealth(30.0, 5.0);
    std::normal_distribution<double> randomStrength(5.0, 1.0);

    Object player
            {
                    "Mr. Orme",
                    std::max(1, (int)randomStrength(engine)),
                    std::max(1, (int)randomHealth(engine))
            };
    std::vector<Object> monsters{ loadMonsters( "monsters.txt" ) };

    std::uniform_int_distribution<int> randomNumMonsters(1, 3);

    int numMonsters{ randomNumMonsters(engine) };
    for (int i{ 0 }; i < numMonsters; i++)
    {
        monsters.push_back({
                                   "monster " + std::to_string(i + 1),
                                   std::max(1, (int)randomStrength(engine)),
                                   std::max(1, (int)randomHealth(engine))
                           });
    }

    std::cout << numMonsters << " monster(s) approaches!!" << std::endl;
    bool allDead{ false };
    while (player.health > 0 && !allDead)
    {

        std::cout << player.name << ": " << player.health << std::endl
                  << "  Monsters: " << std::endl;
        for (int i{ 0 }; i < monsters.size(); i++)
        {
            std::cout << "   " << i + 1 << ". " << monsters[i].name << ": ";
            if (monsters.at(i).health <= 0)
                std::cout << "<DEAD> " << std::endl;
            else
                std::cout << monsters[i].health << std::endl;
        }

        std::cout << "What do you do? (a)ttack (h)eal ";
        char command{  };
        std::cin >> command;
        switch (command)
        {
            case 'a':
            {
                std::cout << "Which Monster: ";
                int monsterNum{ 0 };
                std::cin >> monsterNum;
                if (monsterNum > 0 && monsterNum <= monsters.size())
                {
                    if(monsters.at(monsterNum -1).health >0)
                        monsters[monsterNum - 1].health -= player.strength;
                }
                break;
            }
            case 'h':
                player.health += player.strength * 2;
                break;
            default:
                std::cout << "please enter a or h" << std::endl;
                break;
        }


        std::bernoulli_distribution willAttack(.75);
        allDead = true;
        for (const auto& monster : monsters)
        {
            if (monster.health > 0)
            {
                allDead = false;
                if (willAttack(engine))
                {
                    std::cout << monster.name << " attacks!" << std::endl;
                    player.health -= monster.strength;
                }
                else
                {
                    std::cout << monster.name << " twiddles its thumbs" << std::endl;
                }
            }
        }

        system("PAUSE");
        system("CLS");
    }

    if (player.health <= 0)
    {
        std::cout << "You Have Died" << std::endl;
    }
    if (player.health <= 0 && allDead)
    {
        std::cout << "BUT" << std::endl;
    }
    if (allDead)
    {
        std::cout << "You have killed the monsters!!!" << std::endl;
    }
    system("PAUSE");
}

// This function loads the monsters from the passed in file and returns the vector. You’ll need a for loop.
std::vector<Object> loadMonsters( const std::string& fileName )
{
    // Create the vector of monsters:
    std::vector<Object> monsters;

    // Create the input file stream object:
    std::ifstream fin;

    // Open the file for input:
    fin.open( fileName );

    // Import the data from the file:
    if(!fin.is_open())
        std::cout << "[ERROR]: File not found!" << std::endl;
    else
    {
        // First line is the number of monsters then name, strength and health for each.
        int numMonsters{ 0 };           // Variable to hold the number of monsters
        fin >> numMonsters;             // Retrieve the number of monsters
        for( size_t i{ 0 }; i < numMonsters; ++i )
        {
            // Add a blank monster to avoid the use of a temp object
            monsters.push_back( {} );

            // Assign the values to the previously defined monster object
            fin >> monsters.back().name;
            fin >> monsters.back().strength;
            fin >> monsters.back().health;
        }
    }

    // Close the file object:
    fin.close();

    // Return the reference to the monsters vector back to the calling function:
    return monsters;
}

// Almost a copy and paste from PP1
void displayBattle()
{

}

// Almost a copy and paste from PP1
void monsterAttack()
{

}

// Almost a copy and paste from PP1
void playerAttack()
{

}

// Returns damage done using normal distribution and the passed in object’s strength.
//    I also output the object’s name, along with “ deals “ mean: attacker.strength, stdev: 2.0
void attack()
{

}

// Takes the defending object and the damage being done. subtracts the damage from the defender’s health. outputs:
//    damage << " damage to " << object.name << "!!!" << std::endl;
//    The code for a fight looks like this for the player attacking a monster:
//     defend(monsters[monsterNum - 1], attack(player));
void defend()
{

}

// Uses a normal distribution with median of strength*2 and standard deviation of 3. adds random amount to object’s
//    health.
void heal()
{

}