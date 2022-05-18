#include "search.h"
#include <cassert>
#include "algorithm"
long long Search::Params::convert_to_num(const std::vector<uint8_t> &elem) const {
    long long num = 0;
    long long cur_degree = 1;

    if (type == Search::Input::num) {
        for (unsigned char i : elem) {
            num += i * cur_degree;
            cur_degree *= 1 << 8;
        }
    }

    if (type == Search::Input::raw_bytes) {
        for (size_t i = elem.size() - 1; i >= 0 && i < elem.size(); i--) {
            num += elem[i] * cur_degree;
            cur_degree *= 1 << 8;
        }
    }
    return num;
}

bool Search::ParamsOfSearch::check_coincidence(Search::ResultRaw &raw) const {
    long long num_old = convert_to_num(raw.old_data);
    long long num_new = convert_to_num(raw.cur_data);
    long long num_in = convert_to_num(data_in);
    switch (event) {
        case Action::all: return true;
        case Action::save: return num_new == num_old;
        case Action::changed: return num_new != num_old;
        case Action::encrease: return num_new > num_old;
        case Action::decrease: return num_new < num_old;
        case Action::encrease_or_save: return num_new >= num_old;
        case Action::decrease_or_save: return num_new <= num_old;
        case Action::eq_num: return num_new == num_in;
        case Action::geq_num: return num_new >= num_in;
        case Action::leq_num: return num_new <= num_in;
        case Action::g_num: return num_new > num_in;
        case Action::l_num: return num_new < num_in;
        default: assert(false);
    }
}
