<script lang="ts">
    import {parseFlagDeviceStr} from "../../utils/parse-flag-device-str";
    import FlagDevice from "./FlagDevice.svelte";
    import type {ConnectResult} from "$lib";

    type Props = {
        deviceStr: string;
        characteristics: ConnectResult['characteristics']
    }
    let {deviceStr, characteristics}: Props = $props();

    let devices = $derived(parseFlagDeviceStr(deviceStr).sort((a, b) => a.id.localeCompare(b.id)));
</script>

<div class="space-y-2">
    {#each devices as device (device.id)}
        <FlagDevice {device} {characteristics} />
    {:else}
        <div class="opacity-70 text-xl text-center">
            Keine Flaggen verbunden
        </div>
    {/each}
</div>