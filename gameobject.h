/** @file gameobject.h
 *  @brief Contains all of the game objects that will be contained in the dungeon and used by the player.
 *
 */

#ifndef ITEMS_H
#define ITEMS_H


#include <QGraphicsPixmapItem>
#include <QString>
#include <QDebug>

//Forward Declarations
class Dungeon;

/** @class object
 * @brief Abstract Class. Derived from QGraphicsPixmapItem. Types of game objects include weapons, scrolls, and
 * game advancement objects such as stairways and the winning item. object contains a QString
 * to store its name and a pointer to the dungeon that it's contained in. Derived classes are essentially
 * separated by what image is used to represent them in the QGraphicsScene.
*/
class object :public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:

    /**
     * @brief object default constructor. Should never be used.
     */
    object(){}

    /**
     * @brief object should be used as the primary constructor
     * @param x coordinate
     * @param y coordinate
     * @param dunge dungeon that item is contained in
     */
    object(int x, int y, Dungeon* dunge){this->setPos(x,y); m_dun=dunge;}

    /**
     * @brief ~object destructor
     */
    virtual ~object(){}

    /**
     * @brief get_dun Accessor.
     * @return pointer to item's dungeon.
     */
    Dungeon* get_dun(){return m_dun; }

    /**
     * @brief change_name Mutator. Changes item name
     * @param new_name name to be changed to.
     */
    void change_name(QString new_name){name=new_name;}

    /**
     * @brief get_name Accessor.
     * @return object's name
     */
    QString get_name(){return name;}

    /**
     * @brief use pure virtual function. Will be the object's functionality in derived classes.
     */
    virtual void use()=0;
private:
    QString name; //object's name
    Dungeon * m_dun; //make sure object knows what dungeon it's in
};

/** @class weapon
 * @brief derived from object. Has an integer variable to store how much damage a weapon is capable of dealing.
*/
class weapon : public object
{
public:
    /**
     * @brief weapon default constructor. Should not be used.
     */
    weapon(){}

    /**
     * @brief weapon should be used as the weapon's main constructor. Creates an object with position (x,y)
     * @param x coordinate
     * @param y coordinate
     * @param dungeon dungeon item is to be contained in
     * @param pwr attack power of weapon
     */
    weapon(int x, int y, Dungeon* dungeon, int pwr):object(x,y,dungeon){power=pwr;}

    /**
     * @brief ~weapon virtual destructor
     */
    virtual ~weapon(){}
    void use();

    /**
     * @brief get_power Accessor.
     * @return attack power of weapon
     */
    int get_power(){return power;}

private:
    /** @var power is extra attack power the weapon gives the character */
    int power;
};

/** @class dagger
 *  @brief A type of weapon. Generally has a lower power than a sword. Will be the starting weapon.
 *
 */
class dagger : public weapon
{
public:
    /**
     * @brief dagger default constructor. Should NOT be used.
     */
    dagger(){}

    /**
     * @brief dagger main constructor for dagger. No Pixmap is created.
     * @param x coordinate
     * @param y coordinate
     * @param dungeon dungeon pointer
     * @param identifier dagger's name
     * @param pwr attack power
     */
    dagger(int x, int y, Dungeon * dungeon, QString identifier, int pwr):weapon(x,y,dungeon,pwr)
    {
        change_name(identifier);
    }

    /**
     * @brief ~dagger
     */
    virtual ~dagger(){}
};

/** @class sword
 *  @brief another class derived from weapon. Slightly stronger than a dagger. Can be found in the dungeon.
*/
class sword : public weapon
{
public:
    /**
     * @brief sword Default constructor. Should not be used.
     */
    sword(){}

