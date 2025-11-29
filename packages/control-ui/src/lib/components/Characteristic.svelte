<script module lang="ts">
    import {readable, type Readable} from "svelte/store";
    import {bluetooth} from "$lib";

    const stores: Record<string, Readable<string | undefined>> = {};

    const createCharacteristicStore = (characteristic: BluetoothRemoteGATTCharacteristic, shouldReadNow?: () => boolean): Readable<string | undefined> => {
        if (!characteristic) {
            return readable(undefined);
        }

        const id = characteristic.uuid;

        stores[id] = stores[id] ?? bluetooth.listenToCharacteristic(characteristic, shouldReadNow);

        return stores[id];
    }
</script>

<script lang="ts">
    import {debounce} from "$lib/utils/stores/debounce.js";
    import {dev} from "$app/environment";

    const {
        characteristic,
        onchange,
        content
    }: Record<'characteristic', BluetoothRemoteGATTCharacteristic | undefined> & Partial<Record<'onchange', (data: string) => void>> & HasChildren = $props();

    const isVisible = () =>
        !(document.hidden || document.msHidden || document.webkitHidden || document.mozHidden);

    const value = $derived(debounce(createCharacteristicStore(characteristic)));

    $effect(() => {
        dev && console.log($value);
        onchange?.($value);
    })
</script>

{#if content}
    {@render content($value)}
{:else if $value}
    {$value}
{:else}
    <i>empty</i>
{/if}