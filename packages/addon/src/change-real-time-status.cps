{ Pascal Script Engine }
{ Event: ChangeRealTimeStatus }
{ Parameter: Cockpit.Parameter = <RennStatus> }

{%%FUNCTION.GetRaceStatus%%}
{%%FUNCTION.GetChaosStatus%%}

var
  raceStatus, chaosStatus: Boolean;
begin
  raceStatus := getRaceStatus();
  chaosStatus := getChaosValue(Cockpit.Parameter);

  if (chaosStatus) then
  begin
    cpSetOutput('Flags1', 0);
    cpSetOutput('Flags2', 1);
  end
  else if (raceStatus) then
  begin
    cpSetOutput('Flags1', 1);
    cpSetOutput('Flags2', 0);
  else
  begin
    cpSetOutput('Flags1', 0);
    cpSetOutput('Flags2', 0);
  end;
end.
