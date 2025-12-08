<script lang="ts">
    import {type FlagDevice, OnlineState} from "../../models/flag-device";
    import FlagDeviceBattery from "./FlagDeviceBattery.svelte";
    import {slide} from 'svelte/transition';

    type Props = {
        device: FlagDevice
    }
    let {device}: Props = $props();

</script>

<div class="card bg-base-100 border-1 border-base-300 w-full shadow-lg shadow transition-opacity" transition:slide
     class:opacity-100={device.state !== OnlineState.OFFLINE}
     class:opacity-50={device.state === OnlineState.OFFLINE}
>
    <div class="card-body">
        <div class="flex gap-1">
            <div class="flex-1">
                <h3 class="card-title">{device.mac}</h3>
                <p>
                    {#if device.state === OnlineState.ONLINE}
                        <div class="inline-grid *:[grid-area:1/1]">
                            <div class="status status-success animate-ping"></div>
                            <div class="status status-success"></div>
                        </div> Online
                    {:else if device.state === OnlineState.OFFLINE}
                        <div aria-label="error" class="status status-error"></div> Offline
                    {:else if device.state === OnlineState.HEARTBEAT_MISSED}
                        <div aria-label="warning" class="status status-warning"></div> Heartbeat verpasst
                    {:else}
                        <span class="status"></span> Unbekannt
                    {/if}
                </p>
            </div>

            <div class="card-actions justify-end items-center gap-2">
                <FlagDeviceBattery state={device.battery} />
                {#if device.state !== OnlineState.OFFLINE}
                    <div class="dropdown dropdown-bottom dropdown-end">
                        <div tabindex="0" role="button" class="btn btn-sm m-1">
                            <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5"
                                 stroke="currentColor" class="size-6">
                                <path stroke-linecap="round" stroke-linejoin="round"
                                      d="M12 6.75a.75.75 0 1 1 0-1.5.75.75 0 0 1 0 1.5ZM12 12.75a.75.75 0 1 1 0-1.5.75.75 0 0 1 0 1.5ZM12 18.75a.75.75 0 1 1 0-1.5.75.75 0 0 1 0 1.5Z"/>
                            </svg>
                        </div>
                        <ul tabindex="0" class="dropdown-content menu bg-base-100 text-base-content rounded-box z-[1] w-52 p-2 shadow">
                            <li>
                                <button>Identifizieren</button>
                            </li>
                        </ul>
                    </div>
                {/if}
            </div>
        </div>
    </div>
</div>
{#snippet alertMenu()}
{/snippet}