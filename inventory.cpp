/** @file inventory.cpp
 *  @brief Contains implementation for the player's inventory Objects.
 *
*/

#include "inventory.h"
#include <QDebug>

/**
 * @brief inventory::~inventory destructor. Deallocates memory of the pointers.
 */
inventory::~inventory()
{
    std::vector<object*>::iterator it = m_objects.begin();

    while(it !=m_objects.end())
    {
            delete *it;
            it = m_objects.erase(it);
            --inv_size;
    }

}

/**
 * @brief inventory::remove_item Will remove object from vector AND delete it.
 * @param x item to be removed.
 * @return true object was successfully removed. false otherwise
 */
bool inventory::remove_item(object *x)
{
   std::vector<object*>::iterator it = m_objects.begin();

   while(it !=m_objects.end())
   {
       if(*it == x)
       {
           delete *it;
           it = m_objects.erase(it);
           --inv_size;
           return true;
       }
       else
           {++it;}
   }
        return false;
}

/**
 * @brief inventory::erase_item will erase the pointer from the vector but will NOT deallocate the memory
 * @param x object to be erased
 * @return true if object was erased. false otherwise
 */
bool inventory::erase_item(object *x)
{
   std::vector<object*>::iterator it = m_objects.begin();

   while(it !=m_objects.end())
   {
       if(*it == x)
       {
           it = m_objects.erase(it);
           --inv_size;
           return true;
       }
       else
           {++it;}
   }
        return false;
}
