import {browser} from "$app/environment";
import {readable, type Readable, readonly, type Subscriber, type Unsubscriber} from "svelte/store";

const useEvents = true;

type Characteristics =
    | 'controlEnabled'
    | 'controlValue'
    | 'flagBrightness'
    | 'chaosStyle'
    | 'flagDevices'
    | 'flagDeviceIdentify'
    | 'flagDeviceToggleLight';
export type ConnectResult = {
    device: BluetoothDevice;
    server: BluetoothRemoteGATTServer;
    characteristics: Record<Characteristics, BluetoothRemoteGATTCharacteristic>
}

const BLE_FLAGS_CONTROL_SERVICE_UUID = "d00d3b3f-0ba9-493b-93b8-37c268f7315f";
const BLE_FLAG_DEVICES_SERVICE_UUID = "70295cf8-e75d-4192-83a0-59fb20eb7444";
const BLE_FLAGS_LIGHT_CONFIG_SERVICE_UUID = "4b8b0562-cb00-4469-948f-a36222dd4805";
const BLE_FLAGS_CONTROL_ENABLE_CHARACTERISTICS_UUID = "bf191dbf-5147-440e-96d4-0f8b2080f8ce";
const BLE_FLAGS_CONTROL_VALUE_CHARACTERISTICS_UUID = "010081b9-b828-4f92-ac11-a159ce55ead2";
const BLE_FLAG_DEVICES_STATE_CHARACTERISTICS_UUID = "b32474c1-42d6-495a-a514-47f48ee72965";
const BLE_FLAG_DEVICES_IDENTIFY_CHARACTERISTICS_UUID = "10a1c632-b0a5-4d38-b53e-b9a652adc84b";
const BLE_FLAG_DEVICES_LIGHT_TOGGLE_CHARACTERISTICS_UUID = "69029ed0-711a-4a96-bfec-3f414d020d0f";
const BLE_FLAGS_LIGHT_CONFIG_BRIGHTNESS_CHARACTERISTICS_UUID = "21e1ab05-c437-458e-9d57-ba2071109980";
const BLE_FLAGS_LIGHT_CONFIG_CHAOS_STYLE_CHARACTERISTICS_UUID = "5a5c31d5-bf66-450a-91b2-52915bfdbe5c";

const isSupported = () => browser && typeof navigator?.bluetooth !== "undefined";
const hasPermission = async (): Promise<boolean> => {
    return true;
}

const createConnectionResult = async (device: BluetoothDevice) => {
    if (!device?.gatt) {
        return undefined;
    }

    const server = await device.gatt.connect();
    const flagsControlService = await server.getPrimaryService(BLE_FLAGS_CONTROL_SERVICE_UUID);
    const flagDevicesService = await server.getPrimaryService(BLE_FLAG_DEVICES_SERVICE_UUID);
    const flagLightConfigService = await server.getPrimaryService(BLE_FLAGS_LIGHT_CONFIG_SERVICE_UUID);


    const [controlEnabled, controlValue, flagDevices, flagDeviceIdentify, flagDeviceToggleLight, flagBrightness, chaosStyle] = await Promise.all([
            flagsControlService.getCharacteristic(BLE_FLAGS_CONTROL_ENABLE_CHARACTERISTICS_UUID),
            flagsControlService.getCharacteristic(BLE_FLAGS_CONTROL_VALUE_CHARACTERISTICS_UUID),
            flagDevicesService.getCharacteristic(BLE_FLAG_DEVICES_STATE_CHARACTERISTICS_UUID),
            flagDevicesService.getCharacteristic(BLE_FLAG_DEVICES_IDENTIFY_CHARACTERISTICS_UUID),
            flagDevicesService.getCharacteristic(BLE_FLAG_DEVICES_LIGHT_TOGGLE_CHARACTERISTICS_UUID),
            flagLightConfigService.getCharacteristic(BLE_FLAGS_LIGHT_CONFIG_BRIGHTNESS_CHARACTERISTICS_UUID),
            flagLightConfigService.getCharacteristic(BLE_FLAGS_LIGHT_CONFIG_CHAOS_STYLE_CHARACTERISTICS_UUID),
        ]
            .map(p => p.catch(e => {
                console.error(e);
                return undefined;
            }))
    );

    const characteristics: ConnectResult['characteristics'] = {
        controlEnabled,
        controlValue,
        flagDevices,
        flagDeviceIdentify,
        flagDeviceToggleLight,
        flagBrightness,
        chaosStyle,
    };
    return {device, server, characteristics};
}
const internalTryReconnect = async (device: BluetoothDevice) => {
    await device.watchAdvertisements();
    try {
        return await createConnectionResult(device);
    } catch (e) {
        await tryDisconnect(device);
        throw e;
    }
}
const tryReconnect = async (id: string, timeout?: number): Promise<ConnectResult | undefined> => {
    if (!id || !isSupported()) {
        return undefined;
    }

    try {
        const devices = await navigator?.bluetooth.getDevices();

        const device = devices.find((device: BluetoothDevice) => device.id === id);

        if (device && !timeout) {
            return await internalTryReconnect(device);
        } else if (device) {
            const result = await Promise.race([
                internalTryReconnect(device),
                new Promise(resolve => setTimeout(resolve, timeout)),
            ]);

            if (!result) {
                await tryDisconnect(device);
                return undefined;
            }

            return result as ConnectResult;
        }
    } catch (e) {
        console.error(e);
        return undefined;
    }
}

