#pragma once

#include <QMap>
#include <vector>
#include "nes.h"

class CheatDbHandler {
public:
    CheatDbHandler();
    CheatDbHandler(const CheatDbHandler &) = delete;
    CheatDbHandler(CheatDbHandler &&) = delete;
    CheatDbHandler &operator=(const CheatDbHandler &) = delete;
    CheatDbHandler &operator=(CheatDbHandler &&) = delete;
    ~CheatDbHandler();

    void save_data();
    void add_cheat(quint64 hash, quint64 cheat);
    void add_address(quint64 cheat, quint64 address);
    void clear_name(const QString &name);
    quint64 add_name(const QString &name);
    std::vector<QString> get_names(quint64 hash);
    std::vector<quint64> get_addresses(quint64 cheat);
    std::vector<quint64> get_cheats(quint64 hash);
private:
    QMultiMap<quint64, quint64> cheat_by_hash, address_by_cheat;
    QMap<quint64, QString> name_by_cheat;
};