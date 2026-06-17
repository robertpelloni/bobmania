#include "global.h"
#include "BobcoinBridge.h"
#include "RageLog.h"
#include "json/json.h"
#include "ezsockets.h"

// Actual implementation of JSON-RPC communication for Bobcoin using EzSockets

bool BobcoinBridge::InitRPC() {
    LOG->Trace("BobcoinBridge: Initializing JSON-RPC Connection...");
    // In a real scenario, we'd check if the node is up.
    // For this implementation, we assume connectivity if the singleton initializes.
    return true;
}

static Preference<RString> m_sBobcoinNodeIP("BobcoinNodeIP", "127.0.0.1");
static Preference<int> m_iBobcoinNodePort("BobcoinNodePort", 8080);

static RString CallRPC(const RString& method, const Json::Value& params) {
    EzSockets socket;
    socket.blocking = true;

    RString sIP = m_sBobcoinNodeIP.Get();
    int iPort = m_iBobcoinNodePort.Get();

    if (!socket.create(sIP, iPort)) {
        LOG->Warn("BobcoinBridge: Failed to connect to Bobcoin node at %s:%d", sIP.c_str(), iPort);
        return "";
    }

    Json::Value request;
    request["jsonrpc"] = "2.0";
    request["method"] = method.c_str();
    request["params"] = params;
    request["id"] = 1;

    Json::FastWriter writer;
    RString sPayload = writer.write(request);

    // Construct basic HTTP POST
    RString sHttpRequest = ssprintf(
        "POST / HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n\r\n"
        "%s",
        sIP.c_str(), (int)sPayload.length(), sPayload.c_str()
    );

    socket.sendRaw(sHttpRequest);

    RString sBuffer;
    char buf[4096];
    int bytes;
    while ((bytes = socket.readRaw(buf, sizeof(buf))) > 0) {
        sBuffer.append(buf, bytes);
    }

    // Strip HTTP headers to get the JSON body
    size_t bodyPos = sBuffer.find("\r\n\r\n");
    if (bodyPos != RString::npos) {
        return sBuffer.substr(bodyPos + 4);
    }

    return "";
}

long long BobcoinBridge::GetBalance(const RString& sAddress) {
    if (!IsConnected()) return 0;

    Json::Value params;
    params.append(sAddress.c_str());

    RString sResponse = CallRPC("getbalance", params);
    if (sResponse.empty()) return 0;

    Json::Reader reader;
    Json::Value root;
    if (reader.parse(sResponse.c_str(), root)) {
        if (!root["result"].isNull()) {
            return root["result"].asInt64();
        }
    }

    return 0;
}

bool BobcoinBridge::SendTransaction(const RString& sFrom, const RString& sTo, long long iAmount) {
    if (!IsConnected()) return false;

    Json::Value params;
    params.append(sTo.c_str());
    params.append((Json::Int64)iAmount);

    RString sResponse = CallRPC("sendtoaddress", params);
    if (sResponse.empty()) return false;

    Json::Reader reader;
    Json::Value root;
    if (reader.parse(sResponse.c_str(), root)) {
        return !root["result"].isNull();
    }

    return false;
}

std::vector<TransactionRecord> BobcoinBridge::GetTransactionHistory(const RString& sAddress) {
    std::vector<TransactionRecord> history;
    if (!IsConnected()) return history;

    Json::Value params;
    params.append(sAddress.c_str());

    RString sResponse = CallRPC("listtransactions", params);
    if (sResponse.empty()) return history;

    Json::Reader reader;
    Json::Value root;
    if (reader.parse(sResponse.c_str(), root) && root["result"].isArray()) {
        const Json::Value& txs = root["result"];
        for (unsigned i = 0; i < txs.size(); ++i) {
            TransactionRecord t;
            t.sTxId = txs[i]["txid"].asString();
            t.sType = txs[i]["category"].asString();
            t.iAmount = txs[i]["amount"].asInt64();
            t.iTimestamp = txs[i]["time"].asInt64();
            history.push_back(t);
        }
    }

    return history;
}

std::vector<MarketplaceItem> BobcoinBridge::GetMarketplaceItems() {
    std::vector<MarketplaceItem> items;
    if (!IsConnected()) return items;

    RString sResponse = CallRPC("listitems", Json::Value(Json::arrayValue));
    if (sResponse.empty()) return items;

    Json::Reader reader;
    Json::Value root;
    if (reader.parse(sResponse.c_str(), root) && root["result"].isArray()) {
        const Json::Value& res = root["result"];
        for (unsigned i = 0; i < res.size(); ++i) {
            MarketplaceItem item;
            item.sID = res[i]["id"].asString();
            item.sName = res[i]["name"].asString();
            item.iPrice = res[i]["price"].asInt64();
            items.push_back(item);
        }
    }

    return items;
}
