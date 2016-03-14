/** @file menubutton.h
 *  @brief Contains all of the characters that will be contained in the dungeon.
 *
 */

#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <QPushButton>
#include <gameobject.h>
#include "menu.h"

class MainWindow;
class inventory;

/**
 * @brief The menubutton class derived from QPushButton. It is a QPushButton with a pointer member variable
 * to a game object. The menubutton should call the game object's use() function when it is pressed.
 */
class menubutton : public QPushButton
{
    Q_OBJECT
    friend class my_menu;
public:
    /**
     * @brief menubutton default. Should not actually be called.
     */
    menubutton(){}
    /**
     * @brief menubutton constructor with propert inputs.
     * @param x object to be added
     * @param y menu to be included in
     */
    menubutton(object* x, my_menu* y ){ m_item = x; m_menu=y;}

    /**
     * @brief ~menubutton destructor
     */
    virtual ~menubutton(){}

public slots:
    void activater();
private:
    object* m_item;
    my_menu* m_menu;
};


#endif // MENUBUTTON_H
