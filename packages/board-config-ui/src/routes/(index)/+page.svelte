<script lang="ts">
  import type {PageData} from './$types';
  import {invalidate} from '$app/navigation';
  import {api} from '$lib/api';

  export let data: PageData;

  const updateDemoState = async (event) => {
    await api.demo.put(event.target.checked);
    await invalidate('index:data');
  }
  const updatePitlane = (v1: boolean, v2: boolean) => async () => {
    data.pitlanes = data.pitlanes ?? {};
    data.pitlanes.lane1 = v1;
    data.pitlanes.lane2 = v2;

    await api.pitlanes.put(v1, v2);

    await invalidate('index:data');
  }
  const updateFlags = (value: number) => async () => {
    data.flags = value;

    await api.flags.put(value);

    await invalidate('index:data');
  }
</script>

<div class="mx-auto max-w-xl space-y-8">
    <div class="flex items-center">
        <div class="flex-1">
            <div class="text-lg font-bold">Demo-Modus</div>
            <div class="text-sm font-light">Automatischer Demo-Ablauf</div>
        </div>
        <div>
            <input type="checkbox" class="toggle" bind:checked={data.demo} on:change={updateDemoState}/>
        </div>
    </div>
    <div class="space-y-8" class:opacity-20={data.demo} class:pointer-events-none={data.demo}>
        <div class="divider">oder</div>
        <div class="flex items-start gap-y-4 flex-col min-[500px]:flex-row min-[500px]:items-center">
            <div class="flex-1">
                <div class="text-lg font-bold">Pitlanes</div>
                <div class="text-sm font-light">manuellen Wert setzen</div>
            </div>
            <div>
                <div class="join">
                    <button class="btn join-item" class:btn-primary={!data.pitlanes?.lane1 && !data.pitlanes?.lane2} on:click={updatePitlane(false, false)}>Aus</button>
                    <button class="btn join-item" class:btn-primary={data.pitlanes?.lane1 && !data.pitlanes?.lane2} on:click={updatePitlane(true, false)}>Pitlane 1</button>
                    <button class="btn join-item" class:btn-primary={!data.pitlanes?.lane1 && data.pitlanes?.lane2} on:click={updatePitlane(false, true)}>Pitlane 2</button>
                    <button class="btn join-item" class:btn-primary={data.pitlanes?.lane1 && data.pitlanes?.lane2} on:click={updatePitlane(true, true)}>Alle</button>
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
                    <button class="btn join-item" class:btn-error={data.flags === 0} on:click={updateFlags(0)}>Rot</button>
                    <button class="btn join-item" class:btn-success={data.flags === 1} on:click={updateFlags(1)}>Grün</button>
                    <button class="btn join-item" class:bg-yellow-500={data.flags === 2} class:text-zinc-800={data.flags === 2} class:hover:bg-yellow-600={data.flags === 2} on:click={updateFlags(2)}>Chaos</button>
                </div>
            </div>
        </div>
    </div>
</div>
