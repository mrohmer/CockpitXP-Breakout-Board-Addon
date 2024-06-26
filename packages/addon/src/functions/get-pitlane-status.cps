function getPitlaneStatus(pitlane: Integer): boolean;
var
  slotId: integer;
begin
  slotId := cpGetIntegerVar('Pitlane:' + IntToStr(pitlane) + ':Slot');

  if (slotId = 0) then
  begin
    Result := false;
    exit;
  end;

  Cockpit.Slot := slot;

  if (Int(Cockpit.TankStand) > 95) then
  begin
    Result := false;
  end
  else
  begin
    Result := true;
  end;
end;