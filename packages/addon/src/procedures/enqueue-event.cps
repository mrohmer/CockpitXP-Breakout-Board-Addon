type
  Packages = record
    package1: String;
    package2: String;
    package3: String;
  end;
function NewPackages(package1: String; package2: String; package3: String): Packages;
begin
  Result.package1 = package1;
  Result.package2 = package2;
  Result.package3 = package3;
end;

function GetPackages(decimal: Integer): Packages;
begin
  case decimal of
    11: Result := NewPackages('100', '001', '011');
    21: Result := NewPackages('100', '010', '101');
    22: Result := NewPackages('100', '010', '110');
    28: Result := NewPackages('100', '011', '100');
    31: Result := NewPackages('100', '011', '100');
    35: Result := NewPackages('100', '100', '011');
    37: Result := NewPackages('100', '100', '101');
    38: Result := NewPackages('100', '100', '110');
    41: Result := NewPackages('100', '101', '001');
    42: Result := NewPackages('100', '101', '010');
    49: Result := NewPackages('100', '110', '001');
    50: Result := NewPackages('100', '110', '010');
    55: Result := NewPackages('100', '110', '111');
    59: Result := NewPackages('100', '111', '011');
    61: Result := NewPackages('100', '111', '101');
    62: Result := NewPackages('100', '111', '110');
    73: Result := NewPackages('101', '001', '001');
    74: Result := NewPackages('101', '001', '010');
    75: Result := NewPackages('001', '001', '011');
    76: Result := NewPackages('101', '001', '100');
    77: Result := NewPackages('001', '001', '101');
    78: Result := NewPackages('001', '001', '110');
    79: Result := NewPackages('101', '001', '111');
    81: Result := NewPackages('101', '010', '001');
    82: Result := NewPackages('101', '010', '010');
    83: Result := NewPackages('001', '010', '011');
    84: Result := NewPackages('101', '010', '100');
    85: Result := NewPackages('001', '010', '101');
    86: Result := NewPackages('001', '010', '110');
    87: Result := NewPackages('101', '010', '111');
    89: Result := NewPackages('001', '011', '001');
    90: Result := NewPackages('001', '011', '010');
    91: Result := NewPackages('101', '011', '011');
    92: Result := NewPackages('001', '011', '100');
    97: Result := NewPackages('101', '100', '001');
    98: Result := NewPackages('101', '100', '010');
    99: Result := NewPackages('001', '100', '011');
    105: Result := NewPackages('001', '101', '001');
    106: Result := NewPackages('001', '101', '010');
    107: Result := NewPackages('101', '101', '011');
    108: Result := NewPackages('001', '101', '100');
    113: Result := NewPackages('001', '110', '001');
    114: Result := NewPackages('001', '110', '010');
    115: Result := NewPackages('101', '110', '011');
    116: Result := NewPackages('001', '110', '100');
    121: Result := NewPackages('101', '111', '001');
    122: Result := NewPackages('101', '111', '010');
    123: Result := NewPackages('001', '111', '011');
    124: Result := NewPackages('101', '111', '100');
    137: Result := NewPackages('110', '001', '001');
    138: Result := NewPackages('110', '001', '010');
    139: Result := NewPackages('010', '001', '011');
    140: Result := NewPackages('110', '001', '100');
    145: Result := NewPackages('110', '010', '001');
    146: Result := NewPackages('110', '010', '010');
    147: Result := NewPackages('010', '010', '011');
    148: Result := NewPackages('110', '010', '100');
  end;
end;

procedure EnqueueEvent(highLevel: String; decimal: Integer);
var
  packages: Packages;
begin
  packages := GetPackages(decimal);

  with newQueryAnw do
  begin
    sql.Clear;

    sql.Text := 'INSERT INTO breakout_board_queue (package1, package2, package3, highLevel, decimal) VALUES (:package1, :package2, :package3, :highLevel, :decimal)';
    ParamByName('package1').AsString = packages.package1;
    ParamByName('package2').AsString = packages.package2;
    ParamByName('package3').AsString = packages.package3;
    ParamByName('highLevel').AsString = highLevel;
    ParamByName('decimal').AsString = decimal;

    Open;

    Free;
  end;

end;

procedure EnqueueResetAllEvent;
begin
  EnqueueEvent('Reset All', 11);
end;

