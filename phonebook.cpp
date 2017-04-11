/*

Phonebook manager - Phonebook.

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

// $Revision: 6558 $ $Date:: 2017-04-10 #$ $Author: serge $

#include "phonebook.h"          // self

#include "utils/dummy_logger.h"     // dummy_log_debug
#include "utils/assert.h"           // ASSERT

namespace phonebook
{

Phonebook::Phonebook():
        log_id_( 0 ),
        last_contact_id_( 0 ),
        last_phone_id_( 0 )
{
}

bool Phonebook::init( uint32_t log_id )
{
    log_id_ = log_id;

    return true;
}

bool Phonebook::add_contact(
        uint32_t            * id,
        std::string         * error_msg,
        uint32_t            user_id,
        gender_e            gender,
        const std::string   & name,
        const std::string   & first_name,
        const Date          & birthday,
        const std::string   & notice )
{
    dummy_log_debug( log_id_, "add contact: %u, %u, %s, %s, %s", user_id, static_cast<uint32_t>( gender ), name.c_str(), first_name.c_str(), notice.c_str() );

    auto contact = new Contact;

    contact->gender     = gender;
    contact->name       = name;
    contact->first_name = first_name;
    contact->birthday   = birthday;
    contact->notice     = notice;

    *id = get_next_contact_id();

    contact->id         = *id;

    auto b = map_id_to_contact_.insert( std::make_pair( *id, contact ) ).second;

    ASSERT( b );

    b = map_contact_id_to_user_id_.insert( std::make_pair( *id, user_id )).second;

    ASSERT( b );

    auto it = map_user_id_to_contact_ids_.find( user_id );
    if( it == map_user_id_to_contact_ids_.end() )
    {
        dummy_log_debug( log_id_, "adding contact for new user: %u, contact id %u", user_id, id );

        std::set<uint32_t> ids;

        ids.insert( *id );
    }
    else
    {
        dummy_log_debug( log_id_, "adding contact for existing user: %u, contact id %u", user_id, id );

        auto & ids = it->second;

        ids.insert( *id );

        map_user_id_to_contact_ids_.insert( std::make_pair( *id, ids ) );
    }

    return true;
}

bool Phonebook::modify_contact(
        std::string         * error_msg,
        uint32_t            id,
        gender_e            gender,
        const std::string   & name,
        const std::string   & first_name,
        const Date          & birthday,
        const std::string   & notice )
{
    dummy_log_debug( log_id_, "modify contact: %u, %u, %s, %s, %s", id, static_cast<uint32_t>( gender ), name.c_str(), first_name.c_str(), notice.c_str() );
}

bool Phonebook::delete_contact(
        std::string         * error_msg,
        uint32_t            id )
{
    dummy_log_debug( log_id_, "delete contact: %u", id );
}

bool Phonebook::add_phone(
        uint32_t                * id,
        std::string             * error_msg,
        uint32_t                contact_id,
        ContactPhone::type_e    type,
        const std::string       & phone )
{
    dummy_log_debug( log_id_, "add phone: %u, %u, %s", contact_id, static_cast<uint32_t>( type ), phone.c_str() );
}

bool Phonebook::modify_contact(
        std::string             * error_msg,
        uint32_t                id,
        ContactPhone::type_e    type,
        const std::string       & phone )
{
    dummy_log_debug( log_id_, "add phone: %u, %u, %s", id, static_cast<uint32_t>( type ), phone.c_str() );
}

bool Phonebook::delete_phone(
        std::string         * error_msg,
        uint32_t            id )
{
    dummy_log_debug( log_id_, "add phone: %u", id );
}

std::vector<const Contact *> Phonebook::find_contacts( const std::string & regex, uint32_t page_size, uint32_t page_num )
{
    dummy_log_debug( log_id_, "find_contacts: '%s' %u %u", regex.c_str(), page_size, page_num );

}

const Contact * Phonebook::get_contact( uint32_t id ) const
{

}

const ContactPhone * Phonebook::get_phone( uint32_t id ) const
{

}

uint32_t Phonebook::find_user_id_by_phone_id( uint32_t id ) const
{

}

uint32_t Phonebook::find_user_id_by_contact_id( uint32_t id ) const
{

}

Contact * Phonebook::find_contact_by_phone_id( uint32_t id )
{

}

uint32_t    Phonebook::get_next_contact_id()
{
    return ++last_contact_id_;
}

uint32_t    Phonebook::get_next_phone_id()
{
    return ++last_phone_id_;
}



} // namespace phonebook
