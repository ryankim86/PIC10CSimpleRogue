/** @file character.cpp
 *  @brief Contains implementation for character Objects.
 *
*/
#include "character.h"
#include "dungeon.h"
#include "mainwindow.h"
#include "inventory.h"
#include "gameobject.h"
#include <QDebug>
#include <chrono>
#include <random>
#include <ctime>
#include <QKeyEvent>
#include <cmath>


/**
 * @brief Player Constructor
 * @param hp hit points
 * @param atk attack points
 * @param def defense points
 * @param range NOT USED in game implementation
 * @param r NOT USED in game implementation
 * @param c NOT USED in game implementation
 * @param name player name
 * @param dungeon the pointer keeping track of what dungeon the player is in.
 */
Player::Player(int hp, int atk, int def, int range, int r, int c, QString name, Dungeon* dungeon):character{hp, atk, def, range, r, c, name}
{
    this->set_dungeon(dungeon);
    level = 1;
    exp = 0;
    monsters_killed=0;
    m_items_collected=0;
    frame_switcher = new QTimer;
    m_inventory = new inventory();
    dagger * equipped_weapon = new dagger(0,0,get_dun(),"Crummy Dagger",2);
    this->equip_item(equipped_weapon);
    m_inventory->add_item(equipped_weapon);

    if (name=="Swordsman")
    {
        m_attack_sound = new QMediaPlayer(); m_attack_sound->setMedia(QUrl("qrc:/new/sounds/swordslash.mp3"));
        QPixmap image(":/new/images/swordsman/down_frame_0.gif");
        this->setPixmap(image.scaled(25,25,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        is_frame_1 = false;
        direction = "down";
    }

    if (name=="Archer")
    {
        m_attack_sound = new QMediaPlayer(); m_attack_sound->setMedia(QUrl("qrc:/new/sounds/swordslash.mp3"));
        QPixmap image(":/new/images/archer/down_frame_0.gif");
        this->setPixmap(image.scaled(25,25,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        is_frame_1 = false;
        direction = "down";
    }

    if (name=="Wizard")
    {
        m_attack_sound = new QMediaPlayer(); m_attack_sound->setMedia(QUrl("qrc:/new/sounds/swordslash.mp3"));
        QPixmap image(":/new/images/wiz/down_frame_0.gif");
        this->setPixmap(image.scaled(25,25,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        is_frame_1 = false;
        direction = "down";
    }

    frame_switcher->start(100);
    connect(frame_switcher,SIGNAL(timeout()),this,SLOT(switch_frame()));
        m_attack_sound->setVolume(15);
}

/**
 * @brief Player destructor. Deallocates memory for the timer for switching the character's sprite and the
 * character's inventory.
 */
Player::~Player()
{
    delete this->m_inventory;
    delete this->frame_switcher;

}

/*This function will increment the kill tally and level up the player if appropriate*/

/**
 * @brief Counts the number of monsters the player has killed.
 * and gives the player 5 experience points for a killed monster. This function also takes
 * care of leveling up the player and increasing the player's stats after the level up.
 */
void Player::increment_kill_tally()
{
    ++monsters_killed;
    exp+=5;

    //Check if Player Leveled Up
    if (exp == this->level*15)
    {
        //Reset EXP Count and increment level;
       exp = 0;
       level +=1;

       //Increase Stats
       this->increase_max_hp(5);
       this->reset_hp(); //heal player
       this->increase_atk(2);
       this->increase_def(2);

       //Tell Player that they leveled up!
       QGraphicsTextItem * levelup = new QGraphicsTextItem(QString("Level Up!"));
           //Set level up text Properties
               QFont level_font("tahoma", 15);
               levelup->setFont(level_font);
               levelup->setPos(this->x(),this->y()+30);
               levelup->setDefaultTextColor(QColor(255,255,255)); //color White
        this->get_dun()->addItem(levelup);
        QTimer * countdown = new QTimer;
        countdown->start(500);
        connect(countdown,SIGNAL(timeout()),levelup,SLOT(deleteLater()));
        connect(countdown,SIGNAL(timeout()),countdown,SLOT(deleteLater()));
        this->get_dun()->update_text("Level up!"," ");

    }
}

/**
 * @brief changes player's death status to true
 */
void Player::kill_character()
{
    this->make_dead();
}


/**
 * @brief Takes in the direction the player should be facing, and then switches the sprite so that the character is facing in the direction
 * of its movement
 * @param x the direction the player is currently facing
 */
void Player::change_direction(QString x)
{
    direction=x;
    QString job = get_name();
    QString frame_0;

    //Check character class
    if(job=="Swordsman")
    {
      frame_0=":/new/images/swordsman/"+direction+"_frame_0.gif";

    }
    else if(job=="Archer")
    {
       frame_0=":/new/images/archer/"+direction+"_frame_0.gif";
    }
    else if(job=="Wizard")
    {
        frame_0=":/new/images/wiz/"+direction+"_frame_0.gif";

    }
    QPixmap image(frame_0);
    this->setPixmap(image.scaled(25,25,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    this->is_frame_1=false;

}

/**
 * @brief Public SLOT that is used to switch the character sprite between two different images
 * so simulate the character marching in place.
 */
void Player::switch_frame()
{
    QString job = get_name();
    QString frame_0;
    QString frame_1;
    if(job=="Swordsman")
    {
        frame_0=":/new/images/swordsman/"+direction+"_frame_0.gif";
        frame_1=":/new/images/swordsman/"+direction+"_frame_1.gif";


    }
    else if(job=="Archer")
    {
        frame_0=":/new/images/archer/"+direction+"_frame_0.gif";
        frame_1=":/new/images/archer/"+direction+"_frame_1.gif";

    }
    else if(job=="Wizard")
    {
        frame_0=":/new/images/wiz/"+direction+"_frame_0.gif";
        frame_1=":/new/images/wiz/"+direction+"_frame_1.gif";

    }


    if(is_frame_1==true)
    {
        QPixmap image(frame_0);
        this->setPixmap(image.scaled(25,25,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        is_frame_1=false;
        frame_switcher->start(500);

    }

    else if(is_frame_1==false)
    {
        QPixmap image(frame_1);
        this->setPixmap(image.scaled(25,25,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        is_frame_1=true;
        frame_switcher->start(500);

    }
}


/**
 * @brief Monster Destructor. Will deallocate whatever memory was allocated for the monster equipped weapon.
 */
Monster::~Monster()
{
    delete this->get_equipped_item();
}



/**
 * @brief Will be the standard movement for monsters. This function is virtual because I intend on
 * creating different types of monsters because they'll have different movement patterns. This movement
 * pattern is only random movement and will attack the Player if he/she is next to the monster
 * @param main_player player
 * @param atk display message
 */
void Monster::monster_action(Player * main_player, QString &atk)
{
    /*First check if Player is in attackable range.*/
    if(     ((main_player->x()==this->x()-25)&&(main_player->y()==this->y()))||
            ((main_player->x()==this->x()+25)&&(main_player->y()==this->y()))||
            ((main_player->x()==this->x())&&(main_player->y()==this->y()-25))||
            ((main_player->x()==this->x())&&(main_player->y()==this->y()+25))
       )    {main_player->attack(this,main_player,atk); return; }// Attack if so.

    //Otherwise, move randomly.
    unsigned int seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> direction(1,4);

   int direc = 0;
   direc=direction(generator);

    // Move up
    if (direc==1)
    {
        if(this->y()-25<60) return; //prevent monster from moving higher than GUI
        QPointF pos(this->x()+13,this->y()-13);
        if(this->get_dun()->items(pos).size()==0)
        {this->setPos(x(),y()-25);return;}
    }
    //Right
    else if (direc==2)
    {
        QPointF pos(this->x()+43,this->y()+13);
        if(this->get_dun()->items(pos).size()==0)
         {this->setPos(x()+25,y());return;}
    }
    //down
    else if (direc==3)
    {
        QPointF pos(this->x()+13,this->y()+43);
        if(this->get_dun()->items(pos).size()==0)
        {this->setPos(x(),y()+25);return;}
    }
    //Left
    else if (direc==4)
    {
        QPointF pos(this->x()-13,this->y()+13);
        if(this->get_dun()->items(pos).size()==0)
        {this->setPos(x()-25,y());return;}
    }
    //Otherwise just move up
    else
    {
        QPointF pos(this->x()+13,this->y()-13);
        if(this->get_dun()->items(pos).size()==0)
        {this->setPos(x(),y()-25);return;}
    }

}


/**
 * @brief Troll action function. Troll is follow the player to some extent. The algorithm is not very clever,
 * as it will not move around things to reach the player.
 * @param main_player the player
 * @param atk display message
 */
void Troll::monster_action(Player *main_player, QString &atk)
{
    //Check if Player is more than 5 turns steps away.
    if(std::abs(main_player->x() - this->x())+std::abs(main_player->y() - this->y()) > 125)
        return; //do nothing
    else
    {
        /*Not sure if this is 100% foolproof, but it works, so I'll use it for now until I figure out something smarter.
        First check if Player is in attackable range.*/
        if(     ((main_player->x()==this->x()-25)&&(main_player->y()==this->y()))||
                ((main_player->x()==this->x()+25)&&(main_player->y()==this->y()))||
                ((main_player->x()==this->x())&&(main_player->y()==this->y()-25))||
                ((main_player->x()==this->x())&&(main_player->y()==this->y()+25))
           )    {main_player->attack(this,main_player,atk); return; }// Attack
        // otherwise move towards player. Same implementation as Player Movement after keystroke
    else{

            int differenceX=main_player->x()-this->x();
            int differenceY=main_player->y() - this->y();

        if(differenceY < 0 ) //up
        {
            if(this->y()-25<60) return; //prevent monster from moving higher than GUI
            QPointF pos(this->x()+13,this->y()-13);
            if(this->get_dun()->items(pos).size()==0)
            {this->setPos(x(),y()-25);return;}
        }
        else if(differenceX > 0) //right
        {   QPointF pos(this->x()+43,this->y()+13);
            if(this->get_dun()->items(pos).size()==0)
            {this->setPos(x()+25,y());return;}
        }
        else if(differenceY >0) //down
        {
            QPointF pos(this->x()+13,this->y()+43);
            if(this->get_dun()->items(pos).size()==0)
            {this->setPos(x(),y()+25);return;}
        }
        else if(differenceX <0) //left
        {
            QPointF pos(this->x()-13,this->y()+13);
            if(this->get_dun()->items(pos).size()==0)
            {this->setPos(x()-25,y());return;}
        }
    }
    }
}

/**
 * @brief decrease hit points by amount
 * @param amount
 * @return true if player is dead(hitpoints <0) , and false otherwise. This is so the attack function knows
 * to kill the character and remove it from the scene.
 */
bool character::decrease_hp(int amount)
{
    if(m_dead==true) m_hp=0;
    m_curr_hp -=amount; //subtract HP by damage amount


    if (m_curr_hp <= 0)
    {
        m_curr_hp =0;
        return true;

    }
    else
        return false;
}

/**
 * @brief Heals the character
 * @param x amount to heal
 */
void character::heal_hp(int x)
{

    m_curr_hp +=x;
 if(m_curr_hp >= m_hp)
 {m_curr_hp=m_hp;}

}



/**
 * @brief This function will handle one character attacking another character.
 * @param attacker the attacker
 * @param defender the one being attacked
 * @param msg the message that will be displayed about what happened.
 */
void character::attack(character * attacker, character * defender, QString &msg)
{
   if(std::rand()%100>15)
   {
       //Calculate Damage Based on Attacker's Atk and Defender's Defense
       int damage = attacker->get_atk() + attacker->get_equipped_item()->get_power() - defender->get_def();
       if(damage <= 0 ) damage=1;

       //Set Message if character makes a hit
       msg= attacker->get_name() + QString(" ") + QString("attacked ") + defender->get_name() +QString(" and hits for") + QString::number(damage) + QString(" damage!");
       if(defender->decrease_hp(damage))
       {
           //Change the message if the character kills the other
           msg= attacker->get_name() + QString(" ") + QString("attacked ") + defender->get_name() +QString(" and deals a final blow!");
           defender->kill_character();
       }
   }
   else
       msg= attacker->get_name() + QString(" ") + QString("attacked ") + defender->get_name() +QString(" but missed like a scrub!");

}

/**
 * @brief Dragon's action. Dragons do not move; they just attack the player when player is next to it.
 * @param main_player player
 * @param atk attack message that displayed what happened.
 */
void Dragon::monster_action(Player *main_player, QString &atk)
{
    if(     ((main_player->x()==this->x()-25)&&(main_player->y()==this->y()))||
            ((main_player->x()==this->x()+25)&&(main_player->y()==this->y()))||
            ((main_player->x()==this->x())&&(main_player->y()==this->y()-25))||
            ((main_player->x()==this->x())&&(main_player->y()==this->y()+25))
       )    {main_player->attack(this,main_player,atk); return; }// Attack
}
