/*
 ============================================================================
 Name        : D&DK.c
 Author      : Kaden Boam
 Version     : 3.0.1
 Description : A RPG game in C++
 ============================================================================
 */

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;
/* ((rand() % 6) + 1) random number between 1 and 6 */

typedef struct stats {
	char name[20];
	char inventory[10][20]; /* 10 items, each with a max length of 20 characters */
	char titles[10][20]; /* 10 titles, each with a max length of 20 characters */
    /* Progression exp */
    int exp;
    int exp_gain; /* experience gain */
    /* Exp for the next level */
    int exp_to_next_level;
    /* The different stats, to establish them */
    int strength;
    int intelleigence;
    int agility;
    int luck;
    int skill;
    int stamina;
    int health;
    int magic;
    int phy_def;
    int mg_def; /* externally infulenced */
    int gold;
    int level;
    int level_exp; /* experience needed for the next level */
    int defense; /* total defense */
    int physical; /* total physical attack */
    int mental; /* total mental attack */
    int mp; /* magic points */
    int max_hp; /* maximum health points */
    int hp; /* health points */
    int experience_to_next_level; /* experience needed for the next level */
    struct stats *next;
} stats;

int stats_reset(stats *entity);
int stats_define(stats *entity);
int display_stats(stats *entity);
int create_entity(stats **head, int first_entity);
int fight_monsters(stats *monsters[], int *counts, stats *character);
int define_monsters(stats *monsters[], int *counts);
void attack(stats *character, stats *target);
int run(stats *character, stats *monsters[], int counts[]);
void identify(stats *monsters[], int counts[]);

int main(void) {
	srand(time(NULL)); /* seed the random number generator */
	while (true){
		stats* head = new stats{}; /* allocate memory for the new account */;
		head->next = NULL; /* set the next pointer to NULL */

		cout << "This is a simple text-based RPG game." << "\n";
		cout << "You will be able to create characters, allocate stats, and fight monsters." << "\n";
		cout << "Let's get started!" << "\n";
		cout << "What is your name? " << "\n";

		stats_reset(head); /* reset the stats of the first entity */
		stats_define(head);
		stats *temp = head; // Pointer for traversal
		stats *entity = head; /* create a pointer to the head of the stats linked list */
		cin >> entity->name; /* takes the users name */
		cout << "Welcome " << entity->name << "! Let's create your character." << "\n";
		stats *character = head; /* create a pointer to the head of the stats linked list */
		display_stats(character); /* display the stats of the character */
		cout << "Your character has been created!" << "\n";
		stats *monsters[6];
		for (int i = 0; i < 6; i++) {
			create_entity(&head, 1);
			temp = temp->next;
			monsters[i] = temp;
		}

		while (true){
			if (character->exp >= character->level_exp) {
				character->level += 1; /* level up */
				character->level_exp = character->level_exp * 7 / 4; /* increase exp needed for next level */
				cout << "Congratulations! You leveled up to level " << character->level << "!" << "\n";
				stats_define(character); /* define new stats for the character */
			}
			cout << "Now here are your options!" << "\n";
			cout << "1. Fight Monsters" << "\n";
			cout << "2. Review character stats" << "\n";
			cout << "3. Rest" << "\n";
			cout << "4. Exit game" << "\n";
			cout << "5. Settings" << "\n";
			int choice;
			cin >> choice; /* takes the users choice */
			switch (choice) {
				case 1:{
					int gn = 0; /* goblin count */
					int sn = 0; /* slime count */
					int en = 0; /* serpent count */
					int rn = 0; /* rabbit count */
					int qn = 0; /* squirrel count */
					int pn = 0; /* spider count */
					int counts[6] = {gn, sn, en, rn, qn, pn};
					fight_monsters(monsters, counts, character);/* function to fight monsters */
					character->exp += 1; /* gain experience */
					break;
				}
				case 2:{
					cout << "Here are your character stats:" << "\n";
					display_stats(character);
					break;
				}
				case 3:{
					cout << "You chose to rest!" << "\n";
					character->exp += 1; /* example experience gain */
					// Add rest logic here
					break;
				}
				case 4:{
					cout << "Exiting game..." << "\n";
					return 0; /* exit the game */
				}
				case 5:{
					cout << "Settings:" << "\n";
					cout << "1. Reset Game" << "\n";
					cout << "2. Cancel" << "\n";
					cin >> choice; /* takes the users choice */
					switch (choice) {
					case 1:
						cout << "Are you sure you want to reset the game? (1 for Yes, 0 for No)" << "\n";
						cin >> choice; /* takes the users choice */
						if (choice == 1) {
							cout << "Resetting game..." << "\n";
							break; /* break out of the settings menu to reset the game */
						}
					}
					break;
				}
			default:
				cout << "Invalid choice, please try again." << "\n";
			}
		}
		stats* cur = head; /* gets rid of what was had */
		while (cur) {
			stats* next = cur->next;
			delete cur;
			cur = next;
		}
	}
	return EXIT_SUCCESS;
}

