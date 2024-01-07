{ Pascal Script Engine }
{ Event: SchnellsteRunde }
{ Parameter: Cockpit.Slot=<SlotPlatz> }

{%%PROCEDURE.EnqueueEvent%%}
{%%FUNCTION.IsEventActive%%}
begin
  if (not isEventActive('TrackRecord')) then
  begin
    exit;
  end;
  EnqueueNewTrackRecordEvent();
end.