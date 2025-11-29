<script lang="ts">
    import type {ConnectResult} from "$lib";
    import {slide} from "svelte/transition";

    type Props = {
        characteristics: ConnectResult['characteristics'] | undefined;
        disconnect: Function;
        connecting: boolean;
        disconnecting: boolean;
        isReconnect: boolean;
    }
    let {
        characteristics,
        disconnect,
        connecting,
        disconnecting,
        isReconnect,
    }: Props = $props();
</script>

{#snippet alertMenu()}
    <div class="dropdown dropdown-top dropdown-end">
        <div tabindex="0" role="button" class="btn btn-sm m-1">
            <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5"
                 stroke="currentColor" class="size-6">
                <path stroke-linecap="round" stroke-linejoin="round"
                      d="M12 6.75a.75.75 0 1 1 0-1.5.75.75 0 0 1 0 1.5ZM12 12.75a.75.75 0 1 1 0-1.5.75.75 0 0 1 0 1.5ZM12 18.75a.75.75 0 1 1 0-1.5.75.75 0 0 1 0 1.5Z"/>
            </svg>
        </div>
        <ul tabindex="0" class="dropdown-content menu bg-base-100 text-base-content rounded-box z-[1] w-52 p-2 shadow">
            <li>
                <button onclick={disconnect}>Trennen</button>
            </li>
        </ul>
    </div>
{/snippet}

<div role="alert"
     class="alert transition-colors w-full !grid-flow-col !justify-start !grid-cols-[auto_minmax(auto,_1fr)]"
     class:alert-info={!connecting && !disconnecting}
     transition:slide
>
    {#if connecting || disconnecting}
        <span class="loading loading-spinner loading-md mx-3"></span>
        <div>
            <h3 class="font-bold">
                Verbindung
                wird {connecting ? isReconnect ? 'wieder hergestellt' : 'aufgebaut' : 'getrennt'}
            </h3>
        </div>
    {:else}
        <span class="loading loading-ring loading-md mx-3"></span>
        <div>
            <h3 class="font-bold">Verbunden!</h3>
        </div>

        {@render alertMenu()}
    {/if}
</div>