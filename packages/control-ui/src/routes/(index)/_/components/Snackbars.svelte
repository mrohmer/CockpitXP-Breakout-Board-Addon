<script lang="ts">
    import {createProgress, type Notification} from "../models/notification";
    import Snackbar from "./Snackbar.svelte";

    type Props = {
        notifications: Notification[];
        onDismiss?: (id: string) => void
    }
    let {notifications, ...rest}: Props = $props();

    let n: Notification[] = $state([]);
    let ellipsisNotification: Notification|undefined = $state(undefined);

    const MAX_SNACKBARS = 4;

    $effect(() => {
        n = notifications.length > MAX_SNACKBARS ? notifications.slice(0, MAX_SNACKBARS - 1) : notifications;
    })
    $effect(() => {
        const overMax = notifications.length > MAX_SNACKBARS ? notifications.slice(MAX_SNACKBARS, notifications.length) : [];

        if (!overMax?.length) {
            ellipsisNotification = undefined;
            return;
        }

        const types = new Set(overMax.map(({type}) => type));

        const notification = createProgress('more', `+${Object.values(notifications).length - MAX_SNACKBARS} weitere`);
        if (types.has('progress') || types.size > 1) {
            ellipsisNotification = notification;
            return;
        }

        notification.type = Array.from(types.values())[0];
        ellipsisNotification = notification;
    })
</script>

{#each n as notification (notification.id)}
    <Snackbar {notification} {...rest} />
{/each}
{#if ellipsisNotification}
    <Snackbar notification={ellipsisNotification}/>
{/if}