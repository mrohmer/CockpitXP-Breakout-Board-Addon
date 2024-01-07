{ Pascal Script Engine }
{ Event: StartRealTime }
{ Parameter: Keine }

{%%FUNCTION.BoolToInt%%}
{%%FUNCTION.IsEventInactive%%}

{%%PROCEDURE.InitTable%%}
{%%PROCEDURE.ProcessNextInQueue%%}
{%%PROCEDURE.EnqueueEvent%%}
{%%PROCEDURE.CheckSlotIsFueling%%}
{%%PROCEDURE.CheckSlotNeedsToRefuel%%}
{%%PROCEDURE.CheckVirtualSafetyCar%%}
{%%PROCEDURE.CheckPitlaneStatus%%}

var
  lastCheck: Extended;

begin
  lastCheck := 0;
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
      checkSlotNeedsToRefuel();
      checkVirtualSafetyCar();
      checkPitlaneStatus();

      lastCheck := cpGetSystemTimeMs;
    end;

    ProcessNextInQueue();

    cpSleep(50);
  end;
end.