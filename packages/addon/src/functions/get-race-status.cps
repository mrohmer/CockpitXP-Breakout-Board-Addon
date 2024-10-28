function getRaceStatus: boolean;
var
  value, oldValue: Boolean;
begin
  Result := cpGetRacingStatus() = 'R';
end;