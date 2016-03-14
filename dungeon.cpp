/* @file dungeon.cpp
 * @brief contains implementation for Dungeon class, a type of QGraphicsScene.
*/

//My Headers
#include "gameobject.h"
#include "dungeon.h"
#include "character.h"
#include "mainwindow.h"
#include "menu.h"
#include "inventory.h"

//Qt stuff
#include <QDebug>
#include <QMediaPlaylist>
#include <QMediaPlayer>
#include <stdlib.h>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <stdlib.h>
#include <QPushButton>
#include <vector>
#include <QRectF>
#include <random>
#include <chrono>
#include <ctime>
#include <QLabel>
#include <QMessageBox>
#include <QString>
#include <QKeyEvent>
#include <QTimer>

//Note to self: For the walls of the room, maybe try creating rectanges using the QObject Rectange


/*
*/

/**
 * @brief Main Dungeon Constructor. This will set the intial stage. The general process will be
to Create A player, add the title, add the player information, generate game objects,
then generate the monsters. I will create another member function that will recreate floors
after the player finds the stairway to the next floor. The dungeon will be the central widget of the mainwindow.
It will be implemented by using a sort of grid system where each 25x25 square is a spot.
 * @param parent parent QObject
 * @param x player's class. Used in initializing the player.
 */
Dungeon::Dungeon(MainWindow * parent, QString x): m_mainwindow(parent)
{
    //Play background music
    this->music_playlist = new QMediaPlaylist;
    music_playlist->addMedia(QUrl("qrc:/new/sounds/bgm.mp3"));
    music_playlist->setPlaybackMode(QMediaPlaylist::Loop);
    music = new QMediaPlayer;
    music->setPlaylist(music_playlist);
    music->play();

    this->attack_msg= nullptr;
    floor = 1;
    player_class = x;

    //Add Player
    addplayer();

    //Draw GUI
    this->draw_gui();

    //Create somewhere to keep track of dungeon's items
    m_inventory = new inventory();

    //Add Game Objects
    this->addobjects();

    //Add Monsters
    int num_mon = 6;
    /*For now, the number of monsters on the screen will be 6. Later it will be dependent on floor number.*/
    for (int i=0;i<num_mon;i++)
    {
        addmonster();
        m_mon[i]->set_dungeon(this);
    }

}


/**
 * @brief Dungeon Destructor
 */
Dungeon::~Dungeon()
{
    delete m_inventory;//deallocate items

    //Deallocate music items
    delete music;
    delete this->music_playlist;
}

/**
 * @brief Dungeon::addplayer adds player to the dungeon. Should only be called ONCE!
 */
void Dungeon::addplayer()
{

    QString cname = this->player_class;
    /*Create a Rectange to Represent the Player*/
    if(this->player_class=="Swordsman")
    {m_player= new Player(35,5,6,1,1,1,cname,this);}
    else if (this->player_class=="Archer")
    {m_player = new Player(30,6,5,3,1,1,cname,this);}
    else if(this->player_class=="Wizard")
    {m_player = new Player(25,7,5,2,1,1,cname,this);}
    m_player->setPos(400,250);

    //Add Player to the Scene
    this->addItem(m_player);
    m_player->set_dungeon(this);


}

/**
 * @brief Dungeon::addmonster Adds one monster to the dungeon. The Monster's pointer is also pushed onto
 * member variable m_mon, a vector containing pointers to monsters.
 */
void Dungeon::addmonster()
{
    unsigned int seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> x_unif(4,40);
    std::uniform_int_distribution<int> y_unif(6,23);

   int x_pos=0, y_pos=0;
   bool all_good =false;
   do
   {
       x_pos = x_unif(generator)*25; //Generate "random" number from 0 to 700 in increments of 25
       y_pos = y_unif(generator)*25; //Generate "random number from 150 to 400
       for(int i =0; i<this->items().size();i++)
       {if(x_pos!=items()[i]->x()||y_pos!=items()[i]->y()) all_good=true;}

   } while(all_good==false);

   //"Randomly" decide whether to generate a basic monster, slime, or troll based on floor number
    if((x_pos%3==0)&&(floor>=3))
        this->m_mon.push_back(new Troll());
    else if((y_pos%2==0)&&(floor>1))
        this->m_mon.push_back(new Slime());
    else
        this->m_mon.push_back(new Monster(10,3,2,1,1,1,"Monster"));

    //Add Monster to the scene.
    m_mon.back()->setPos(x_pos,y_pos);
    Monster * created_monster =m_mon.back();
    created_monster->set_dungeon(this);
    this->addItem(this->m_mon.back()); // Add it to the scene

}

