/*

String Helper. Provides to_string() function.

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

// $Revision: 6597 $ $Date:: 2017-04-12 #$ $Author: serge $

#ifndef LIB_PHONEBOOK_STR_HELPER_H
#define LIB_PHONEBOOK_STR_HELPER_H

#include <string>

#include "contact.h"        // ContactPhone::type_e

namespace phonebook
{

class StrHelper
{
public:
    static const std::string & to_string( const ContactPhone::type_e l );
    static const std::string & to_string( const gender_e l );

    static const std::string to_string( const Date & l );
    static const std::string to_string( const Contact & l );
    static const std::string to_string( const ContactPhone & l );
};

} // namespace phonebook

#endif // LIB_PHONEBOOK_STR_HELPER_H
