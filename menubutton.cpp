/** @file menubutton.cpp
 *  @brief Contains implementation for menubutton, derived from the QPushButton. It essentially associates a
 *  pushbutton to an item.
 *
*/

#include "menubutton.h"
#include "menu.h"
#include "mainwindow.h"
#include <QGraphicsView>
#include "dungeon.h"
#include "menu.h"

/**
 * @brief menubutton::activater SLOT
 * When button is pressed, call the associated game object's use() function to activate it.
 * It will then close the menu and bring the player back to the dungeon scene.
 */
void menubutton::activater()
{
    m_item->use();

    QGraphicsView* dungeon_view = new QGraphicsView(m_menu->m_dungeon);

    //Prevent Scrolling
    dungeon_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    dungeon_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    dungeon_view->setFixedSize(1100,650);

    this->m_menu->m_main_window->setCentralWidget(dungeon_view);
    m_menu->m_main_window->centralWidget()->grabKeyboard();

}
