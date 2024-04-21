#ifndef game
#define game

bool quitGame = false;
bool player_die = false;

const string MENU[]={"Home","Inventory","Training","Explore","Finish the day!","Next Page", "Stats","Shopping", "Save game","Load game","Quit game","Previous Page"};
const int PAGE_OFFSET = 6;
int cur_page;

Character player;

void main_menu();
void new_game();
void start_game();
void check_stat();
void finish_the_day();
void explore();
void changed_fatigue(int targetFatigue, string msg);
void encounter_mob(const int& targetAreaIndex, string& reward_list);
void mob_attack(const string& mob_name, const int& mob_strength);
bool check_equipped_weapon();
void gain_exp();
void training();
void show_inventory();
void select_inventory(const int& targetIndex);
void apply_buff(int targetBuff);
void level_up(int targetLevel);
void changed_health(int targetAmount, string msg);
void go_home();
void save_game();

void main_menu(){

    write_line("Welcome to TaskD4 Adventure Story!");
    
    string input;
    bool out_of_range = false;

    do{

        write_line("(1) New game");
        write_line("(2) Load game");
        write_line("(3) Quit game");

        write("Please select an option: ");

        input = read_line();
        out_of_range = false;

        if(!is_integer(input)
            || (std::stoi(input) < 1 || std::stoi(input) > 3)){

            write_line("Invalid option. Please try again.");

            out_of_range = true;

        }else{


            switch(std::stoi(input)){
                case 1:
                    new_game();
                    break;
                case 3:
                   
                    write_line("See ya");
                    
                    quitGame = true;
                   
                    break;
                default:
                    write_line("work in progress...");
                    break;
            }

        }

    }while(out_of_range);

}

void new_game(){

    write_line("New game!");

    //create a new character
    player = new_character();

    write_line("Enjoy your journey!");
    
    start_game();

}

void start_game(){

    string input;

    while(!quitGame){

        write_line("Day ["+ std::to_string(31-player.remaining_turn) +"] What would you like to do?");

        //paging system, cuz there are too many selection
        int start = cur_page * PAGE_OFFSET;
        int end = start + PAGE_OFFSET;

        for(int i=start;i<end;i++){
            write_line("("+ std::to_string(i+1) +") " + MENU[i]);
        }
        /*write_line("(1) Home");
        write_line("(2) Inventory");
        write_line("(3) Training");
        write_line("(4) Explore");
        write_line("(5) Finish the day!");
        write_line("(6) Next page");*/
        /*write_line("(7) Stats");
        write_line("(8) Shopping");
        write_line("(9) Save game");
        write_line("(10) Load game");
        write_line("(11) Quit game");
        write_line("(12) Previous page");*/
        write("Select an option: ");

        input = read_line();
        //out_of_range = false;

        while(!is_integer(input)
            || (std::stoi(input) < 1 || std::stoi(input) > 12)){

            write_line("Invalid option. Please try again.");

            write("Select an option: ");
            input = read_line();

        }
        
        //if you know the menu number then can direct input the number, not restricted to certain page
        switch(std::stoi(input)){
            case 1:
                //home, go home fully recover health
                go_home();
                break;
            case 2: 
                //inventory
                show_inventory();
                break;
            case 3:
                //training
                training();
                break;
            case 4:
                //explore
                explore();
                break;
            case 6:
                
                //right now only has two pages, if there are more selection then need use calculation to calculate the page
                if(cur_page != 1){
                    cur_page = 1;
                }

                break;
            case 7:
                //check stat
                check_stat();
                break;
            case 11:
                
                write_line("See ya");
                
                quitGame = true;
                
                break;
            case 12:

                if(cur_page != 0){
                    cur_page = 0;
                }

                break;
            default:
                write_line("work in progress...");
                break;
        }

    }

}

//print player stat 
void check_stat(){

    write_line("Your stat: ");
    write_line("Name: "+ player.name);
    write_line("Level: "+ std::to_string(player.level));
    write_line("Experience: "+ std::to_string(player.experience));
    write_line("Health: "+ std::to_string(player.cur_health));
    write_line("Fatigue: "+ std::to_string(player.fatigue));
    write_line("Gold: "+ std::to_string(player.gold));
    write_line("Day: "+ std::to_string(player.remaining_turn));

    write_line("Press 0 to go back");
    string input;

    input = read_line();

    while(!is_integer(input) || std::stoi(input) != 0){

        write_line("Invalid input. Try again.");

        input = read_line();

    }

}

void finish_the_day(){

    write_line("The day is finished. Let's go to sleep");
    save_game();



}

