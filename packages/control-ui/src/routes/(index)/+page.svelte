<script lang="ts">
    import {bluetooth, type ConnectResult} from "$lib";
    import {onMount} from "svelte";
    import ConnectionAlert from "./_/components/ConnectionAlert.svelte";
    import {createProgress, createSuccess, type Notification} from "./_/models/notification";
    import Snackbars from "./_/components/Snackbars.svelte";
    import FloatingBottomBar from "$lib/components/FloatingBottomBar.svelte";
    import Characteristic from "$lib/components/Characteristic.svelte";
    import FlagDeviceList from "./_/components/flag-devices/FlagDeviceList.svelte";
    import type {FlagDevice} from "./_/models/flag-device";

    let connecting = $state(false);
    let isReconnect = $state(false);
    let disconnecting = $state(false);
    let device = $state<ConnectResult['device'] | undefined>(undefined);
    let server = $state<ConnectResult['server'] | undefined>(undefined);
    let service = $state<ConnectResult['service'] | undefined>(undefined);
    let characteristics = $state<ConnectResult['characteristics'] | undefined>(undefined);

    let notifications: Record<string, Notification> = $state({});

    const listenToDisconnect = () => {
        if (!device) {
            return;
        }
        device.ongattserverdisconnected = () => {
            device = undefined;
            server = undefined;
            service = undefined;
            characteristics = undefined;
        }
    }
    const tryReconnect = async (deviceId: string) => {
        if (!deviceId) {
            return;
        }

        try {
            connecting = true;
            const result = await bluetooth.tryReconnect(deviceId, 5000);


            device = result?.device;
            server = result?.server;
            service = result?.service;
            characteristics = result?.characteristics;

            listenToDisconnect();
        } finally {
            connecting = false;
        }
    }
    const disconnect = () => async () => {
        if (!device) {
            return;
        }

        try {
            disconnecting = true;

            await bluetooth.disconnect(device);

            device = undefined;
            server = undefined;
            service = undefined;
            characteristics = undefined;
        } finally {
            disconnecting = false;
        }
    }
    const connect = () => async () => {
        if (device) {
            return;
        }

        try {
            connecting = true;
            isReconnect = false;

            const result = await bluetooth.connect();

            device = result?.device;
            server = result?.server;
            service = result?.service;
            characteristics = result?.characteristics;

            listenToDisconnect();
        } finally {
            connecting = false;
        }
    }

    const setNotification = (notification: Notification) => {
        notifications[notification.id] = {...notification};
    }
    const unsetNotification = (id: string) => {
        notifications = Object.fromEntries(
            Object.entries(notifications)
                .filter(([key]) => key !== id)
        )
    }

    const onDismissNotification = () => unsetNotification

    onMount(() => {
        const deviceId = localStorage.getItem("ble:deviceId");
        if (deviceId) {
            isReconnect = true;
            tryReconnect(deviceId);
        }
    });

    const changeEnabled = async (enabled: boolean) => {
        await bluetooth.writeCharacteristicWithoutResponse(
            characteristics.controlEnabled,
            enabled ? "true" : "false"
        );
    }
    const changeValue = async (value: string) => {
        await bluetooth.writeCharacteristicWithoutResponse(
            characteristics.controlValue,
            value
        );
    }
    const changeBrightness = async (event) => {
        await bluetooth.writeCharacteristicWithoutResponse(
            characteristics.flagBrightness,
            (event.target as HTMLInputElement).value.toString()
        );
    }
    const changeChaosStyle = async (event) => {
        await bluetooth.writeCharacteristicWithoutResponse(
            characteristics.chaosStyle,
            (event.target as HTMLInputElement).value.toString()
        );
    }
</script>

