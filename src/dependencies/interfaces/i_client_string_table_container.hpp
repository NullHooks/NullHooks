#pragma once

class i_network_string_table;

class i_client_string_table_container {
public:
    i_network_string_table* find_table(const char* tableName) {
        using original_fn = i_network_string_table* (__thiscall*)(void*, const char*);
        return (*(original_fn**)this)[3](this, tableName);
    }
};