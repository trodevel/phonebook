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

// $Revision: 6561 $ $Date:: 2017-04-10 #$ $Author: serge $

#include <iostream>                         // std::cout

#include "utils/dummy_logger.h"             // dummy_log_set_log_level
#include "utils/logfile_time_writer.h"      // LogfileTimeWriter

#include "phonebook.h"

void test_01( uint32_t log_id )
{
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

int main()
{
    dummy_logger::set_log_level( log_levels_log4j::DEBUG );

    auto log_id             = dummy_logger::register_module( "Phonebook" );

    test_01( log_id );

    std::cout << "exit" << std::endl;

    return 0;
}
