/** @file mainwindow.cpp
 *  @brief Implementation of mainwindow.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dungeon.h"
#include "menu.h"
#include "character.h"
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QPainter>
#include <QColor>
#include <QMessageBox>
#include <memory>
#include <QIcon>

/**
 * @brief MainWindow::MainWindow Constructor. Will set up the game. Ui is set up in the designer form.
 * Has various connect statements to connect the push/radiobuttons used in order to start the game.
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    class_is_selected(false),
    is_gameover(false)
{
    ui->setupUi(this);
    m_scene = nullptr;



    //Radio Button Selection
    connect(ui->radioButton,SIGNAL(toggled(bool)),this, SLOT(select_swordsman()));
    connect(ui->radioButton_2,SIGNAL(toggled(bool)),this, SLOT(select_archer()));
    connect(ui->radioButton_3,SIGNAL(toggled(bool)),this, SLOT(select_wizard()));

    //Start the game when button is pressed.
    connect(ui->pushButton, SIGNAL(clicked()),this, SLOT(begin_game()));


    //Quit
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));

    //Instructions
    connect(ui->actionInstructions, SIGNAL(triggered()), this, SLOT(instruct()));

    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(about()));
}

/**
 * @brief MainWindow::~MainWindow Deletes UI and all associated widgets
 */
MainWindow::~MainWindow()
{
    //Delete UI and all associated widgets

     if((this->is_gameover==false)&&(this->is_class_selected()==true))
     {
         delete this->m_menu;
         delete this->m_scene;
     }

    delete ui;

}

/**
 * @brief MainWindow::begin_game SLOT Starts the game!
 * Creates a Dungeon and sets its properties. Then it creates a my_menu and sets its properties.
 * Then sets the created Dungeon to be the central widget and starts the game.
 */
void MainWindow::begin_game()
{
    //Look into QStackedWidgets

    if(this->is_class_selected()==false)
    {QMessageBox error;
    error.critical(0, "Uh-oh", "Please Select a Class!");
    error.setFixedSize(500,200);
    return;
    }

    //Create a Graphics Scene and Give MainWindow Access to it
    Dungeon * scene = new Dungeon(this, this->classname);
    scene->setParent(this);
    this->m_scene = scene;
    scene->setSceneRect(0,0,1100, 650);

    //make the scene BLACK
    this->m_scene->setBackgroundBrush(QBrush(QColor(0,0,0)));


    //Create a graphics view to display the scene
    QGraphicsView* m_dungeon_view = new QGraphicsView(scene);

    //Prevent Scrolling
    m_dungeon_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_dungeon_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_dungeon_view->setFixedSize(1100,650);


    this->setCentralWidget(m_dungeon_view); //Set central widget of mainwindow to this graphics scene

    //Create Menu
       m_menu=  new my_menu(m_scene->get_player(),m_scene,this);
       m_menu->setParent(this);
       m_scene->set_menu(m_menu);
    this->centralWidget()->grabKeyboard();

}

/**
 * @brief MainWindow::instruct SLOT Displays the instuctions in a separate window
 */
void MainWindow::instruct()
{

        QMessageBox instructions;
        instructions.setWindowTitle("Instructions");
        instructions.information(this,"Instructions",  " Descend the dungeon by finding the staircase in each level. \n"
                                                       " Each floor will have increasing number of monsters based on its level. \n"
                                                       " Collect Scrolls and Weapons to power up your character. \n"
                                                       " Complete the game by making it to the 5th floor and collecting the Gold Item.\n"
                                                       " The Golden Item will be guarded by 4 powerful dragons. Be prepared!\n"
                                                       "   Controls: Movement Using the Arrow Keys \n"
                                                       "                  W: Attack Up \n"
                                                       "                  A: Attack Left \n"
                                                       "                  S: Attack Down \n"
                                                       "                  D: Attack Right \n"
                                                       "                  E: Pick up Item or Descend Staircase \n"
                                                       "                  M: Open/Close Equipment Menu \n"
                                                       );

}

/**
 * @brief MainWindow::about SLOT Shows information about the application in a separate window
 */
void MainWindow::about()
{
    QMessageBox about;
    about.setWindowTitle("About");
    about.setText("Simple Rogue\n"
                "Author: Ryan Kim \n"
                  "Date: March 2016 \n"
                  "Graphics Found on videogamesprites.net \n"
                  );
    about.exec();
}