/**
 * @brief Dungeon::addobjects Adds game objects to the screen. Similar to the monster generation, except with game objects.
 * Added game objects are also indexed within the dungeon's inventory.
 */
void Dungeon::addobjects()
{
    unsigned int seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> x_unif(4,40);
    std::uniform_int_distribution<int> y_unif(6,22);
     std::uniform_int_distribution<int> includeornot(1,2);


    bool all_good =false;
   int x_pos=0, y_pos=0;

   //Add the winning item
   if(floor==5)
   {
       all_good=false;
       do
       {
           x_pos = x_unif(generator)*25; //Generate "random" number from 0 to 700 in increments of 25
           y_pos = y_unif(generator)*25; //Generate "random number from 150 to 400
           for(int i =0; i<this->items().size();i++)
           {if(x_pos!=items()[i]->x()||y_pos!=items()[i]->y()) all_good=true;}

       } while(all_good==false);

       goldenitem * win = new goldenitem(x_pos,y_pos,this);
       this->addItem(win);
       this->m_winner=win;

   }

   if(floor != 5)
   {do
   {
       x_pos = x_unif(generator)*25; //Generate "random" number from 0 to 1000 in increments of 25
       y_pos = y_unif(generator)*25; //Generate "random" number from 150 to 575
       for(int i =0; i<this->items().size();i++)
       {if(x_pos!=items()[i]->x()||y_pos!=items()[i]->y()) all_good=true;}

   } while(all_good==false);

    stairs * stair = new stairs(x_pos,y_pos,this);
    this->addItem(stair);
    this->m_stairs=stair;
   }

    //Generate position for scrolls

   int include = includeornot(generator);//decide whether to include heal scroll or not.
   if(include == 1)
    {
    do
    {
        x_pos = x_unif(generator)*25; //Generate "random" number from 0 to 1000 in increments of 25
        y_pos = y_unif(generator)*25; //Generate "random" number from 150 to 575
        for(int i =0; i<this->items().size();i++)
        {if(x_pos!=items()[i]->x()||y_pos!=items()[i]->y()) all_good=true;}

    } while(all_good==false);
       heal_scroll * h_s = new heal_scroll(x_pos,y_pos,this);
       this->addItem(h_s);
       m_inventory->add_item(h_s);

   }

//Defense Scroll
    include = includeornot(generator);
    if(include==1)
    {
        do
        {
            x_pos = x_unif(generator)*25; //Generate "random" number from 0 to 1000 in increments of 25
            y_pos = y_unif(generator)*25; //Generate "random" number from 150 to 575
            for(int i =0; i<this->items().size();i++)
            {if(x_pos!=items()[i]->x()||y_pos!=items()[i]->y()) all_good=true;}

        } while(all_good==false);

    def_scroll * d_s = new def_scroll(x_pos,y_pos,this);
    this->addItem(d_s);
    m_inventory->add_item(d_s);
    }

//Attack Scroll
    include = includeornot(generator);
    if(include==1)
    {
        do
        {
            x_pos = x_unif(generator)*25; //Generate "random" number from 0 to 1000 in increments of 25
            y_pos = y_unif(generator)*25; //Generate "random" number from 150 to 575
            for(int i =0; i<this->items().size();i++)
            {if(x_pos!=items()[i]->x()||y_pos!=items()[i]->y()) all_good=true;}

        } while(all_good==false);

        atk_scroll * a_s = new atk_scroll(x_pos,y_pos,this);
        this->addItem(a_s);
        m_inventory->add_item(a_s);
    }

    //Generate Random position for floor weapon
    do
    {
        x_pos = x_unif(generator)*25; //Generate "random" number from 0 to 1000 in increments of 25
        y_pos = y_unif(generator)*25; //Generate "random" number from 150 to 575
        for(int i =0; i<this->items().size();i++)
        {if(x_pos!=items()[i]->x()||y_pos!=items()[i]->y()) all_good=true;}

    } while(all_good==false);

    //Decide which weapon to add
    if(floor==1)
    {
        weapon * w_s = new sword(x_pos,y_pos,this,"Short Sword",3);
        this->addItem(w_s);
        m_inventory->add_item(w_s);
    }
    else if(floor==2)
    {
        if(include==1)
        {
            whip * r_s = new whip(x_pos,y_pos,this,"Whip",5);
            this->addItem(r_s);
            m_inventory->add_item(r_s);
        }
        else
        {
            whip * r_s = new whip(x_pos,y_pos,this,"Crummy Whip",3);
            this->addItem(r_s);
            m_inventory->add_item(r_s);
        }

    }
    else if(floor==3)
    {
        int include2 = includeornot(generator);

        if((include2==2))
        {
            if(include==1)
            {
                whip * r_s = new whip(x_pos,y_pos,this,"Cool Whip",6);
                this->addItem(r_s);
                m_inventory->add_item(r_s);
            }
            else
            {
                sword * r_s = new sword(x_pos,y_pos,this,"Long Sword",5);
                this->addItem(r_s);
                m_inventory->add_item(r_s);
            }
        }
        else
        {
            glove * g = new glove(x_pos,y_pos,this,"Boxing Glove",4);
            this->addItem(g);
            m_inventory->add_item(g);
        }


    }
    else if(floor==4)
    {
        if(include==1)
        {
            glove * r_s = new glove(x_pos,y_pos,this,"Garden Gloves",1);
            this->addItem(r_s);
            m_inventory->add_item(r_s);
        }
        else
        {
            glove * r_s = new glove(x_pos,y_pos,this,"Gauntlet",6);
            this->addItem(r_s);
            m_inventory->add_item(r_s);
        }
    }
    else if (floor==5)
    {
        if(include==1)
        {
            hammer * r_s = new hammer(x_pos,y_pos,this,"Mjolnir",10);
            this->addItem(r_s);
            m_inventory->add_item(r_s);
        }
        else
        {
            hammer * r_s = new hammer(x_pos,y_pos,this,"Plastic Hammer",2);
            this->addItem(r_s);
            m_inventory->add_item(r_s);
        }
    }

}

