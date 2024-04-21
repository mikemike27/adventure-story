#ifndef item
#define item

enum ItemType{
    
    Consumable, //potion
    Equipment

};

struct Item{

    ItemType item_type;

    string title;
    string description;
    int amount;

    int level;
    int attack;
    int defense;

};

Item generate_item(ItemType targetType, string title, int amount, int level, int attack, int defense){

    Item new_item;
    new_item.item_type = targetType;
    new_item.title = title;
    new_item.amount = amount;
    new_item.level = level;
    new_item.attack = attack;
    new_item.defense = defense;

    return new_item;

}

#endif