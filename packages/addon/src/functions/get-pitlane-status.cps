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

  entranceTime := cpGetFloatVar('Pitlane:' + IntToStr(pitlane) + ':Slot:Time');

  if (entranceTime = 0) or (Cockpit.SlotId < 1) or ((cpGetSystemTimeMs - entranceTime > cpGetIntegerVar('PitlaneSecurityTime')) and (not Cockpit.EsWirdGetankt)) then
  begin
    // entrance time is 0 => should not happen when a car is in pit
    // we don't have a slot associated with this nbr
    // entrance time is above security time and the car is not refueling anymore
    Result := false;
    exit;
  end;

  if (Int(Cockpit.TankStand) > 95) then
  begin
    Result := false;
  end
  else
  begin
    Result := true;
  end;
end;