/**
 * @brief Dungeon::advance_floor brings player to the next floor. Removes old items from the scene
 * and then calls funtions to add in different ones to replace them.
 */
void Dungeon::advance_floor()
{
    this->floor++;

    //get rid of the old monsters
    for(int i=0;i<m_mon.size();++i)
    {
        delete m_mon.back();
        m_mon.pop_back();
    }

    //Get Rid of Old Items
    delete this->m_inventory;

    //Create a new Item List
    m_inventory = new inventory();

    //Get rid of the stairs
    delete this->m_stairs;

    //Update the text
    this->update_text("You found the staircase! Huzzah","");

    //Add objects
    this->addobjects();

    //Add Dragons if player is on last floor
    if(floor==5)
    {
        int gold_item_x = m_winner->x();
        int gold_item_y = m_winner->y();

        //Left Dragon
        Dragon* dragon1 = new Dragon();
        dragon1->setPos(gold_item_x-25,gold_item_y);
        addItem(dragon1);
        m_mon.push_back(dragon1);

        //Top Dragon
        Dragon* dragon2 = new Dragon();
        dragon2->setPos(gold_item_x,gold_item_y-25);
        addItem(dragon2);
        m_mon.push_back(dragon2);

        //Bottom Dragon
        Dragon* dragon3 = new Dragon();
        dragon3->setPos(gold_item_x,gold_item_y+25);
        addItem(dragon3);
        m_mon.push_back(dragon3);

        //Right Dragon
        Dragon* dragon4 = new Dragon();
        dragon4->setPos(gold_item_x+25,gold_item_y);
        addItem(dragon4);
        m_mon.push_back(dragon4);
    }


    //Add monsters
    for(int i=0;i<6+floor;++i) this->addmonster();


}

/**
 * @brief Dungeon::win Called after the player successfully collects the golden item.
 * Will also delete the dungeon.
 */
