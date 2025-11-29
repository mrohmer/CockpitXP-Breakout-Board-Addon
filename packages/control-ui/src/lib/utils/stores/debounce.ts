import {derived, get, type Readable} from "svelte/store";

export const debounce = <T>(store: Readable<T>, timeoutMs = 100) =>
    derived(
        store,
        ($value, set) => {
            const intervalId = setTimeout(() => set($value), timeoutMs);
            return () => clearTimeout(intervalId);
        },
        get(store)
    );