-- ScreenTournamentLadder overlay
-- Visualizes the top 100 players fetched from the Node.js PostgreSQL backend

local t = Def.ActorFrame{}

t[#t+1] = Def.Quad{
    InitCommand=cmd(FullScreen; diffuse,color("0.1,0.15,0.2,0.9")),
}

t[#t+1] = Def.BitmapText{
    Font="Common Normal",
    Text="Unified Global Leaderboard (K=32 Elo)",
    InitCommand=cmd(x, SCREEN_CENTER_X; y, 40; zoom, 1.5; diffuse, color("1,0.8,0,1")),
}

t[#t+1] = Def.Actor{
    InitCommand=function(self)
        local players = GetTopRankedPlayers() or {}
        local yPos = 100

        -- Header
        self:GetParent():AddChild(
            Def.BitmapText{
                Font="Common Normal",
                Text="RANK  |  USERNAME  |  ELO RATING",
                InitCommand=cmd(x, SCREEN_CENTER_X; y, 80; diffuse, color("0.5,0.5,0.5,1"))
            }
        )

        for i, p in ipairs(players) do
            local colorStr = "1,1,1,1" -- White default
            if i == 1 then colorStr = "1,0.8,0,1" end -- Gold for #1
            if i == 2 then colorStr = "0.8,0.8,0.8,1" end -- Silver for #2
            if i == 3 then colorStr = "0.8,0.5,0.2,1" end -- Bronze for #3

            self:GetParent():AddChild(
                Def.BitmapText{
                    Font="Common Normal",
                    Text=string.format("#%d    %s    %d", p.rank, p.username, p.elo),
                    InitCommand=cmd(x, SCREEN_CENTER_X; y, yPos; diffuse, color(colorStr))
                }
            )
            yPos = yPos + 30
        end
    end
}

return t
