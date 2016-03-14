/** @file gameobject.cpp
 *  @brief Contains implementation for the game objects.
 *
*/

#include "gameobject.h"
#include <inventory.h>
#include "dungeon.h"

/**
 * @brief heal_scroll::use heals the player's hit points by 10 and removes itself from player's inventory.
 */
void heal_scroll::use()
{
    this->get_dun()->get_player()->heal_hp(10);
    get_dun()->update_text("You used a healing scroll to heal 10 HP","");
    get_dun()->get_player()->get_inv()->remove_item(this);
}

/**
 * @brief weapon::use makes the player equip the weapon
 */
void weapon::use()
{
    get_dun()->get_player()->equip_item(this);
    get_dun()->update_text("You equipped " + this->get_name() +"!" ,"");
}

/**
 * @brief atk_scroll::use increases player's attack power by 2, then removes itself from inventory
 */
void atk_scroll::use()
{
    get_dun()->get_player()->increase_atk(2);
    get_dun()->update_text("You used an Attack Enhancement Scroll and gained 2 attack points!", "");
    get_dun()->get_player()->get_inv()->remove_item(this);
}

/**
 * @brief def_scroll::use increases player's defense power by 2, then removes itself from inventory
 */
void def_scroll::use()
{
    get_dun()->get_player()->increase_def(2);
    get_dun()->update_text("You used an Defense Enhancement Scroll and gained 2 defense points!", "");
    get_dun()->get_player()->get_inv()->remove_item(this);
}
