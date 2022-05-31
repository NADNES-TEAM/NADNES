#pragma once
#include <sqlite3.h>
#include <QString>
#include <string>

class CheatDbHandler {
public:
    CheatDbHandler();
    CheatDbHandler(const CheatDbHandler &) = delete;
    CheatDbHandler(CheatDbHandler &&) = delete;
    CheatDbHandler &operator=(const CheatDbHandler &) = delete;
    CheatDbHandler &operator=(CheatDbHandler &&) = delete;
    ~CheatDbHandler();

    void add_cheat(uint64_t hash, uint64_t cheat);
    void add_address(uint64_t cheat, uint8_t address);
    uint64_t add_name(const QString &name);

    static int callback_name(void *data, int argc, char **argv, char **azColName);

private:
    void check_rc(int rc, char *error_message);
    void exec(const std::string &command);
    sqlite3 *m_database{};
    static int return_value;
};
