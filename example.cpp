/*

Example.

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

// $Revision: 6603 $ $Date:: 2017-04-12 #$ $Author: serge $

#include <iostream>                         // std::cout

#include "utils/dummy_logger.h"             // dummy_log_set_log_level
#include "utils/logfile_time_writer.h"      // LogfileTimeWriter

#include "phonebook.h"
#include "str_helper.h"                     // StrHelper

void test_01( uint32_t log_id )
{
    std::cout << "test 01" << "\n";

    phonebook::Phonebook pb;

    pb.init( log_id );

    uint32_t    id;
    std::string error_msg;

    phonebook::Date date    = { 1970, 01, 01 };

    auto b = pb.add_contact( & id, & error_msg, 1, phonebook::gender_e::MALE, "Doe", "John", date, "test contact" );

    if( b )
    {
        std::cout << "OK: contact added "<< id << "\n";
    }
    else
    {
        std::cout << "ERROR: cannot add contact " << error_msg << "\n";
    }
}

void test_02( uint32_t log_id )
{
    std::cout << "test 02" << "\n";

    phonebook::Phonebook pb;

    pb.init( log_id );

    uint32_t    id;
    std::string error_msg;

    phonebook::Date date    = { 1970, 01, 01 };

    auto b = pb.add_contact( & id, & error_msg, 1, phonebook::gender_e::MALE, "Doe", "John", date, "test contact" );

    if( b )
    {
        std::cout << "OK: contact added "<< id << "\n";

        b = pb.modify_contact( & error_msg, id, phonebook::gender_e::FEMALE, "Doe", "Alice", date, "test contact 2" );

        if( b )
        {
            std::cout << "OK: modified contact "<< id << "\n";
        }
        else
        {
            std::cout << "ERROR: cannot modify contact "<< id << "\n";
        }
    }
    else
    {
        std::cout << "ERROR: cannot add contact " << error_msg << "\n";
    }
}

void test_03( uint32_t log_id )
{
    std::cout << "test 03" << "\n";

    phonebook::Phonebook pb;

    pb.init( log_id );

    uint32_t    id = 777;
    std::string error_msg;

    phonebook::Date date    = { 1970, 01, 01 };

    auto b = pb.modify_contact( & error_msg, id, phonebook::gender_e::FEMALE, "Doe", "Alice", date, "test contact 2" );

    if( b )
    {
        std::cout << "ERROR: modified non-existing contact " << id << "\n";
    }
    else
    {
        std::cout << "OK: non-existing contact cannot be modified " << id << "\n";
    }
}

void test_04( uint32_t log_id )
{
    std::cout << "test 04" << "\n";

    phonebook::Phonebook pb;

    pb.init( log_id );

    uint32_t    id;
    std::string error_msg;

    phonebook::Date date    = { 1970, 01, 01 };

    auto b = pb.add_contact( & id, & error_msg, 555, phonebook::gender_e::MALE, "Doe", "John", date, "test contact" );

    if( b )
    {
        std::cout << "OK: contact added "<< id << "\n";

        auto b = pb.delete_contact( & error_msg, id );

        if( b )
        {
            std::cout << "OK: deleted contact " << id << "\n";
        }
        else
        {
            std::cout << "ERROR: cannot deleted contact " << id << "\n";
        }
    }
    else
    {
        std::cout << "ERROR: cannot add contact " << error_msg << "\n";
    }
}

void test_05( uint32_t log_id )
{
    std::cout << "test 05" << "\n";

    phonebook::Phonebook pb;

    pb.init( log_id );

    uint32_t    id = 777;
    std::string error_msg;

    auto b = pb.delete_contact( & error_msg, id );

    if( b )
    {
        std::cout << "ERROR: deleted non-existing contact " << id << "\n";
    }
    else
    {
        std::cout << "OK: non-existing contact cannot be deleted " << id << "\n";
    }
}

void test_06( uint32_t log_id )
{
    std::cout << "test 06" << "\n";

    phonebook::Phonebook pb;

    pb.init( log_id );

    uint32_t    id;
    std::string error_msg;

    phonebook::Date date    = { 1970, 01, 01 };

    auto b = pb.add_contact( & id, & error_msg, 1, phonebook::gender_e::MALE, "Doe", "John", date, "test contact" );

    if( b )
    {
        std::cout << "OK: contact added "<< id << "\n";

        uint32_t phone_id;

        b = pb.add_phone( & phone_id, & error_msg, id, phonebook::ContactPhone::type_e::LANDLINE, "+491234567890" );

        if( b )
        {
            std::cout << "OK: phone added "<< phone_id << "\n";
        }
        else
        {
            std::cout << "ERROR: cannot add phone " << error_msg << "\n";
        }
    }
    else
    {
        std::cout << "ERROR: cannot add contact " << error_msg << "\n";
    }
}

void test_07( uint32_t log_id )
{
    std::cout << "test 07" << "\n";

    phonebook::Phonebook pb;

    pb.init( log_id );

    uint32_t    id;
    std::string error_msg;

    phonebook::Date date    = { 1970, 01, 01 };

    auto b = pb.add_contact( & id, & error_msg, 1, phonebook::gender_e::MALE, "Doe", "John", date, "test contact" );

    if( b )
    {
        std::cout << "OK: contact added "<< id << "\n";

        uint32_t phone_id;

        b = pb.add_phone( & phone_id, & error_msg, id, phonebook::ContactPhone::type_e::LANDLINE, "+491234567890" );

        if( b )
        {
            std::cout << "OK: phone added " << phone_id << "\n";

            b = pb.modify_phone( & error_msg, phone_id, phonebook::ContactPhone::type_e::MOBILE, "+490987654321" );

            if( b )
            {
                std::cout << "OK: modified phone " << phone_id << "\n";
            }
            else
            {
                std::cout << "ERROR: cannot modify phone " << error_msg << "\n";
            }
        }
        else
        {
            std::cout << "ERROR: cannot add phone " << error_msg << "\n";
        }
    }
    else
    {
        std::cout << "ERROR: cannot add contact " << error_msg << "\n";
    }
}

void test_08( uint32_t log_id )
{
    std::cout << "test 08" << "\n";

    phonebook::Phonebook pb;

    pb.init( log_id );

    uint32_t    id;
    std::string error_msg;

    phonebook::Date date    = { 1970, 01, 01 };

    auto b = pb.add_contact( & id, & error_msg, 1, phonebook::gender_e::MALE, "Doe", "John", date, "test contact" );

    if( b )
    {
        std::cout << "OK: contact added "<< id << "\n";

        uint32_t phone_id;

        b = pb.add_phone( & phone_id, & error_msg, id, phonebook::ContactPhone::type_e::LANDLINE, "+491234567890" );

        if( b )
        {
            std::cout << "OK: phone added " << phone_id << "\n";

            b = pb.delete_phone( & error_msg, phone_id );

            if( b )
            {
                std::cout << "OK: deleted phone " << phone_id << "\n";
            }
            else
            {
                std::cout << "ERROR: cannot delete phone " << error_msg << "\n";
            }
        }
        else
        {
            std::cout << "ERROR: cannot add phone " << error_msg << "\n";
        }
    }
    else
    {
        std::cout << "ERROR: cannot add contact " << error_msg << "\n";
    }
}

void test_09( uint32_t log_id )
{
    std::cout << "test 09" << "\n";

    phonebook::Phonebook pb;

    pb.init( log_id );

    uint32_t    id = 999;
    std::string error_msg;

    auto b = pb.delete_phone( & error_msg, id );

    if( b )
    {
        std::cout << "ERROR: deleted non-existing phone " << id << "\n";
    }
    else
    {
        std::cout << "OK: non-existing phone cannot be deleted " << id << "\n";
    }
}

void test_10( uint32_t log_id )
{
    std::cout << "test 10" << "\n";

    phonebook::Phonebook pb;

    pb.init( log_id );

    uint32_t    id;
    std::string error_msg;

    phonebook::Date date    = { 1970, 01, 01 };

    auto b = pb.add_contact( & id, & error_msg, 1, phonebook::gender_e::MALE, "Doe", "John", date, "test contact" );

    if( b )
    {
        std::cout << "OK: contact added "<< id << "\n";

        uint32_t phone_id;

        b = pb.add_phone( & phone_id, & error_msg, id, phonebook::ContactPhone::type_e::LANDLINE, "+491234567890" );

        if( b )
        {
            std::cout << "OK: phone added "<< phone_id << "\n";
        }
        else
        {
            std::cout << "ERROR: cannot add phone " << error_msg << "\n";
        }

        b = pb.add_phone( & phone_id, & error_msg, id, phonebook::ContactPhone::type_e::MOBILE, "+498976543210" );

        if( b )
        {
            std::cout << "OK: phone added "<< phone_id << "\n";
        }
        else
        {
            std::cout << "ERROR: cannot add phone " << error_msg << "\n";
        }

        auto contact = pb.find_contact( id );

        if( contact != nullptr )
        {
            std::cout << "OK: found contact: " << phonebook::StrHelper::to_string( *contact ) << "\n";
        }
        else
        {
            std::cout << "ERROR: cannot find contact id " << id << "\n";
        }
    }
    else
    {
        std::cout << "ERROR: cannot add contact " << error_msg << "\n";
    }
}


void test_11( uint32_t log_id )
{
    std::cout << "test 11" << "\n";

    phonebook::Phonebook pb;

    pb.init( log_id );

    uint32_t    id;
    std::string error_msg;

    phonebook::Date date    = { 1970, 01, 01 };

    auto b = pb.add_contact( & id, & error_msg, 1, phonebook::gender_e::MALE, "Doe", "John", date, "test contact" );

    if( b )
    {
        std::cout << "OK: contact added "<< id << "\n";

        uint32_t phone_id;

        b = pb.add_phone( & phone_id, & error_msg, id, phonebook::ContactPhone::type_e::LANDLINE, "+491234567890" );

        if( b )
        {
            std::cout << "OK: phone added "<< phone_id << "\n";

            auto phone = pb.find_phone( phone_id );

            if( phone != nullptr )
            {
                std::cout << "OK: found phone: " << phonebook::StrHelper::to_string( *phone ) << "\n";
            }
            else
            {
                std::cout << "ERROR: cannot find phone id " << phone_id << "\n";
            }
        }
        else
        {
            std::cout << "ERROR: cannot add phone " << error_msg << "\n";
        }
    }
    else
    {
        std::cout << "ERROR: cannot add contact " << error_msg << "\n";
    }
}

int main()
{
    dummy_logger::set_log_level( log_levels_log4j::DEBUG );

    auto log_id             = dummy_logger::register_module( "Phonebook" );

    test_01( log_id );
    test_02( log_id );
    test_03( log_id );
    test_04( log_id );
    test_05( log_id );
    test_06( log_id );
    test_07( log_id );
    test_08( log_id );
    test_09( log_id );
    test_10( log_id );
    test_11( log_id );

    std::cout << "exit" << std::endl;

    return 0;
}
