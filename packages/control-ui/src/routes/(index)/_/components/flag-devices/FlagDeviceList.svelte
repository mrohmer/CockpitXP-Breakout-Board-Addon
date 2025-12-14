<script lang="ts">
    import {parseFlagDeviceStr} from "../../utils/parse-flag-device-str";
    import FlagDevice from "./FlagDevice.svelte";
    import {bluetooth, type ConnectResult} from "$lib";
    import {createProgress, createSuccess, dismissAfter, type Notification} from "../../models/notification";
    import {OnlineState} from "../../models/flag-device";

    type Props = {
        deviceStr: string;
        characteristics: ConnectResult['characteristics'],
        onPublishNotification: (notification: Notification) => void,
    }
    let {deviceStr, characteristics, onPublishNotification}: Props = $props();

    let devices = $derived(parseFlagDeviceStr(deviceStr).sort((a, b) => a.id.localeCompare(b.id)));
    let onlineDevices = $derived(devices.filter(d => d.state !== OnlineState.OFFLINE));
    let isTogglingAllLights = $state(false);

    const delay = (ms: number) => new Promise(resolve => setTimeout(resolve, ms));
    const execToggleAllLights = async () => {
        for (const device of onlineDevices) {
            try {
                await bluetooth.writeCharacteristicWithoutResponse(characteristics.flagDeviceToggleLight, device.id);
                await delay(100);
            } catch (e) {
                console.error('Could not toggle light light', e);
            }
        }
    }
    const toggleAllLights = () => async () => {
        if (isTogglingAllLights || !onlineDevices?.length) {
            return;
        }
        const id = 'toggle-all-lights';
        onPublishNotification(createProgress(id, 'Lichter werden umgeschaltet'));
        isTogglingAllLights = true;
        await Promise.allSettled([execToggleAllLights(), delay(1000)]);
        isTogglingAllLights = false;
        onPublishNotification(dismissAfter(createSuccess(id, 'Alle Lichter umgeschaltet'), 5000));
    }
</script>

<div class="flex gap-1">
    <h2 class="text-2xl flex-1">
        Flaggen
    </h2>
    {#if onlineDevices?.length > 1}
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
                    <button onclick={toggleAllLights()}>Alle Lichter umschalten</button>
                </li>
            </ul>
        </div>
    {/if}
</div>
<div class="space-y-2">
    {#each devices as device (device.id)}
        <FlagDevice {device} {characteristics} {onPublishNotification} />
    {:else}
        <div class="opacity-70 text-xl text-center">
            Keine Flaggen verbunden
        </div>
    {/each}
</div>