//explore, will get random reward
void explore(){

    if(!check_equipped_weapon()){
        write_line("You don't have a weapon yet.");

        return;
    }

    if(player.health <=0){
        
        write_line("You are in bad condition for exploration.");

        return;

    }

    string input;
    bool out_of_range = false;

    string area[] = { "Garden", "Forest", "Mountain", "Swamp", "Cave" };

    write_line("Let's go explore!");
    write_line("Area: ");
    for(int i=0; i<5; i++){

        write_line("("+std::to_string(i+1)+")"+ area[i]);

    }
    
    write("Choose an area to explore: ");
    input = read_line();

    while(!is_integer(input) || (std::stoi(input) < 1 || std::stoi(input)>5)){

        write_line("Invalid input. Try again.");

        write("Choose an area to explore: ");
        input = read_line();

    }

    int targetIndex = std::stoi(input)-1;
    string reward_list ="";

    int mob_encounter_counter = 0;
    //generate mob encounter counter, the difficulty increase in ascending order for area
    switch(targetIndex){
        case 0:
            //garden
            mob_encounter_counter = rnd(1,4);
            break;
        case 1:
            //forest
            mob_encounter_counter = rnd(3,5);
            break;
        case 2:
            //mountain
            mob_encounter_counter = rnd(4,8);
            break;
        case 3:
            //swamp
            mob_encounter_counter = rnd(5,9);
            break;
        case 4:
            //cave
            mob_encounter_counter = rnd(6,10);
            break;
    }

    int counter = mob_encounter_counter;
    
    while(counter > 0){

    
        write("Exploring "+area[targetIndex]);

        for(int i=0; i<5; i++){

            delay(700);
            write(".");

        }

        encounter_mob(targetIndex, reward_list);

        if(player_die){
            break;
        }

        counter--;

    }

    //exploration done, print the reward list, the item already stored inside player inventory, so here only print the list
    write_line("Exploration done. You received :");
    if(reward_list==""){
        write_line("Nothing.");
    }
    else{
        write_line(reward_list);
    }

}

void encounter_mob(const int& targetAreaIndex, string& reward_list){

    string mob_garden[] = {"Mushroom", "Butterfly", "Caterpillar", "Cocoon", "Bee", "Pig", "Slime", "Duck", "Chicken", "Milk Cow"};

    string mob_forest[] = {"Spider", "Stump", "Hornet", "Evil Slime", "Fairy","Birdie","Ox", "Angry Ox", "Dog", "Cat"};

    string mob_mountain[] = {"Lion", "Wild Boar", "Tiger", "Mutant Pig","Mutant Slime", "Snake", "Goat", "Anonymous Species-A", "Eagle"};

    string mob_swamp[] = {"Alligator", "Evil Eye", "Bat", "Robot", "Parasite", "Anonymous Species-B", "Mermaid", "Gold Fish", "Witch", "Poison Mushroom"};

    string mob_cave[] = {"Balrog", "Golem", "Taurus", "Centipede", "Ghost", "Anonymous Species-X", "Dracula", "Zombie", "Clown","Mutant Spider"};

    int rand = rnd(0,9);

    //get mob detail
    string mob; 
    int mob_hp;
    int escape_factor;
    int mob_strength; //mob atk power

    switch(targetAreaIndex){
        case 0:
            mob = mob_garden[rand];
            mob_hp = rnd(10,15);
            escape_factor = 95; //95
            mob_strength = rnd(5,10);
            break;
        case 1:
            mob = mob_forest[rand];
            mob_hp = rnd(30,55);
            escape_factor = 85; //85
            mob_strength = rnd(15,25);
            break;
        case 2:
            mob = mob_mountain[rand];
            mob_hp = rnd(60,120);
            escape_factor = 75;
            mob_strength = rnd(32,45);
            break;
        case 3:
            mob = mob_swamp[rand];
            mob_hp = rnd(100,300);
            escape_factor = 65;
            mob_strength = rnd(55,60);
            break;
        case 4:
            mob = mob_cave[rand];
            mob_hp = rnd(400,600);
            escape_factor = 50;
            mob_strength = rnd(65,80);
            break;
    }
    
    string input;

    write_line("Watch out! A "+ mob + " has appeared. HP["+std::to_string(mob_hp)+"/"+std::to_string(mob_hp)+"]");
    write_line("Action: ");
    write_line("(1) Fight");
    write_line("(2) Run");
    write("What do you want to do? ");

    input = read_line();

    while(!is_integer(input) || (std::stoi(input)<1 || std::stoi(input)>2)){

        write_line("Invalid input. Try again.");

        write("What do you want to do? ");
        
        input = read_line();

    }

    
    if(std::stoi(input) == 2){

        //player try to escape the fight, based on escape factor
        int esc_rand = rnd(0,100);
        if(esc_rand >= escape_factor){


            write_line("You escaped the fight.");

            return;

        }
        else{

            //escape failed. mob attack you
            write_line("Escape failed");
            mob_attack(mob, mob_strength);

        }
    }
    else{
        
        //fight
        //for WIP purpose, directly kill the mob
        int gold_rnd = rnd(10,30);

        write_line("You attack the "+mob+". "+mob+" damaged by "+ std::to_string(999999));
        write_line("Yay! You destroyed the monster "+mob+". You got "+std::to_string(gold_rnd)+" golds and a Red Potion");

        if(player.cur_item_count < 2){
            Item reward_item;
            reward_item.title = "Red Potion";
            reward_item.amount = 1;
            player.inventory[1] = reward_item;

        }
        else{

            player.inventory[1].amount += 1;

        }

        player.gold += gold_rnd;

        reward_list = (std::to_string(gold_rnd)+" golds, Red Potion x 1");

    }
}

