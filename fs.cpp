/*
 MIT License
 
 Copyright (c) 2019 Fredrik B
 
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

#include "poddl.hpp"

#ifdef _WIN32
bool FileSystem::directory_exists(std::wstring path) {
    DWORD file_attributes = GetFileAttributesW(path.c_str());
    return file_attributes != INVALID_FILE_ATTRIBUTES && (file_attributes & FILE_ATTRIBUTE_DIRECTORY) > 0;
}

bool FileSystem::create_directory(std::wstring path) {
    return CreateDirectoryW(path.c_str(), NULL) != 0;
}

bool FileSystem::delete_directory(std::wstring path) {
    return RemoveDirectoryW(path.c_str()) != 0;
}

bool FileSystem::file_exists(std::wstring path) {
    DWORD file_attributes = GetFileAttributesW(path.c_str());
    return file_attributes != INVALID_FILE_ATTRIBUTES && !(file_attributes & FILE_ATTRIBUTE_DIRECTORY);
}

bool FileSystem::move_file(std::wstring from, std::wstring to) {
    return MoveFileW(from.c_str(), to.c_str()) != 0;
}

bool FileSystem::directory_is_empty(std::wstring path) {
    bool exists = directory_exists(path);
    if (!exists) {
        return false;
    }

    return PathIsDirectoryEmptyW(path.c_str());
}

bool FileSystem::create_directory_if_not_exists(std::wstring path) {
    if (!directory_exists(path)) {
        if (!create_directory(path)) {
            return false;
        }
    }
    return true;
}

#else

bool FileSystem::directory_exists(std::string path) {
    struct stat st;
    return stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
}

bool FileSystem::create_directory(std::string path) {
    return mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0;
}

bool FileSystem::delete_directory(std::string path) {
    return rmdir(path.c_str()) == 0;
}

bool FileSystem::file_exists(std::string path) {
    struct stat st;
    return stat(path.c_str(), &st) == 0;
}

bool FileSystem::move_file(std::string from, std::string to) {
    return rename(from.c_str(), to.c_str()) == 0;
}

bool FileSystem::directory_is_empty(std::string path) {
    bool exists = directory_exists(path);
    if (!exists) {
        return false;
    }

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
}

bool FileSystem::create_directory_if_not_exists(std::string path) {
    if (!directory_exists(path)) {
        if (!create_directory(path)) {
            return false;
        }
    }
    return true;
}

#endif