{#if device && characteristics?.controlEnabled && characteristics?.controlValue}
    <div class="flex flex-col gap-16">
        <div class="space-y-20">
            <div class="space-y-3">
                <Characteristic characteristic={characteristics?.flagDevices}>
                    {#snippet content(value)}
                        <FlagDeviceList deviceStr={value} {characteristics} onPublishNotification={setNotification}/>
                    {/snippet}
                </Characteristic>
            </div>
        </div>
        <div class="space-y-20">
            <div class="space-y-3">
                <h2 class="text-2xl">
                    Konfiguration
                </h2>
                <Characteristic characteristic={characteristics?.flagBrightness}>
                    {#snippet content(value)}
                        <div class="flex gap-2 items-center">
                            <label for="brightness" class="block w-40">
                                Helligkeit
                            </label>
                            <div class="flex-1 transition-opacity" class:opacity-20={!value && value !== 0} class:pointer-events-none={!value && value !== 0}>
                                <input id="brightness" type="range" min="0" max="255" value={value ?? 0} class="range" aria-label="Helligkeit" onchange={changeBrightness} />
                            </div>
                        </div>
                    {/snippet}
                </Characteristic>
                <Characteristic characteristic={characteristics?.chaosStyle}>
                    {#snippet content(value)}
                        {#if value} <!-- as it may not be exposed by the esp yet -->
                            <div class="flex gap-2 items-center">
                                <label class="block w-40" for="chaos_style">
                                    Chaos Darstellung
                                </label>
                                <div class="flex-1 transition-opacity" class:opacity-20={!value} class:pointer-events-none={!value} onchange={changeChaosStyle}>
                                    <select id="chaos_style" class="select" {value}>
                                        <option value="CHECKERED">Flagge</option>
                                        <option value="DIAGONAL">Fixe Diagonale</option>
                                    </select>
                                </div>
                            </div>
                        {/if}
                    {/snippet}
                </Characteristic>
            </div>
        </div>
        <div class="space-y-20">
            <div class="space-y-3">
                <Characteristic characteristic={characteristics?.controlEnabled}>
                    {#snippet content(enabled)}
                        <div class="flex gap-1">
                            <h2 class="text-2xl flex-1">
                                Manuelle Steuerung
                            </h2>
                            <div>
                                <div>
                                    <label class="label">
                                        <input type="checkbox" checked={enabled === "true"} class="toggle" onchange={e => changeEnabled(e.target.checked)} />
                                    </label>
                                </div>
                            </div>
                        </div>
                        <Characteristic characteristic={characteristics?.controlValue}>
                            {#snippet content(value)}
                                <div class="join">
                                    <input class="join-item btn" type="radio" name="control_value_options" checked={value === "RED" || !value} value="RED" aria-label="RED" onchange={e => e.target.checked && changeValue(e.target.value)} />
                                    <input class="join-item btn" type="radio" name="control_value_options" checked={value === "GREEN"} value="GREEN" aria-label="GREEN" onchange={e => e.target.checked && changeValue(e.target.value)} />
                                    <input class="join-item btn" type="radio" name="control_value_options" checked={value === "CHAOS"} value="CHAOS" aria-label="CHAOS" onchange={e => e.target.checked && changeValue(e.target.value)} />
                                    <input class="join-item btn" type="radio" name="control_value_options" checked={value === "FINISH"} value="FINISH" aria-label="FINISH" onchange={e => e.target.checked && changeValue(e.target.value)} />
                                </div>
                            {/snippet}
                        </Characteristic>
                    {/snippet}
                </Characteristic>
            </div>
        </div>
    </div>
{:else if !device && !connecting && !disconnecting}
    <div class="flex-1 max-h-96 flex flex-col justify-center items-center">
        <button class="flex flex-col justify-center items-center gap-10 cursor-pointer py-4 w-full" onclick={connect()}>
            <svg xmlns="http://www.w3.org/2000/svg" fill="currentColor" viewBox="0 0 24 24" class="size-32">
                <path fill-rule="evenodd"
                      d="M10.5857864,12 L1.29289322,2.70710678 L2.70710678,1.29289322 L22.7071068,21.2928932 L21.2928932,22.7071068 L16,17.4142136 L11,22.4142136 L11,14.4142136 L7.70710678,17.7071068 L6.29289322,16.2928932 L10.5857864,12 Z M13,14.4142136 L13,17.5857864 L14.5857864,16 L13,14.4142136 Z M11,6.78578649 L11,1.58578644 L17.4142136,8 L14.8142135,10.6 L13.4,9.18578646 L14.5857864,8 L13,6.41421356 L13,8.78578649 L11,6.78578649 Z"/>
            </svg>
            <div class="btn">Mit CMA Center Verbinden</div>
        </button>
    </div>
{/if}

<FloatingBottomBar>
    <Snackbars notifications={Object.values(notifications)} onDismiss={onDismissNotification()}/>
    {#if connecting || disconnecting || device}
        <ConnectionAlert {connecting}
                         {disconnecting}
                         {isReconnect}
                         {characteristics}
                         disconnect={disconnect()}
        />
    {/if}
</FloatingBottomBar>



