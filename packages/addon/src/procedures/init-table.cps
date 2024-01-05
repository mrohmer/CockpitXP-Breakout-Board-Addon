procedure DropTable;
begin
  with newQueryAnw do
  begin
    sql.Clear;

    sql.Text := 'DROP TABLE breakout_board_queue IF EXISTS';

    Open;

    Free;
  end;
end;

procedure CreateTable;
begin
  with newQueryAnw do
  begin
    sql.Clear;

    sql.Text := 'CREATE TABLE breakout_board_queue (id INTEGER PRIMARY KEY, date DATETIME DEFAULT CURRENT_TIMESTAMP, package1 STRING, package2 STRING, package3 STRING, highLevel STRING, decimal INTEGER)';

    Open;

    Free;
  end;
end;


procedure InitTable;
begin
  // Drop the table to have a clean queue
  DropTable();
  CreateTable();
end;