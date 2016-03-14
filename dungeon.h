/** @file dungeon.h
 *  @brief contains Dungeon Class declaration and some definitions of member functions
 *
 */


#ifndef DUNGEON_H
#define DUNGEON_H

#include "character.h"
#include <QWidget>
#include <QObject>
#include "gameobject.h"
#include <vector>
#include <QGraphicsScene>
#include <QMediaPlayer>
#include "inventory.h"

//forward declarations
class MainWindow;
class my_menu;


/** @class Dungeon
 *  @brief This will be where the all the action happens. The Dungeon is derived from QGraphicsScene, so different
 *  items (characters and objects) will be added to the scene. Contains various pointers that will be used to access
 *  different things within itself and allow other classes to safely access/alter different dungeon elements.
 */
class Dungeon : public QGraphicsScene
{
    Q_OBJECT

public:
   Dungeon(MainWindow * parent =0, QString player_class="None"); //Dungeon should will never be called with the default parameter values
   virtual ~Dungeon();
   virtual void keyPressEvent(QKeyEvent * event);

   void update_text(QString msg1,QString msg2);
   void updatemon();

   //Adding things to the DUNGEON
   void addplayer();
   void addmonster();
   void addobjects();

   //Next Floor!
   void advance_floor();

   //Win Game
   void win();

   //Draw GUI elements
   void draw_gui();

   /**
    * @brief Mutator
    * @param the_menu changes the my_menu associated to the equipment/characterinfo screen.
    */
   void set_menu(my_menu* the_menu){m_menu=the_menu;}

   //Call to move monsters
   void move_monsters(QString & atk);

   //Accessors

   /**
    * @brief get_floor
    * @return return floor number
    */
   int get_floor(){return floor;}
   bool is_monster();

   /**
    * @brief get_mainwind
    * @return pointer to the game's mainwindow
    */
   MainWindow * get_mainwind(){return m_mainwindow;}

   /**
    * @brief get_player
    * @return pointer to the dungeon's player
    */
   Player * get_player(){return m_player;}


private:
    MainWindow * m_mainwindow; //parent
    QString player_class; //String holding the name of the character's class. Only used in one or two functions.
    QGraphicsTextItem * title; //Pointer to the Text that represent the Title of the game. "Rogue"
    QGraphicsTextItem * information; //Player Information text
    Player * m_player; // The Player
    std::vector<Monster*> m_mon; //vector of monster pointers.
    inventory* m_inventory; //Inventory that contains all of the objects that are inside the dungeon but not in player inventory
    int floor; // Current Floor
    stairs * m_stairs; // Pointer to stairs to advance to floor
    goldenitem* m_winner; //pointer to the object to win the game on the fifth floor
    QGraphicsTextItem * attack_msg; //Message for player actions
    QGraphicsTextItem * attack_msg_2; //Message for Monster actions
    QMediaPlayer * music; //Background music
    QMediaPlaylist * music_playlist; //Used to loop the background music
    my_menu * m_menu; //Character information and Equipment menus
};

#endif // DUNGEON_H
