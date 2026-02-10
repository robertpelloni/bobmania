local t = Def.ActorFrame {}

-- Background
t[#t+1] = Def.Quad {
    InitCommand = function(self) self:zoomto(SCREEN_WIDTH, SCREEN_HEIGHT):Center():diffuse(0,0,0,1) end
}

-- Header
t[#t+1] = LoadFont("Common Large") .. {
    Text = "WALLET HISTORY",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 50):zoom(0.8):diffuse(1,1,0,1) end
}

-- History List Container
local history = Def.ActorFrame {
    InitCommand = function(self) self:xy(SCREEN_CENTER_X - 250, 120) end
}

-- Fetch Data
local txns = {}
if ECONOMYMAN then
    txns = ECONOMYMAN:GetHistory()
else
    -- Mock Data if backend missing
    txns = {
        { Date="2023-10-27 12:00", Description="Mining Reward (99.5%)", Amount=50 },
        { Date="2023-10-27 11:45", Description="Purchased Song Pack 1", Amount=-500 },
        { Date="2023-10-27 10:30", Description="Tournament Prize (1st)", Amount=1000 },
    }
end

local MAX_ITEMS = 12
for i=1, math.min(#txns, MAX_ITEMS) do
    local txn = txns[i]
    local yPos = (i-1) * 35

    local row = Def.ActorFrame {
        InitCommand = function(self) self:y(yPos) end
    }

    -- Date
    row[#row+1] = LoadFont("Common Normal") .. {
        Text = txn.Date,
        InitCommand = function(self) self:zoom(0.6):diffuse(0.7,0.7,0.7,1):halign(0) end
    }

    -- Description
    row[#row+1] = LoadFont("Common Normal") .. {
        Text = txn.Description,
        InitCommand = function(self) self:x(180):zoom(0.7):diffuse(1,1,1,1):halign(0) end
    }

    -- Amount
    local amtColor = (txn.Amount >= 0) and color("#00FF00") or color("#FF0000")
    local amtText = (txn.Amount >= 0) and "+"..txn.Amount or tostring(txn.Amount)

    row[#row+1] = LoadFont("Common Normal") .. {
        Text = amtText .. " BOBC",
        InitCommand = function(self) self:x(500):zoom(0.7):diffuse(amtColor):halign(1) end
    }

    history[#history+1] = row
end

t[#t+1] = history

-- Back Button
t[#t+1] = LoadFont("Common Normal") .. {
    Text = "Press BACK to Exit",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, SCREEN_HEIGHT - 50):zoom(0.8) end
}

-- Input Handling
local function Input(event)
    if not event.PlayerNumber then return end
    if event.type ~= "InputEventType_FirstPress" then return end

    if event.GameButton == "Back" then
        SCREENMAN:SetNewScreen("ScreenUnifiedDashboard")
    end
end

t[#t+1] = Def.ActorFrame {
    OnCommand = function(self) SCREENMAN:GetTopScreen():AddInputCallback(Input) end
}

-- Help Overlay
t[#t+1] = HelpOverlay.Create()

return t
