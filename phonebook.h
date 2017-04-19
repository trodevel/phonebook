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

// $Revision: 6668 $ $Date:: 2017-04-18 #$ $Author: serge $

#ifndef LIB_PHONEBOOK_PHONEBOOK_H
#define LIB_PHONEBOOK_PHONEBOOK_H

#include <set>              // std::set
#include <vector>           // std::vector

#include "contact.h"        // Contact
#include "status.h"         // Status


namespace phonebook
{

class Phonebook
{
public:
    Phonebook();

    bool init( uint32_t log_id );

    const Status    * get_status() const;
    void            init_status( const Status & status );

    bool add_contact(
            uint32_t            * id,
            std::string         * error_msg,
            uint32_t            user_id,
            gender_e            gender,
            const std::string   & name,
            const std::string   & first_name,
            const Date          & birthday,
            const std::string   & notice );

    bool modify_contact(
            std::string         * error_msg,
            uint32_t            id,
            gender_e            gender,
            const std::string   & name,
            const std::string   & first_name,
            const Date          & birthday,
            const std::string   & notice );

    bool delete_contact(
            std::string         * error_msg,
            uint32_t            id );

    bool add_phone(
            uint32_t                * id,
            std::string             * error_msg,
            uint32_t                contact_id,
            ContactPhone::type_e    type,
            const std::string       & phone );

    bool modify_phone(
            std::string             * error_msg,
            uint32_t                id,
            ContactPhone::type_e    type,
            const std::string       & phone );

    bool delete_phone(
            std::string         * error_msg,
            uint32_t            id );

    std::vector<const Contact *> find_contacts( const std::string & regex, uint32_t page_size, uint32_t page_num );

    Contact * find_contact( uint32_t id );
    const Contact * find_contact( uint32_t id ) const;
    const ContactPhone * find_phone( uint32_t id ) const;

    uint32_t find_user_id_by_phone_id( uint32_t id ) const;
    uint32_t find_user_id_by_contact_id( uint32_t id ) const;

    uint32_t        get_log_id() const;

private:

    typedef std::map<uint32_t,Contact*>  MapIdToContact;

    typedef std::map<uint32_t,uint32_t>  MapIdToId;

    typedef std::map<uint32_t,std::set<uint32_t>>  MapIdToIdSet;

private:

    bool add_contact_with_id(
            std::string         * error_msg,
            uint32_t            id,
            uint32_t            user_id,
            gender_e            gender,
            const std::string   & name,
            const std::string   & first_name,
            const Date          & birthday,
            const std::string   & notice );

    bool add_phone_with_id(
            std::string             * error_msg,
            uint32_t                id,
            uint32_t                contact_id,
            ContactPhone::type_e    type,
            const std::string       & phone );

    Contact * find_contact_by_phone_id( uint32_t id );
    const Contact * find_contact_by_phone_id( uint32_t id ) const;

    uint32_t    get_next_contact_id();
    uint32_t    get_next_phone_id();

    void        clear();
    void        import( const ContactFlat & c );
    void        import( uint32_t contact_id, uint32_t phone_id, const ContactPhone & c );

private:

    uint32_t        log_id_;

    uint32_t        last_contact_id_;
    uint32_t        last_phone_id_;

    MapIdToContact  map_id_to_contact_;             // map: contact id --> Contact*

    MapIdToIdSet    map_user_id_to_contact_ids_;    // map: user id --> list of contact ids
    MapIdToId       map_contact_id_to_user_id_;     // map: contact id --> user id
    MapIdToId       map_phone_id_to_contact_id_;    // map: phone id --> contact id
};

} // namespace phonebook

#endif // LIB_PHONEBOOK_PHONEBOOK_H
