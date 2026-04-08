#include "bobcoin.h"
#include <iostream>

namespace Bobcoin {

// Static Storage
static std::map<std::string, long long> g_Ledger;

void Ledger::Init() {
    g_Ledger["genesis"] = 1000000000;
}

long long Ledger::GetBalance(const std::string& address) {
    if (g_Ledger.find(address) == g_Ledger.end()) return 0;
    return g_Ledger[address];
}

void Ledger::SetBalance(const std::string& address, long long amount) {
    g_Ledger[address] = amount;
}

bool Ledger::Transfer(const std::string& from, const std::string& to, long long amount) {
    if (amount <= 0) return false;
    if (g_Ledger[from] < amount) return false;

    g_Ledger[from] -= amount;
    g_Ledger[to] += amount;
    return true;
}

// Wallet
Wallet::Wallet(const std::string& address) : m_Address(address) {}
Wallet::~Wallet() {}

long long Wallet::GetBalance() const {
    return Ledger::GetBalance(m_Address);
}

bool Wallet::Transfer(const std::string& to_address, long long amount) {
    return Ledger::Transfer(m_Address, to_address, amount);
}

std::string Wallet::GetAddress() const {
    return m_Address;
}

Wallet* Wallet::Create() {
    static int id = 0;
    std::string addr = "0xBob" + std::to_string(++id);
    Ledger::SetBalance(addr, 1000); // Starter funds
    return new Wallet(addr);
}

}
