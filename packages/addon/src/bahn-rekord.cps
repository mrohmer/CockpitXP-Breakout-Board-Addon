{ Pascal Script Engine }
{ Event: SchnellsteRunde }
{ Parameter: Cockpit.Slot=<SlotPlatz> }

{%%PROCEDURE.EnqueueEvent%%}
{%%FUNCTION.IsEventInactive%%}
begin
  if (isEventInactive('TrackRecord')) then
  begin
    exit;
  end;
  EnqueueNewTrackRecordEvent();
end.