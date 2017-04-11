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

// $Revision: 6584 $ $Date:: 2017-04-11 #$ $Author: serge $

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
    dummy_log_debug( log_id_, "add contact: user id %u, %u, %s, %s, %s", user_id, static_cast<uint32_t>( gender ), name.c_str(), first_name.c_str(), notice.c_str() );

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
        dummy_log_debug( log_id_, "adding contact for new user id %u, contact id %u", user_id, * id );

        std::set<uint32_t> ids;

        ids.insert( *id );

        map_user_id_to_contact_ids_.insert( std::make_pair( user_id, ids ) );
    }
    else
    {
        dummy_log_debug( log_id_, "adding contact for existing user id %u, contact id %u", user_id, * id );

        auto & ids = it->second;

        ids.insert( *id );
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
    dummy_log_debug( log_id_, "modify contact: contact id %u, %u, %s, %s, %s", id, static_cast<uint32_t>( gender ), name.c_str(), first_name.c_str(), notice.c_str() );

    auto * contact = find_contact( id );

    if( contact == nullptr )
    {
        dummy_log_error( log_id_, "modify contact: contact id %u not found", id );
        * error_msg = "contact id " + std::to_string( id ) + " not found";
        return false;
    }

    contact->gender     = gender;
    contact->name       = name;
    contact->first_name = first_name;
    contact->birthday   = birthday;
    contact->notice     = notice;

    return true;
}

bool Phonebook::delete_contact(
        std::string         * error_msg,
        uint32_t            id )
{
    dummy_log_debug( log_id_, "delete contact: contact id %u", id );

    auto * contact = find_contact( id );

    if( contact == nullptr )
    {
        dummy_log_error( log_id_, "delete contact: contact id %u not found", id );
        * error_msg = "contact id " + std::to_string( id ) + " not found";
        return false;
    }

    // delete phone ids
    auto & phone_ids = contact->map_id_to_phone;
    for( auto p : phone_ids )
    {
        dummy_log_debug( log_id_, "delete contact: contact id %u, phone id %u", id, p.first );

        auto n = map_phone_id_to_contact_id_.erase( p.first );

        ASSERT( n > 0 );
    }

    auto user_id     = find_user_id_by_contact_id( id );

    ASSERT( user_id );

    dummy_log_debug( log_id_, "delete contact: contact id %u, user id %u", id, user_id );

    delete contact;

    {
        auto it = map_user_id_to_contact_ids_.find( user_id );

        ASSERT( it != map_user_id_to_contact_ids_.end() );

        auto & ids = it->second;

        auto n = ids.erase( id );

        ASSERT( n > 0 );
    }

    return true;
}

bool Phonebook::add_phone(
        uint32_t                * id,
        std::string             * error_msg,
        uint32_t                contact_id,
        ContactPhone::type_e    type,
        const std::string       & phone )
{
    dummy_log_debug( log_id_, "add phone: contact id %u, %u, %s", contact_id, static_cast<uint32_t>( type ), phone.c_str() );

    auto * contact = find_contact( contact_id );

    if( contact == nullptr )
    {
        dummy_log_error( log_id_, "add phone: contact id %u not found", contact_id );
        * error_msg = "contact id " + std::to_string( contact_id ) + " not found";
        return false;
    }

    *id = get_next_phone_id();

    ContactPhone contact_phone;

    contact_phone.id           = *id;
    contact_phone.type         = type;
    contact_phone.phone_number = phone;

    auto b = contact->map_id_to_phone.insert( std::make_pair( *id, contact_phone ) ).second;

    ASSERT( b );

    b = map_phone_id_to_contact_id_.insert( std::make_pair( *id, contact_id ) ).second;

    ASSERT( b );

    dummy_log_debug( log_id_, "add phone: contact id %u, phone id %u", contact_id, *id);

    return true;
}

bool Phonebook::modify_phone(
        std::string             * error_msg,
        uint32_t                id,
        ContactPhone::type_e    type,
        const std::string       & phone )
{
    dummy_log_debug( log_id_, "modify phone: phone id %u, %u, %s", id, static_cast<uint32_t>( type ), phone.c_str() );

    auto * contact = find_contact_by_phone_id( id );

    if( contact == nullptr )
    {
        dummy_log_error( log_id_, "modify phone: cannot find contact for phone id %u", id );
        * error_msg = "cannot find contact for phone id " + std::to_string( id );
        return false;
    }

    auto it = contact->map_id_to_phone.find( id );

    ASSERT( it != contact->map_id_to_phone.end() );

    auto & contact_phone = it->second;

    contact_phone.type         = type;
    contact_phone.phone_number = phone;

    return true;
}

bool Phonebook::delete_phone(
        std::string         * error_msg,
        uint32_t            id )
{
    dummy_log_debug( log_id_, "delete phone: phone id %u", id );

    auto * contact = find_contact_by_phone_id( id );

    if( contact == nullptr )
    {
        dummy_log_error( log_id_, "delete phone: cannot find contact for phone id %u", id );
        * error_msg = "cannot find contact for phone id " + std::to_string( id );
        return false;
    }

    auto n = contact->map_id_to_phone.erase( id );

    ASSERT( n > 0 );

    n = map_phone_id_to_contact_id_.erase( id );

    ASSERT( n > 0 );

    return true;
}

std::vector<const Contact *> Phonebook::find_contacts( const std::string & regex, uint32_t page_size, uint32_t page_num )
{
    dummy_log_debug( log_id_, "find_contacts: '%s' %u %u", regex.c_str(), page_size, page_num );

}

const Contact * Phonebook::get_contact( uint32_t id ) const
{
    auto it = map_id_to_contact_.find( id );

    if( it == map_id_to_contact_.end() )
        return nullptr;

    return it->second;
}

const ContactPhone * Phonebook::get_phone( uint32_t id ) const
{

}

uint32_t Phonebook::find_user_id_by_phone_id( uint32_t id ) const
{

}

uint32_t Phonebook::find_user_id_by_contact_id( uint32_t id ) const
{
    auto it = map_contact_id_to_user_id_.find( id );

    if( it != map_contact_id_to_user_id_.end() )
        return it->second;

    return 0;
}

Contact * Phonebook::find_contact( uint32_t id )
{
    auto it = map_id_to_contact_.find( id );

    if( it == map_id_to_contact_.end() )
        return nullptr;

    return it->second;
}

Contact * Phonebook::find_contact_by_phone_id( uint32_t id )
{
    auto it = map_phone_id_to_contact_id_.find( id );

    if( it == map_phone_id_to_contact_id_.end() )
        return nullptr;

    return find_contact( it->second );
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
