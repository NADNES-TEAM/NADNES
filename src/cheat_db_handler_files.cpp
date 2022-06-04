#include "cheat_db_handler_files.h"
#include <QFile>

CheatDbHandler::CheatDbHandler() {
    QFile file("db.dat");
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    in >> cheat_by_hash >> address_by_cheat >> name_by_cheat;
}

CheatDbHandler::~CheatDbHandler() {
    save_data();
}

void CheatDbHandler::add_cheat(quint64 hash, quint64 cheat) {
    cheat_by_hash.insert(hash, cheat);
}

void CheatDbHandler::add_address(quint64 cheat, quint64 address) {
    address_by_cheat.insert(cheat, address);
}

quint64 CheatDbHandler::add_name(const QString &name) {
    auto res = (name_by_cheat.empty() ? 0 : name_by_cheat.lastKey() + 1);
    name_by_cheat[res] = name;
    return res;
}
std::vector<QString> CheatDbHandler::get_names(quint64 hash) {
    std::vector<QString> ans;
    for (auto it = cheat_by_hash.lowerBound(hash); it != cheat_by_hash.end() && it.key() == hash; it++) {
        ans.push_back(name_by_cheat[it.value()]);
    }
    return ans;
}

std::vector<quint64> CheatDbHandler::get_addresses(quint64 cheat) {
    std::vector<quint64> ans;
    for (auto it = address_by_cheat.lowerBound(cheat); it != address_by_cheat.end() && it.key() == cheat; ++it) {
        ans.push_back(it.value());
    }
    return ans;
}

std::vector<quint64> CheatDbHandler::get_cheats(quint64 hash) {
    std::vector<quint64> ans;
    for (auto it = cheat_by_hash.lowerBound(hash); it != cheat_by_hash.end() && it.key() == hash; it++) {
        ans.push_back(it.value());
    }
    return ans;
}

void CheatDbHandler::save_data() {
    QFile file("db.dat");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << cheat_by_hash << address_by_cheat << name_by_cheat;
}

void CheatDbHandler::clear_name(const QString &name) {
    quint64 cheat{std::numeric_limits<uint64_t>::max()};
    quint64 hash{std::numeric_limits<uint64_t>::max()};
    for (auto it = name_by_cheat.begin(); it != name_by_cheat.end(); ++it) {
        if (name == it.value()) {
            cheat = it.key();
            break;
        }
    }
    if (cheat == std::numeric_limits<uint64_t>::max()) {
        return;
    }
    for (auto it = cheat_by_hash.begin(); it != cheat_by_hash.end(); ++it) {
        if (cheat == it.value()) {
            hash = it.key();
            break;
        }
    }
    name_by_cheat.remove(cheat);
    address_by_cheat.remove(cheat);
    cheat_by_hash.remove(hash, cheat);
}
