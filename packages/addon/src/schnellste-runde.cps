{ Pascal Script Engine }
{ Event: SchnellsteRunde }
{ Parameter: Cockpit.Slot=<SlotPlatz> }

{%%PROCEDURE.EnqueueEvent%%}
{%%FUNCTION.IsEventInactive%%}
begin
  if (isEventInactive('SessionRecord')) then
  begin
    exit;
  end;
  EnqueueNewSessionRecordEvent();
end.