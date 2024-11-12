{ Pascal Script Engine }
{ Event: StartRealTime }
{ Parameter: Keine }

{%%PROCEDURE.CheckPitlaneStatus%%}

{ Pascal Script Engine }
{ Event: StartRealTime }
{ Parameter: Keine }

function getPitlaneStatus(pitlane: Integer): Integer;
var
  slotId: integer;
begin
  slotId := cpGetIntegerVar('Pitlane:' + IntToStr(pitlane) + ':Slot');

  if (slotId = 0) then
  begin
    Result := 0;
    exit;
  end;

  Cockpit.Slot := slotId;

  if (Int(Cockpit.TankStand) &gt; 95) then
  begin
    Result := 0;
  end
  else
  begin
    Result := 1;
  end;
end;

procedure checkPitlaneStatus;
var
  pitlane: Integer;
begin
  cpSetOutput('Pitlane1', getPitlaneStatus(1) = 1);
  cpSetOutput('Pitlane2', getPitlaneStatus(2) = 1);
end;

var
  sleepTime: Integer;
begin
  cpSetIntegerVar('Exit', 0);
  cpSetIntegerVar('IsFinished', 0);

  sleepTime := cpGetIntegerVar('TimingSleep');

  while (True) do
  begin
    if (cpGetIntegerVar('Exit') = 1) then
    begin
      exit;
    end;

    checkPitlaneStatus();

    if (Cockpit.CUStartampelStatus > 1) and (cpGetIntegerVar('IsChaos') = 1) then
    begin
      cpSetOutput('Flags1', false);
      cpSetOutput('Flags2', false);
    end;

    cpSleep(sleepTime);
  end;
end.