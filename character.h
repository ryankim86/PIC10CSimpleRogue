/** @file character.h
 *  @brief Contains all of the characters that will be contained in the dungeon.
 *
 */

#ifndef CHARACTER_H
#define CHARACTER_H

#include <QString>
#include <QDebug>
#include <QObject>
#include <QColor>
#include <QTimer>
#include <QBrush>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <vector>
#include <QLabel>
#include <QMediaPlayer>
#include "gameobject.h"

class Dungeon; //forward declaration
class inventory;


/** @class character
 *  @brief Generic Character class, this will eventually split into two derived classes--monsters and player.
 *  Each character has member variables for their statistics/name. The class is currently derived from
 *  QGraphicsPixMapItem
 *
*/
class character :  public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:

    /**Default Constructor. Should never be called.*/
    character(){}
    /** Constructor with parameters. Takes various inputs for character statistics. Primarily used by derived classes */
    character(int hp, int atk, int def, int range, int row, int col, QString name)
    {
      m_hp=hp; m_atk=atk; m_def=def; m_range= range; m_row = row; m_col = col; m_name = name;
      m_curr_hp=hp; m_dead=false;
    }

    /**Destructor */
    virtual ~character(){}


    //Private Member variable mutators
    /**Mutator. Changes character's dungeon.*/
    void set_dungeon(Dungeon * dung){m_dungeon=dung;}
    /**Mutator. lowers character's Hit points.*/
    bool decrease_hp(int amount);
    /**Mutator. Changes character's status to dead.*/
    virtual void kill_character(){m_dead=true;}
    /**Mutator. Changes character's status to dead.*/
    void make_dead(){m_dead=true;}
    /**Mutator. Heals character's Hit points by x points*/
    void heal_hp(int x);
    /**Mutator. Increase character's maximum hit points.*/
    void increase_max_hp(int x){m_hp+=x;}
    /**Mutator. increases character's attack power*/
    void increase_atk(int x){m_atk+=x;}
    /**Mutator. Increase character's defense power.*/
    void increase_def(int x){m_def+=x;}
    /**Mutator. Sets character's current HP to its maximum value*/
    void reset_hp(){m_curr_hp=m_hp;}
    /**Mutator. Changes the weapon that the character has equipped.*/
    void equip_item(weapon* x){equipped_weapon=x;}

    //Accessors

    /**Accessor. Returns character's maximum HP*/
    int get_hp(){return m_hp;}
    /**Accessor. Returns character's dungeon pointer*/
    Dungeon * get_dun(){return m_dungeon;}
    /**Accessor. Returns character's attack power*/
    int get_atk(){return m_atk;}
    /**Accessor. Returns character's defense points*/
    int get_def(){return m_def;}
    /**Accessor. Returns character's current number of hit points*/
    int get_curr_hp(){return m_curr_hp;}
    /**Accessor. Returns character's name*/
    QString get_name(){return m_name;}
    /**Accessor. Returns character's death status*/
    bool is_dead(){return m_dead;}
    /**Accessor. Returns pointer to the weapon character has equipped.*/
    weapon* get_equipped_item(){return equipped_weapon;}

    /**Accessor. Returns range, a member variable that was not used for any important implementation*/
    int get_range(){return m_range;}
    /**Accessor. Returns row, a member variable that was not used for any important implementation*/
    int get_row(){return m_row;}
    /**Accessor. Returns col, a member variable that was not used for any important implementation*/
    int get_col(){return m_col;}

    /*Attack Function*/
    void attack(character * attacker, character * defender, QString &msg);


private:
    /** @var m_hp is the character's maximum HP*/
    int m_hp;
    /** @var m_curr_hp is the character's current HP*/
    int m_curr_hp;
    /** @var m_atk is the character's attack power*/
    int m_atk;
    /** @var m_def is the character's defense points*/
    int m_def;
    /** @var m_dead is true if the character is dead. false if character is alive*/
    bool m_dead;
    /** @var equipped_weapon is the weapon that the character currently has equipped*/
    weapon* equipped_weapon;
    /** @var m_name is the character's name*/
    QString m_name;
    /** @var m_dungeon is the dungeon that the character is currently in.*/
    Dungeon * m_dungeon;

    /*These Variables are not used for anything, was part of an older implementation that I later changed.*/
    int m_range;
    int m_row;
    int m_col;
};

