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

// $Revision: 7746 $ $Date:: 2017-08-25 #$ $Author: serge $

#include "phonebook.h"          // self

#include "utils/dummy_logger.h"     // dummy_log_debug
#include "utils/assert.h"           // ASSERT
#include "utils/mutex_helper.h"     // MUTEX_SCOPE_LOCK
#include "utils/match_filter.h"     // utils::match_filter()

#include "str_helper.h"             // StrHelper

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
        contact_id_t        * id,
        std::string         * error_msg,
        user_id_t           user_id,
        gender_e            gender,
        const std::string   & name,
        const std::string   & first_name,
        const Date          & birthday,
        const std::string   & notice )
{
    MUTEX_SCOPE_LOCK( mutex_ );

    dummy_log_debug( log_id_, "add contact: user id %u, %s, %s, %s, %s", user_id, StrHelper::to_string( gender ).c_str(), name.c_str(), first_name.c_str(), notice.c_str() );

    *id = get_next_contact_id();

    return add_contact_with_id( error_msg, *id, user_id, gender, name, first_name, birthday, notice );
}

bool Phonebook::add_contact_with_id(
        std::string         * error_msg,
        contact_id_t        id,
        user_id_t           user_id,
        gender_e            gender,
        const std::string   & name,
        const std::string   & first_name,
        const Date          & birthday,
        const std::string   & notice )
{
    dummy_log_debug( log_id_, "add_contact_with_id: contact id %u, user id %u, %s, %s, %s, %s", id, user_id, StrHelper::to_string( gender ).c_str(), name.c_str(), first_name.c_str(), notice.c_str() );

    auto contact = new Contact;

    contact->gender     = gender;
    contact->name       = name;
    contact->first_name = first_name;
    contact->birthday   = birthday;
    contact->notice     = notice;

    auto b = map_id_to_contact_.insert( std::make_pair( id, contact ) ).second;

    if( b == false )
    {
        dummy_log_error( log_id_, "add_contact_with_id: contact id %u already exists", id );
        * error_msg = "contact id " + std::to_string( id ) + " already exists";
        return false;
    }

    b = map_contact_id_to_user_id_.insert( std::make_pair( id, user_id )).second;

    ASSERT( b );

    auto it = map_user_id_to_contact_ids_.find( user_id );
    if( it == map_user_id_to_contact_ids_.end() )
    {
        dummy_log_debug( log_id_, "adding contact for new user id %u, contact id %u", user_id, id );

        std::set<contact_id_t> ids;

        ids.insert( id );

        map_user_id_to_contact_ids_.insert( std::make_pair( user_id, ids ) );
    }
    else
    {
        dummy_log_debug( log_id_, "adding contact for existing user id %u, contact id %u", user_id, id );

        auto & ids = it->second;

        ids.insert( id );
    }

    return true;
}

bool Phonebook::modify_contact(
        std::string         * error_msg,
        contact_id_t        id,
        gender_e            gender,
        const std::string   & name,
        const std::string   & first_name,
        const Date          & birthday,
        const std::string   & notice )
{
    MUTEX_SCOPE_LOCK( mutex_ );

    dummy_log_debug( log_id_, "modify contact: contact id %u, %s, %s, %s, %s", id, StrHelper::to_string( gender ).c_str(), name.c_str(), first_name.c_str(), notice.c_str() );

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
        contact_id_t        id )
{
    MUTEX_SCOPE_LOCK( mutex_ );

    dummy_log_debug( log_id_, "delete contact: contact id %u", id );

    auto it_contact = map_id_to_contact_.find( id );
    if( it_contact == map_id_to_contact_.end() )
    {
        dummy_log_error( log_id_, "delete contact: contact id %u not found", id );
        * error_msg = "contact id " + std::to_string( id ) + " not found";
        return false;
    }

    auto * contact = it_contact->second;

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
    map_id_to_contact_.erase( it_contact );

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
        contact_phone_id_t      * id,
        std::string             * error_msg,
        contact_id_t            contact_id,
        ContactPhone::type_e    type,
        const std::string       & phone )
{
    MUTEX_SCOPE_LOCK( mutex_ );

    dummy_log_debug( log_id_, "add phone: contact id %u, %s, %s", contact_id, StrHelper::to_string( type ).c_str(), phone.c_str() );

    auto * contact = find_contact( contact_id );

    if( contact == nullptr )
    {
        dummy_log_error( log_id_, "add phone: contact id %u not found", contact_id );
        * error_msg = "contact id " + std::to_string( contact_id ) + " not found";
        return false;
    }

    *id = get_next_phone_id();

    return add_phone_with_id( error_msg, *id, contact_id, type, phone );
}

