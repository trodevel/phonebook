/*

Phonebook manager - Status.

Copyright (C) 2017 Sergey Kolevatov

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

*/

// $Revision: 7457 $ $Date:: 2017-08-01 #$ $Author: serge $

#ifndef LIB_PHONEBOOK_STATUS_H
#define LIB_PHONEBOOK_STATUS_H

#include "contact.h"        // Contact

#include <vector>           // std::vector

namespace phonebook
{

struct ContactFlat
{
    ContactFlat():
        id( 0 ),
        user_id( 0 ),
        contact( nullptr )
    {
    }

    ContactFlat( contact_id_t id, user_id_t user_id, const Contact * contact ):
        id( id ),
        user_id( user_id ),
        contact( contact )
    {
    }

    contact_id_t        id;
    user_id_t           user_id;
    const Contact       * contact;
};

struct Status
{
    typedef std::vector<ContactFlat>  VectorContactFlat;

    contact_id_t        last_contact_id;
    contact_phone_id_t  last_phone_id;
    VectorContactFlat   contacts;
};

} // namespace phonebook

#endif // LIB_PHONEBOOK_STATUS_H
