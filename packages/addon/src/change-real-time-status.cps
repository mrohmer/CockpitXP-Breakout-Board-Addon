{ Pascal Script Engine }
{ Event: ChangeRealTimeStatus }
{ Parameter: Cockpit.Parameter = <RennStatus> }

{%%FUNCTION.GetRaceStatus%%}
{%%FUNCTION.GetChaosStatus%%}

var
  raceStatus, chaosStatus: Boolean;
begin
  raceStatus := getRaceStatus();
  chaosStatus := getChaosStatus(Cockpit.Parameter);

  if (chaosStatus) then
  begin
    cpSetOutput('Flags1', false);
    cpSetOutput('Flags2', true);
  end
  else if (raceStatus) then
  begin
    cpSetOutput('Flags1', true);
    cpSetOutput('Flags2', false);
  end
  else
  begin
    cpSetOutput('Flags1', false);
    cpSetOutput('Flags2', false);
  end;
end.