void Dungeon::win()
{
    this->updatemon(); //make sure all dead monsters are taken out of the vector.
    update_text(QString("You found the Golden Item of Justice and Unicorn butts"), "");
    QLabel * gameover = new QLabel();
    gameover->setText("You have found the Golden Item of Justice and Unicorn barf. Wow.\n"
                      "Here are your stats:\n"
                      "Character Class: " +m_player->get_name()+"\n"
                      "Character Level: " +QString::number(m_player->get_lvl()) +"\n"
                      "Monsters Killed: "+QString::number(m_player->get_kills())+"\n"
                      "Items Collected: "+QString::number(m_player->get_items_collected())+"\n"
                      );
    gameover->setAlignment(Qt::AlignCenter);
    this->m_mainwindow->make_gameover();
    delete this->m_menu;
    this->m_mainwindow->setCentralWidget(gameover);
    delete this; // delete the dungeon.
}

/**
 * @brief Dungeon::draw_gui draws GUI elements.
 */
void Dungeon::draw_gui()
{
    //Draw GUI Elements
        QGraphicsRectItem * left_panel = new QGraphicsRectItem(0,0,25,645);
        QGraphicsRectItem * right_panel = new QGraphicsRectItem(1075,0,25,645);
        QGraphicsRectItem * top_panel = new QGraphicsRectItem(25,0,1050,55);
        QGraphicsRectItem * bottom_panel = new QGraphicsRectItem(25,600,1050,45);
        QBrush brush;
        brush.setColor(Qt::darkCyan);
        brush.setStyle(Qt::SolidPattern);
        bottom_panel->setBrush(brush);
        left_panel->setBrush(brush);
        right_panel->setBrush(brush);
        top_panel->setBrush(brush);
        this->addItem(bottom_panel);
        this->addItem(right_panel);
        this->addItem(left_panel);
        this->addItem(top_panel);

        //Create Title
        QGraphicsTextItem * title = new QGraphicsTextItem(QString("Rogue"));
            //Set Title Properties
                QFont title_font("tahoma", 40);
                title->setFont(title_font);
                title->setPos(450,-15); // Top Middleish of the screen
                title->setDefaultTextColor(QColor(255,255,255)); //color White
           // Add item to dungeon and set it as parent
                this->addItem(title);
                title->setParent(this);

        //Show Player Information

                information = new QGraphicsTextItem();
            //Set what the Text will say. The text will change as the game changes eventually. Will probably be done with some sort of Dungeon update() function
                information->setPlainText(QString("Class: ")+this->m_player->get_name()+QString("     Level: 1      ") +QString("HP: ")+QString::number(this->m_player->get_curr_hp())+QString("/")+QString::number(this->m_player->get_hp())+QString("       Equipped Weapon: Crummy Dagger      ") +QString("Floor: ")+QString::number(this->get_floor())+QString("      Status: Normal"));
                information->setPos(30,620);
                information->setDefaultTextColor(QColor(255,255,255));
           //Add to dungeon and set it as parent
                this->addItem(information);
                information->setParent(this);


                //Create a message that will be displayed above the information.
               this->attack_msg = new QGraphicsTextItem();
                         attack_msg->setPlainText(QString(""));
                        attack_msg->setPos(200,600);
                        attack_msg->setDefaultTextColor(QColor(255,255,255));
                   //Add to dungeon and set it as parent
                        this->addItem(attack_msg);
                        attack_msg->setParent(this);

                        //Create another message that will be displayed above the information.
                       this->attack_msg_2 = new QGraphicsTextItem();
                                 attack_msg_2->setPlainText(QString(""));
                                attack_msg_2->setPos(600,600);
                                attack_msg_2->setDefaultTextColor(QColor(255,255,255));
                           //Add to dungeon and set it as parent
                                this->addItem(attack_msg_2);
                                attack_msg_2->setParent(this);
}


/**
 * @brief Dungeon::move_monsters will call each monster's action function to decide how the monsters will move.
 * @param atk Display message of what happened
 */
void Dungeon::move_monsters(QString &atk)
{

    for(int i=0;i<m_mon.size();i++)
    {
      this->m_mon[i]->monster_action(m_player, atk);
    }
}