    /**
     * @brief sword primary constructor. Creates a weapon object with coordinates (x,y) with attack power of pwr. sets the pixmap
     * @param x coordinate
     * @param y coodtinate
     * @param dungeon pointer to dungeon
     * @param identifier name
     * @param pwr attack power
     */
    sword(int x, int y, Dungeon * dungeon, QString identifier, int pwr):weapon(x,y,dungeon,pwr)
    {
        QPixmap image(":/new/images/sword.gif");
        this->setPixmap(image.scaled(25,25,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        change_name(identifier);
    }

    /**
     * @brief ~sword
     */
    virtual ~sword(){}
};

/**
 * @class whip
 * @brief The whip class. Another type of weapon.
 */
class whip : public weapon
{
  public:
    /**
     * @brief whip should be used as the main constructor. Creates a gameobject at coordinates (x,y). Also sets the pixmap
     * @param x coordinate
     * @param y coordinate
     * @param dungeon pointer
     * @param identifier name
     * @param pwr attack points
     */
    whip(int x, int y, Dungeon *dungeon, QString identifier, int pwr): weapon(x,y,dungeon,pwr)
    {
        QPixmap image(":/new/images/whip.gif");
        this->setPixmap(image.scaled(25,25,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        change_name(identifier);
    }

    /**
     * @brief ~whip
     */
    virtual ~whip(){}
};

/**
 * @class hammer
 * @brief The hammer class. another weapon.
 */
class hammer: public weapon
{
public:
    /**
     * @brief hammer Should be used as the main constructor for hammer objects. Creates a gameobject at coordinate (x,y) and sets the PixMap.
     * @param x coordinate
     * @param y coordinate
     * @param dungeon pointer
     * @param identifier name
     * @param pwr attack points
     */
    hammer(int x, int y, Dungeon *dungeon,QString identifier, int pwr):weapon(x,y,dungeon,pwr)
    {
        QPixmap image(":/new/images/hammer.gif");
        this->setPixmap(image.scaled(25,25,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        change_name(identifier);
    }

    /**
     * @brief ~hammer
     */
    virtual ~hammer(){}
};

/**
 * @class glove Another type of weapon
 * @brief The glove class
 */
class glove: public weapon
{
public:
    /**
     * @brief glove main constructor. Sets position to (x,y) and sets the pixmap
     * @param x coordinate
     * @param y coordinate
     * @param dungeon pointer to dungeon
     * @param identifier object name
     * @param pwr attack points
     */
    glove(int x, int y, Dungeon * dungeon, QString identifier, int pwr):weapon(x,y,dungeon,pwr)
    {
        QPixmap image(":/new/images/glove.gif");
        this->setPixmap(image.scaled(25,25,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        change_name(identifier);
    }

    /**
     * @brief ~glove
     */
    virtual ~glove(){}
};

/** @class goldenitem
 *  @brief This item will be generated in the dungeon after the player makes it to the 5th floor.
 *  Once the player collects this item, they win.
 */
class goldenitem: public object
{
public:
    /**
     * @brief goldenitem Default constructor
     */
    goldenitem(){}

    /**
      *@brief ~goldenitem destructor
      */
    virtual ~goldenitem(){}

    /**
     * @brief goldenitem constructor. Creates a gameobject with coordinates (x,y). sets the pixmap
     * @param x coordinate
     * @param y coordinate
     * @param dungeon pointer to dungeon it's contained in.
     */
    goldenitem(int x, int y, Dungeon* dungeon):object(x,y,dungeon){QPixmap image(":/new/images/goldenidol.png");
                                         this->setPixmap(image.scaled(25,25,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
                                         this->setPos(x,y);}
    /**
     * @brief use Not actually used. Just here so it doesn't become an abstact class.
     */
    void use(){}

};

/** @class stairs
 *  @brief This object is used to advance the floor level.
*/
class stairs: public object
{
public:
    /**
     * @brief stairs Default Constructor
     */
    stairs(){}

    /**
     *@brief stairs destructor
    */
    virtual ~stairs(){}

    /**
     * @brief stairs main constructor. Sets coordinates to (x,y). sets the pixmap
     * @param x coordinate
     * @param y coodrinate
     * @param dungeon pointer to dungeon stairs are contained in
     */
    stairs(int x, int y, Dungeon* dungeon):object(x,y,dungeon){
        QPixmap image(":/new/images/stairway.png");
        this->setPixmap(image.scaled(25,25,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        this->setPos(x,y);
    }
    /**
    * @brief use not actually used. Only declared so this doesn't become an abstract class.
    */
   void use(){}
};

/** @class scroll
 *  @brief An abstract class. There will be three types of scrolls. Scrolls that heal hit points,
 *  raise attack power, or raise defense of the player.
*/
class scroll: public object
{
public:
    /**
     * @brief scroll default constructor
     */
    scroll(){}
    /**
     * @brief scroll main constructor.
     * @param x coordinate
     * @param y coodrinate
     * @param dungeon pointer
     */
    scroll(int x, int y, Dungeon* dungeon):object(x,y, dungeon){}
    /**
     * @brief ~scroll destructor
     */
    virtual ~scroll(){}
    /**
     * @brief use only to be used by derived classes
     */
    virtual void use()=0;

};

/** @class heal_scroll
 *  @brief one of the scrolls. Will heal the Player's hit points.
*/
class heal_scroll: public scroll
{
public:
    /**
     * @brief heal_scroll default constructor
     */
    heal_scroll(){}
    /**
     * @brief heal_scroll main constuctor. Sets the pixmap
     * @param x coordinate
     * @param y coordinate
     * @param dungeon where the scroll will be displayed.
     */
    heal_scroll(int x,int y, Dungeon* dungeon):scroll(x,y, dungeon){
                                        QPixmap image(":/new/images/heal_scroll.gif");
                                        this->setPixmap(image.scaled(25,25,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
                                        this->setPos(x,y);
                                        change_name(QString("Healing Scroll"));
                                        }
    /**
     * @brief ~heal_scroll destructor
     */
    virtual ~heal_scroll(){}
    void use();

};

/** @class atk_scroll
 *  @brief one of the scrolls. Will increase the player's attack power.
*/
class atk_scroll: public scroll
{
public:
    /**
     * @brief atk_scroll
     */
    atk_scroll(){}
    /**
     * @brief atk_scroll constructor that will be used throughout code. Sets the pixmap
     * @param x
     * @param y
     * @param dungeon
     */
    atk_scroll(int x,int y, Dungeon* dungeon):scroll(x,y, dungeon){
                                        QPixmap image(":/new/images/atk_scroll.gif");
                                        this->setPixmap(image.scaled(25,25,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
                                        this->setPos(x,y);
                                        change_name(QString("Attack Scroll"));
                                        }
    /**
     * @brief ~atk_scroll destructor
     */
    virtual ~atk_scroll(){}
    void use();

};

/** @class def_scroll
 *  @brief one of the scrolls. Will increase the player's defense.
*/
class def_scroll: public scroll
{
public:
    /**
     * @brief def_scroll should not be used.
     */
    def_scroll(){}
    /**
     * @brief def_scroll constructor that will be used throughout code. Sets the Pixmap
     * @param x coordinate
     * @param y coordinate
     * @param dungeon pointer to where scroll will be contained
     */
    def_scroll(int x,int y, Dungeon* dungeon):scroll(x,y, dungeon){
                                        QPixmap image(":/new/images/def_scroll.gif");
                                        this->setPixmap(image.scaled(25,25,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
                                        this->setPos(x,y);
                                        change_name(QString("Defense Scroll"));
                                        }
    /**
     * @brief ~def_scroll destructor
     */
    virtual ~def_scroll(){}
    void use();

};

#endif // ITEMS_H
