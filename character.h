#ifndef character
#define character

const int MAX_PLAYER_LEVEL = 100;

struct Character{

    string name;

    int level;
    float experience;

    int health; //the max health
    int cur_health; //the stored current health <= health
    int fatigue; //for training, if too high cannot train anymore
    int gold;

    //player inventory, include potion, equips, etc
    Item inventory[10];
    int cur_item_count;

    //the equipped equipment from inventory, store the index only cuz reference to the index in the inventory
    int holding_equip;

    Skill learned_skill[2];

    //buff
    Buff buff_list[3];
    int cur_buff_count;

    //game turn
    int remaining_turn;

};

Character new_character(){

    Character player;

    //initialise new created player data
    player.level = 1;
    player.experience = 0;
    player.health = 100;
    player.cur_health = 100;
    player.fatigue = 0;
    player.gold = 0;
    player.cur_item_count = 0;
    player.cur_buff_count = 0;
    player.remaining_turn = 30; 
    player.holding_equip = -1; //set to -1 to indicate no weapon holding

    write("What's your name? ");
    player.name = read_line();

    string input;
    bool out_of_range = false;

    string beginner_weapon[] = {"Beginner Sword", "Blunt Dagger", "Beginner Wand", "Wooden Bow"};

    do{

        write_line("Weapon :");
        for(int i=0; i<4; i++){

            write_line("("+std::to_string(i+1)+") "+ beginner_weapon[i]);

        }
        
        write("Select a weapon: ");

        input = read_line();
        out_of_range = false;

        if(!is_integer(input)
            || (std::stoi(input) < 1 && std::stoi(input) > 4)){

            write_line("Invalid option. Please try again.");

            out_of_range = true;

        }

        Item new_item = generate_item((ItemType)1, beginner_weapon[(std::stoi(input)-1)], 1, 1, 10, 10);
        player.inventory[0] = new_item;
        player.cur_item_count++;

    }while(out_of_range);

    return player;

}

#endif