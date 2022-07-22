#pragma once

#define INVALID_STRING_TABLE -1
#define INVALID_STRING_INDEX (std::uint16_t)~0

class i_network_string_table {
public:
	virtual					~i_network_string_table() { }
	virtual const char* GetTableName() const = 0;
	virtual int				GetTableId() const = 0;
	virtual int				GetNumStrings() const = 0;
	virtual int				GetMaxStrings() const = 0;
	virtual int				GetEntryBits() const = 0;
	virtual void			SetTick(int iTick) = 0;
	virtual bool			ChangedSinceTick(int iTick) const = 0;
	virtual int				AddString(bool bIsServer, const char* szValue, int iLength = -1, const void* pUserData = 0) = 0;
	virtual const char* GetString(int nString) = 0;
	virtual void			SetStringUserData(int nString, int iLength, const void* pUserData) = 0;
	virtual const void* GetStringUserData(int nString, int* iLength) = 0;
	virtual int				FindStringIndex(char const* szString) = 0;
};

class i_client_string_table_container {
public:
    i_network_string_table* find_table(const char* tableName) {
        using original_fn = i_network_string_table* (__thiscall*)(void*, const char*);
        return (*(original_fn**)this)[3](this, tableName);
    }
};