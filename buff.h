#ifndef buff
#define buff

enum BuffType{
    Attack,
    Defense,
    Speed
};

struct Buff{

    BuffType buff_type;
    int remaining_turn;

};

#endif