bool Phonebook::add_phone_with_id(
        std::string             * error_msg,
        contact_phone_id_t      id,
        contact_id_t            contact_id,
        ContactPhone::type_e    type,
        const std::string       & phone )
{
    dummy_log_debug( log_id_, "add_phone_with_id: contact id %u, phone id %u, %s, %s", contact_id, id, StrHelper::to_string( type ).c_str(), phone.c_str() );

    auto * contact = find_contact( contact_id );

    if( contact == nullptr )
    {
        dummy_log_error( log_id_, "add_phone_with_id: contact id %u not found", contact_id );
        * error_msg = "contact id " + std::to_string( contact_id ) + " not found";
        return false;
    }

    ContactPhone contact_phone;

    contact_phone.type         = type;
    contact_phone.phone_number = phone;

    auto b = contact->map_id_to_phone.insert( std::make_pair( id, contact_phone ) ).second;

    ASSERT( b );

    b = map_phone_id_to_contact_id_.insert( std::make_pair( id, contact_id ) ).second;

    ASSERT( b );

    dummy_log_debug( log_id_, "add_phone_with_id: contact id %u, phone id %u", contact_id, id);

    return true;
}

bool Phonebook::modify_phone(
        std::string             * error_msg,
        contact_phone_id_t      id,
        ContactPhone::type_e    type,
        const std::string       & phone )
{
    MUTEX_SCOPE_LOCK( mutex_ );

    dummy_log_debug( log_id_, "modify phone: phone id %u, %s, %s", id, StrHelper::to_string( type ).c_str(), phone.c_str() );

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
        contact_phone_id_t  id )
{
    MUTEX_SCOPE_LOCK( mutex_ );

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

void Phonebook::lock() const
{
    mutex_.lock();
}

void Phonebook::unlock() const
{
    mutex_.unlock();
}

std::map<contact_id_t,const Contact *> Phonebook::find_contacts(
        uint32_t * total_size,
        user_id_t user_id, const std::string & filter, uint32_t page_size, uint32_t page_num ) const
{
    dummy_log_debug( log_id_, "find_contacts: %u '%s' %u %u", user_id, filter.c_str(), page_size, page_num );

    std::map<contact_id_t,const Contact *> res;

    * total_size = 0;

    auto offset     = page_size * page_num;
    auto offset_end = offset + page_size;

    auto it = map_user_id_to_contact_ids_.find( user_id );

    if( it == map_user_id_to_contact_ids_.end() )
    {
        dummy_log_info( log_id_, "find_contacts: user id %u not found", user_id );

        return res;
    }

    auto & contact_ids = it->second;

    unsigned i = 0;

    for( auto c : contact_ids )
    {
        auto contact = find_contact( c );

        ASSERT( contact );

        if( is_match( * contact, filter ) )
        {
            // return only those elements, which belong to the desired page
            if( i >= offset && i < offset_end )
            {
                res.insert( std::make_pair( c, contact ) );
            }

            i++;
        }
    }

    * total_size  = i;

    dummy_log_debug( log_id_, "find_contacts: user id %u, filter '%s' - found %u records, %u returned", user_id, filter.c_str(), * total_size, res.size() );

    return res;
}

const ContactPhone * Phonebook::find_phone( contact_phone_id_t id ) const
{
    auto * contact = find_contact_by_phone_id( id );

    if( contact == nullptr )
        return nullptr;

    auto & phones = contact->map_id_to_phone;

    auto it = phones.find( id );

    if( it == phones.end() )
        return nullptr;

    return & it->second;
}

contact_id_t Phonebook::find_contact_id_by_phone_id( contact_phone_id_t id ) const
{
    auto it = map_phone_id_to_contact_id_.find( id );

    if( it == map_phone_id_to_contact_id_.end() )
        return 0;

    return it->second;
}

contact_id_t Phonebook::find_contact_id_by_phone_id( contact_phone_id_t id )
{
    auto it = map_phone_id_to_contact_id_.find( id );

    if( it == map_phone_id_to_contact_id_.end() )
        return 0;

    return it->second;
}

user_id_t Phonebook::find_user_id_by_phone_id( contact_phone_id_t id ) const
{
    auto contact_id = find_contact_id_by_phone_id( id );

    if( contact_id == 0 )
        return 0;

    return find_user_id_by_contact_id( contact_id );
}

user_id_t Phonebook::find_user_id_by_contact_id( contact_id_t id ) const
{
    auto it = map_contact_id_to_user_id_.find( id );

    if( it != map_contact_id_to_user_id_.end() )
        return it->second;

    return 0;
}

Contact * Phonebook::find_contact( contact_id_t id )
{
    auto it = map_id_to_contact_.find( id );

    if( it == map_id_to_contact_.end() )
        return nullptr;

    return it->second;
}

const Contact * Phonebook::find_contact( contact_id_t id ) const
{
    auto it = map_id_to_contact_.find( id );

    if( it == map_id_to_contact_.end() )
        return nullptr;

    return it->second;
}

Contact * Phonebook::find_contact_and_contact_id_by_phone_id( contact_id_t * contact_id, contact_phone_id_t id )
{
    * contact_id = find_contact_id_by_phone_id( id );

    if( * contact_id == 0 )
        return nullptr;

    return find_contact( * contact_id );
}

Contact * Phonebook::find_contact_by_phone_id( contact_phone_id_t id )
{
    contact_id_t contact_id;

    return find_contact_and_contact_id_by_phone_id( & contact_id, id );
}

const Contact * Phonebook::find_contact_by_phone_id( contact_phone_id_t id ) const
{
    auto contact_id = find_contact_id_by_phone_id( id );

    if( contact_id == 0 )
        return nullptr;

    return find_contact( contact_id );
}

contact_id_t Phonebook::get_next_contact_id()
{
    return ++last_contact_id_;
}

contact_phone_id_t Phonebook::get_next_phone_id()
{
    return ++last_phone_id_;
}

const Status * Phonebook::get_status() const
{
    Status * res = new Status;

    res->last_contact_id     = last_contact_id_;
    res->last_phone_id       = last_phone_id_;

    for( auto c : map_id_to_contact_ )
    {
        auto user_id   = find_user_id_by_contact_id( c.first );

        res->contacts.emplace_back( c.first, user_id, c.second );
    }

    return res;
}

void Phonebook::init_status( const Status & s )
{
    MUTEX_SCOPE_LOCK( mutex_ );

    dummy_log_debug( log_id_, "init: last contact %u last phone %u, size %u", s.last_contact_id, s.last_phone_id, s.contacts.size() );

    clear();

    last_contact_id_    = s.last_contact_id;
    last_phone_id_      = s.last_phone_id;

    for( auto & c : s.contacts )
    {
        import( c );
    }
}

void Phonebook::clear()
{
    dummy_log_debug( log_id_, "clear" );

    last_contact_id_    = 0;
    last_phone_id_      = 0;

    map_id_to_contact_.clear();
    map_user_id_to_contact_ids_.clear();
    map_contact_id_to_user_id_.clear();
    map_phone_id_to_contact_id_.clear();
}

void Phonebook::import( const ContactFlat & c )
{
    std::string error_msg;

    auto b = add_contact_with_id( & error_msg, c.id, c.user_id, c.contact->gender, c.contact->name, c.contact->first_name, c.contact->birthday, c.contact->notice );

    if( b  )
    {
        dummy_log_debug( log_id_, "imported contact id %u", c.id );

        for( auto & p : c.contact->map_id_to_phone )
        {
            import( c.id, p.first, p.second );
        }
    }
    else
    {
        dummy_log_error( log_id_, "cannot import contact id %u: %s", c.id, error_msg.c_str() );
    }
}

void Phonebook::import( contact_id_t contact_id, contact_phone_id_t phone_id, const ContactPhone & c )
{
    std::string error_msg;

    auto b = add_phone_with_id( & error_msg, phone_id, contact_id, c.type, c.phone_number );

    if( b  )
    {
        dummy_log_debug( log_id_, "imported phone id %u (contact id %u)", phone_id, contact_id );
    }
    else
    {
        dummy_log_error( log_id_, "cannot import phone id %u (contact id %u): %s", phone_id, contact_id, error_msg.c_str() );
    }
}

uint32_t Phonebook::get_log_id() const
{
    return log_id_;
}

std::mutex      & Phonebook::get_mutex() const
{
    return mutex_;
}

std::string to_string_euro( const Date & d )
{
    return std::to_string( d.day ) + "."
            + std::to_string( d.month ) + "."
            + std::to_string( d.year );
}

std::string to_string_us( const Date & d )
{
    return std::to_string( d.month ) + "/"
            + std::to_string( d.day ) + "/"
            + std::to_string( d.year );
}

bool Phonebook::is_match( const Contact & c, const std::string & filter )
{
    if( utils::match_filter( c.name, filter, true )
        || utils::match_filter( c.first_name, filter, true )
        || utils::match_filter( c.notice, filter, true ) )
        return true;

    for( auto & p : c.map_id_to_phone )
    {
        if( utils::match_filter( p.second.phone_number, filter ) )
            return true;
    }

    if( utils::match_filter( to_string_euro( c.birthday ), filter ) )
        return true;

    if( utils::match_filter( to_string_us( c.birthday ), filter ) )
        return true;

    return false;
}

} // namespace phonebook
