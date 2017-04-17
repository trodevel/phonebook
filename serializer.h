/*

Serializer.

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

// $Revision: 6630 $ $Date:: 2017-04-13 #$ $Author: serge $

#ifndef LIB_PHONEBOOK_SERIALIZER_H
#define LIB_PHONEBOOK_SERIALIZER_H

#include <iostream>         // std::istream

#include "serializer/versionable_loader_t.h"        // serializer::VersionableLoaderT
#include "contact.h"        // Contact
#include "status.h"         // Status

namespace serializer
{
//phonebook::ContactFlat** load( std::istream & is, phonebook::ContactFlat** e );
phonebook::ContactFlat* load( std::istream & is, phonebook::ContactFlat* e );
phonebook::ContactPhone* load( std::istream & is, phonebook::ContactPhone* e );
bool save( std::ostream & os, const phonebook::ContactPhone & e );
bool save( std::ostream & os, const phonebook::ContactFlat & e );
}

#include "serializer/serializer.h"   // serializer::

namespace phonebook
{

class Serializer: public serializer::VersionableLoaderT<Serializer>
{
    friend serializer::VersionableLoaderT<Serializer>;

public:
    static Date* load( std::istream & is, Date* e );
    static bool save( std::ostream & os, const Date & e );

    static ContactPhone* load( std::istream & is, ContactPhone* e );
    static bool save( std::ostream & os, const ContactPhone & e );

    static Contact* load( std::istream & is, Contact* e );
    static bool save( std::ostream & os, const Contact & e );

    static ContactFlat* load( std::istream & is, ContactFlat* e );
    static bool save( std::ostream & os, const ContactFlat & e );

    static Status* load( std::istream & is, Status* e );
    static bool save( std::ostream & os, const Status & e );

private:

    static Date* load_1( std::istream & is, Date* e );
    static ContactPhone* load_1( std::istream & is, ContactPhone* e );
    static Contact* load_1( std::istream & is, Contact* e );
    static ContactFlat* load_1( std::istream & is, ContactFlat* e );
    static Status* load_1( std::istream & is, Status* e );
};

} // namespace phonebook

#endif // LIB_PHONEBOOK_SERIALIZER_H
