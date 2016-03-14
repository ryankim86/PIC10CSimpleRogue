/** @file menu.cpp
 *  @brief Contains implementation for the my_menu, a class that is used to represent
 *  the equipment and character information menu screen.
 *
*/
#include "menu.h"
#include "mainwindow.h"
#include "menubutton.h"
#include "inventory.h"
#include <QDebug>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsProxyWidget>
#include <QKeyEvent>
#include <QBrush>
#include <QFont>

/**
 * @brief my_menu::my_menu Default Constructor. Should not actually be called.
 */
my_menu::my_menu(){}

/**
 * @brief my_menu::my_menu constructor with proper input parameters.
 * @param player
 * @param dungeon
 * @param main_window
 */
my_menu::my_menu(Player *player, Dungeon *dungeon, MainWindow *main_window)
{
    this->m_dungeon = dungeon;
    this->m_player = player;
    this->m_main_window=main_window;


    //Draw GUI Elements
        QGraphicsRectItem * left_panel = new QGraphicsRectItem(0,0,25,645);
        QGraphicsRectItem * right_panel = new QGraphicsRectItem(1075,0,25,645);
        QGraphicsRectItem * top_panel = new QGraphicsRectItem(25,0,1050,50);
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
        QGraphicsTextItem * title = new QGraphicsTextItem(QString("Menu"));
            //Set Title Properties
                QFont title_font("tahoma", 40);
                title->setFont(title_font);
                title->setPos(450,-15); // Top Middleish of the screen
                title->setDefaultTextColor(QColor(255,255,255)); //color White
           // Add item to dungeon and set it as parent
                this->addItem(title);
                title->setParent(this);


        //Display Inventory Title
                QGraphicsTextItem *inv_title = new QGraphicsTextItem(QString("Player Inventory"));
                QFont inv_font("tahoma", 30);
                inv_title->setFont(inv_font);
                inv_title->setPos(30,125);
                this->addItem(inv_title);

        QString job = m_player->get_name();
//Display Character Information
       player_icon = new QGraphicsPixmapItem;


       if(job=="Archer")
            {QPixmap image(":/new/images/archer/down_frame_0.gif");
           player_icon->setPixmap(image.scaled(50,50,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));}
       else if(job=="Swordsman")
            {QPixmap image(":/new/images/swordsman/down_frame_0.gif");
         player_icon->setPixmap(image.scaled(50,50,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));}
       else if (job=="Wizard")
            {QPixmap image(":/new/images/wiz/down_frame_0.gif");
        player_icon->setPixmap(image.scaled(50,50,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));}
       else
            {QPixmap image(":/new/images/wiz/down_frame_0.gif");
        player_icon->setPixmap(image.scaled(50,50,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));}



           player_icon->setPos(650,55);
             this->addItem(player_icon);
             player_title = new QGraphicsTextItem(QString("Player Information"));
             player_info = new QGraphicsTextItem(QString("Character Class: ") + m_player->get_name()+ QString("\n"
                 "Level: ") + QString::number(m_player->get_lvl())
                      +QString("\n""HP: ")+QString::number(m_player->get_curr_hp()) + QString("/") +QString::number(m_player->get_hp())
                         +QString("\n""Attack: ")+QString::number(m_player->get_atk())
                       +QString("\n""Defense: ") +QString::number(m_player->get_def())
                       +QString("\n""Exp: ") +QString::number(m_player->get_exp()) +QString("/") +QString::number(m_player->get_lvl()*15));
     QFont bottom_font("tahoma", 15);
  player_title->setFont(inv_font);
         player_title->setPos(500,125);
  player_info->setFont(bottom_font);
  player_info->setPos(500,175);
   this->addItem(player_title);
   this->addItem(player_info);


//Display "Press M to exit Menu"
  //Display Inventory Title
          QGraphicsTextItem *inv_direc = new QGraphicsTextItem(QString("Press M to Exit Menu!"));
          inv_direc->setFont(inv_font);
          inv_direc->setPos(650,590);
          inv_direc->setDefaultTextColor(QColor(255,255,255));
          this->addItem(inv_direc);
}

/**
 * @brief my_menu::~my_menu Destructor. Deallocates widgets in scene
 */
my_menu::~my_menu()
{
    std::vector<menubutton*>::iterator it = displayed_items.begin();
    while(it != displayed_items.begin())
    {
            delete *it;
            it=displayed_items.erase(it);

    }
}



/**
 * @brief my_menu::keyPressEvent handles keypresses. Only key event it recognizes is "M," which will close the menu.
 * @param event key press
 */
void my_menu::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_M)
    {
        QGraphicsView* dungeon_view = new QGraphicsView(m_dungeon);

        //Prevent Scrolling
        dungeon_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        dungeon_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        dungeon_view->setFixedSize(1100,650);

        m_main_window->setCentralWidget(dungeon_view);
        this->m_main_window->centralWidget()->grabKeyboard();
    }
}

/**
 * @brief my_menu::remove_displayed_item deallocates widget memory and removes the pointer from vector of displayed items
 * @param x object to be removed
 * @return
 */
bool my_menu::remove_displayed_item(object *x)
{
    std::vector<menubutton*>::iterator it = displayed_items.begin();
    while(it != displayed_items.begin())
    {
        if((*it)->m_item==x)
        {
            delete *it;
            it=displayed_items.erase(it);
            return true;

        }
        else
        {++it;}
    }
            return false;
}

/**
 * @brief my_menu::update_menu recreates the objects that are to be displayed in the menu
 * or updates the information if appropriate
 */
void my_menu::update_menu()
{
    inventory * player_inv = m_player->get_inv();
    int size = player_inv->get_size();
    player_inv->get_vector().begin();



     //Get Rid of old displyed items
     std::vector<menubutton*>::iterator it = displayed_items.begin();
     while(it!=displayed_items.end())
     {
         delete *it;
         it=displayed_items.erase(it);

     }


    //Display Player's Items
    for(int i=0;i<size;++i)
    {
        QString item_name = player_inv->get_vector()[i]->get_name();
        menubutton * an_item = new menubutton(player_inv->get_vector()[i],this);
        an_item->setText(item_name);
        an_item->setGeometry(QRect(30,180+(i*28),100,25));
        QFont fontz("tahoma", 10);
        an_item->setFont(fontz);
        displayed_items.push_back(an_item);
        this->addWidget(an_item);
        connect(an_item,SIGNAL(clicked()),an_item,SLOT(activater()));
    }

    //update player information
    player_info->setPlainText(QString("Character Class: ") + m_player->get_name()+ QString("\n"
                                                                                           "Level: ") + QString::number(m_player->get_lvl())
                                                                                            +QString("\n""HP: ")+QString::number(m_player->get_curr_hp()) + QString("/") +QString::number(m_player->get_hp())
                                                                                            +QString("\n""Attack: ")+QString::number(m_player->get_atk())
                                                                                            +QString("\n""Defense: ") +QString::number(m_player->get_def())
                                                                                            +QString("\n""Exp: ") +QString::number(m_player->get_exp()) +QString("/") +QString::number(m_player->get_lvl()*15)
                              );



    }
