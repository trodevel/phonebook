/*

Phonebook manager - Save/Load.

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

// $Revision: 6636 $ $Date:: 2017-04-13 #$ $Author: serge $

#ifndef LIB_PHONEBOOK_SAVELOAD_H
#define LIB_PHONEBOOK_SAVELOAD_H

#include "phonebook.h"  // Phonebook


namespace phonebook
{

bool save( std::string * error_msg, const Phonebook & pb, const std::string & filename );
bool load( Phonebook * pb, std::string * error_msg, const std::string & filename );

} // namespace phonebook

#endif // LIB_PHONEBOOK_SAVELOAD_H
