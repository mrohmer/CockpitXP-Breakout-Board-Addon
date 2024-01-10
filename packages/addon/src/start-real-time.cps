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
  sleepTime, timeBetweenPolls, signalHoldTime, eventTransferTime, timePerCycle: Integer;
begin
  lastCheck := 0;
  cpSetIntegerVar('Exit', 0);

  sleepTime := cpGetIntegerVar('TimingSleep');
  signalHoldTime := cpGetIntegerVar('TimingSignalSwitch');
  eventTransferTime := signalHoldTime * 6;
  timePerCycle := eventTransferTime + sleepTime;

  timeBetweenPolls := cpGetIntegerVar('TimingEventPollling');
  if (timeBetweenPolls < timePerCycle * 2) then
  begin
    // should definelty never check that events more then every second cycle. this is already a lot.
    timeBetweenPolls := timePerCycle * 2;
  end;

  InitTable();
  EnqueueResetAllEvent();

  while (True) do
  begin
    if (cpGetIntegerVar('Exit') = 1) then
    begin
      exit;
    end;

    if (cpGetSystemTimeMs - lastCheck >= timeBetweenPolls) then
    begin
      checkSlotIsFueling();
      checkSlotNeedsToRefuel();
      checkVirtualSafetyCar();
      checkPitlaneStatus();

      lastCheck := cpGetSystemTimeMs;
    end;

    ProcessNextInQueue(signalHoldTime);

    cpSleep(sleepTime);
  end;
end.