void mob_attack(const string& mob_name, const int& mob_strength){

    int health_left = player.cur_health - mob_strength;
    if(health_left <= 0){
        health_left = 0;
    }

    write_line(mob_name+" attacked you. You damaged by "+ std::to_string(mob_strength)+". HP["+std::to_string(health_left)+"/"+std::to_string(player.health)+"]");

    changed_health(-mob_strength, "decreased");

}

bool check_equipped_weapon(){

    return player.holding_equip != -1;

}

//generage random experience from training
void training(){

    if(!check_equipped_weapon()){

        write_line("You don't have a weapon equipped yet.");

        return;

    }

    //check if too tired
    if(player.fatigue >= 100 || player.cur_health <=0){

        write_line("You are too tired to train.");

        return;

    }

    write_line("Let's train!");
    write("Training");

    for(int i=0; i<5; i++){

        delay(700);
        write(".");

    }


    write_line("");

    gain_exp();

    //gain fatigue
    int cur_level = player.level;
    int fatigue = 0;
    if(cur_level <=20){
        fatigue = rnd(1,3);
    }
    else if(cur_level <= 40){
        fatigue = rnd(4,6);
    }
    else if(cur_level <= 60){
        fatigue = rnd(13,19);
    }
    else if(cur_level <= 80){
        fatigue = rnd(32,40);
    }

    changed_fatigue(fatigue, "increased");

}

void changed_fatigue(int targetFatigue, string msg){

    player.fatigue += targetFatigue;

    if(player.fatigue >= 100){
        player.fatigue = 100;
    }
    else if(player.fatigue <= 0){
        player.fatigue = 0;
    }

    write_line("Your fatigue has "+msg+" by " + std::to_string(targetFatigue)+". ("+std::to_string(player.fatigue)+"/100)");

}

void gain_exp(){

    //gain exp, higher level lower exp
    int cur_level = player.level;
    float factor;
    if(cur_level <=20){
        factor = 0.4f;
    }
    else if(cur_level <= 40){
        factor = 0.3f;
    }
    else if(cur_level <= 60){
        factor = 0.2f;
    }
    else if(cur_level <= 80){
        factor = 0.1f;
    }

    float rand = rnd();

    while(rand <= 0){

        rand = rnd();

    }

    float exp = rand * 100 * factor;

    player.experience += exp;
    bool leveling = false;

    if(player.experience > 100){

        //level up
        player.experience = 0;
        leveling = true;

    }

    write_line("You gained "+std::to_string(exp)+" experience. ("+std::to_string(player.experience)+"/100)");
    
    if(leveling){

        level_up(1);

    }
}

//show inventory and perform action
void show_inventory(){

    string input;
    bool out_of_range = false;

    write_line("Your invetory: ");

    int count = player.cur_item_count;
    for(int i=0; i<count;i++){

        bool holding_equip = player.holding_equip == i;

        write_line("("+ std::to_string(i+1) +") "+ player.inventory[i].title + " x " + std::to_string(player.inventory[i].amount) + (holding_equip? " (equipped)":""));

    }

    write_line("Type '0' to go back.");
    write("Please select an option: ");

    input = read_line();

    while(!is_integer(input) || (std::stoi(input)<0 || std::stoi(input)>count)){
        
        write_line("Invalid option. Please try again.");
        write("Please select an option: ");

        input = read_line();

    }

    //first check if player wants to go back
    if(std::stoi(input) == 0){
        
        start_game();
        
        return;

    }

    //perform action to selected target item
    int targetIndex = std::stoi(input) - 1;
    select_inventory(targetIndex);

}

