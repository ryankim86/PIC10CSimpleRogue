/** @file inventory.h
 *  @brief Contains declaration of the inventory class, which will be used to manage the dungeon's and player's gameobjects
 *
 */

#ifndef INVENTORY_H
#define INVENTORY_H

#include "gameobject.h"
#include "menu.h"
#include <vector>


/** @class inventory
 * @brief This class will manage the items the player and dungeon has. Items will be stored in a vector of object pointers.
 * Various member functions are used to remove/erase/add objects to the inventory.
*/
class inventory
{
public:
    /**
     * @brief inventory default constructor. Will initialize inv_size to 0.
     */
    inventory(){inv_size=0;}
    /**
     * @brief ~inventory destructor
     */
    virtual ~inventory();

    //Add and Remove
        /**
         * @brief add_item Adds item to the inventory by pushing it onto the vector of objects.
         * @param x object pointer to be added
         */
        void add_item(object* x){m_objects.push_back(x); ++inv_size;}

        bool remove_item(object* x);
        bool erase_item(object *x);
    //Accessors
        /**
         * @brief get_size Accessor.
         * @return inventory size
         */
        int get_size(){return inv_size;}

        /**
         * @brief get_vector Accessor.
         * @return reference to the vector that contains the pointers to the objects.
         */
        std::vector<object*>& get_vector(){ return m_objects; }
private:
    std::vector<object*> m_objects;
    int inv_size;


};


#endif // INVENTORY_H
