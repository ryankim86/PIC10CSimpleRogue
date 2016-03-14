/** @file menu.h
 *  @brief Contains declaration of my_menu, a class used to represent the equipment/character information
 *  menu that appears when the player presses 'm'.
 *
 */

#ifndef MENU_H
#define MENU_H

//Forward Declarations
class Player;
class MainWindow;
class Dungeon;
class menubutton;
class object;

#include <QGraphicsScene>
#include <vector>

/** @class my_menu
 *  @brief This is class derived from QGraphicsScene used to simulate a menu.
 *  The menu will display menubuttons, which are QPushButtons that are associated
 *  to a game object. They are stored in a std::vector.
 *  Additionally, my_menu will display more detailed character information.
*/
class my_menu : public QGraphicsScene
{
    friend class menubutton;
public:
    my_menu();
    my_menu(Player * player, Dungeon * dungeon, MainWindow* main_window);
    virtual ~my_menu();
    virtual void keyPressEvent(QKeyEvent * event);
    bool remove_displayed_item(object* x);
    void update_menu();
private:

    /*These Pointers are to access information about player or dungeon*/
    Player * m_player;
    MainWindow* m_main_window;
    Dungeon * m_dungeon;

    /*These pointers point to various items that will be displayed in my_menu*/
    QGraphicsTextItem * player_title;
    QGraphicsTextItem * player_info;
    QGraphicsPixmapItem * player_icon;
    std::vector<menubutton*> displayed_items;

};


#endif // MENU_H