/** @class Player
 *  @brief The player class. Has a few extra member variables compared to character class.
 *  These variables are level, experience points, item inventory, and various counters.
 *  Additional member functions are added for added functionality.
*/
class Player : public character
{
    Q_OBJECT
public:
    Player(){}
    Player(int hp, int atk, int def, int range, int r, int c, QString name, Dungeon* dungeon);
    virtual ~Player();

//Accessors

    /**
     * @brief Accessor. returns number of monsters player has killed
     * @return number of kills
     */
    int get_kills(){return monsters_killed;}

    /**
     * @brief Accessor. Return player level
     * @return player level
     */
    int get_lvl(){return level;}

    /**
     * @brief Accessor. Return player experience points
     * @return experience points
     */
    int get_exp(){return exp;}

    /**
     * @brief Accessor.
     * @return number of items player has collected in his/her adventures
     */
    int get_items_collected(){return m_items_collected;}
    /**
     * @brief Accessor.
     * @return pointer to player's inventory
     */
    inventory * get_inv(){return m_inventory;}
    /**
     * @brief Accessor.
     * @return QString that states which direction player is facing. "up", "left", "down", or "right".
     */
    QString get_direction(){return direction;}
    /**
     * @brief Accessor.
     * @return pointer to the sound played whenever the character attacks.
     */
    QMediaPlayer* get_attack_sound(){return m_attack_sound;}

    //Mutators
    void kill_character();
    void increment_kill_tally();
    void change_direction(QString x);

    /**
     * @brief increments number of monsters player has killed
     */
    void increment_item_tally(){++m_items_collected;}

public slots:

    //Switches the images to simulate animation for the characters.
    void switch_frame();

private:
    int level;
    QString direction;
    bool is_frame_1;
    int monsters_killed;
    int exp;
    int inventory_size;
    int m_items_collected;
    QTimer * frame_switcher;
    QMediaPlayer * m_attack_sound;
    inventory* m_inventory;
};

/** @class Monster
 *  @brief Base Monster class, will be derived from to make individual monster races: slimes, trolls, and dragons.
 *  The base monster will only use random movement and will be fairly easy to defeat. The four types are monsters
 *  are essentially characters initialized with different values that have different QPixMaps to represent them.
*/
class Monster : public character
{
public:
    /**
     * @brief Monster Constructor. Should be the only one ever called. Initiailizes a character using the input parameters.
     * @param hp
     * @param atk
     * @param def
     * @param range
     * @param r
     * @param c
     * @param name
     */
    Monster(int hp, int atk, int def, int range, int r, int c, QString name): character(hp, atk, def, range, r, c, name)
    {
        QPixmap image(":/new/images/enemy.png");
        this->setPixmap(image.scaled(25,25,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        this->equip_item(new dagger(0,0,get_dun(),"dagger",3));
    }
    virtual ~Monster();
    virtual void monster_action(Player * main_character, QString & atk);
};


/** @class Slime
 *  @brief Another Type of monster. Will be slightly stronger than the base monster
 *  Will only appear after the second floor. They also only use random movement.
*/
class Slime : public Monster
{
public:
    Slime():Monster(15,5,2,1,1,1,"Slime")
    {
        QPixmap image(":/new/images/slime.gif");
        this->setPixmap(image.scaled(25,25,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        this->equip_item(new dagger(0,0,get_dun(),"dagger",3));
    }

};

/** @class Troll
 *  @brief Another Type of monster. Will be slightly stronger than the Slime
 *  will only appear after the second floor. Will follow the player to some extent.
*/
class Troll : public Monster
{
public:
    Troll():Monster(20,6,3,1,1,1,"Troll")
    {
        QPixmap image(":/new/images/troll.gif");
        this->setPixmap(image.scaled(25,25,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        this->equip_item(new dagger(0,0,get_dun(),"dagger",3));
    }
    void monster_action(Player * main_character, QString & atk);

};

/** @class Dragon
 *  @brief Another Type of monster. The strongest monster in the game. Four dragons will surround the
 *  golden object needed to win the game. (so the player has to defeat at least one to win). Will NOT move.
*/
class Dragon : public Monster
{
public:
    Dragon():Monster(30,7,3,1,1,1,"Dragon")
    {
        QPixmap image(":/new/images/dragon.gif");
        this->setPixmap(image.scaled(25,25,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        this->equip_item(new dagger(0,0,get_dun(),"dagger",3));
    }
    void monster_action(Player * main_character, QString & atk);

};

#endif // CHARACTER_H


