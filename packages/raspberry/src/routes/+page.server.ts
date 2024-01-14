import {z} from 'zod';
import {message, superValidate} from 'sveltekit-superforms/server';
import {fail} from '@sveltejs/kit';
import {Category} from '$lib/models/category';
import {SlotEvent} from '$lib/models/slot-event';
import {send} from '$lib/server/gpio';

const resetSchema = z.object({
  category: z.enum([Category.Reset]),
});
const slotSchema = z.object({
  category: z.enum([Category.Slot]),
  event: z.nativeEnum(SlotEvent),
  slot: z.number().min(0).max(6),
  state: z.enum(['on', 'off']),
});
const virtualSafetyCarSchema = z.object({
  category: z.enum([Category.VirtualSafetyCar]),
  state: z.enum(['on', 'off']),
});
const pitlaneSchema = z.object({
  category: z.enum([Category.Pitlane]),
  lane: z.number().min(1).max(2),
  value: z.number().min(-1).max(14),
});
const startLightSchema = z.object({
  category: z.enum([Category.StartLight]),
  value: z.number().min(0).max(7),
});
const recordSchema = z.object({
  category: z.enum([Category.Record]),
  value: z.enum(['track', 'session']),
});
const raceStateSchema = z.object({
  category: z.enum([Category.RaceState]),
  state: z.enum(['on', 'off'])
});
const raceProgressSchema = z.object({
  category: z.enum([Category.RaceProgress]),
  value: z.number().min(0).max(12),
});
// Name has a default value just to display something in the form.
const schema = z.object({
  signalHoldTime: z.number().min(1).max(1000),
  event: z.union([
    resetSchema,
    slotSchema,
    virtualSafetyCarSchema,
    pitlaneSchema,
    startLightSchema,
    recordSchema,
    raceStateSchema,
    raceProgressSchema,
  ])
});

export const load = (async () => {
  // Server API:
  const form = await superValidate({
    event: {} as any,
    signalHoldTime: 25,
  }, schema);

  // Unless you throw, always return { form } in load and form actions.
  return {form};
});
export const actions = {
  default: async ({request}) => {
    const form = await superValidate(request, schema);

    if (!form.valid) {
      return fail(400, {form});
    }

    const event = transformFormValueToEvent(form.data.event);

    console.log(event, form.data.event);

    await send(event, form.data.signalHoldTime);

    return message(form, `Event ${event} successfully sent`);
  }
};

const transformFormValueToEvent = (event: z.infer<typeof schema>['event']): number => {
  switch (event.category) {
    case Category.Reset:
      return 11;
    case Category.Record:
      if (event.value === 'track') {
        return 49;
      }
      return 50;
    case Category.VirtualSafetyCar:
      if (event.state === 'on') {
        return 21;
      }
      return 22;
    case Category.RaceState:
      if (event.state === 'on') {
        return 110;
      }
      return 111;
    case Category.RaceProgress:
      return transformRaceProgressValueToEvent(event);
    case Category.Slot:
      return transformSlotValueToEvent(event);
    case Category.Pitlane:
      return transformPitlaneValueToEvent(event);
    case Category.StartLight:
      return transformStartLightValueToEvent(event);
  }
}
const transformSlotValueToEvent = (event: z.infer<typeof slotSchema>): number => {
  const map: Record<`${SlotEvent}.${number}.${'on'|'off'}`, number> = {
    [`${SlotEvent.NeedsRefueling}.1.on`]: 137,
    [`${SlotEvent.NeedsRefueling}.1.off`]: 138,
    [`${SlotEvent.NeedsRefueling}.2.on`]: 139,
    [`${SlotEvent.NeedsRefueling}.2.off`]: 140,
    [`${SlotEvent.NeedsRefueling}.3.on`]: 141,
    [`${SlotEvent.NeedsRefueling}.3.off`]: 142,
    [`${SlotEvent.NeedsRefueling}.4.on`]: 143,
    [`${SlotEvent.NeedsRefueling}.4.off`]: 145,
    [`${SlotEvent.NeedsRefueling}.5.on`]: 146,
    [`${SlotEvent.NeedsRefueling}.5.off`]: 147,
    [`${SlotEvent.NeedsRefueling}.6.on`]: 148,
    [`${SlotEvent.NeedsRefueling}.6.off`]: 149,
    [`${SlotEvent.IsRefueling}.1.on`]: 161,
    [`${SlotEvent.IsRefueling}.1.off`]: 162,
    [`${SlotEvent.IsRefueling}.2.on`]: 163,
    [`${SlotEvent.IsRefueling}.2.off`]: 164,
    [`${SlotEvent.IsRefueling}.3.on`]: 165,
    [`${SlotEvent.IsRefueling}.3.off`]: 166,
    [`${SlotEvent.IsRefueling}.4.on`]: 167,
    [`${SlotEvent.IsRefueling}.4.off`]: 169,
    [`${SlotEvent.IsRefueling}.5.on`]: 170,
    [`${SlotEvent.IsRefueling}.5.off`]: 171,
    [`${SlotEvent.IsRefueling}.6.on`]: 172,
    [`${SlotEvent.IsRefueling}.6.off`]: 173,
  };

  return map[`${event.event}.${event.slot}.${event.state}`];
}
const transformPitlaneValueToEvent = (event: z.infer<typeof pitlaneSchema>): number => {
  const map: Record<`${number}.${'-'|''}${number}`, number> = {
    [`1.-1`]: 55,
    [`1.0`]: 59,
    [`1.1`]: 61,
    [`1.2`]: 62,
    [`1.3`]: 73,
    [`1.4`]: 74,
    [`1.5`]: 75,
    [`1.6`]: 76,
    [`1.7`]: 77,
    [`1.8`]: 78,
    [`1.9`]: 79,
    [`1.10`]: 81,
    [`1.11`]: 82,
    [`1.12`]: 83,
    [`1.13`]: 84,
    [`1.14`]: 85,
    [`2.-1`]: 89,
    [`2.0`]: 90,
    [`2.1`]: 91,
    [`2.2`]: 92,
    [`2.3`]: 93,
    [`2.4`]: 94,
    [`2.5`]: 95,
    [`2.6`]: 97,
    [`2.7`]: 98,
    [`2.8`]: 99,
    [`2.9`]: 100,
    [`2.10`]: 101,
    [`2.11`]: 102,
    [`2.12`]: 103,
    [`2.13`]: 105,
    [`2.14`]: 106,
  }
  return map[`${event.lane}.${event.value}`];
}
const transformStartLightValueToEvent = (event: z.infer<typeof startLightSchema>): number => {
  const map: Record<`${number}`, number> = {
    '0': 28,
    '1': 31,
    '2': 35,
    '3': 37,
    '4': 38,
    '5': 41,
    '6': 44,
    '7': 42,
  }

  return map[`${event.value}`];
}
const transformRaceProgressValueToEvent = (event: z.infer<typeof raceProgressSchema>): number => {
  const map: Record<`${number}`, number> = {
    '0': 113,
    '1': 114,
    '2': 115,
    '3': 116,
    '4': 116,
    '5': 118,
    '6': 119,
    '7': 121,
    '8': 122,
    '9': 123,
    '10': 124,
    '11': 125,
    '12': 126,
  }

  return map[`${event.value}`];
}
