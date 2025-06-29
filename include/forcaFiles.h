#ifndef FORCA_FILES_H
#define FORCA_FILES_H

#include <string>
#include <cstring>
#include <map>

namespace forcaFiles {

    namespace utils {

        std::string normalizePath( std::string path, bool unify = true );

        std::string root_realpath( std::string path = "", bool unify = true );

        std::string current_realpath( std::string path = "", bool unify = true );

        bool fileExist( std::string filepath );

        bool canRead( std::string filepath );

        bool canWrite( std::string filepath );

    }

    namespace create {

        bool createFile( std::string filepath, std::string content = "" );

    }

    namespace read {

        std::string getContent( std::string filepath );

    }

    namespace utils {

        bool isEmpty( std::string filename );

    }

}

#endif