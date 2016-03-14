/** @file mainwindow.h
 *  @brief contains Main Window Class Declaration
 *
 */


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "dungeon.h"
#include "menu.h"

class QPushButton;

namespace Ui {
class MainWindow;
}

/** @class MainWindow
 *  @brief This will be what will be responsible for displaying what's happening in the dungeon onto the screen.
 *  will take a GraphicsSceneView as a central widget when the game starts. Also contains a SLOT to allow it to
 *  know when to start the game.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    /*These slots are so the mainwindow radiobuttons/pushbuttons can change values*/
public slots:
    //starts the game when "START" is pushed
    void begin_game();

    //Slots for Radio Buttons
    /**
     * @brief select_swordsman SLOT. Will make classname "Swordsman" which will be used during dungeon initialization.
     */
    void select_swordsman(){classname = "Swordsman"; class_is_selected=true;}

    /**
     * @brief select_archer SLOT. Will make classname "Archer" which will be used during dungeon initialization.
     */
    void select_archer(){classname = "Archer"; class_is_selected=true;}
    /**
     * @brief select_wizard SLOT. Will make classname "Wizard" which will be used during dungeon initialization.
     */
    void select_wizard(){classname = "Wizard"; class_is_selected=true;}

    /**
     * @brief is_class_selected Accessor.
     * @return boolean whether or class was selected in start screen. Used to make sure user actually selects a class before starting the game.
     */
    bool is_class_selected(){return class_is_selected;}

    //Change Gameover boolean
    /**
     * @brief make_gameover Mutator. Makes the gameover flag true.
     */
    void make_gameover(){is_gameover=true;}

    //Instructions
    void instruct();

    //About
    void about();

    //Close
private:
    Ui::MainWindow *ui;

    //Player's Class
    QString classname;

    //The Dungeon
    Dungeon * m_scene;

    //The Equipment menu
    my_menu* m_menu;

    /*Booleans used to make sure certain events happened within the application.*/
    bool is_gameover; //if the game is over, true. false otherwise
    bool class_is_selected; //This boolean is to make sure a class was selected at the main screen.
};

#endif // MAINWINDOW_H
