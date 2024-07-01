<script lang="ts">
  import type {PageData} from './$types';
  import {demo} from '$lib/api/demo';
  import {invalidate} from '$app/navigation';

  export let data: PageData;

  const update = async (event) => {
    await demo.put(event.target.checked);
    await invalidate('index:demo');
  }
</script>

<div class="mx-auto max-w-xl">
    <div class="flex items-center">
        <div class="flex-1">
            <div class="text-lg font-bold">Demo</div>
            <div class="text-sm font-light">Demo-Modus aktivieren</div>
        </div>
        <div>
            <input type="checkbox" class="toggle" bind:checked={data.demo.active} on:change={update}/>
        </div>
    </div>
</div>