int stats_reset(stats *entity) {
    /* Progression exp */
    entity->exp = 0;
    entity->exp_to_next_level = 10; /* Set reasonable default */

    /* The different stats, give minimum values */
    entity->strength = (rand() % 3) + 1;
    entity->intelleigence = (rand() % 3) + 1;
    entity->agility = (rand() % 3) + 1;
    entity->luck = (rand() % 3) + 1;
    entity->skill = (rand() % 3) + 1;
    entity->stamina = (rand() % 3) + 1;
    entity->health = (rand() % 3) + 5;
    entity->magic = (rand() % 3) + 1;
    entity->phy_def = (rand() % 2) + 1;
    entity->mg_def = (rand() % 2) + 1;

    entity->gold = (rand() % 10);
    entity->level = 1; /* start at level 1 */
    entity->level_exp = 10; /* Adjusted for better progression */

    return 0;
}

int create_entity(stats **head, int first_entity) {
	stats* new_entity = new stats{};
    if (!new_entity) {
        cout << "Memory allocation failed" << "\n";
        return -1;
    }

    // Initialize the entity
    new_entity->next = NULL;
    if (*head == NULL) {
        // First entity case
        *head = new_entity;
    } else {
        // Traverse to the last node
        stats *temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_entity;  // Link the new entity correctly
    }

    stats_define(new_entity); // Set stats for the new entity
    return 0;
}

int stats_define(stats *entity) {
	/* this was a cool find to shorten the code */
	int *stats[] = { &entity->strength, &entity->intelleigence,
			&entity->agility, &entity->luck, &entity->skill, &entity->stamina,
			&entity->health, &entity->magic, &entity->phy_def, &entity->mg_def };
	for (int i = 0; i < 10; i++) {
		*stats[i] += ((rand() % 6) + 1) * entity->level;
	}
	entity-> defense = entity->phy_def + entity->mg_def; /* calculate defense */
	entity-> physical = entity->strength + entity->agility + entity->stamina + entity->health; /* calculate physical attack */
	entity-> mental = entity->intelleigence + entity->luck + entity->skill + entity->magic; /* calculate mental attack */
	entity->mp = entity->magic + entity->intelleigence; /* calculate magic points */
	entity->max_hp = entity->stamina + entity->health; /* calculate maximum health points */
	entity->hp = entity->max_hp; /* set health points to maximum */
	entity->experience_to_next_level = entity->level_exp - entity->exp; /* experience needed for the next level */
   return 0;
 }

int display_stats(stats *entity){
	int *stats[] = { &entity->strength, &entity->intelleigence,
				&entity->agility, &entity->luck, &entity->skill, &entity->stamina,
				&entity->health, &entity->magic, &entity->phy_def, &entity->mg_def };
	const char *stat_names[] = { "Strength", "Intelligence", "Agility", "Luck",
	                             "Skill", "Stamina", "Health", "Magic",
	                             "Physical Defense", "Magic Defense" };
	for (int i = 0; i < 10; i++) {
	    cout << stat_names[i] << ": " << *stats[i] << "\n";
	}

	return 0;
}



int define_monsters(stats *monsters[], int *counts) {
    cout << "Defining monsters..." << "\n";
    int number_of_monsters = ((rand() % 6) + 1); /* example number of monsters */

    for (int i = 0; i < number_of_monsters; i++) {
        int monster_index = (rand() % 6); /* Pick a random monster type */

        if (!monsters[monster_index]) {
			monsters[monster_index] = new stats{};
            if (!monsters[monster_index]) {
                cout << "Memory allocation failed for monster." << "\n";
                return -1;
            }

            /* Assign the name based on the monster type */
            switch (monster_index) {
            	/* Learned that sn printf is a safe alternatice to printf that allows formatting and prevents buffer overflows */
                case 0: snprintf(monsters[monster_index]->name, 20, "Goblin"); break;
                case 1: snprintf(monsters[monster_index]->name, 20, "Slime"); break;
                case 2: snprintf(monsters[monster_index]->name, 20, "Serpent"); break;
                case 3: snprintf(monsters[monster_index]->name, 20, "Rabbit"); break;
                case 4: snprintf(monsters[monster_index]->name, 20, "Squirrel"); break;
                case 5: snprintf(monsters[monster_index]->name, 20, "Spider"); break;
            }
        }

        counts[monster_index] += 1; /* Increment count */
        stats_define(monsters[monster_index]); /* Define stats */
    }

    return 0;
}


