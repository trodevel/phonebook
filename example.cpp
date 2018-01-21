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

// $Revision: 8576 $ $Date:: 2018-01-19 #$ $Author: serge $

#include <iostream>                         // std::cout

#include "utils/dummy_logger.h"             // dummy_log_set_log_level

#include "phonebook.h"
#include "str_helper.h"                     // StrHelper
#include "saveload.h"                       // save()

void test_01( uint32_t log_id )
{
    std::cout << "test 01" << "\n";

    phonebook::Phonebook pb;

    pb.init( log_id );

    phonebook::contact_id_t id;
    std::string             error_msg;

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

    phonebook::contact_id_t id;
    std::string             error_msg;

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

    phonebook::contact_id_t id = 777;
    std::string             error_msg;

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

    phonebook::contact_id_t id;
    std::string             error_msg;

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

    phonebook::contact_id_t id = 777;
    std::string             error_msg;

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

    phonebook::contact_id_t id;
    std::string             error_msg;

    phonebook::Date date    = { 1970, 01, 01 };

    auto b = pb.add_contact( & id, & error_msg, 1, phonebook::gender_e::MALE, "Doe", "John", date, "test contact" );

    if( b )
    {
        std::cout << "OK: contact added "<< id << "\n";

        phonebook::contact_phone_id_t phone_id;

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

    phonebook::contact_id_t id;
    std::string             error_msg;

    phonebook::Date date    = { 1970, 01, 01 };

    auto b = pb.add_contact( & id, & error_msg, 1, phonebook::gender_e::MALE, "Doe", "John", date, "test contact" );

    if( b )
    {
        std::cout << "OK: contact added "<< id << "\n";

        phonebook::contact_phone_id_t phone_id;

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

    phonebook::contact_id_t id;
    std::string             error_msg;

    phonebook::Date date    = { 1970, 01, 01 };

    auto b = pb.add_contact( & id, & error_msg, 1, phonebook::gender_e::MALE, "Doe", "John", date, "test contact" );

    if( b )
    {
        std::cout << "OK: contact added "<< id << "\n";

        phonebook::contact_phone_id_t   phone_id;

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

    phonebook::contact_phone_id_t   id = 999;
    std::string                     error_msg;

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

    phonebook::contact_id_t id;
    std::string             error_msg;

    phonebook::Date date    = { 1970, 01, 01 };

    auto b = pb.add_contact( & id, & error_msg, 1, phonebook::gender_e::MALE, "Doe", "John", date, "test contact" );

    if( b )
    {
        std::cout << "OK: contact added "<< id << "\n";

        phonebook::contact_phone_id_t phone_id;

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

    phonebook::contact_id_t id;
    std::string             error_msg;

    phonebook::Date date    = { 1970, 01, 01 };

    auto b = pb.add_contact( & id, & error_msg, 1, phonebook::gender_e::MALE, "Doe", "John", date, "test contact" );

    if( b )
    {
        std::cout << "OK: contact added "<< id << "\n";

        phonebook::contact_phone_id_t phone_id;

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

phonebook::Date create_date( uint32_t year, uint32_t month, uint32_t day )
{
    phonebook::Date res;

    res.year   = year;
    res.month  = month;
    res.day    = day;

    return res;
}

void add_contact(
        phonebook::Phonebook    * pb,
        phonebook::user_id_t    user_id,
        phonebook::gender_e     gender,
        const std::string       & name,
        const std::string       & first_name,
        uint32_t                year,
        uint32_t                month,
        uint32_t                day,
        const std::string       & notice,
        const std::string       & ll_phone  = std::string(),
        const std::string       & mm_phone  = std::string() )
{
    phonebook::contact_id_t id;
    std::string             error_msg;

    pb->add_contact( & id, & error_msg, user_id, gender, name, first_name, create_date( year, month, day ), notice );

    phonebook::contact_phone_id_t phone_id;

    if( ll_phone.empty() == false )
        pb->add_phone( & phone_id, & error_msg, id, phonebook::ContactPhone::type_e::LANDLINE, ll_phone );

    if( mm_phone.empty() == false )
        pb->add_phone( & phone_id, & error_msg, id, phonebook::ContactPhone::type_e::MOBILE, mm_phone );
}

void init_phonebook( phonebook::Phonebook * pb, uint32_t log_id )
{
    std::cout << "init_phonebook" << "\n";

    pb->init( log_id );

    add_contact( pb, 1, phonebook::gender_e::MALE, "Doe", "John",       1970, 1, 1, "test contact", "+491234567890", "+498976543210" );
    add_contact( pb, 1, phonebook::gender_e::MALE, "王", "张",           1970, 1, 1, "Chinese name" );
    add_contact( pb, 1, phonebook::gender_e::MALE, "Иванов", "Иван",    1970, 1, 1, "Russian name" );
    add_contact( pb, 1, phonebook::gender_e::MALE, "Müller", "Markus",  1970, 1, 1, "German name" );

    add_contact( pb, 2, phonebook::gender_e::FEMALE, "Müller",   "Laura",    1970, 1, 11, "German female name", "+4930987654321" );
    add_contact( pb, 2, phonebook::gender_e::FEMALE, "Schmidt",  "Emilia",   1971, 2, 12, "German female name", "+4940987654321" );
    add_contact( pb, 2, phonebook::gender_e::FEMALE, "Schneider","Lina",     1972, 3, 13, "German female name", "+4989987654321" );
    add_contact( pb, 2, phonebook::gender_e::FEMALE, "Fischer",  "Lena",     1973, 4, 14, "German female name", "+49221987654321" );
    add_contact( pb, 2, phonebook::gender_e::FEMALE, "Weber",    "Julia",    1974, 5, 15, "German female name", "+49211987654321" );
    add_contact( pb, 2, phonebook::gender_e::FEMALE, "Meyer",    "Lea",      1975, 6, 16, "German female name", "+4969987654321" );
    add_contact( pb, 2, phonebook::gender_e::FEMALE, "Wagner",   "Anna",     1976, 7, 17, "German female name", "+49421987654321" );
    add_contact( pb, 2, phonebook::gender_e::FEMALE, "Becker",   "Lara",     1977, 8, 18, "German female name", "+49711987654321" );
    add_contact( pb, 2, phonebook::gender_e::FEMALE, "Schulz",   "Elena",    1978, 9, 19, "German female name", "+49351987654321" );
    add_contact( pb, 2, phonebook::gender_e::FEMALE, "Schwarz",  "Sarah",    1979, 10, 8, "German female name", "+49201987654321" );

    add_contact( pb, 2, phonebook::gender_e::MALE,   "Hoffmann", "Liam",     1970, 1, 11, "German male name", "", "+491511123456789" );
    add_contact( pb, 2, phonebook::gender_e::MALE,   "Schäfer",  "Jonas",    1971, 2, 12, "German male name", "", "+491520123456789" );
    add_contact( pb, 2, phonebook::gender_e::MALE,   "Koch",     "Julian",   1972, 3, 13, "German male name", "", "+491573123456789" );
    add_contact( pb, 2, phonebook::gender_e::MALE,   "Bauer",    "Tim",      1973, 4, 14, "German male name", "", "+491590123456789" );
    add_contact( pb, 2, phonebook::gender_e::MALE,   "Richter",  "Linus",    1974, 5, 15, "German male name", "", "+491512123456789" );
    add_contact( pb, 2, phonebook::gender_e::MALE,   "Klein",    "Milan",    1975, 6, 16, "German male name", "", "+491522123456789" );
    add_contact( pb, 2, phonebook::gender_e::MALE,   "Wolf",     "Elias",    1976, 7, 17, "German male name", "", "+491575123456789" );
    add_contact( pb, 2, phonebook::gender_e::MALE,   "Schröder", "Michael",  1977, 8, 18, "German male name", "", "+49176123456789" );
    add_contact( pb, 2, phonebook::gender_e::MALE,   "Neumann",  "Luca",     1978, 9, 19, "German male name", "", "+49175123456789" );
    add_contact( pb, 2, phonebook::gender_e::MALE,   "Zimmermann","Levi",    1979, 10, 8, "German male name", "", "+49163123456789" );
}

void test_12( uint32_t log_id )
{
    std::cout << "test 12" << "\n";

    phonebook::Phonebook pb;

    init_phonebook( & pb, log_id );

    std::string error_msg;

    phonebook::save( & error_msg, pb, "phonebook.dat" );
}

void test_13( uint32_t log_id )
{
    std::cout << "test 13" << "\n";

    phonebook::Phonebook pb;

    pb.init( log_id );

    std::string error_msg;

    phonebook::load( & pb, & error_msg, "example.dat" );
}

void print_contacts( const std::map<phonebook::contact_id_t,const phonebook::Contact *> & contacts )
{
    for( auto & e : contacts )
    {
        std::cout << e.first << " " << phonebook::StrHelper::to_string( * e.second ) << "\n";
    }
}

void test_14( const phonebook::Phonebook & pb )
{
    std::cout << "test 14" << "\n";

    uint32_t total_size;

    auto res = pb.find_contacts( & total_size, 1, "", 100, 0 );

    print_contacts( res );
}

void test_15( const phonebook::Phonebook & pb  )
{
    std::cout << "test 15" << "\n";

    uint32_t total_size;

    auto res = pb.find_contacts( & total_size, 1, "ва", 100, 0 );

    print_contacts( res );
}

void test_16( const phonebook::Phonebook & pb  )
{
    std::cout << "test 16" << "\n";

    uint32_t total_size;

    auto res = pb.find_contacts( & total_size, 2, "sch", 100, 0 );

    print_contacts( res );
}

void test_17( const phonebook::Phonebook & pb  )
{
    std::cout << "test 17" << "\n";

    uint32_t total_size;

    auto res = pb.find_contacts( & total_size, 2, "mann", 100, 0 );

    print_contacts( res );
}

void test_18( const phonebook::Phonebook & pb  )
{
    std::cout << "test 18" << "\n";

    uint32_t total_size;

    auto res = pb.find_contacts( & total_size, 2, "176", 100, 0 );

    print_contacts( res );
}

void test_19( const phonebook::Phonebook & pb  )
{
    std::cout << "test 19" << "\n";

    uint32_t total_size;

    auto res = pb.find_contacts( & total_size, 2, "8.10", 100, 0 );

    print_contacts( res );
}

void test_20( const phonebook::Phonebook & pb  )
{
    std::cout << "test 20" << "\n";

    uint32_t total_size;

    auto res = pb.find_contacts( & total_size, 2, "10/8", 100, 0 );

    print_contacts( res );
}

void test_21( const phonebook::Phonebook & pb  )
{
    std::cout << "test 21" << "\n";

    uint32_t total_size;

    auto res = pb.find_contacts( & total_size, 2, "1976", 100, 0 );

    print_contacts( res );
}

void test_22( const phonebook::Phonebook & pb  )
{
    std::cout << "test 22" << "\n";

    uint32_t total_size;

    auto res = pb.find_contacts( & total_size, 3, "", 100, 0 );

    print_contacts( res );
}

void test_23( const phonebook::Phonebook & pb  )
{
    std::cout << "test 23" << "\n";

    uint32_t total_size;

    auto res = pb.find_contacts( & total_size, 2, "", 10, 0 );

    print_contacts( res );
}

void test_24( const phonebook::Phonebook & pb  )
{
    std::cout << "test 24" << "\n";

    uint32_t total_size;

    auto res = pb.find_contacts( & total_size, 2, "", 10, 1 );

    print_contacts( res );
}

void test_25( const phonebook::Phonebook & pb  )
{
    std::cout << "test 25" << "\n";

    uint32_t total_size;

    auto res = pb.find_contacts( & total_size, 2, "", 10, 2 );

    print_contacts( res );
}

void test_search( uint32_t log_id )
{
    std::cout << "test search" << "\n";

    phonebook::Phonebook pb;

    pb.init( log_id );

    std::string error_msg;

    phonebook::load( & pb, & error_msg, "example.dat" );

    test_14( pb );
    test_15( pb );
    test_16( pb );
    test_17( pb );
    test_18( pb );
    test_19( pb );
    test_20( pb );
    test_21( pb );
    test_22( pb );
    test_23( pb );
    test_24( pb );
    test_25( pb );
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
    test_12( log_id );
    test_13( log_id );
    test_search( log_id );

    std::cout << "exit" << std::endl;

    return 0;
}
