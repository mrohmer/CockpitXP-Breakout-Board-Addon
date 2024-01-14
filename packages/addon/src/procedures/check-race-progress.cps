function getMaxLaps:Integer;
var
  maxLaps, laps, slot: Integer;
begin
  maxLaps := 0;
  for slot := 1 to cpCountOfSlots do
  begin
    Cockpit.Slot := slot;
    laps := Cockpit.Runde;

    if (laps > maxLaps) then
    begin
      maxLaps := laps;
    end;
  end;

  Result := maxLaps;
end;

function getRaceProgressWithNeededLaps(neededLaps: Integer): Integer;
begin
  Result := Int(getMaxLaps / neededLaps);
end;

function getMaxTime: Integer;
var
  maxTime, time, slot: Integer;
begin
  maxTime := 0;
  for slot := 1 to cpCountOfSlots do
  begin
    Cockpit.Slot := slot;
    time := Cockpit.GesamtZeit;

    if (time > maxTime) then
    begin
      maxTime := time;
    end;
  end;

  Result := maxTime;
end;

function getRaceProgressWithTime(time: Integer): Integer;
begin
  Result := Int(getMaxTime / time);
end;

function getRaceProgress: Integer;
var
  neededLaps, neededTime: Integer;
begin
  neededLaps := cpGetTargetLap();
  if (neededLaps > 0) then
  begin
    Result := getRaceProgressWithNeededLaps(neededLaps);
  end;

  neededTime := cpGetTargetTime();
  if (neededTime > 0) then
  begin
    Result := getRaceProgressWithTime(neededTime);
  end;

  Result := 0;
end;

function transformRaceProgressToEventValue(value: Integer): Integer;
begin
  case value of
    0..3: Result := 0;     //   0 ->   0 ->   3   0.00000
    4..12: Result := 1;    //   4 ->   8 ->  12   8.33333
    13..20: Result := 2;   //  13 ->  17 ->  20  16.66667
    21..28: Result := 3;   //  21 ->  25 ->  28  25.00000
    29..37: Result := 4;   //  29 ->  33 ->  37  33.33333
    38..45: Result := 5;   //  38 ->  42 ->  45  41.66667
    46..53: Result := 6;   //  46 ->  50 ->  53  50.00000
    54..62: Result := 7;   //  54 ->  58 ->  62  58.33333
    63..70: Result := 8;   //  63 ->  67 ->  70  66.66667
    71..78: Result := 9;   //  71 ->  75 ->  78  75.00000
    79..87: Result := 10;  //  79 ->  83 ->  87  83.33333
    88..95: Result := 11;  //  88 ->  92 ->  95  91.66667
    96..100: Result := 12; //  96 -> 100 -> 100 100.00000
  end;
end;

procedure checkRaceProgress;
var
  value, oldValue: Boolean;
begin
  if (isEventInactive('RaceProgress')) then
  begin
    exit;
  end;

  value := transformRaceProgressToEventValue(getRaceProgress);
  oldValue := cpGetIntegerVar('RaceProgress:OldValue');
  if (oldValue = value) then
  begin
    exit;
  end;

  EnqueueVirtualSafetyCarEvent(value);
  cpSetIntegerVar('RaceProgress:OldValue', value);
end;