//eg similar function as deleting item in array, perform shifting
void update_inventory(const int& targetIndex){

    int count = player.cur_item_count;

    for(int i=0; i<count;i++){

        if(i < targetIndex){

            continue;

        }

        player.inventory[i] = player.inventory[i+1];

        //need to update player holding equip index as well
        if(player.holding_equip == i){

            player.holding_equip = i+1;

        }

    }

    player.cur_item_count -= 1;

}

//potion, equips
void select_inventory(const int& targetIndex){

    Item targetItem = player.inventory[targetIndex];
    int amount = targetItem.amount;
    string input;
    bool try_again = false;

    while(true){

        write_line("Item: " + targetItem.title +", amount: " + std::to_string(amount));

        switch(targetItem.item_type){
            case 0:
                //consumable item
                
                do{
                    
                    write("Consume? [y/n] ");

                    input = read_line();
                    try_again = false;

                    if(is_integer(input) || (to_lowercase(input) != "y" && to_lowercase(input) != "n")){

                        write_line("Invalid input. Try again.");

                        try_again = true;

                    }
                    else{

                        if(to_lowercase(input) == "n"){
                            
                            //back to inventory
                            show_inventory();

                            return;

                        }

                        if(targetItem.title == "Red potion"){
                            changed_health(20, "increased");
                        }
                        else if(targetItem.title == "Rare potion"){
                            level_up(1);
                        }
                        else if(targetItem.title == "Attack potion"){
                            apply_buff(0);
                        }
                        else if(targetItem.title == "Defense potion"){
                            apply_buff(1);
                        }
                        else if(targetItem.title == "Speed potion"){
                            apply_buff(2);
                        }

                        amount--;
                        player.inventory[targetIndex].amount = amount;
                    
                        if(amount <= 0){

                            //the consumable item has no more
                            update_inventory(targetIndex);

                        }

                    }

                }while(try_again);
                

                break;
            case 1:
                //equipment
                
                do{
                    
                    write("Equip? [y/n] ");

                    input = read_line();
                    try_again = false;

                    if(is_integer(input) || (to_lowercase(input) != "y" && to_lowercase(input) != "n")){

                        write_line("Invalid input. Try again.");

                        try_again = true;

                    }
                    else{
                        
                        if(to_lowercase(input) == "n"){
                            
                            //check if already equipped, if yes then unequipped
                            if(targetIndex == player.holding_equip){
                                //already equipped
                            
                                player.holding_equip = -1;

                            }

                            //back to inventory
                            show_inventory();

                            return;

                        }

                        player.holding_equip = targetIndex;

                        write_line("You changed your weapon to "+ targetItem.title);
                        show_inventory();

                        return;

                    }

                }while(try_again);

                break;

        }

    }

}

void apply_buff(int targetBuff){

    //first need to check if player has any slot for buff
    int cur_buff_count = player.cur_buff_count;
    if(cur_buff_count >= 3){

        //max amount of buff, cannot apply anymore
        return;

    }

    Buff new_buff;

    switch(targetBuff){
        case 0:
            new_buff.buff_type = (BuffType)0;
            new_buff.remaining_turn = 5;
            break;
        case 1:
            new_buff.buff_type = (BuffType)1;
            new_buff.remaining_turn = 6;
            break;
        case 2:
            new_buff.buff_type = (BuffType)2;
            new_buff.remaining_turn = 4;
            break;
    }

    player.buff_list[cur_buff_count] = new_buff;

    cur_buff_count++;

}

void level_up(int targetLevel){
    
    player.level += targetLevel;

    if(player.level > MAX_PLAYER_LEVEL){

        player.level = MAX_PLAYER_LEVEL;

    }

    write_line("You leveled up by 1 level.");

}

void changed_health(int targetAmount, string msg){

    player.cur_health += targetAmount;

    if(msg== "increased" && player.cur_health > player.health){

        player.cur_health = player.health;

        write_line("Your health has "+ msg +" by 20.");
    }
    else if(msg=="decreased" && player.cur_health <= 0){

        //player fainted, back to main menu, miss one turn
        write_line("You lost and ran out of health.");
        write_line(".........................");
        
        player_die = true;
        player.cur_health = 0; //regenerate a little health

    }


}

//regenerate health at home
void go_home(){

    player.cur_health = player.health;

    write_line("You go home and have a nice rest.");
    write("Sleeping");
    
    for(int i=0; i <5; i++){

        delay(700);
        write(".");

    }

    write_line("");
    write_line("You have recovered your health back to " + std::to_string(player.cur_health) + ".");

}

void save_game(){

    write("Saving");

    for(int i=0; i<10; i++){

        delay(800);
        write(".");

    }

    std::ofstream outputFile("save.txt");

    if(outputFile.is_open()){
        outputFile << player.name << std::endl;
    }

}

#endif