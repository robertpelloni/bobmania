local HelpOverlay = {}

-- Global help registry
HelpOverlay.ScreenData = {}

function HelpOverlay.Register(screenName, data)
    HelpOverlay.ScreenData[screenName] = data
end

function HelpOverlay.Create()
    return Def.ActorFrame {
        Name = "HelpOverlay",
        InitCommand = function(self)
            self:visible(false):draworder(2000) -- Very high draw order
            self:Center()
        end,
        HelpToggleMessageCommand = function(self)
            local vis = not self:GetVisible()
            self:visible(vis)
            if vis then
                self:playcommand("UpdateContent")
            end
        end,

        -- Dark Background
        Def.Quad {
            InitCommand = function(self) self:zoomto(SCREEN_WIDTH, SCREEN_HEIGHT):diffuse(0,0,0,0.85) end
        },

        -- Title
        LoadFont("Common Normal") .. {
            Text = "Help & Information",
            InitCommand = function(self) self:y(-200):zoom(1.5):diffuse(1,1,0,1) end
        },

        -- Content Area
        LoadFont("Common Normal") .. {
            Name = "HelpText",
            InitCommand = function(self) self:y(-50):zoom(0.8):wrapwidthpixels(600):diffuse(1,1,1,1) end,
            UpdateContentCommand = function(self)
                local screen = SCREENMAN:GetTopScreen()
                if not screen then return end

                local sName = screen:GetName()
                local text = "No specific help available for " .. sName

                -- Check registry
                if HelpOverlay.ScreenData[sName] then
                    text = HelpOverlay.ScreenData[sName].Description or text
                else
                    -- Check metrics
                    if THEME:HasMetric(sName, "HelpDescription") then
                        text = THEME:GetMetric(sName, "HelpDescription")
                    end
                end

                self:settext(text)
            end
        },

        -- Close Hint
        LoadFont("Common Normal") .. {
            Text = "Press SELECT or Back to Close",
            InitCommand = function(self) self:y(200):zoom(0.7):diffuse(0.6,0.6,0.6,1) end
        }
    }
end

-- Export globally
_G.HelpOverlay = HelpOverlay
