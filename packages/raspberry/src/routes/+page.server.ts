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
    case Category.Slot:
      return transformSlotValueToEvent(event);
    case Category.Pitlane:
      return transformPitlaneValueToEvent(event);
    case Category.StartLight:
      return transformStartLightValueToEvent(event);
  }
}
const transformSlotValueToEvent = (event: z.infer<typeof slotSchema>): number => {
  const map: Record<`${SlotEvent}.${number}`, number> = {
    [`${SlotEvent.NeedsRefueling}.1`]: 145,
    [`${SlotEvent.NeedsRefueling}.2`]: 153,
    [`${SlotEvent.NeedsRefueling}.3`]: 161,
    [`${SlotEvent.NeedsRefueling}.4`]: 169,
    [`${SlotEvent.NeedsRefueling}.5`]: 177,
    [`${SlotEvent.NeedsRefueling}.6`]: 185,
    [`${SlotEvent.IsRefueling}.1`]: 147,
    [`${SlotEvent.IsRefueling}.2`]: 155,
    [`${SlotEvent.IsRefueling}.3`]: 163,
    [`${SlotEvent.IsRefueling}.4`]: 171,
    [`${SlotEvent.IsRefueling}.5`]: 179,
    [`${SlotEvent.IsRefueling}.6`]: 188,
  };

  const value = map[`${event.event}.${event.slot}`];

  return value + +(event.state === 'off');
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
