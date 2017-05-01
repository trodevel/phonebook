/*

Phonebook manager - Save/Load.

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

// $Revision: 6791 $ $Date:: 2017-04-28 #$ $Author: serge $

#include "saveload.h"  // self

#include <fstream>      // std::ofstream
#include <memory>       // std::unique_ptr

#include "utils/mutex_helper.h"         // MUTEX_SCOPE_LOCK
#include "utils/dummy_logger.h"         // dummy_log_debug
#include "utils/rename_and_backup.h"    // utils::rename_and_backup
#include "serializer.h" // Serializer

namespace phonebook
{

bool save_intern( std::string * error_msg, const Phonebook & pb, const std::string & filename )
{
    std::ofstream os( filename );

    auto & mutex = pb.get_mutex();

    MUTEX_SCOPE_LOCK( mutex );

    std::unique_ptr<const Status> status( pb.get_status() );

    bool b = Serializer::save( os, *status.get() );

    auto log_id = pb.get_log_id();

    if( b )
    {
        dummy_log_info( log_id, "save: saved %d contact(s)", status->contacts.size() );
    }
    else
    {
        * error_msg = "cannot save contacts";
        dummy_log_error( log_id, "save: cannot save contacts" );
    }

    return b;
}

bool save( std::string * error_msg, const Phonebook & pb, const std::string & filename )
{
    auto temp_name  = filename + ".tmp";

    auto b = save_intern( error_msg, pb, temp_name );

    if( b == false )
        return false;

    utils::rename_and_backup( temp_name, filename );

    return true;
}

bool load( Phonebook * pb, std::string * error_msg, const std::string & filename )
{
    auto log_id = pb->get_log_id();

    std::ifstream is( filename );

    if( is.good() == false )
    {
        * error_msg = "file doesn't exist";
        dummy_log_warn( log_id, "load: file %s doesn't exist", filename.c_str() );
        return true;
    }

    if( is.fail() )
    {
        * error_msg = "cannot open file";
        dummy_log_warn( log_id, "load: cannot open file %s", filename.c_str() );
        return false;
    }

    Status status;

    auto res = Serializer::load( is, & status );

    if( res == nullptr )
    {
        dummy_log_error( log_id, "load: cannot load file %s", filename.c_str() );
        return false;
    }

    dummy_log_info( log_id, "load: loaded %d contact(s)", status.contacts.size() );

    pb->init_status( status );

    return true;
}

} // namespace phonebook
