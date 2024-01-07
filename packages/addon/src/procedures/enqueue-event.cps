procedure EnqueueEvent(highLevel: String; decimal: Integer);
begin
  with newQueryAnw do
  begin
    sql.Clear;

    sql.Text := 'INSERT INTO breakout_board_queue (highLevel, decimal) VALUES (:highLevel, :decimal)';
    ParamByName('highLevel').AsString := highLevel;
    ParamByName('decimal').AsInteger := decimal;

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

procedure EnqueueFalseStartEvent;
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
  begin
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
    begin
      exit;
    end;
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
    begin
      exit;
    end;
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
