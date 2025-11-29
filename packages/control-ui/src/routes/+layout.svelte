<script lang="ts">
	import './layout.css';
	import favicon from '$lib/assets/favicon.svg';
    import {browser} from "$app/environment";
	
	let { children } = $props();
</script>

<svelte:head>
	<link rel="icon" href={favicon} />
    <title>CMA Center Control</title>
</svelte:head>

<div class="w-full max-w-lg mx-auto px-4 py-4 gap-y-4 min-h-[95vh] !min-h-[95svh] flex flex-col">

    <div class="flex gap-2 items-center justify-center mb-5 p-3 text-stone-400">
        <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width=".85"
             stroke="currentColor" class="size-10">
            <path stroke-linecap="round" stroke-linejoin="round"
                  d="M6.429 9.75 2.25 12l4.179 2.25m0-4.5 5.571 3 5.571-3m-11.142 0L2.25 7.5 12 2.25l9.75 5.25-4.179 2.25m0 0L21.75 12l-4.179 2.25m0 0 4.179 2.25L12 21.75 2.25 16.5l4.179-2.25m11.142 0-5.571 3-5.571-3"/>
        </svg>
        <h1 class="text-4xl font-light">CMA Center</h1>
    </div>

    {#if browser}
        {@render children()}
    {:else}
        <div class="fixed inset-0 flex justify-center items-center">
            <div>loading...</div>
        </div>
    {/if}
</div>