
import {persisted} from 'svelte-persisted-store'

export interface FlagDeviceName {
    name: string,
    synced: boolean,
}

export const flagDeviceNames = persisted<Record<string, string|FlagDeviceName>>("cma-center:flag-device-names", {});