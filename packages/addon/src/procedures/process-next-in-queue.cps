Type
  Event = record
    id: Integer;
    highLevel: String;
    decimal: Integer;
    package1: String;
    package2: String;
    package3: String;
  end;

function GetNext(): Event;
begin
  with newQueryAnw do
  begin
    sql.Clear;

    sql.Text := 'SELECT id, package1, package2, package3, highLevel, decimal FROM breakout_board_queue ORDER BY date DESC LIMIT 1';

    Open;

    if (IsEmpty) then
    begin
      Result.id := -1;
      exit;
    end;

    First;

    Result.id := FieldByName('id').AsInteger;
    Result.package1 := FieldByName('package1').AsString;
    Result.package2 := FieldByName('package2').AsString;
    Result.package3 := FieldByName('package3').AsString;
    Result.highLevel := FieldByName('highLevel').AsString;
    Result.decimal := FieldByName('decimal').AsInteger;

    Free;
  end;
end;

procedure DeleteEvent(id: Integer);
begin
  with newQueryAnw do
  begin
    sql.Clear;

    sql.Text := 'DELETE FROM breakout_board_queue WHERE id = :id';
    ParamByName('id').AsInteger = id;

    Open;

    Free;
  end;
end;

procedure DeleteEventSafe(id: Integer);
begin
  try
    DeleteEvent(id);
  except
      // just to be sure...
    exit;
  end;
end;

procedure SendPackage(package: String);
begin
  cpSetOutput('Pin1', LeftStr(package, 1) = '1');
  cpSetOutput('Pin2', RightStr(LeftStr(package, 2), 1) = '1');
  cpSetOutput('Pin3', RightStr(package, 1) = '1');
  cpSetOutput('Clock', true);
end;

function EventToStr(event: Event): String;
begin
  Result := '(' + IntToStr(event.id) + ', ' + event.highLevel + ', ' + IntToStr(event.decimal) + ', ' + event.package1 + event.package2 + event.package3 + ')';
end;
procedure LogEventSuccess(event: Event);
begin
  Cockpit.WriteLog := 'Processed event: ' + EventToStr(event);
end;
procedure LogEventFailure(event: Event);
begin
  Cockpit.WriteLog := 'Processing event failed: ' + EventToStr(event); // todo: add error
end;

procedure SendEvent(event: Event);
begin
  try
    SendPackage(event.package1);

    cpSleep(100);
    cpSetOutput('Clock', false);
    cpSleep(100);

    SendPackage(event.package2);

    cpSleep(100);
    cpSetOutput('Clock', false);
    cpSleep(100);

    SendPackage(event.package3);

    cpSleep(100);
  finally
      // definitely reset the clock so it can be used for the next event
    cpSetOutput('Clock', false);
    cpSleep(100);
  end;
end;

procedure ProcessNextInQueue;
var
  next: Event;
begin
  try
    next := GetNext();

    if (next.id = -1) then
    begin
      exit;
    end;
  except
    exit;
  end;

  try
    SendEvent(next);
    LogEventSuccess(next);
  except
    LogEventFailure(next);
  finally
      // delete it in any case to prevent piling up in the queue
    DeleteEventSafe(next.id);
  end;
end;