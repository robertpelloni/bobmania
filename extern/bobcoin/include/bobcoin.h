#ifndef BOBCOIN_H
#define BOBCOIN_H

#include <string>
#include <map>

namespace Bobcoin {

class Wallet {
public:
    Wallet(const std::string& address);
    ~Wallet();

    long long GetBalance() const;
    bool Transfer(const std::string& to_address, long long amount);
    std::string GetAddress() const;

    static Wallet* Create();

private:
    std::string m_Address;
};

// Global Ledger (Mock)
class Ledger {
public:
    static void Init();
    static long long GetBalance(const std::string& address);
    static void SetBalance(const std::string& address, long long amount);
    static bool Transfer(const std::string& from, const std::string& to, long long amount);
};

}

#endif
