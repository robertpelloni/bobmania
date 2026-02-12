local t = Def.ActorFrame {};

-- Original Evaluation Logic
if not GAMESTATE:IsCourseMode() then
	t[#t+1] = Def.ActorFrame {
		LoadActor(THEME:GetPathG("ScreenEvaluation", "StageDisplay")) .. {
			InitCommand=cmd(x,SCREEN_RIGHT-290;y,SCREEN_TOP+49);
			OffCommand=cmd(sleep,0.175;decelerate,0.4;addy,-105),
		}
	}
else
	t[#t+1] =  Def.ActorFrame {
		InitCommand=cmd(x,SCREEN_RIGHT-290;y,SCREEN_TOP+49);
		OffCommand=cmd(sleep,0.175;decelerate,0.4;addy,-105),
			LoadActor(THEME:GetPathG("", "_sortFrame"))  .. {
				InitCommand=cmd(diffusealpha,0.9;zoom,1.5);
				BeginCommand=function(self)
					self:playcommand("Set")
				end;
				SetCommand=function(self)
					local curStage = GAMESTATE:GetCurrentStage();
					self:diffuse(StageToColor(curStage));
				end
			};
			LoadFont("Common Italic Condensed") .. {
				InitCommand=cmd(y,-1;zoom,1;shadowlength,1;uppercase,true);
				BeginCommand=function(self)
					self:playcommand("Set")
				end;
				CurrentSongChangedMessageCommand= cmd(playcommand,"Set"),
				SetCommand=function(self)
					local curStage = GAMESTATE:GetCurrentStage();
					local course = GAMESTATE:GetCurrentCourse()
					self:settext(string.upper(ToEnumShortString( course:GetCourseType() )))
					-- StepMania is being stupid so we have to do this here;
					self:diffuse(StageToColor(curStage)):diffusetopedge(ColorLightTone(StageToColor(curStage)));
					self:diffusealpha(0):smooth(0.3):diffusealpha(1);
				end;
			};
	}
end;

	
if GAMESTATE:HasEarnedExtraStage() then
	t[#t+1] =  Def.ActorFrame {
		InitCommand=cmd(x,SCREEN_RIGHT-290;y,SCREEN_TOP+49);
		OffCommand=cmd(sleep,0.175;decelerate,0.4;addy,-105),
			LoadActor(THEME:GetPathG("", "_sortFrame"))  .. {
				InitCommand=cmd(diffusealpha,0.9;zoom,1.5);
				BeginCommand=function(self)
					self:playcommand("Set")
				end;
				SetCommand=function(self)
					local curStage = GAMESTATE:GetCurrentStage();
					self:diffuse(StageToColor(curStage));
				end
			};
			LoadActor(THEME:GetPathG("", "_sortFrame"))  .. {
				InitCommand=cmd(diffusealpha,0.9;zoom,1.5;diffuse,color("#FFFFFF");blend,'add');
				BeginCommand=cmd(diffuseshift;effectcolor2,color("1,1,1,0.3");effectcolor2,color("1,1,1,0");effectperiod,2);
			};
			LoadFont("Common Italic Condensed") .. {
				InitCommand=cmd(y,-1;zoom,1;shadowlength,1;uppercase,true;maxwidth,220);
				BeginCommand=function(self)
					self:playcommand("Set")
				end;
				CurrentSongChangedMessageCommand= cmd(playcommand,"Set"),
				SetCommand=function(self)
					local curStage = GAMESTATE:GetCurrentStage();
					local text = string.upper(THEME:GetString("ScreenEvaluation", "ExtraUnlocked"))
					self:settext(text)
					-- StepMania is being stupid so we have to do this here;
					self:diffuse(StageToColor(curStage)):diffusetopedge(ColorLightTone(StageToColor(curStage)));
					self:diffusealpha(0):smooth(0.3):diffusealpha(1);
				end;
			};
	}
end;

-- Tournament Result Reporting Logic
t[#t+1] = Def.ActorFrame {
    InitCommand=function(self)
        if _G.ActiveTournamentMatchID and TOURNAMENTMAN then
            -- Get Player Score (P1 for now)
            local pss = STATSMAN:GetCurStageStats():GetPlayerStageStats(PLAYER_1)
            local score = pss:GetScore()

            -- Report to Backend
            TOURNAMENTMAN:ReportMatchResult(_G.ActiveTournamentMatchID, score)

            -- Clear Flag
            _G.ActiveTournamentMatchID = nil

            -- Show Message
            self:playcommand("ShowResult", {score=score})
        end
    end,

    ShowResultCommand=function(self, params)
        SCREENMAN:SystemMessage("Tournament Match Complete! Score: " .. params.score)
    end,

    LoadFont("Common Normal") .. {
        Text="Tournament Match Recorded",
        InitCommand=function(self)
            self:xy(SCREEN_CENTER_X, SCREEN_TOP + 100):zoom(1.0):diffuse(Color.Green):visible(false)
        end,
        ShowResultCommand=function(self)
            self:visible(true):pulse():effectmagnitude(1.1,1.0,1.0)
        end
    }
}

return t;
