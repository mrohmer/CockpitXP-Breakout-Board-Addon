<script lang="ts">
    import {createEventDispatcher} from 'svelte';

    const dispatcher = createEventDispatcher();

    const dispatch = (event: string) => () => {
      dispatcher(event)
    }
</script>
<div class="join">
    <div>
        <button class="btn btn-square join-item z-10" on:click={dispatch('dec')} type="button">
            <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor" class="w-6 h-6">
                <path stroke-linecap="round" stroke-linejoin="round" d="M5 12h14" />
            </svg>

        </button>
    </div>
    <slot />
    <div>
        <button class="btn btn-square join-item z-10" on:click={dispatch('inc')} type="button">
            <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor" class="w-6 h-6">
                <path stroke-linecap="round" stroke-linejoin="round" d="M12 4.5v15m7.5-7.5h-15" />
            </svg>
        </button>
    </div>
</div>
