import type {PageLoad} from "./$types";
import {bluetooth} from "$lib";
import {redirect} from "@sveltejs/kit";
import {browser} from "$app/environment";

const assertBluetooth = async () => {
    if (!browser) {
        return;
    }

    if (!bluetooth.isSupported()) {
        throw redirect(302, '/not-supported');
    }

    if (!(await bluetooth.hasPermission())) {
        throw redirect(302, '/no-permission');
    }
}
export const load: PageLoad = async () => {
    await assertBluetooth();

    return {
        allowed: true,
    };
}