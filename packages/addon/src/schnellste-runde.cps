{ Pascal Script Engine }
{ Event: SchnellsteRunde }
{ Parameter: Cockpit.Slot=<SlotPlatz> }

{%%PROCEDURE.EnqueueEvent%%}
{%%FUNCTION.IsEventActive%%}
begin
  if (not isEventActive('SessionRecord')) then
  begin
    exit;
  end;
  EnqueueNewSessionRecordEvent();
end.