
function GetNext(column: String): Integer;
begin
  with newQueryAnw do
  begin
    sql.Clear;

    sql.Text := 'SELECT id, decimal FROM breakout_board_queue ORDER BY date DESC LIMIT 1';

    Open;

    if (IsEmpty) then
    begin
      Result := -1;
      exit;
    end;

    First;

    Result := FieldByName(column).AsInteger;

    Free;
  end;
end;

procedure DeleteNextEvent();
var
  id: Integer;
begin
  id := GetNext('id');
  with newQueryAnw do
  begin
    sql.Clear;

    sql.Text := 'DELETE FROM breakout_board_queue WHERE id=:id';
    ParamByName('id').AsInteger := id;

    Open;

    Free;
  end;
end;

procedure DeleteNextEventSafe();
begin
  //try
    DeleteNextEvent();
  //except
      // just to be sure...
  //  exit;
  //end;
end;

procedure SendPackage(nbr: Integer; pkg: Integer);
var
  arr: array [0..8] of Integer;
begin
  case pkg of
    11: arr := [1,0,0, 0,0,1, 0,1,1];
    21: arr := [1,0,0, 0,1,0, 1,0,1];
    22: arr := [1,0,0, 0,1,0, 1,1,0];
    28: arr := [1,0,0, 0,1,1, 1,0,0];
    31: arr := [1,0,0, 0,1,1, 1,0,0];
    35: arr := [1,0,0, 1,0,0, 0,1,1];
    37: arr := [1,0,0, 1,0,0, 1,0,1];
    38: arr := [1,0,0, 1,0,0, 1,1,0];
    41: arr := [1,0,0, 1,0,1, 0,0,1];
    42: arr := [1,0,0, 1,0,1, 0,1,0];
    49: arr := [1,0,0, 1,1,0, 0,0,1];
    50: arr := [1,0,0, 1,1,0, 0,1,0];
    55: arr := [1,0,0, 1,1,0, 1,1,1];
    59: arr := [1,0,0, 1,1,1, 0,1,1];
    61: arr := [1,0,0, 1,1,1, 1,0,1];
    62: arr := [1,0,0, 1,1,1, 1,1,0];
    73: arr := [1,0,1, 0,0,1, 0,0,1];
    74: arr := [1,0,1, 0,0,1, 0,1,0];
    75: arr := [0,0,1, 0,0,1, 0,1,1];
    76: arr := [1,0,1, 0,0,1, 1,0,0];
    77: arr := [0,0,1, 0,0,1, 1,0,1];
    78: arr := [0,0,1, 0,0,1, 1,1,0];
    79: arr := [1,0,1, 0,0,1, 1,1,1];
    81: arr := [1,0,1, 0,1,0, 0,0,1];
    82: arr := [1,0,1, 0,1,0, 0,1,0];
    83: arr := [0,0,1, 0,1,0, 0,1,1];
    84: arr := [1,0,1, 0,1,0, 1,0,0];
    85: arr := [0,0,1, 0,1,0, 1,0,1];
    86: arr := [0,0,1, 0,1,0, 1,1,0];
    87: arr := [1,0,1, 0,1,0, 1,1,1];
    89: arr := [0,0,1, 0,1,1, 0,0,1];
    90: arr := [0,0,1, 0,1,1, 0,1,0];
    91: arr := [1,0,1, 0,1,1, 0,1,1];
    92: arr := [0,0,1, 0,1,1, 1,0,0];
    97: arr := [1,0,1, 1,0,0, 0,0,1];
    98: arr := [1,0,1, 1,0,0, 0,1,0];
    99: arr := [0,0,1, 1,0,0, 0,1,1];
    105: arr := [0,0,1, 1,0,1, 0,0,1];
    106: arr := [0,0,1, 1,0,1, 0,1,0];
    107: arr := [1,0,1, 1,0,1, 0,1,1];
    108: arr := [0,0,1, 1,0,1, 1,0,0];
    113: arr := [0,0,1, 1,1,0, 0,0,1];
    114: arr := [0,0,1, 1,1,0, 0,1,0];
    115: arr := [1,0,1, 1,1,0, 0,1,1];
    116: arr := [0,0,1, 1,1,0, 1,0,0];
    121: arr := [1,0,1, 1,1,1, 0,0,1];
    122: arr := [1,0,1, 1,1,1, 0,1,0];
    123: arr := [0,0,1, 1,1,1, 0,1,1];
    124: arr := [1,0,1, 1,1,1, 1,0,0];
    137: arr := [1,1,0, 0,0,1, 0,0,1];
    138: arr := [1,1,0, 0,0,1, 0,1,0];
    139: arr := [0,1,0, 0,0,1, 0,1,1];
    140: arr := [1,1,0, 0,0,1, 1,0,0];
    145: arr := [1,1,0, 0,1,0, 0,0,1];
    146: arr := [1,1,0, 0,1,0, 0,1,0];
    147: arr := [0,1,0, 0,1,0, 0,1,1];
    148: arr := [1,1,0, 0,1,0, 1,0,0];
  end;

  cpSetOutput('Data1', arr[(nbr - 1) * 3] = 1);
  cpSetOutput('Data2', arr[(nbr - 1) * 3 + 1] = 1);
  cpSetOutput('Data3', arr[(nbr - 1) * 3 + 2] = 1);
  cpSetOutput('Clock', true);
end;

function EventToStr(event: Integer): String;
begin
  Result := '(' + IntToStr(event) + ')';
end;
procedure LogEventSuccess(event: Integer);
begin
  Cockpit.WriteLog := 'Processed event: ' + EventToStr(event);
end;
procedure LogEventFailure(event: Integer);
begin
  Cockpit.WriteLog := 'Processing event failed: ' + EventToStr(event); // todo: add error
end;

procedure SendEvent(event: Integer);
begin
  //try
    SendPackage(1, event);

    cpSleep(25);
    cpSetOutput('Clock', false);
    cpSleep(25);

    SendPackage(2, event);

    cpSleep(25);
    cpSetOutput('Clock', false);
    cpSleep(25);

    SendPackage(3, event);

    cpSleep(25);
  //finally
      // definitely reset the clock so it can be used for the next event
    cpSetOutput('Clock', false);
    cpSleep(25);
  //end;
end;

procedure ProcessNextInQueue;
var
  next: Integer;
begin
  // try
    next := GetNext('decimal');

    if (next = -1) then
    begin
      exit;
    end;
  //except
  //  exit;
  //end;

  //try
    SendEvent(next);
    LogEventSuccess(next);
  //except
  //  LogEventFailure(next);
  //finally
    // delete it in any case to prevent piling up in the queue
    DeleteNextEventSafe();
  //end;
end;