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

  if (Int(Cockpit.TankStand) > 95) then
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