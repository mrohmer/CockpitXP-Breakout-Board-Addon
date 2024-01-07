{ Pascal Script Engine }
{ Event: StartRealTime }
{ Parameter: Keine }

{%%FUNCTION.BoolToInt%%}

{%%PROCEDURE.InitTable%%}
{%%PROCEDURE.ProcessNextInQueue%%}
{%%PROCEDURE.EnqueueEvent%%}
{%%PROCEDURE.CheckSlotIsFueling%%}

var
  lastCheck: Extended;

begin
  cpSetIntegerVar('Exit', 0);

  InitTable();
  EnqueueResetAllEvent();

  while (True) do
  begin
    if (cpGetIntegerVar('Exit') = 1) then
    begin
      exit;
    end;

    if (cpGetSystemTimeMs - lastCheck >= 2000) then
    begin
      checkSlotIsFueling();

      lastCheck := cpGetSystemTimeMs;
    end;

    ProcessNextInQueue();

    cpSleep(50);
  end;
end.