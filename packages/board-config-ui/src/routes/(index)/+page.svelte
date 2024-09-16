<script lang="ts">
  import type {PageData} from './$types';
  import {invalidate} from '$app/navigation';
  import {api} from '$lib/api';

  export let data: PageData;

  let lane1 = false;
  let lane2 = false;
  let flags = 0;

  const updateDemoState = async (event) => {
    await api.demo.put(event.target.checked);
    await invalidate('index:demo');
  }
  const updatePitlane = (v1: boolean, v2: boolean) => async () => {
    lane1 = v1;
    lane2 = v2;

    const response = await api.pitlanes.put(lane1, lane2);

    lane1 = response.lane1 ?? false;
    lane2 = response.lane2 ?? false;

    await invalidate('index:demo');
  }
  const updateFlags = (value: number) => async () => {
    flags = value;

    const response = await api.flags.put(flags);

    flags = response.state ?? -1;

    await invalidate('index:demo');
  }
</script>

<div class="mx-auto max-w-xl space-y-8">
    <div class="flex items-center">
        <div class="flex-1">
            <div class="text-lg font-bold">Demo-Modus</div>
            <div class="text-sm font-light">Automatischer Demo-Ablauf</div>
        </div>
        <div>
            <input type="checkbox" class="toggle" bind:checked={data.demo.state} on:change={updateDemoState}/>
        </div>
    </div>
    <div class="space-y-8" class:opacity-20={data.demo.state} class:pointer-events-none={data.demo.state}>
        <div class="divider">oder</div>
        <div class="flex items-start gap-y-4 flex-col min-[500px]:flex-row min-[500px]:items-center">
            <div class="flex-1">
                <div class="text-lg font-bold">Pitlanes</div>
                <div class="text-sm font-light">manuellen Wert setzen</div>
            </div>
            <div>
                <div class="join">
                    <button class="btn join-item" class:btn-primary={!lane1 && !lane2} on:click={updatePitlane(false, false)}>Aus</button>
                    <button class="btn join-item" class:btn-primary={lane1 && !lane2} on:click={updatePitlane(true, false)}>Pitlane 1</button>
                    <button class="btn join-item" class:btn-primary={!lane1 && lane2} on:click={updatePitlane(false, true)}>Pitlane 2</button>
                    <button class="btn join-item" class:btn-primary={lane1 && lane2} on:click={updatePitlane(true, true)}>Alle</button>
                </div>
            </div>
        </div>
        <div class="flex items-start gap-y-4 flex-col min-[500px]:flex-row min-[500px]:items-center">
            <div class="flex-1">
                <div class="text-lg font-bold">Flags</div>
                <div class="text-sm font-light">manuellen Wert setzen</div>
            </div>
            <div>
                <div class="join w-full">
                    <button class="btn join-item" class:btn-error={flags === 0} on:click={updateFlags(0)}>Rot</button>
                    <button class="btn join-item" class:btn-success={flags === 1} on:click={updateFlags(1)}>Grün</button>
                    <button class="btn join-item" class:bg-yellow-500={flags === 2} class:text-zinc-800={flags === 2} class:hover:bg-yellow-600={flags === 2} on:click={updateFlags(2)}>Chaos</button>
                </div>
            </div>
        </div>
    </div>
</div>
