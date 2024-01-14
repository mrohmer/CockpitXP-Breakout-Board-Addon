<script lang="ts">
  import type {PageData} from './$types';
  import {superForm} from 'sveltekit-superforms/client';
  import {Category} from '$lib/models/category';
  import {SlotEvent} from '$lib/models/slot-event';
  import { slide } from 'svelte/transition';

  export let data: PageData;

  // Client API:
  const {form, constraints, enhance, submitting, timeout, message} = superForm(data.form, {
    dataType: 'json',
    timeoutMs: 1000,
  });

  const onIncPitlane = () => () => {
    form.update((value) => {
      return {
        ...value,
        event: {
          ...(value?.event ?? {}),
          value: Math.min((value?.event?.value ?? 0) + 1, 14),
        }
      }
    })
  }
  const onDecPitlane = () => () => {
    form.update((value) => {
      return {
        ...value,
        event: {
          ...(value?.event ?? {}),
          value: Math.max((value?.event?.value ?? 0) - 1, -1),
        }
      }
    })
  }
</script>

<div class="max-w-2xl mx-auto px-2 py-10">
    <form method="POST" class="flex flex-col gap-9" use:enhance>
        {#if $message}
            <div role="alert" class="alert" transition:slide>
                <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" class="stroke-info shrink-0 w-6 h-6"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M13 16h-1v-4h-1m1-4h.01M21 12a9 9 0 11-18 0 9 9 0 0118 0z"></path></svg>
                <span>{$message}</span>
            </div>
        {/if}
        <div class="flex flex-col gap-3">
            <div class="flex">
                <label class="flex-1" for="signal-hold-time">Signal Hold Time</label>
                <input type="number" id="signal-hold-time" placeholder="10"
                       class="input input-bordered w-44 text-center pr-0"
                       bind:value={$form.signalHoldTime}
                       {...$constraints.signalHoldTime}
                />
            </div>
            <div class="divider"></div>


            <div class="flex">
                <label class="flex-1" for="event-type">Type</label>
                <select class="select select-bordered w-44 text-center" id="event-type"
                        bind:value={$form.event.category}
                        {...$constraints?.event?.category}
                >
                    <option disabled selected value="">Type</option>
                    {#each Object.values(Category) as category}
                        <option>{category}</option>
                    {/each}
                </select>
            </div>

            {#if $form.event.category === Category.Slot}
                <div class="flex">
                    <label class="flex-1" for="event-slot-event">Event</label>
                    <select class="select select-bordered w-44 text-center" id="event-slot-event"
                            bind:value={$form.event.event}
                            {...$constraints?.event?.event}
                    >
                        {#each Object.values(SlotEvent) as event}
                            <option>{event}</option>
                        {/each}
                    </select>
                </div>
                <div class="flex">
                    <label class="flex-1" for="event-slot">Slot</label>
                    <select class="select select-bordered w-44 text-center" id="event-slot-slot"
                            bind:value={$form.event.slot}
                            {...$constraints?.event?.slot}
                    >
                        {#each Array.from(Array(6)) as _, index}
                            <option>{index + 1}</option>
                        {/each}
                    </select>
                </div>
                <div class="flex">
                    <label class="flex-1" for="event-slot-state">State</label>
                    <select class="select select-bordered w-44 text-center" id="event-slot-state"
                            bind:value={$form.event.state}
                            {...$constraints?.event?.state}
                    >
                        {#each Array.from(Array(2)) as _, index}
                            <option>{index === 1 ? 'on' : 'off'}</option>
                        {/each}
                    </select>
                </div>
            {:else if $form.event.category === Category.VirtualSafetyCar}
                <div class="flex">
                    <label class="flex-1" for="event-vsc-state">State</label>
                    <select class="select select-bordered w-44 text-center" id="event-vsc-state"
                            bind:value={$form.event.state}
                            {...$constraints?.event?.state}
                    >
                        {#each Array.from(Array(2)) as _, index}
                            <option>{index === 1 ? 'on' : 'off'}</option>
                        {/each}
                    </select>
                </div>
            {:else if $form.event.category === Category.RaceState}
                <div class="flex">
                    <label class="flex-1" for="event-race-state">State</label>
                    <select class="select select-bordered w-44 text-center" id="event-race-state"
                            bind:value={$form.event.state}
                            {...$constraints?.event?.state}
                    >
                        {#each Array.from(Array(2)) as _, index}
                            <option>{index === 1 ? 'on' : 'off'}</option>
                        {/each}
                    </select>
                </div>
            {:else if $form.event.category === Category.Pitlane}
                <div class="flex">
                    <label class="flex-1" for="event-pitlane-lane">Lane</label>
                    <select class="select select-bordered w-44 text-center" id="event-pitlane-lane"
                            bind:value={$form.event.lane}
                            {...$constraints?.event?.lane}
                    >
                        {#each Array.from(Array(2)) as _, index}
                            <option>{index + 1}</option>
                        {/each}
                    </select>
                </div>
                <div class="flex">
                    <label class="flex-1" for="event-pitlane-value">Value</label>

                    <div class="join">
                        <div>
                            <button class="btn btn-square join-item z-10" on:click={onDecPitlane()} type="button">
                                <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor" class="w-6 h-6">
                                    <path stroke-linecap="round" stroke-linejoin="round" d="M5 12h14" />
                                </svg>

                            </button>
                        </div>
                        <select class="select select-bordered w-20 text-center join-item z-0" id="event-pitlane-value"
                                bind:value={$form.event.value}
                                {...$constraints?.event?.value}
                        >
                            {#each Array.from(Array(16)) as _, index}
                                {#if index === 0}
                                    <option value={-1}>off</option>
                                {:else}
                                    <option>{index - 1}</option>
                                {/if}
                            {/each}
                        </select>
                        <div>
                            <button class="btn btn-square join-item z-10" on:click={onIncPitlane()} type="button">
                                <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor" class="w-6 h-6">
                                    <path stroke-linecap="round" stroke-linejoin="round" d="M12 4.5v15m7.5-7.5h-15" />
                                </svg>
                            </button>
                        </div>
                    </div>

                </div>
            {:else if $form.event.category === Category.Record}
                <div class="flex">
                    <label class="flex-1" for="event-record-value">Type</label>
                    <select class="select select-bordered w-44 text-center" id="event-record-value"
                            bind:value={$form.event.value}
                            {...$constraints?.event?.value}
                    >
                        <option value="track">Track Record</option>
                        <option value="session">Session Record</option>
                    </select>
                </div>
            {:else if $form.event.category === Category.StartLight}
                <div class="flex">
                    <label class="flex-1" for="event-sl-value">Type</label>
                    <select class="select select-bordered w-44 text-center" id="event-sl-value"
                            bind:value={$form.event.value}
                            {...$constraints?.event?.value}
                    >
                        {#each Array.from(Array(8)) as _, index}
                            {#if index === 0}
                                <option value={0}>off</option>
                            {:else if index === 6}
                                <option value={6}>green</option>
                            {:else if index === 7}
                                <option value={7}>false start</option>
                            {:else}
                                <option>{index}</option>
                            {/if}
                        {/each}
                    </select>
                </div>
            {/if}
        </div>

        <div>
            <button class="btn w-full" type="submit" disabled={!$form.event.category}>
                {#if $submitting || $timeout}
                    <div class="flex flex-col justify-center items-center h-10">
                        <div class="loading loading-dots loading-xs"></div>
                    </div>
                {:else}
                    Submit
                {/if}
            </button>
        </div>
    </form>
</div>
