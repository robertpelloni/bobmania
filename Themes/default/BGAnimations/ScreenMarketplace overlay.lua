local t = Def.ActorFrame {}

-- Background
t[#t+1] = Def.Quad {
    InitCommand = function(self) self:zoomto(SCREEN_WIDTH, SCREEN_HEIGHT):Center():diffuse(0,0,0,1) end
}

-- Header
t[#t+1] = LoadFont("Common Large") .. {
    Text = "MARKETPLACE",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 50):zoom(0.8):diffuse(1,1,0,1) end
}

-- Balance
t[#t+1] = LoadFont("Common Normal") .. {
    Text = "Wallet: ",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 80):zoom(0.7):diffuse(1,1,1,1) end,
    UpdateBalanceCommand = function(self)
        if ECONOMYMAN then
            self:settext("Wallet: " .. ECONOMYMAN:GetBalance() .. " BOBC")
        else
            self:settext("Wallet: 1,000 BOBC (Mock)")
        end
    end
}

-- Item Selection
local items = {}
if ECONOMYMAN then
    items = ECONOMYMAN:GetMarketplaceItems()
else
    items = {
        { ID="song_pack_1", Name="Song Pack 1", Price=500 },
        { ID="avatar_frame_gold", Name="Gold Avatar Frame", Price=2000 },
        { ID="xp_boost_1h", Name="XP Boost (1 Hour)", Price=100 },
    }
end

local selectedIndex = 1

local function UpdateList(self)
    self:runcommandonchildren("UpdateSelection")
end

-- List Container
local list = Def.ActorFrame {
    InitCommand = function(self) self:xy(SCREEN_CENTER_X - 150, 150) end
}

for i, item in ipairs(items) do
    local yPos = (i-1) * 40

    local row = Def.ActorFrame {
        InitCommand = function(self) self:y(yPos) end,
        UpdateSelectionCommand = function(self)
            if i == selectedIndex then
                self:diffuse(1,1,0,1):zoom(1.1)
            else
                self:diffuse(1,1,1,1):zoom(1.0)
            end
        end
    }

    -- Name
    row[#row+1] = LoadFont("Common Normal") .. {
        Text = item.Name,
        InitCommand = function(self) self:zoom(0.7):halign(0) end
    }

    -- Price
    row[#row+1] = LoadFont("Common Normal") .. {
        Text = item.Price .. " BOBC",
        InitCommand = function(self) self:x(300):zoom(0.7):halign(1) end
    }

    list[#list+1] = row
end

t[#t+1] = list
list:queuecommand("UpdateSelection")

-- Buy Result Message
local msg = Def.ActorFrame {
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, SCREEN_HEIGHT - 100):visible(false) end,
    ShowMessageCommand = function(self, params)
        self:visible(true):stoptweening():zoom(0.8):diffuse(params.color)
        self:runcommandonchildren("SetText", {text=params.text})
        self:sleep(2):linear(0.5):diffusealpha(0):visible(false)
    end
}

msg[#msg+1] = LoadFont("Common Normal") .. {
    Name = "MessageText",
    SetTextCommand = function(self, params) self:settext(params.text) end
}
t[#t+1] = msg

-- Input Handling
local function Input(event)
    if not event.PlayerNumber then return end
    if event.type ~= "InputEventType_FirstPress" then return end

    if event.GameButton == "MenuDown" then
        selectedIndex = selectedIndex + 1
        if selectedIndex > #items then selectedIndex = 1 end
        list:playcommand("UpdateSelection")
    elseif event.GameButton == "MenuUp" then
        selectedIndex = selectedIndex - 1
        if selectedIndex < 1 then selectedIndex = #items end
        list:playcommand("UpdateSelection")
    elseif event.GameButton == "Start" then
        local item = items[selectedIndex]
        if ECONOMYMAN then
            if ECONOMYMAN:HasItem(item.ID) then
                msg:playcommand("ShowMessage", {text="Already Owned!", color=color("#FFFF00")})
            else
                if ECONOMYMAN:BuyItem(item.ID) then
                    msg:playcommand("ShowMessage", {text="Purchased " .. item.Name .. "!", color=color("#00FF00")})
                    t:playcommand("UpdateBalance")
                else
                    msg:playcommand("ShowMessage", {text="Insufficient Funds!", color=color("#FF0000")})
                end
            end
        else
            msg:playcommand("ShowMessage", {text="Economy Disconnected", color=color("#FF0000")})
        end
    elseif event.GameButton == "Back" then
        SCREENMAN:SetNewScreen("ScreenUnifiedDashboard")
    end
end

t[#t+1] = Def.ActorFrame {
    OnCommand = function(self)
        SCREENMAN:GetTopScreen():AddInputCallback(Input)
        self:queuecommand("UpdateBalance")
    end
}

-- Help Overlay
t[#t+1] = HelpOverlay.Create()

return t
