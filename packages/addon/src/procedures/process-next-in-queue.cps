function GetNext(column: String; log: Boolean): Integer;
var
  decimal, res: Integer;
begin
  with newQueryAnw do
  begin
    sql.Clear;

    sql.Text := 'SELECT id, highLevel, decimal FROM breakout_board_queue ORDER BY date DESC LIMIT 1';

    Open;

    if (IsEmpty) then
    begin
      Result := -1;
      exit;
    end;

    First;

    decimal := FieldByName(column).AsInteger;

    if (decimal < 1) then
    begin
      Result := -1;
      exit;
    end;


    if (log) then
    begin
      Cockpit.WriteLog := 'Found Event: ' + FieldByName('highLevel').AsString + '(' + IntToStr(decimal) + ')';
    end;

    res := FieldByName(column).AsInteger;
    Result := res;

    Free;
  end;
end;

procedure DeleteNextEvent();
var
  id: Integer;
begin
  id := GetNext('id', false);
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

function calcParityBit(arr: array [0..8] of Integer): Integer;
var
  i, value: Integer;
begin
  value := 0;
  for i := 0 to 8 do
  begin
    if (arr[i] = 1) and (value = 1) then
    begin
      value := 0;
    end
    else if (arr[i] = 1) and (value = 0) then
    begin
      value := 1;
    end;
  end;
  Result := value;
end;

procedure LogChunk(chunk: Integer; arr: array [0..8] of Integer);
var
  i, start: Integer;
  str: String;
begin
  str := '';
  start := (chunk - 1) * 3;
  for i := start to start + 2 do
  begin
    str := str + IntToStr(arr[i]);
  end;
  Cockpit.WriteLog := 'Chunk ' + IntToStr(chunk) + ': ' + str;
end;

procedure SendPackage(nbr: Integer; pkg: Integer);
var
  i, value, rest, tmp: Integer;
  arr: array [0..8] of Integer;
begin
  value := pkg;
  for i := 0 to 8 do
  begin
    tmp := Int(value / 2);
    rest := value - tmp * 2;
    arr[8 - i] := rest;
    value := tmp;
  end;

  arr[0] := calcParityBit(arr);

  LogChunk(nbr, arr);

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

procedure SendEvent(event: Integer; signalHoldTime: Integer);
begin
  //try
  SendPackage(1, event);

  cpSleep(signalHoldTime);
  cpSetOutput('Clock', false);
  cpSleep(signalHoldTime);

  SendPackage(2, event);

  cpSleep(signalHoldTime);
  cpSetOutput('Clock', false);
  cpSleep(signalHoldTime);

  SendPackage(3, event);

  cpSleep(signalHoldTime);
  //finally
  // definitely reset the clock so it can be used for the next event
  cpSetOutput('Clock', false);
  cpSleep(signalHoldTime);
  //end;
end;

procedure ProcessNextInQueue(signalHoldTime: Integer);
var
  next: Integer;
begin
  // try
  next := GetNext('decimal', true);

  if (next < 1) then
  begin
    exit;
  end;
  //except
  //  exit;
  //end;

  //try
  SendEvent(next, signalHoldTime);
  LogEventSuccess(next);
  //except
  //  LogEventFailure(next);
  //finally
  // delete it in any case to prevent piling up in the queue
  DeleteNextEventSafe();
  //end;
end;