{ Pascal Script Engine }
{ Event: DigInputEvent }
{ Parameter: Cockpit.Slot=<SlotPlatz;0/1=Status Eingang;>,
             Cockpit.Station=<EingangNummer>,
             Cockpit.Parameter=<DeviceNummer>
}

{%%FUNCTION.GetPitlaneStatus%%}

begin
  if (Cockpit.Parameter <> 2) then
  begin
    exit;
  end;

  if (Cockpit.Station = cpGetIntegerVar('Pitlane1StartSensor')) then
  begin
    cpSetIntegerVar('Pitlane:1:Slot', Cockpit.Slot);
  end
  else if (Cockpit.Station = cpGetIntegerVar('Pitlane1EndSensor')) then
  begin
    cpSetIntegerVar('Pitlane:1:Slot', 0);
  end
  else if (Cockpit.Station = cpGetIntegerVar('Pitlane2StartSensor')) then
  begin
    cpSetIntegerVar('Pitlane:2:Slot', Cockpit.Slot);
  end
  else if (Cockpit.Station = cpGetIntegerVar('Pitlane2EndSensor')) then
  begin
    cpSetIntegerVar('Pitlane:2:Slot', 0);
  end;

  cpSetOutput('Pitlane1', getPitlaneStatus(1));
  cpSetOutput('Pitlane2', getPitlaneStatus(2));
end.