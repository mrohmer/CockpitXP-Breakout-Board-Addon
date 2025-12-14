<script lang="ts">
    import {type FlagDevice, OnlineState} from "../../models/flag-device";
    import FlagDeviceBattery from "./FlagDeviceBattery.svelte";
    import {slide} from 'svelte/transition';
    import {getRandomFlagDeviceName} from "../../utils/get-random-flag-device-name";
    import {bluetooth, type ConnectResult} from "$lib";
    import {createProgress, createSuccess, dismissAfter, type Notification} from "../../models/notification";
    import {type FlagDeviceName, flagDeviceNames} from "../../stores/flag-device-names";
    import type {ApiFlag} from "$lib/models/api-flag";

    type Props = {
        device: FlagDevice,
        characteristics: ConnectResult['characteristics'],
        onPublishNotification: (notification: Notification) => void,
    }
    let {device, characteristics, onPublishNotification}: Props = $props();
    let isIdentifying = $state(false);
    let isTogglingLights = $state(false);
    let isTogglingDone = $state(false);
    let syncing = false;

    $effect(() => {
        if (!device) {
            return;
        }
        const storedName = $flagDeviceNames?.[device.id];
        if (!storedName) {
            generateNewName();
            return;
        }

        if (typeof storedName === "string" || !storedName.synced) {
            syncFlagNamesWithApi(getFlagDeviceName(storedName)!);
        }
    });

    const generateNewName = async () => {
        const generated = getRandomFlagDeviceName();

        flagDeviceNames.update((state) => ({...state, [device.id]: {name: generated, synced: false}}));

        await syncFlagNamesWithApi(generated);
    }
    const syncFlagNamesWithApi = async (name: string) => {
        if (syncing) {
            return;
        }

        syncing = true;
        try {
            const response = await updateFlagNamesOnApi(name);
            if (!response) {
                return;
            }
            flagDeviceNames.update((state) => ({...state, [device.id]: {name: response.name, synced: true}}));
        } finally {
            syncing = false;
        }
    }
    const updateFlagNamesOnApi = async (name: string): Promise<ApiFlag> => {
        try {
            const response = await fetch(
                `/api/flags/${device.mac}`,
                {
                    method: "POST",
                    body: JSON.stringify({ name }),
                }
            );

            if (response.status !== 200 && response.status !== 409) {
                throw new Error(await response.text());
            }

            return await response.json();
        } catch (e) {
            console.error(e);
            return undefined;
        }
    }

    const getFlagDeviceName = (name: string|FlagDeviceName|undefined): string|undefined => {
        if (!name) {
            return undefined;
        }

        if (typeof name === "string") {
            return name;
        }

        return name?.name;
    }

    const delay = (ms: number) => new Promise(resolve => setTimeout(resolve, ms));
    const identify = () => async () => {
        if (isIdentifying) {
            return;
        }
        isIdentifying = true;
        const id = `identify-${device.id}`;
        const name = $flagDeviceNames[device.id] ?? device.mac;
        onPublishNotification(dismissAfter(createProgress(id, `${name} identifizieren`), 3000));
        await Promise.allSettled([bluetooth.writeCharacteristicWithoutResponse(characteristics.flagDeviceIdentify, device.id), delay(3000)]);
        isIdentifying = false;
    }
    const toggleLight = () => async () => {
        if (isTogglingLights || !device?.id) {
            return;
        }
        isTogglingLights = true;
        const id = `toggle-light-${device.id}`;
        const name = $flagDeviceNames[device.id] ?? device.mac;
        onPublishNotification(createProgress(id, `Licht von ${name} wird umgeschaltet`));
        await Promise.allSettled([bluetooth.writeCharacteristicWithoutResponse(characteristics.flagDeviceToggleLight, device.id), delay(1000)]);
        isTogglingLights = false;
        onPublishNotification(dismissAfter(createSuccess(id, `Licht von ${name} umgeschaltet`), 5000));
    }
</script>

<div class="card bg-base-100 border-1 border-base-300 w-full shadow-lg shadow transition-opacity" transition:slide
     class:opacity-100={device.state !== OnlineState.OFFLINE}
     class:opacity-50={device.state === OnlineState.OFFLINE}
>
    <div class="card-body">
        <div class="flex gap-1">
            <div class="flex-1">
                <h3 class="card-title">{getFlagDeviceName($flagDeviceNames[device.id]) ?? device.mac}</h3>
                <p>
                    {#if isIdentifying}
                        <div class="inline-grid *:[grid-area:1/1]">
                            <div class="status status-info animate-ping"></div>
                            <div class="status status-info"></div>
                        </div>
                        Identifiziert
                    {:else if device.state === OnlineState.ONLINE}
                        <div class="inline-grid *:[grid-area:1/1]">
                            <div class="status status-success animate-ping"></div>
                            <div class="status status-success"></div>
                        </div>
                        Online
                    {:else if device.state === OnlineState.OFFLINE}
                        <div aria-label="error" class="status status-error"></div>
                        Offline
                    {:else if device.state === OnlineState.HEARTBEAT_MISSED}
                        <div aria-label="warning" class="status status-warning"></div>
                        Heartbeat verpasst
                    {:else}
                        <span class="status"></span> Unbekannt
                    {/if}
                </p>
            </div>

            <div class="card-actions justify-end items-center gap-2">
                <FlagDeviceBattery state={device.battery}/>
                {#if device.state !== OnlineState.OFFLINE}
                    <div class="dropdown dropdown-bottom dropdown-end">
                        <div tabindex="0" role="button" class="btn btn-sm m-1">
                            <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5"
                                 stroke="currentColor" class="size-6">
                                <path stroke-linecap="round" stroke-linejoin="round"
                                      d="M12 6.75a.75.75 0 1 1 0-1.5.75.75 0 0 1 0 1.5ZM12 12.75a.75.75 0 1 1 0-1.5.75.75 0 0 1 0 1.5ZM12 18.75a.75.75 0 1 1 0-1.5.75.75 0 0 1 0 1.5Z"/>
                            </svg>
                        </div>
                        <ul tabindex="0"
                            class="dropdown-content menu bg-base-100 text-base-content rounded-box z-[1] w-52 p-2 shadow">
                            <li>
                                <button onclick={toggleLight()}>Licht umschalten</button>
                            </li>
                            <li>
                                <button onclick={identify()}>Identifizieren</button>
                            </li>
                        </ul>
                    </div>
                {/if}
            </div>
        </div>
    </div>
</div>