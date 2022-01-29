//
//  main.cpp
//
//  Project: UALR - Programming 2 - Spring 22 - Power Play 2
//  Created by: Mark McCorkle on 20220128
//  Based on: Power Play 1 by Sean Orme
//  IDE: CLion 2021.2.3     - VERIFIED WORKING
//  IDE: XCode              - VERIFIED/UNVERIFIED
//  IDE: Visual Studio 2022 - VERIFIED/UNVERIFIED
//  IDE: Linux via g++      - VERIFIED/UNVERIFIED
//
#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

// Globals:
struct Object
{
    std::string name{ "" };
    int strength{ 0 };
    int health{ 0 };
};
std::random_device seed;
std::default_random_engine engine( seed() );
bool allDead{ false };

// Function Prototypes:
std::vector<Object> loadMonsters( const std::string& );
void displayBattle( const Object&, const std::vector<Object>& );
void monsterAttack( Object&, const std::vector<Object>& );
void playerAttack( const Object&, std::vector<Object>& );
int attack( const Object& );
void defend( Object&, int );
void heal( Object& );

int main()
{
    std::normal_distribution<double> randomHealth(30.0, 5.0);
    std::normal_distribution<double> randomStrength(5.0, 1.0);

    // Create the player object and the monster container:
    Object player
    {
        "Fred",
        std::max( 1, (int)randomStrength(engine) ),
        std::max( 1, (int)randomHealth(engine) )
    };
    std::vector<Object> monsters{ loadMonsters( "monsters.txt" ) };

    // Proper grammar :)
    if( monsters.size() > 1 )
        std::cout << monsters.size() << " monsters approach!!" << std::endl;
    else
        std::cout << monsters.size() << " monster approaches!!" << std::endl;

    while (player.health > 0 && !allDead)
    {

        // Display player and monster stats moved to the displayBattle function:
        displayBattle( player, monsters );

        std::cout << "What do you do? (a)ttack (h)eal ";
        char command{  };
        std::cin >> command;
        switch (command)
        {
            case 'a':
            {
                // Player attack functionality has been moved to the playerAttack function:
                playerAttack( player, monsters );
                break;
            }
            case 'h':
                // Healing functionality has been moved to the heal function:
                heal( player );
                break;
            default:
                std::cout << "please enter a or h" << std::endl;
                break;
        }

        // Monster attack functionality has been moved to the monsterAttack function:
        monsterAttack( player, monsters );

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
void displayBattle( const Object& player, const std::vector<Object>& monsters )
{
    std::cout   << player.name
                << ": "
                << player.health
                << " hp\n"
                << "Monsters: " << std::endl;
    for (int i{ 0 }; i < monsters.size(); i++)
    {
        std::cout << "   " << i + 1 << ". " << monsters[i].name << ": ";
        if (monsters.at(i).health <= 0)
            std::cout << "<DEAD> " << std::endl;
        else
            std::cout << monsters[i].health << " hp\n";
    }
}

// Almost a copy and paste from PP1
void monsterAttack( Object& player, const std::vector<Object>& monsters )
{
    std::bernoulli_distribution willAttack(0.75);
    allDead = true;
    for (const auto& monster : monsters)
    {
        if (monster.health > 0)
        {
            allDead = false;
            if( willAttack(engine) )
            {
                std::cout << monster.name << " attacks!" << std::endl;
                defend( player, attack( monster ) );
            }
            else
            {
                std::cout << monster.name << " twiddles its thumbs" << std::endl;
            }
        }
    }
}

// Almost a copy and paste from PP1
void playerAttack( const Object& player, std::vector<Object>& monsters )
{
    std::cout << "Which Monster: ";
    int monsterNum{ 0 };
    std::cin >> monsterNum;
    if( monsterNum > 0 && monsterNum <= monsters.size() )
    {
        if( monsters.at(monsterNum -1).health > 0 )
            defend( monsters[monsterNum - 1], attack( player ) );
    }
}

// Returns damage done using normal distribution and the passed in object’s strength.
//    I also output the object’s name, along with “ deals “ mean: attacker.strength, stdev: 2.0
int attack( const Object& object )
{
    std::normal_distribution<double> damageDone( object.strength, 2.0 );
    std::cout << object.name << " deals ";
    return std::max( 1, static_cast<int>( damageDone( engine ) ) );
}

// Takes the defending object and the damage being done. subtracts the damage from the defender’s health.
// outputs:
//    damage << " damage to " << object.name << "!!!" << std::endl;
//    The code for a fight looks like this for the player attacking a monster:
//     defend(monsters[monsterNum - 1], attack(player));
void defend( Object& object, int damage )
{
    std::cout << damage << " damage to " << object.name << "!!!" << std::endl;
    object.health -= damage;
}

// Uses a normal distribution with median of strength*2 and standard deviation of 3. adds random amount to object’s
//    health.
void heal( Object& object )
{
    std::normal_distribution<double> healAmount( object.strength * 2.0, 3.0 );
    int healPoints{ std::max( 1, static_cast<int>( healAmount( engine ))) };
    std::cout << object.name << " is healed by " << healPoints << " hp!\n";
    object.health += healPoints;
}