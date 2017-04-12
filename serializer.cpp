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

// $Revision: 6612 $ $Date:: 2017-04-12 #$ $Author: serge $

#include "serializer.h"     // self

#include <stdexcept>        // std::invalid_argument

#include "utils/nonascii_hex_codec.h"   // nonascii_hex_codec::decode
#include "utils/hex_codec.h"            // utils::unhex_string

namespace serializer
{

phonebook::Contact* load( std::istream & is, phonebook::Contact* e )
{
    if( e != nullptr )
        throw std::invalid_argument( "Serializer::load: e must be null" );

    auto el = new phonebook::Contact;

    auto res = phonebook::Serializer::load( is, static_cast< phonebook::Contact *>( el ) );

    if( res == nullptr )
    {
        delete el;
        return nullptr;
    }

    return el;
}

phonebook::Contact** load( std::istream & is, phonebook::Contact** e )
{
    auto res = load( is, static_cast< phonebook::Contact *>( nullptr ) );

    if( res == nullptr )
    {
        return nullptr;
    }

    *e =  res;

    return e;
}

phonebook::ContactPhone* load( std::istream & is, phonebook::ContactPhone* e )
{
    return phonebook::Serializer::load( is, e );
}

bool save( std::ostream & os, const phonebook::ContactPhone & e )
{
    return phonebook::Serializer::save( os, e );
}

bool save( std::ostream & os, const phonebook::Contact * e )
{
    return phonebook::Serializer::save( os, * e );
}

}

namespace phonebook
{

Date* Serializer::load_1( std::istream & is, Date* e )
{
    if( e == nullptr )
        throw std::invalid_argument( "Serializer::load: Date is null" );

    uint32_t    year;
    uint32_t    month;
    uint32_t    day;

    is >> year >> month >> day;

    if( is.fail() )
        return nullptr;

    e->year         = year;
    e->month        = static_cast<uint8_t>( month );
    e->day          = static_cast<uint8_t>( day );

    return e;
}

Date* Serializer::load( std::istream & is, Date* e )
{
    return load_t_1( is, e );
}

bool Serializer::save( std::ostream & os, const Date & e )
{
    static const unsigned int VERSION = 1;

    os << VERSION << " ";

    os << e.year     << " "
            << static_cast<unsigned>( e.month ) << " "
            << static_cast<unsigned>( e.day ) << " ";

    if( os.fail() )
        return false;

    return true;
}

ContactPhone* Serializer::load_1( std::istream & is, ContactPhone* e )
{
    if( e == nullptr )
        throw std::invalid_argument( "Serializer::load: ContactPhone is null" );

    uint32_t    id;
    uint32_t    type;
    std::string phone_number;

    is >> id >> type >> phone_number;

    if( is.fail() )
        return nullptr;

    e->id           = id;
    e->type         = static_cast<ContactPhone::type_e>( type );
    e->phone_number   = utils::nonascii_hex_codec::decode( phone_number );

    return e;
}

ContactPhone* Serializer::load( std::istream & is, ContactPhone* e )
{
    return load_t_1( is, e );
}

bool Serializer::save( std::ostream & os, const ContactPhone & e )
{
    static const unsigned int VERSION = 1;

    os << VERSION << " ";

    os << e.id     << " "
            << static_cast<unsigned>( e.type ) << " "
            << utils::nonascii_hex_codec::encode( e.phone_number );

    if( os.fail() )
        return false;

    return true;
}

Contact* Serializer::load_1( std::istream & is, Contact* e )
{
    if( e == nullptr )
        throw std::invalid_argument( "Serializer::load: Contact is null" );

    uint32_t    id;
    uint32_t    gender;
    std::string name;
    std::string first_name;
    std::string notice;

    is >> id >> gender >> name >> first_name;

    auto b = load( is, & e->birthday );

    if( b == nullptr )
        return nullptr;

    is >> notice;

    if( is.fail() )
        return nullptr;

    e->id           = id;
    e->gender       = static_cast<gender_e>( gender );
    e->name         = utils::nonascii_hex_codec::decode( name );
    e->first_name   = utils::nonascii_hex_codec::decode( first_name );
    e->notice       = utils::nonascii_hex_codec::decode( notice );

    auto res = serializer::load( is, & e->map_id_to_phone );

    if( res == nullptr )
    {
        return nullptr;
    }

    return e;
}

Contact* Serializer::load( std::istream & is, Contact* e )
{
    return load_t_1( is, e );
}

bool Serializer::save( std::ostream & os, const Contact & e )
{
    static const unsigned int VERSION = 1;

    os << VERSION << " ";

    os << e.id     << " "
            << static_cast<unsigned>( e.gender ) << " "
            << utils::nonascii_hex_codec::encode( e.name ) << " "
            << utils::nonascii_hex_codec::encode( e.first_name );

    if( save( os, e.birthday ) == false )
        return false;

    os << utils::nonascii_hex_codec::encode( e.notice ) << " ";

    if( os.fail() )
        return false;

    bool b = serializer::save( os, e.map_id_to_phone );

    if( b == false )
        return false;

    return true;
}

Status* Serializer::load_1( std::istream & is, Status* e )
{
    if( e == nullptr )
        throw std::invalid_argument( "Serializer::load: Status is null" );

    uint32_t                last_contact_id;
    uint32_t                last_phone_id;
    Status::MapIdToContact  map_id_to_contact;

    is >> last_contact_id >> last_phone_id ;

    if( is.fail() )
        return nullptr;

    e->last_contact_id  = last_contact_id;
    e->last_phone_id    = last_phone_id;

    auto res = serializer::load( is, & e->map_id_to_contact );

    if( res == nullptr )
    {
        return nullptr;
    }

    return e;
}

Status* Serializer::load( std::istream & is, Status* e )
{
    return load_t_1( is, e );
}

bool Serializer::save( std::ostream & os, const Status & e )
{
    static const unsigned int VERSION = 1;

    os << VERSION << " ";

    os << e.last_contact_id << " "
            << e.last_phone_id << " ";

    if( serializer::save( os, e.map_id_to_contact ) == false )
        return false;

    return true;
}

} // namespace phonebook
