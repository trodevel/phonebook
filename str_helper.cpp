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

// $Revision: 7706 $ $Date:: 2017-08-24 #$ $Author: serge $

#include "str_helper.h"             // self

#include <sstream>                  // std::ostringstream
#include <map>

namespace phonebook
{

#define TUPLE_VAL_STR(_x_)  _x_,#_x_
#define TUPLE_STR_VAL(_x_)  #_x_,_x_

#define MAP_INSERT_VAL( _m, _val )      _m.insert( Map::value_type( _val ) )

const std::string & StrHelper::to_string( const ContactPhone::type_e l )
{
    typedef std::map< ContactPhone::type_e, std::string > Map;
    static const Map m =
    {
            { ContactPhone::type_e:: TUPLE_VAL_STR( DFLT ) },
            { ContactPhone::type_e:: TUPLE_VAL_STR( LANDLINE ) },
            { ContactPhone::type_e:: TUPLE_VAL_STR( MOBILE ) },
    };

    auto it = m.find( l );

    static const std::string undef( "?" );

    if( it == m.end() )
        return undef;

    return it->second;
}

const std::string & StrHelper::to_string( const gender_e l )
{
    typedef std::map< gender_e, std::string > Map;
    static const Map m =
    {
            { gender_e:: TUPLE_VAL_STR( UNDEF ) },
            { gender_e:: TUPLE_VAL_STR( MALE ) },
            { gender_e:: TUPLE_VAL_STR( FEMALE ) },
    };

    auto it = m.find( l );

    static const std::string undef( "?" );

    if( it == m.end() )
        return undef;

    return it->second;
}

const std::string StrHelper::to_string( const Date & l )
{
    std::ostringstream s;

    s << l.year << "-" << (uint32_t) l.month << "-" << (uint32_t) l.day;

    return s.str();
}

const std::string StrHelper::to_string( const Contact & l )
{
    std::ostringstream s;

    s << to_string( l.gender ) << " " << l.name << " " << l.first_name << " " << to_string( l.birthday ) << " {"
            << l.map_id_to_phone.size() << ": ";

    for( auto & c : l.map_id_to_phone )
    {
        s << " " << to_string( c.second );
    }

    s << " }";

    s << " '" << l.notice << "'";

    return s.str();
}

const std::string StrHelper::to_string( const ContactPhone & l )
{
    std::ostringstream s;

    s << to_string( l.type ) << " " << l.phone_number;

    return s.str();
}

} // namespace phonebook
