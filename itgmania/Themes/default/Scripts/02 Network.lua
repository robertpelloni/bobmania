-- Network Helper Script
+
+-- Global wrappers for C++ Singletons
+
+function GetPlayerBalance()
+    if ECONOMYMAN then
+        return ECONOMYMAN:GetBalance("WALLET_PLAYER")
+    end
+    return 0
+end
+
+function GetPlayerElo()
+    if ECONOMYMAN then
+        return ECONOMYMAN:GetPlayerElo()
+    end
+    return 1000
+end
+
+function BuyItem(name, cost)
+    if ECONOMYMAN then
+        return ECONOMYMAN:Transfer("WALLET_PLAYER", "WALLET_SHOP", cost, "Buy " .. name)
+    end
+    return false
+end

function WatchMatch(matchID)
    if SPECTATORMAN then
        SPECTATORMAN:ConnectToMatch(matchID)
    end
end

function SyncAssets(gameName)
    if ASSETSYNCMAN then
        return ASSETSYNCMAN:SyncWithGame(gameName)
    end
    return 0
end
+