/**
 * @brief Dungeon::is_monster displays all of the monsters' coordinates and the player coordinates.
 * @return Returns true if function succesfully runs its course
 */
bool Dungeon::is_monster()
{
    std::vector<Monster*>::iterator it = m_mon.begin();
    for(it; it!=m_mon.end(); ++it)
    {
        qDebug() << (*it)->pos();

    }
    qDebug() << "Player: " << m_player->pos();
return true;
}

/**
 * @brief Dungeon::update_text Will update the text that is shown on the bottom.
 * @param msg1 Displays the player's action
 * @param msg2 Displays a monsters action against the player
 */
void Dungeon::update_text(QString msg1,QString msg2)
{
    //Update information
  this->information->setPlainText(QString("Class: ")
                                  +this->m_player->get_name()
                                  +QString("     Level: ")+QString::number(this->m_player->get_lvl())+QString("      ")+QString("HP: ")
                                  +QString::number(this->m_player->get_curr_hp())
                                  +QString("/")+QString::number(this->m_player->get_hp())
                                  +QString("       Equipped Weapon: ")+m_player->get_equipped_item()->get_name()+"      "
                                  +QString("Floor: ")+QString::number(this->get_floor())
                                  +QString("      Status: Normal"));
    //Action message, if applicable
    if(msg1 != "")
        attack_msg->setPlainText(msg1);
    if(msg2 !="")
        attack_msg_2->setPlainText(msg2);


}

/**
 * @brief Dungeon::updatemon called every time after movement to make sure dead monsters are properly removed from the scene
 */
void Dungeon::updatemon()
 {
     //update vector list if a monster dies
     std::vector<Monster*>::iterator it =m_mon.begin();
     while (it != m_mon.end())
     {
         if ((*it)->is_dead() == true)
         {
             this->m_player->increment_kill_tally();
             delete *it;
             it = m_mon.erase(it);
         }
         else
             ++it;
     }
     return;

 }

/**
 * @brief Dungeon::keyPressEvent Handles keypresses. Probably the most important function in the game.
 * The function first checks if the player is alive. If alive, it will check which key was pressed.
 * WASD are attacks. Arrow keys are movement. E is the item interaction button. M will open the menu.
 * @param event keypress
 */
