function getPitlaneStatusValueByNbr(pitlane: Integer): Integer;
var
  slot: Integer;
  entranceTime: Extended;
begin
  slot := cpGetIntegerVar('Pitlane:' + IntToStr(pitlane) + ':Slot:Nbr');

  if (slot < 1) then
  begin
    Result := 0;
    exit;
  end;

  Cockpit.Slot := slot;

  entranceTime := cpGetFlotVar('Pitlane:' + IntToStr(pitlane) + ':Slot:Time');

  if (entranceTime = 0) or (Cockpit.SlotId < 1) or ((cpGetSystemTimeMs - entranceTime > cpGetIntegerVar('PitlaneSecurityTime')) and (not Cockpit.EsWirdGetankt)) then
  begin
    // entrance time is 0 => should not happen when a car is in pit
    // we don't have a slot associated with this nbr
    // entrance time is above security time and the car is not refueling anymore
    Result := 0;
    exit;
  end;

  case Cockpit.TankStand of
    0..4: Result := 0;
    5..14: Result := 10;
    15..24: Result := 20;
    25..34: Result := 30;
    35..44: Result := 40;
    45..54: Result := 50;
    55..64: Result := 60;
    65..74: Result := 70;
    75..84: Result := 80;
    85..94: Result := 90;
    95..100: Result := 100;
  end;
end;
procedure checkPitlaneStatusByNbr(pitlane: Integer);
var
  value, oldValue: Integer;
begin
  oldValue := cpGetIntegerVar('Pitlane:' + IntToStr(pitlane) + ':OldValue');
  value := getPitlaneStatusValueByNbr(pitlane);

  if (oldValue = value) then
  begin
    exit;
  end;

  EnqueuePitlaneEvent(pitlane, value);
  cpSetIntegerVar('Pitlane:' + IntToStr(pitlane) + ':OldValue', value);
end;

procedure checkPitlaneStatus;
var
  slot: Integer;
begin
  if (isEventInactive('Pitlane')) then
  begin
    exit;
  end;
  for pitlane := 1 to 2 do
  begin
    checkPitlaneStatusByNbr(pitlane)
  end;
end;