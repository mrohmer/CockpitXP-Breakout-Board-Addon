<script lang="ts">
    let {children} = $props();

    let height = $state(0);
</script>

{#if children}
    <div class="fixed max-w-lg mx-auto inset-x-4 bottom-4 z-50 space-y-3"
         bind:clientHeight={height}>
        {@render children()}
    </div>
    <div style:height={`${height}px`}></div>
{/if}