/*

Phonebook manager - Contact.

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

// $Revision: 7281 $ $Date:: 2017-07-20 #$ $Author: serge $

#ifndef LIB_PHONEBOOK_CONTACT_H
#define LIB_PHONEBOOK_CONTACT_H

#include <string>               // std::string
#include <map>                  // std::map

#include "types.h"              // contact_phone_id_t

namespace phonebook
{

struct ContactPhone
{
    enum class type_e
    {
        DFLT        = 0,
        LANDLINE    = 1,
        MOBILE      = 2,
    };

    type_e          type;
    std::string     phone_number;
};

enum class gender_e
{
    UNDEF   = 0,
    MALE    = 1,
    FEMALE  = 2,
};

struct Date
{
    uint32_t        year;
    uint8_t         month;
    uint8_t         day;
};

struct Contact
{
    gender_e                gender;
    std::string             name;
    std::string             first_name;
    Date                    birthday;
    std::string             notice;

    std::map<contact_phone_id_t,ContactPhone> map_id_to_phone;
};

} // namespace phonebook

#endif // LIB_PHONEBOOK_CONTACT_H
