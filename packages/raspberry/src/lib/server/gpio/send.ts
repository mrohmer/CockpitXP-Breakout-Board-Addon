import { Gpio } from "onoff";

let clock: Gpio;
let data1: Gpio;
let data2: Gpio;
let data3: Gpio;
export const send = async (event: number, signalHoldTime: number) => {
  await setup(signalHoldTime);

  const binaryString = event.toString(2).padStart(8, '0');
  const arr = binaryString.split('').map(v => v === '1');
  const parity = arr.reduce(
    (prev, curr) => !curr ? prev : !prev,
  );
  const chunks = [
    [parity, arr[0], arr[1]],
    [arr[2], arr[3], arr[4]],
    [arr[5], arr[6], arr[7]],
  ];

  await setResetChunk();
  await flush(signalHoldTime);

  await setChunk(chunks[0]);
  await flush(signalHoldTime);

  await setChunk(chunks[1]);
  await flush(signalHoldTime);

  await setChunk(chunks[2]);
  await flush(signalHoldTime, true);
}

const setup = async (signalHoldTime: number) => {
  data1 = data1 ?? new Gpio(16, 'out');
  data2 = data2 ?? new Gpio(20, 'out');
  data3 = data3 ?? new Gpio(21, 'out');

  const clockInitialised = !!clock;
  clock = clock ?? new Gpio(12, 'out');

  if (!clockInitialised) {
    await clock.write(0);
    await wait(signalHoldTime);
  }
}

const setResetChunk = () => setChunk([false, false, false]);
const setChunk = async (chunk: boolean[]) => Promise.all([
  data1.write(chunk[0] ? 1 : 0),
  data2.write(chunk[1] ? 1 : 0),
  data3.write(chunk[2] ? 1 : 0),
]);
const wait = (time: number) => new Promise(resolve => setTimeout(
  resolve,
  time,
));
const flush = async (signalHoldTime: number, isLast = false) => {
  await wait(signalHoldTime);
  await clock.write(1);

  await wait(signalHoldTime);
  await clock.write(0);

  if (!isLast) {
    await wait(signalHoldTime);
  }
}
