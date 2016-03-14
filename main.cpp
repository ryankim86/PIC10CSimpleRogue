/** @mainpage Simple Rogue
 *  @title Simple Rogue
 *  @author Ryan Kim
 *  @date 3-10-2016
 *
 *  This program is a simplified version of the 1980s game Rogue. This is much simpler than the original game.
 *  The player will go through floors, fighting monsters and collecting items. The game ends on the 5th floor.
 *  There was a lot more that I wanted to do with this game, but I ran out of time.
*/



/** @file main.cpp
 *  @brief Main Function
*/

#include "mainwindow.h"
#include "dungeon.h"
#include "character.h"
#include <QApplication>

/**
 * @brief main Main function. Nothing special.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   MainWindow w;
    w.show();

    return a.exec();
}
