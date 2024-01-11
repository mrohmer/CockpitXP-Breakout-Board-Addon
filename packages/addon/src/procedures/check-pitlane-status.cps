function getPitlaneStatusValueByNbr(pitlane: Integer): Integer;
var
  slot: Integer;
  entranceTime: Extended;
begin
  slot := cpGetIntegerVar('Pitlane:' + IntToStr(pitlane) + ':Slot:Nbr');

  if (slot < 1) then
  begin
    Result := -1;
    exit;
  end;

  Cockpit.Slot := slot;

  entranceTime := cpGetFloatVar('Pitlane:' + IntToStr(pitlane) + ':Slot:Time');

  if (entranceTime = 0) or (Cockpit.SlotId < 1) or ((cpGetSystemTimeMs - entranceTime > cpGetIntegerVar('PitlaneSecurityTime')) and (not Cockpit.EsWirdGetankt)) then
  begin
    // entrance time is 0 => should not happen when a car is in pit
    // we don't have a slot associated with this nbr
    // entrance time is above security time and the car is not refueling anymore
    Result := -1;
    exit;
  end;

  case Int(Cockpit.TankStand) of
    0..3: Result := 0;     //  0 ->   0 ->   3   0.00000
    4..10: Result := 1;    //  4 ->   7 ->  10   7.14286
    11..17: Result := 2;   // 11 ->  14 ->  17  14.28571
    18..24: Result := 3;   // 18 ->  21 ->  24  21.42857
    25..31: Result := 4;   // 25 ->  29 ->  31  28.57143
    32..38: Result := 5;   // 32 ->  36 ->  38  35.71429
    39..45: Result := 6;   // 39 ->  43 ->  45  42.85714
    46..53: Result := 7;   // 46 ->  50 ->  53  50.00000
    54..60: Result := 8;   // 54 ->  57 ->  60  57.14286
    61..67: Result := 9;   // 61 ->  64 ->  67  64.28571
    68..74: Result := 10;  // 68 ->  71 ->  74  71.42857
    75..81: Result := 11;  // 75 ->  79 ->  81  78.57143
    82..88: Result := 12;  // 82 ->  86 ->  88  85.71429
    89..95: Result := 13;  // 89 ->  93 ->  95  92.85714
    96..100: Result := 14; // 96 -> 100 -> 100 100.00000
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
  pitlane: Integer;
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