<script lang="ts">
    import {parseFlagDeviceStr} from "../../utils/parse-flag-device-str";
    import FlagDevice from "./FlagDevice.svelte";

    type Props = {
        deviceStr: string;
    }
    let {deviceStr}: Props = $props();

    let devices = $derived(parseFlagDeviceStr(deviceStr).sort((a, b) => a.id.localeCompare(b.id)));
</script>

<div class="space-y-2">
    {#each devices as device (device.id)}
        <FlagDevice {device} />
    {:else}
        <div class="opacity-70 text-xl text-center">
            Keine Flaggen verbunden
        </div>
    {/each}
</div>