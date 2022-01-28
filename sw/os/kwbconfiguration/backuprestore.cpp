#include "backuprestore.h"

#include <fstream>
#include <iomanip>
#include <iostream>

#include <nlohmann/json.hpp>

#include "exceptions.h"

using namespace nlohmann;


//----------------------------------------------------------------------------
BackupRestore::BackupRestore()
{
}

//----------------------------------------------------------------------------
bool BackupRestore::backup(uint16_t moduleId, std::string regValueFile, std::string regLayoutFile)
{
    (void)moduleId;
    (void)regValueFile;
    (void)regLayoutFile;

    json layout = loadLayoutFile(regLayoutFile);

    json modules_array = json::array();
    json module = json::object();
    module["nodeid"] = moduleId;


    modules_array.emplace_back(module);
    std::cout << modules_array << std::endl;

    std::ofstream o(regValueFile);
    o << std::setw(4) << modules_array << std::endl;
    return false;
}

//----------------------------------------------------------------------------
bool BackupRestore::restore(uint16_t moduleId, std::string regValueFile, std::string regLayoutFile)
{
    (void)moduleId;
    (void)regValueFile;
    (void)regLayoutFile;
    return false;
}

//----------------------------------------------------------------------------
json BackupRestore::loadLayoutFile(std::string filename)
{
    json layout;
    try {
        std::ifstream ifs(filename, std::ios::in);
        ifs >> layout;
    }
    catch (std::exception & e) {
        throw ResourceMissing(LOC, "Unable to read layout file! %s", e.what());
    }

    json reg_array = layout["register"];
    if (!reg_array.is_array()) {
        throw InvalidParameter(LOC, "layout file %s format wrong. expected entity 'register' beeing an array!", filename.c_str());
    }

    for (json::iterator it = reg_array.begin(); it != reg_array.end(); ++it) {
        std::cout << *it << '\n';
    }
    return reg_array;
}
