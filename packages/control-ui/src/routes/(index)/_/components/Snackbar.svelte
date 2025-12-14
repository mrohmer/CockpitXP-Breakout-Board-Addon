<script lang="ts">
    import type {Notification} from "../models/notification";
    import {onMount} from "svelte";
    import {slide} from "svelte/transition";

    type Props = {
        notification: Notification;
        onDismiss?: (id: string) => void
    }
    let {notification, onDismiss}: Props = $props();
    let t = $state(new Date());
    let dismissed = $derived(notification?.dismissAfter ? +notification.dismissAfter < +t : false);

    const callOnDismiss = () => {
        if (!onDismiss || notification.type === 'progress') {
            return;
        }

        onDismiss(notification.id);
    }
    const onDismissClick = () => callOnDismiss;


    $effect(() => {
        dismissed && callOnDismiss();
    });
    onMount(() => {
        const interval = setInterval(() => {
            t = new Date();
        });
        return () => {
            clearInterval(interval);
        }
    });
</script>

{#if notification && !dismissed}
    <button transition:slide
         class="card card-compact bg-base-200 w-full shadow-xl overflow-hidden pb-0"
         class:pb-2={!notification.type}
         onclick={onDismissClick()}
    >
        <div class="card-body !py-3">
            <p class="font-bold">{notification.text}</p>
        </div>

        {#if notification.type === 'progress' && notification.progress !== undefined}
            <progress class="progress w-full !h-1" value={notification.progress * 100} max="100"></progress>
        {:else if notification.type === 'progress'}
            <progress class="progress w-full !h-1"></progress>
        {:else}
            <progress class="progress w-full !h-1" value="100" max="100"
                      class:progress-error={notification.type === 'error'}
                      class:progress-success={notification.type === 'success'}
            ></progress>
        {/if}
    </button>
{/if}