void Dungeon::keyPressEvent(QKeyEvent *event)
{
    //Reset the message if the player attacked the previous turn
    if(attack_msg!=nullptr) attack_msg->setPlainText(QString(""));
    if(attack_msg_2!=nullptr) attack_msg_2->setPlainText(QString(""));

    QString atk2="";
    QString atk="";


    if(!this->m_player->is_dead()) // check if player is alive
{
    //Moving Left
   if(event->key()==Qt::Key_Left)
    {
       m_player->change_direction("left");
       if((this->m_player->x()-10 >= 0)) //Prevent character from moving farther left than view
        {
           //Create a point sort of in the middle of the coordinate left to the player
           QPointF pos(m_player->x()-13,m_player->y()+13);

           //If there is a monster at that point, then the player cannot move past it.
           if(this->items(pos).size()==0)
               this->m_player->setPos(m_player->x()-25,m_player->y());
        }
       move_monsters(atk2);
              this->update_text(atk, atk2);
     }


   //Moving Right
   else if((event->key()==Qt::Key_Right))
         {
       m_player->change_direction("right");
       QPointF pos(m_player->x()+43,m_player->y()+13);
           if(this->m_player->x()+25 < 1100) //Prevent character from moving farther right than view
                if(this->items(pos).size()==0)//If there is an item immediately below player, cannot move there.
                    this->m_player->setPos(m_player->x()+25,m_player->y());
            move_monsters(atk2);
       this->update_text(atk, atk2);
         }

   //Moving Up
   else if((event->key()==Qt::Key_Up))
           {
       m_player->change_direction("up");
        QPointF pos(m_player->x()+13,m_player->y()-13);
            if(this->m_player->y()>90) //Prevent character from moving farther up than view
                if(this->items(pos).size()==0) //If there is an item immediately above player, cannot move there.
                    this->m_player->setPos(m_player->x(),m_player->y()-25);
            move_monsters(atk2);
                   this->update_text(atk, atk2);
            }

    //Moving Down
   else if((event->key()==Qt::Key_Down))
           {
            m_player->change_direction("down");
            QPointF pos(m_player->x()+13,m_player->y()+43);
            if(this->m_player->y()<650) //Prevent character from moving farther down than view
                if(this->items(pos).size()==0) //If there is a monster immediately below player, cannot move there.
                this->m_player->setPos(m_player->x(),m_player->y()+25);
            move_monsters(atk2);
                   this->update_text(atk, atk2);

            }


/*Attacking uses more or less the same idea as movement. Create a
 * point to check if there is a monster immediately next to the player.
*/
        //Attack Left (each attack direction has the same implementation)
   else if(event->key()==Qt::Key_A)
           {
               m_player->change_direction("left"); //Change sprite direction

               //Play Attack sound
               if(this->m_player->get_attack_sound()->state() == QMediaPlayer::PlayingState) m_player->get_attack_sound()->setPosition(0);
               else if (this->m_player->get_attack_sound()->state() == QMediaPlayer::StoppedState) m_player->get_attack_sound()->play();

               QString atk("");

               //Check if there is monster immediately to left player.
               QPointF pos(m_player->x()-(13),m_player->y()+13);
               if(this->items(pos).size()>0)
                {
                   //If there is a mosnter, player will attack monster.
                    if(dynamic_cast<character*>(items(pos).first()))
                   {
                        character * monster = dynamic_cast<character*>(items(pos).first());
                        this->m_player->attack(m_player, monster, atk);
                   }
                    else
                        update_text("You can't attack items!", "");
                }
                 else atk="You try to attack the air. Loser.";

               //After Player attacks, the monsters will move
                      move_monsters(atk2);
               this->update_text(atk, atk2);
                      updatemon(); //update monster list if game is still going on.

           }

        //Attack Down
   else if(event->key()==Qt::Key_S)
           {
                //Change Sprite
               m_player->change_direction("down");

               //Attack sound
               if(this->m_player->get_attack_sound()->state() == QMediaPlayer::PlayingState) m_player->get_attack_sound()->setPosition(0);
               else if (this->m_player->get_attack_sound()->state() == QMediaPlayer::StoppedState) m_player->get_attack_sound()->play();

               QString atk("");

            //If there is a monster immediately below player attack it.
               QPointF pos(m_player->x()+13,m_player->y()+43);
               if(this->items(pos).size()>0)
               {
                  if(dynamic_cast<character*>(items(pos).first()))
                 {
                      character * monster = dynamic_cast<character*>(items(pos).first());
                      this->m_player->attack(m_player, monster, atk);
                  }
                  else
                      update_text("You can't attack items!", "");
               }
                else atk="You try to attack the air. Loser.";
              move_monsters(atk2);
                 this->update_text(atk, atk2);
            updatemon(); //update monster list if game is still going on.
           }

        //Attack Right
   else if(event->key()==Qt::Key_D)
       {
           m_player->change_direction("right");
           if(this->m_player->get_attack_sound()->state() == QMediaPlayer::PlayingState) m_player->get_attack_sound()->setPosition(0);
           else if (this->m_player->get_attack_sound()->state() == QMediaPlayer::StoppedState) m_player->get_attack_sound()->play();
                   QString atk("");

              //If there is a monster immediately to right of player, attack it.
            QPointF pos(m_player->x()+43,m_player->y()+13);
               if(this->items(pos).size()>0)
                {
                   if(dynamic_cast<character*>(items(pos).first()))
                  {
                       character * monster = dynamic_cast<character*>(items(pos).first());
                       this->m_player->attack(m_player, monster, atk);
                  }
                   else
                       update_text("You can't attack items!", "");
                }
         else atk="You try to attack the air. Loser.";

       move_monsters(atk2);
            this->update_text(atk, atk2);
       updatemon(); //update monster list if game is still going on.
       }

        //Attack Up
   else if(event->key()==Qt::Key_W)
           {
                m_player->change_direction("up");
                if(this->m_player->get_attack_sound()->state() == QMediaPlayer::PlayingState) m_player->get_attack_sound()->setPosition(0);
                else if (this->m_player->get_attack_sound()->state() == QMediaPlayer::StoppedState) m_player->get_attack_sound()->play();
                QString atk("");
                QPointF pos(m_player->x()+13,m_player->y()-13);
               if(this->items(pos).size()>0)//If there is a monster immediately below player attack it.
                {
                   if(dynamic_cast<character*>(items(pos).first()))
                  {
                       character * monster = dynamic_cast<character*>(items(pos).first());
                       this->m_player->attack(m_player, monster, atk);
                   }
                   else
                       update_text("You can't attack items!", "");
                }

               //If there's nothing there
               else atk="You try to attack the air. Loser.";

               move_monsters(atk2);
                    this->update_text(atk, atk2);
               updatemon(); //update monster list if game is still going on.
           }

   /*Item Interaction: Will first check which way the player is facing. Similar to the attack button. Will then dynamically cast whatever
    is in front of the player to the appropriate gameobject. Then perform the appropriate action (pick up, advance level, etc)
    */
   else if(event->key()==Qt::Key_E)
           {
       if(m_player->get_direction()=="up")
       {
            QPointF up(m_player->x()+13,m_player->y()-13);
            if(this->items(up).size()>0)
            {
                if(dynamic_cast<stairs*>(items(up).first()))
                     {this->advance_floor();return;}
                if(dynamic_cast<scroll*>(items(up).first())||dynamic_cast<weapon*>(items(up).first()))
                {
                    //Add Item to Player Inventory first. Then Remove from dungeon.
                    if(dynamic_cast<scroll*>(items(up).first()))
                    {
                        QString pick_name =dynamic_cast<scroll*>(items(up).first())->get_name();
                        this->update_text("Picked up "+pick_name,"");
                        m_player->get_inv()->add_item(dynamic_cast<scroll*>(items(up).first()));
                        m_inventory->erase_item(dynamic_cast<scroll*>(items(up).first()));
                        removeItem(items(up).first());
                        m_player->increment_item_tally();
                    }
                    else
                    {
                        QString pick_name =dynamic_cast<weapon*>(items(up).first())->get_name();
                        this->update_text("Picked up "+pick_name,"");
                        m_player->get_inv()->add_item(dynamic_cast<weapon*>(items(up).first()));
                        m_inventory->erase_item(dynamic_cast<weapon*>(items(up).first()));
                        removeItem(items(up).first());
                        m_player->increment_item_tally();
                    }
                    return;
                }

                if(dynamic_cast<goldenitem*>(items(up).first()))
                {this->win();}
            }
            else
                 {update_text(QString("There's nothing to interact with."),"");}
       }
       else if(m_player->get_direction()=="left")
       {
           QPointF left(m_player->x()-(13),m_player->y()+13); //left
           if  (this->items(left).size()>0)
                       {
                           if(dynamic_cast<stairs*>(items(left).first()))
                           {this->advance_floor();return;}
                           if(dynamic_cast<scroll*>(items(left).first())||dynamic_cast<weapon*>(items(left).first()))
                           {
                               if(dynamic_cast<scroll*>(items(left).first()))
                               {
                                   QString pick_name =dynamic_cast<scroll*>(items(left).first())->get_name();
                                   this->update_text("Picked up "+pick_name,"");
                                   m_player->get_inv()->add_item(dynamic_cast<scroll*>(items(left).first()));
                                   m_inventory->erase_item(dynamic_cast<scroll*>(items(left).first()));
                                   removeItem(items(left).first());
                                   m_player->increment_item_tally();
                               }
                               else
                               {
                                   QString pick_name =dynamic_cast<weapon*>(items(left).first())->get_name();
                                   this->update_text("Picked up "+pick_name,"");
                                   m_player->get_inv()->add_item(dynamic_cast<weapon*>(items(left).first()));
                                   m_inventory->erase_item(dynamic_cast<weapon*>(items(left).first()));
                                   removeItem(items(left).first());
                                   m_player->increment_item_tally();
                               }
                               return;
                           }
                           if(dynamic_cast<goldenitem*>(items(left).first()))this->win();
                       }
           else
                {update_text(QString("There's nothing to interact with."),"");}
       }
       else if(m_player->get_direction()=="down")
       {
           QPointF down(m_player->x()+13,m_player->y()+40); //down
           if  (this->items(down).size()>0)
                       {
                          if(dynamic_cast<stairs*>(items(down).first()))
                                {this->advance_floor();return;}
                           if(dynamic_cast<scroll*>(items(down).first())||dynamic_cast<weapon*>(items(down).first()))
                           {
                               if(dynamic_cast<scroll*>(items(down).first()))
                               {
                                   QString pick_name =dynamic_cast<scroll*>(items(down).first())->get_name();
                                   this->update_text("Picked up "+pick_name,"");
                                   m_player->get_inv()->add_item(dynamic_cast<scroll*>(items(down).first()));
                                   m_inventory->erase_item(dynamic_cast<scroll*>(items(down).first()));
                                   removeItem(items(down).first());
                                   m_player->increment_item_tally();
                               }
                               else
                               {
                                   QString pick_name =dynamic_cast<weapon*>(items(down).first())->get_name();
                                   this->update_text("Picked up "+pick_name,"");
                                   m_player->get_inv()->add_item(dynamic_cast<weapon*>(items(down).first()));
                                   m_inventory->erase_item(dynamic_cast<weapon*>(items(down).first()));
                                   removeItem(items(down).first());
                                   m_player->increment_item_tally();
                               }
                               return;
                           }
                           if(dynamic_cast<goldenitem*>(items(down).first())) {this->win();}
                       }
           else
                {update_text(QString("There's nothing to interact with."),"");}

       }
       else if(m_player->get_direction()=="right")
       {
            QPointF right(m_player->x()+40,m_player->y()+13); // right
            if (this->items(right).size()>0)
                        {
                            if(dynamic_cast<stairs*>(items(right).first()))
                                 {this->advance_floor();return;}
                           if(dynamic_cast<scroll*>(items(right).first())||dynamic_cast<weapon*>(items(right).first()))
                            {
                               if(dynamic_cast<scroll*>(items(right).first()))
                               {
                                   QString pick_name =dynamic_cast<scroll*>(items(right).first())->get_name();
                                   this->update_text("Picked up "+pick_name,"");
                                   m_player->get_inv()->add_item(dynamic_cast<scroll*>(items(right).first()));
                                   m_inventory->erase_item(dynamic_cast<scroll*>(items(right).first()));
                                   removeItem(items(right).first());
                                   m_player->increment_item_tally();
                               }
                               else
                               {
                                    QString pick_name =dynamic_cast<weapon*>(items(right).first())->get_name();
                                    this->update_text("Picked up "+pick_name,"");
                                    m_player->get_inv()->add_item(dynamic_cast<weapon*>(items(right).first()));
                                    m_inventory->erase_item(dynamic_cast<weapon*>(items(right).first()));
                                    removeItem(items(right).first());
                                    m_player->increment_item_tally();}
                                    return;
                            }
                            if(dynamic_cast<goldenitem*>(items(right).first()))
                                this->win();
                        }
            else
                 {update_text(QString("There's nothing to interact with."),"");}
       }
}
   /*Menu Button. Will essentially switch the main window's central widget to the menu*/
   else if(event->key()==Qt::Key_M)
           {
                //update menu elements
               m_menu->update_menu();
               QGraphicsView* menu_view = new QGraphicsView(m_menu);

               //Prevent Scrolling
               menu_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
               menu_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
               menu_view->setFixedSize(1100,650);

               //Change the central widget to menu
               m_mainwindow->setCentralWidget(menu_view);

               //Give menu focus
               this->m_mainwindow->centralWidget()->grabKeyboard();
               return;
           }

   //Invalid command
   else
       update_text(QString("Not a Valid Command!"), QString(""));
}
    // If the player is dead--gameover
    else
    {
        this->updatemon(); //make sure all dead monsters are taken out of the vector.
        update_text(QString("You are dead. Please quit and try again."), QString("You are dead. Please quit and try again."));
        QLabel * gameover = new QLabel("Game over! Please quit and try again!"); // Create a label to tell the player the game is over and to quit/try again.
        gameover->setAlignment(Qt::AlignCenter);
        this->m_mainwindow->make_gameover();
        this->m_mainwindow->setCentralWidget(gameover);
        delete this->m_menu;
        delete this; // delete the dungeon.

    }
}

