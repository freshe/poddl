/*
 MIT License
 
 Copyright (c) 2019 Fredrik Blank
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <stdio.h>
#include <iostream>
#include <cstring>
#include "fs.hpp"
#ifdef _WIN32
#include <windows.h>
#include <shlwapi.h>
#include <direct.h>
#pragma comment(lib, "Shlwapi.lib")
#else
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#endif

bool FileSystem::directory_exists(std::string path) {
#ifdef _WIN32
    DWORD file_attributes = GetFileAttributesA(path.c_str());
    return file_attributes != INVALID_FILE_ATTRIBUTES && (file_attributes & FILE_ATTRIBUTE_DIRECTORY) > 0;
#else
    struct stat st;
    return stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
#endif
}

bool FileSystem::create_directory(std::string path) {
#ifdef _WIN32
    return CreateDirectoryA(path.c_str(), NULL) != 0;
#else
    return mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0;
#endif
}

bool FileSystem::delete_directory(std::string path) {
#ifdef _WIN32
    return RemoveDirectoryA(path.c_str()) != 0;
#else
    return rmdir(path.c_str()) == 0;
#endif
}

bool FileSystem::file_exists(std::string path) {
#ifdef _WIN32
    DWORD file_attributes = GetFileAttributesA(path.c_str());
    return file_attributes != INVALID_FILE_ATTRIBUTES && !(file_attributes & FILE_ATTRIBUTE_DIRECTORY);
#else
    struct stat st;
    return stat(path.c_str(), &st) == 0;
#endif
}

bool FileSystem::move_file(std::string from, std::string to) {
#ifdef _WIN32
    return MoveFileEx(from.c_str(), to.c_str(), MOVEFILE_REPLACE_EXISTING) != 0;
#else
    return rename(from.c_str(), to.c_str()) == 0;
#endif
}

bool FileSystem::directory_is_empty(std::string path) {
    bool exists = directory_exists(path);
    if (!exists) {
        return false;
    }
#ifdef _WIN32
    return PathIsDirectoryEmptyA(path.c_str());
#else
    int c = 0;
    dirent* d;
    DIR* dir = opendir(path.c_str());

    if (dir == NULL) {
        return false;
    }

    while ((d = readdir(dir)) != NULL) {
        if (c > 0) {
            break;
        }

        if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0) {
            continue;
        }

        c++;
    }

    closedir(dir);
    return c == 0;
#endif
}

bool FileSystem::create_directory_if_not_exists(std::string path) {
    if (!directory_exists(path)) {
        if (!create_directory(path)) {
            return false;
        }
    }
    return true;
}