int fight_monsters(stats *monsters[], int *counts, stats *character) {
    cout << "You chose to fight monsters!" << "\n";
    define_monsters(monsters, counts);
    cout << "There are " << counts[0] << " goblins, " << counts[1] << " slimes, " << counts[2] << " serpents, " << counts[3] << " rabbits, " << counts[4] << " squirrels, and " << counts[5] << " spiders." << "\n";
    int fight = 8; /* infinite fight */
    while (fight == 8){
		cout << "What would you like to do?" << "\n";
		cout << "1. Fight a monster" << "\n";
		cout << "2. Check Monster stats" << "\n";
		cout << "3. Run away" << "\n";
		int choice;
		cin >> choice; /* takes the users choice */
		switch (choice) {
		case 1:
		    cout << "You chose to fight a monster!" << "\n";
		    cout << "Which monster would you like to fight?" << "\n";
		    cout << "0: Goblin (" << counts[0] << "), 1: Slime (" << counts[1] << "), 2: Serpent (" << counts[2] << "), 3: Rabbit (" << counts[3] << "), 4: Squirrel (" << counts[4] << "), 5: Spider (" << counts[5] << ")" << "\n";

		    int monster_choice;
		    cin >> monster_choice; /* takes the user's choice */

		    if (monster_choice >= 0 && monster_choice < 6 && counts[monster_choice] > 0) {
		        attack(character, monsters[monster_choice]); /* Correct call */
		    } else {
		        cout << "Invalid choice or no monsters of that type remaining." << "\n";
		    }
		    break;
		case 2:
			identify(monsters, counts);
			cout << "Displaying monster stats" << "\n";
			break;
		case 3:
			cout << "You chose to run away!" << "\n";
			run(character,monsters, counts);
			character->exp_gain += 1; /* example experience gain */
			break;
		default:
			cout << "Invalid choice, please try again." << "\n";
		}
		if (character->hp <= 0) {
			cout << "You have been defeated! Game over." << "\n";
			fight = 0; /* exit the fight */
			return 0;
		}
		if (counts[0] == 0 && counts[1] == 0 && counts[2] == 0 && counts[3] == 0
				&& counts[4] == 0 && counts[5] == 0) {
			cout << "All monsters defeated! You win!" << "\n";
			fight = 0; /* exit the fight */
			character->exp_gain += 10; /* example experience gain */
			return 0;
		}
    }
    return 0;
}

void attack(stats *character, stats *target) {
    cout << "You attack the " << target->name << "!" << "\n";

    int defense_roll = target->phy_def + ((rand() % 6) + 1);
    int attack_roll = character->strength + ((rand() % 6) + 1);

    if (target->agility + ((rand() % 6) + 1) > character->agility + ((rand() % 6) + 1)) {
        cout << "The " << target->name << " dodged!" << "\n";
    } else if (defense_roll > attack_roll) {
        cout << "You hit, but the " << target->name << " defended itself." << "\n";
    } else if (target->intelleigence + ((rand() % 6) + 1) > character->intelleigence + ((rand() % 6) + 1)) {
        cout << "It outsmarted you! You lost 1 HP..." << "\n";
        character->hp -= 1; // fix this to apply damage properly
    } else {
        int damage = attack_roll - defense_roll; // fix this to calculate damage properly
		if (damage < 0) damage = 0;
        cout << "You strike the " << target->name << ", dealing " << damage << " damage!" << "\n";
        target->hp -= damage; // fix this to apply damage properly
    }
}

int run(stats *character, stats *monsters[], int counts[]) {
    for (int i = 0; i < 6; i++) {
        if (counts[i] > 0) {
            int escape_chance = (character->agility + ((rand() % 6) + 1)) > (monsters[i]->agility + ((rand() % 6) + 1));
            int intelligence_check = (character->intelleigence + ((rand() % 6) + 1)) > (monsters[i]->intelleigence + ((rand() % 6) + 1));

            if (escape_chance || intelligence_check) {
                cout << "You escaped the " << monsters[i]->name << "!" << "\n";
                counts[i] = 0;
            } else {
                cout << "You couldn't escape from the " << monsters[i]->name << "!" << "\n";
            }
        }
    }
    return 0;
}

void identify(stats *monsters[], int counts[]) {
    if (((rand() % 6) + 1) < 3) {
        for (int i = 0; i < 6; i++) {
            if (counts[i] > 0) {
                cout << monsters[i]->name << ":\n";
                display_stats(monsters[i]);
                cout << "" << "\n";
            }
        }
    } else {
        cout << "Identification has failed." << "\n";
    }
}
