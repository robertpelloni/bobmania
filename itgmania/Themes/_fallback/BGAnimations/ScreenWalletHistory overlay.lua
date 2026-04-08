local t = Def.ActorFrame {}

t[#t+1] = HelpOverlay.Create()
HelpOverlay.Register("ScreenWalletHistory", {
    Description = "Wallet History\n\nReview your incoming and outgoing transactions.\nMining rewards and winnings are shown in green.\nPurchases and Tips are in red."
})

-- Background
t[#t+1] = Def.Quad {
    InitCommand = function(self) self:zoomto(SCREEN_WIDTH, SCREEN_HEIGHT):Center():diffuse(0,0,0,1) end
}

-- Header
t[#t+1] = LoadFont("Common Large") .. {
    Text = "WALLET HISTORY",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 50):zoom(0.8):diffuse(1,0.8,0,1) end
}

-- Balance Summary
t[#t+1] = LoadFont("Common Normal") .. {
    Text = "Current Balance: ",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 90):zoom(0.7):diffuse(1,1,1,1) end,
    UpdateBalanceCommand = function(self)
        if ECONOMYMAN then
            self:settext("Current Balance: " .. ECONOMYMAN:GetBalance() .. " BOBC")
        else
            self:settext("Current Balance: 1,000 BOBC (Mock)")
        end
    end
}

-- Fetch Transactions
local history = {}
if ECONOMYMAN then
    history = ECONOMYMAN:GetHistory()
else
    history = {
        { Date = "2026-02-10", Description = "Purchased Song Pack 1", Amount = -500 },
        { Date = "2026-02-09", Description = "Mining Reward (95.4%)", Amount = 12 },
        { Date = "2026-02-08", Description = "Won Tournament #442", Amount = 1500 },
        { Date = "2026-02-07", Description = "Tip to 'ArrowSmasher'", Amount = -50 },
        { Date = "2026-02-06", Description = "Mining Reward (88.1%)", Amount = 8 }
    }
end

local selectedIndex = 1

local function UpdateList(self)
    self:runcommandonchildren("UpdateSelection")
end

-- List Container
local list = Def.ActorFrame {
    InitCommand = function(self) self:xy(SCREEN_CENTER_X - 250, 150) end
}

for i, tx in ipairs(history) do
    if i > 8 then break end -- Show only top 8
    local yPos = (i-1) * 35

    local row = Def.ActorFrame {
        InitCommand = function(self) self:y(yPos) end,
        UpdateSelectionCommand = function(self)
            if i == selectedIndex then
                self:diffuse(1,1,1,1)
                self:GetChild("BGLight"):visible(true)
            else
                self:diffuse(0.7,0.7,0.7,1)
                self:GetChild("BGLight"):visible(false)
            end
        end
    }

    row[#row+1] = Def.Quad {
        Name = "BGLight",
        InitCommand = function(self) self:zoomto(550, 30):x(250):diffuse(1,1,1,0.2):visible(false) end
    }

    -- Date
    row[#row+1] = LoadFont("Common Normal") .. {
        Text = tx.Date,
        InitCommand = function(self) self:zoom(0.6):halign(0):diffuse(0.6,0.6,0.6,1) end
    }

    -- Description
    row[#row+1] = LoadFont("Common Normal") .. {
        Text = tx.Description,
        InitCommand = function(self) self:x(100):zoom(0.7):halign(0):maxwidth(300) end
    }

    -- Amount
    row[#row+1] = LoadFont("Common Normal") .. {
        Text = (tx.Amount > 0 and "+" or "") .. tx.Amount .. " BOBC",
        InitCommand = function(self) self:x(500):zoom(0.7):halign(1)
            if tx.Amount > 0 then self:diffuse(0,1,0,1)
            else self:diffuse(1,0,0,1) end
        end
    }

    list[#list+1] = row
end

t[#t+1] = list

-- Controls Hint
t[#t+1] = LoadFont("Common Normal") .. {
    Text = "Up/Down: Browse | SELECT: Help | BACK: Dashboard",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, SCREEN_HEIGHT - 50):zoom(0.6):diffuse(0.5,0.5,0.5,1) end
}

-- Input Handling
local function Input(event)
    if not event.PlayerNumber or event.type ~= "InputEventType_FirstPress" then return false end

    if event.GameButton == "MenuDown" then
        selectedIndex = selectedIndex + 1
        if selectedIndex > #history then selectedIndex = 1 end
        list:playcommand("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "MenuUp" then
        selectedIndex = selectedIndex - 1
        if selectedIndex < 1 then selectedIndex = #history end
        list:playcommand("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "Back" then
        SCREENMAN:SetNewScreen("ScreenUnifiedDashboard")
    elseif event.GameButton == "Select" then
        MESSAGEMAN:Broadcast("HelpToggle")
        SOUND:PlayOnce(THEME:GetPathS("Common", "value"))
    end
    return true
end

t.OnCommand = function(self)
    SCREENMAN:GetTopScreen():AddInputCallback(Input)
    self:playcommand("UpdateBalance")
    list:playcommand("UpdateSelection")
end

return t