procedure EnqueueVirtualSafetyCarEvent(isOn: Boolean);
begin
  if (isOn) then
  begin
    EnqueueEvent('Virtual Safety Car On', 21);
  end
  else
  begin
    EnqueueEvent('Virtual Safety Car Off', 22);
  end;
end;

procedure EnqueueStartingLightNbrEvent(lightsOn: Integer);
begin
  case lightsOn of
    0: EnqueueEvent('Starting light off', 28);
    1: EnqueueEvent('Starting light 1', 31);
    2: EnqueueEvent('Starting light 2', 35);
    3: EnqueueEvent('Starting light 3', 37);
    4: EnqueueEvent('Starting light 4', 38);
    5: EnqueueEvent('Starting light 5', 41);
  end;
end;

procedure EnqueueFalseStartEvent(lightsOn: Integer);
begin
  EnqueueEvent('False start', 42);
end;

procedure EnqueueNewTrackRecordEvent;
begin
  EnqueueEvent('New track record', 49);
end;

procedure EnqueueNewSessionRecordEvent;
begin
  EnqueueEvent('New session record', 50);
end;

procedure EnqueuePitlaneEvent(pitlaneNbr: Integer; percentage: Integer);
begin
  if (pitlaneNbr = 1) then
  begin
    case percentage of
      0..4: EnqueueEvent('Pitlane 1 off', 55);
      5..14: EnqueueEvent('Pitlane 1 10%', 59);
      15..24: EnqueueEvent('Pitlane 1 20%', 61);
      25..34: EnqueueEvent('Pitlane 1 30%', 62);
      35..44: EnqueueEvent('Pitlane 1 40%', 73);
      45..54: EnqueueEvent('Pitlane 1 50%', 74);
      55..64: EnqueueEvent('Pitlane 1 60%', 75);
      65..74: EnqueueEvent('Pitlane 1 70%', 76);
      75..84: EnqueueEvent('Pitlane 1 80%', 77);
      85..94: EnqueueEvent('Pitlane 1 90%', 78);
      95..100: EnqueueEvent('Pitlane 1 100%', 79);
    end;
  end
  else if (pitlaneNbr = 2) then
    case percentage of
      0..4: EnqueueEvent('Pitlane 2 off', 81);
      5..14: EnqueueEvent('Pitlane 2 10%', 82);
      15..24: EnqueueEvent('Pitlane 2 20%', 83);
      25..34: EnqueueEvent('Pitlane 2 30%', 84);
      35..44: EnqueueEvent('Pitlane 2 40%', 85);
      45..54: EnqueueEvent('Pitlane 2 50%', 86);
      55..64: EnqueueEvent('Pitlane 2 60%', 87);
      65..74: EnqueueEvent('Pitlane 2 70%', 89);
      75..84: EnqueueEvent('Pitlane 2 80%', 90);
      85..94: EnqueueEvent('Pitlane 2 90%', 91);
      95..100: EnqueueEvent('Pitlane 2 100%', 92);
    end;
begin
end;
end;

procedure EnqueueSlotNeedsFuelingEvent(slotId: Integer; isOn: Boolean);
var
  decimal: Integer;
  highLevel: String;
begin
  case slotId of
    1: decimal := 97;
    2: decimal := 105;
    3: decimal := 113;
    4: decimal := 121;
    5: decimal := 137;
    6: decimal := 145;
  else
    exit;
  end;

  if (isOn) then
  begin
    highLevel := 'Slot ' + IntToStr(slotId) + ' nees to refuel';
  end
  else
  begin
    highLevel := 'Slot ' + IntToStr(slotId) + ' does not need to refuel anymore';
    decimal := decimal + 1;
  end;

  EnqueueEvent(highLevel, decimal);
end;

procedure EnqueueSlotIsFuelingEvent(slotId: Integer; isOn: Boolean);
var
  decimal: Integer;
  highLevel: String;
begin
  case slotId of
    1: decimal := 99;
    2: decimal := 107;
    3: decimal := 115;
    4: decimal := 123;
    5: decimal := 139;
    6: decimal := 147;
  else
    exit;
  end;

  if (isOn) then
  begin
    highLevel := 'Slot ' + IntToStr(slotId) + ' starts refueling';
  end
  else
  begin
    highLevel := 'Slot ' + IntToStr(slotId) + ' stops refueling';
    decimal := decimal + 1;
  end;

  EnqueueEvent(highLevel, decimal);
end;