const connect = async (): Promise<ConnectResult | undefined> => {
    if (!isSupported() || !(await hasPermission())) {
        return undefined;
    }

    const device = await navigator?.bluetooth.requestDevice({
        filters: [{namePrefix: "CMA"}],
        optionalServices: [BLE_FLAGS_CONTROL_SERVICE_UUID, BLE_FLAG_DEVICES_SERVICE_UUID, BLE_FLAGS_LIGHT_CONFIG_SERVICE_UUID],
    });
    console.log(device);
    try {
        return await createConnectionResult(device);
    } catch (e) {
        await tryDisconnect(device);
        throw e;
    }
}

const disconnect = async (device: BluetoothDevice) => device?.gatt?.disconnect?.();
const tryDisconnect = async (device: BluetoothDevice): Promise<void> => {
    try {
        await disconnect(device);
    } catch (de) {
        console.error(de);
    }
}

const decodeValue = (value: DataView<ArrayBufferLike> | undefined): string | undefined => {
    if (!value) {
        return undefined;
    }

    const decoded = new TextDecoder().decode(value);

    if (!decoded) {
        return undefined;
    }

    return decoded;
}
let interactingWithCharacteristic = false;
const readCharacteristic = async (characteristic: BluetoothRemoteGATTCharacteristic): Promise<string | undefined> => {
    while (interactingWithCharacteristic) {
        await new Promise(resolve => setTimeout(resolve, 5));
    }
    interactingWithCharacteristic = true;

    try {
        const v = await characteristic.readValue();
        const decoded = new TextDecoder().decode(v);

        if (!decoded) {
            return undefined;
        }

        return decoded;
    } finally {
        interactingWithCharacteristic = false;
    }
}
const writeCharacteristicWithoutResponse = async (characteristic: BluetoothRemoteGATTCharacteristic, value: string): Promise<void> => {
    while (interactingWithCharacteristic) {
        await new Promise(resolve => setTimeout(resolve, 5));
    }
    interactingWithCharacteristic = true;

    try {
        return await characteristic.writeValueWithoutResponse(new TextEncoder().encode(value));
    } finally {
        interactingWithCharacteristic = false;
    }
}

const listenToCharacteristic = (characteristic: BluetoothRemoteGATTCharacteristic, shouldReadNow?: () => boolean): Readable<string | undefined> => {
    if (!characteristic) {
        return readonly(readable<string>(undefined));
    }

    return {
        subscribe(run: Subscriber<string | undefined>, invalidate?: () => void): Unsubscriber {
            let v: string | undefined = undefined;

            let cleanedUp = false;
            let reading = false;
            const cleanup = () => {
                useEvents && characteristic.removeEventListener("characteristicvaluechanged", eventHandler);
                clearInterval(interval);
                cleanedUp = true;
            }

            const eventHandler = () => {
                if (cleanedUp) {
                    // somehow it's still called
                    return;
                }
                const value = decodeValue(characteristic.value);

                if (value) {
                    postUpdate(value);
                }
            };
            const postUpdate = (value: string | undefined) => {
                const cleaned = !['', 'null', 'undefined'].includes(value as any) ? value : undefined;

                if (cleaned === v) {
                    return;
                }

                v = cleaned;
                run(cleaned);
            }
            const readValue = async () => {
                try {
                    if (reading) {
                        return;
                    }
                    reading = true;
                    const value = await readCharacteristic(characteristic);

                    postUpdate(value);
                } catch (e) {
                    console.error(characteristic.uuid, e);
                    if (!characteristic?.service?.device?.id) {
                        cleanup();
                    }
                } finally {
                    reading = false;
                }

            }
            useEvents && characteristic.addEventListener("characteristicvaluechanged", eventHandler);

            const interval = setInterval(() => {
                if (typeof shouldReadNow === 'function' && !shouldReadNow()) {
                    return;
                }
                readValue();
            }, 1000);

            readValue();

            return cleanup
        }
    }
}

export const bluetooth = {
    isSupported,
    hasPermission,
    tryReconnect,
    connect,
    disconnect,
    listenToCharacteristic,
    readCharacteristic,
    writeCharacteristicWithoutResponse,
}