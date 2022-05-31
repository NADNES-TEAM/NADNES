#include "cheat_db_handler.h"
#include <QString>
#include <iostream>
#include <string>

int CheatDbHandler::return_value;


CheatDbHandler::CheatDbHandler() {
    int rc = sqlite3_open("cheats.db", &m_database);
    if (rc) {
        sqlite3_close(m_database);
        throw std::runtime_error(std::string("Can't open database: ") + sqlite3_errmsg(m_database));
    }
    exec("CREATE TABLE IF NOT EXISTS cheat_by_hash(hash INT, cheat INTEGER);");
    exec("CREATE TABLE IF NOT EXISTS address_by_cheat(cheat INTEGER, address INT);");
    exec(
        "CREATE TABLE IF NOT EXISTS name_by_cheat(cheat INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT);");
}

void CheatDbHandler::check_rc(int rc, char *error_message) {
    if (rc) {
        std::cerr << "sql3 error: " << error_message << std::endl;
        std::string message = std::string("sql3 error: ") + error_message;
        sqlite3_close(m_database);
        throw std::runtime_error(message);
    }
}

void CheatDbHandler::exec(const std::string &command) {
    char *error_message{};
    int rc = sqlite3_exec(m_database, command.c_str(), nullptr, nullptr, &error_message);
    check_rc(rc, error_message);
}

void CheatDbHandler::add_cheat(uint64_t hash, uint64_t cheat) {
    exec(QString("INSERT INTO cheat_by_hash (hash, cheat) VALUES (%1, %2);")
             .arg(hash)
             .arg(cheat)
             .toStdString());
}

CheatDbHandler::~CheatDbHandler() {
    sqlite3_close(m_database);
}

void CheatDbHandler::add_address(uint64_t cheat, uint8_t address) {
    exec(QString("INSERT INTO address_by_cheat (cheat, address) VALUES(%1, %2);")
             .arg(cheat)
             .arg(address)
             .toStdString());
}

uint64_t CheatDbHandler::add_name(const QString &name) {
    exec(QString("INSERT INTO name_by_cheat (name) VALUES('%1');")
             .arg(name)
             .toStdString());
    char *error_message;
    int rc = sqlite3_exec(m_database,
                          "select last_insert_rowid();",
                          callback_name,
                          nullptr,
                          &error_message);
    check_rc(rc, error_message);
    return CheatDbHandler::return_value;
}
int CheatDbHandler::callback_name(void *data, int argc, char **argv, char **azColName) {
    return_value = std::stoi(argv[0]);
    return return_value;
}
